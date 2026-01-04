#include "renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>

Renderer::Renderer() 
    : vao(0), vbo(0), ebo(0), shaderProgram(0), 
      vertexCount(0), indexCount(0) {
    initializeRenderer();
}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::initializeRenderer() {
    // Initialize shader program
    shaderProgram = createShaderProgram();
    
    // Generate VAO, VBO, and EBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    // Setup vertex attributes
    setupVertexAttributes();
    
    std::cout << "Renderer initialized successfully" << std::endl;
}

GLuint Renderer::createShaderProgram() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 normal;
        layout (location = 2) in vec2 texCoord;
        layout (location = 3) in float blockType;
        
        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoord;
        out float BlockType;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            FragPos = vec3(model * vec4(position, 1.0));
            Normal = mat3(transpose(inverse(model))) * normal;
            TexCoord = texCoord;
            BlockType = blockType;
            
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoord;
        in float BlockType;
        
        out vec4 FragColor;
        
        uniform sampler2D texture0;
        uniform sampler2D texture1;
        uniform sampler2D texture2;
        uniform sampler2D texture3;
        
        void main() {
            vec4 texColor;
            
            // Select texture based on block type
            if (BlockType < 0.5) {
                texColor = texture(texture0, TexCoord);
            } else if (BlockType < 1.5) {
                texColor = texture(texture1, TexCoord);
            } else if (BlockType < 2.5) {
                texColor = texture(texture2, TexCoord);
            } else {
                texColor = texture(texture3, TexCoord);
            }
            
            // Lighting calculation
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
            
            float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * texColor.rgb;
            
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * texColor.rgb;
            
            vec3 result = (ambient + diffuse);
            FragColor = vec4(result, texColor.a);
        }
    )";
    
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader, "VERTEX");
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader, "FRAGMENT");
    
    // Link shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    checkProgramLinking(program);
    
    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

void Renderer::setupVertexAttributes() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                         sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                         sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
                         sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);
    
    // Block type attribute
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 
                         sizeof(Vertex), (void*)offsetof(Vertex, blockType));
    glEnableVertexAttribArray(3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::renderChunk(const Chunk& chunk, const glm::mat4& view, 
                          const glm::mat4& projection) {
    if (chunk.getVertexCount() == 0) {
        return;
    }
    
    glUseProgram(shaderProgram);
    
    // Set up transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, chunk.getPosition());
    
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Bind chunk's VAO and draw
    glBindVertexArray(chunk.getVAO());
    glDrawElements(GL_TRIANGLES, chunk.getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::renderChunks(const std::vector<Chunk>& chunks, 
                           const glm::mat4& view, const glm::mat4& projection) {
    for (const auto& chunk : chunks) {
        renderChunk(chunk, view, projection);
    }
}

void Renderer::renderBlock(const Block& block, const glm::mat4& view, 
                          const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    
    // Set up transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, block.getPosition());
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Bind and draw
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::uploadVertexData(const std::vector<Vertex>& vertices, 
                               const std::vector<GLuint>& indices) {
    vertexCount = vertices.size();
    indexCount = indices.size();
    
    glBindVertexArray(vao);
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                 vertices.data(), GL_DYNAMIC_DRAW);
    
    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
                 indices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::setTexture(GLuint textureUnit, GLuint textureID) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    GLint texLoc = glGetUniformLocation(shaderProgram, 
                                       ("texture" + std::to_string(textureUnit)).c_str());
    glUniform1i(texLoc, textureUnit);
}

void Renderer::enableFaceCulling(bool enable) {
    if (enable) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::enableDepthTesting(bool enable) {
    if (enable) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::setWireframeMode(bool wireframe) {
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::clear(const glm::vec4& clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::checkShaderCompilation(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "Shader compilation error [" << type << "]: " 
                  << infoLog << std::endl;
    }
}

void Renderer::checkProgramLinking(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "Program linking error: " << infoLog << std::endl;
    }
}

void Renderer::cleanup() {
    if (vao != 0) glDeleteVertexArrays(1, &vao);
    if (vbo != 0) glDeleteBuffers(1, &vbo);
    if (ebo != 0) glDeleteBuffers(1, &ebo);
    if (shaderProgram != 0) glDeleteProgram(shaderProgram);
    
    std::cout << "Renderer cleanup completed" << std::endl;
}

GLuint Renderer::getVAO() const {
    return vao;
}

GLuint Renderer::getVBO() const {
    return vbo;
}

GLuint Renderer::getEBO() const {
    return ebo;
}

GLuint Renderer::getShaderProgram() const {
    return shaderProgram;
}

size_t Renderer::getVertexCount() const {
    return vertexCount;
}

size_t Renderer::getIndexCount() const {
    return indexCount;
}
