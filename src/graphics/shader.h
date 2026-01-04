#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

/**
 * @class Shader
 * @brief Manages OpenGL shader programs for rendering
 * 
 * Provides functionality to compile, link, and manage vertex and fragment shaders.
 * Supports setting uniforms and managing the active shader program.
 */
class Shader
{
public:
    /**
     * @brief Constructor - creates and links a shader program
     * @param vertexPath Path to the vertex shader source file
     * @param fragmentPath Path to the fragment shader source file
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Destructor - deletes the shader program
     */
    ~Shader();

    /**
     * @brief Activates the shader program
     */
    void use() const;

    /**
     * @brief Sets a boolean uniform
     * @param name The name of the uniform variable
     * @param value The boolean value to set
     */
    void setBool(const std::string& name, bool value) const;

    /**
     * @brief Sets an integer uniform
     * @param name The name of the uniform variable
     * @param value The integer value to set
     */
    void setInt(const std::string& name, int value) const;

    /**
     * @brief Sets a float uniform
     * @param name The name of the uniform variable
     * @param value The float value to set
     */
    void setFloat(const std::string& name, float value) const;

    /**
     * @brief Sets a vec2 uniform
     * @param name The name of the uniform variable
     * @param value The glm::vec2 value to set
     */
    void setVec2(const std::string& name, const glm::vec2& value) const;

    /**
     * @brief Sets a vec2 uniform with individual components
     * @param name The name of the uniform variable
     * @param x The x component
     * @param y The y component
     */
    void setVec2(const std::string& name, float x, float y) const;

    /**
     * @brief Sets a vec3 uniform
     * @param name The name of the uniform variable
     * @param value The glm::vec3 value to set
     */
    void setVec3(const std::string& name, const glm::vec3& value) const;

    /**
     * @brief Sets a vec3 uniform with individual components
     * @param name The name of the uniform variable
     * @param x The x component
     * @param y The y component
     * @param z The z component
     */
    void setVec3(const std::string& name, float x, float y, float z) const;

    /**
     * @brief Sets a vec4 uniform
     * @param name The name of the uniform variable
     * @param value The glm::vec4 value to set
     */
    void setVec4(const std::string& name, const glm::vec4& value) const;

    /**
     * @brief Sets a vec4 uniform with individual components
     * @param name The name of the uniform variable
     * @param x The x component
     * @param y The y component
     * @param z The z component
     * @param w The w component
     */
    void setVec4(const std::string& name, float x, float y, float z, float w) const;

    /**
     * @brief Sets a mat2 uniform
     * @param name The name of the uniform variable
     * @param mat The glm::mat2 matrix to set
     */
    void setMat2(const std::string& name, const glm::mat2& mat) const;

    /**
     * @brief Sets a mat3 uniform
     * @param name The name of the uniform variable
     * @param mat The glm::mat3 matrix to set
     */
    void setMat3(const std::string& name, const glm::mat3& mat) const;

    /**
     * @brief Sets a mat4 uniform
     * @param name The name of the uniform variable
     * @param mat The glm::mat4 matrix to set
     */
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    /**
     * @brief Gets the shader program ID
     * @return The OpenGL shader program ID
     */
    GLuint getId() const { return ID; }

private:
    GLuint ID;

    /**
     * @brief Compiles a shader from source code
     * @param source The shader source code
     * @param shaderType The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
     * @return The compiled shader ID
     */
    GLuint compileShader(const std::string& source, GLenum shaderType) const;

    /**
     * @brief Reads shader source code from a file
     * @param filePath Path to the shader file
     * @return The shader source code as a string
     */
    std::string readShaderFile(const char* filePath) const;

    /**
     * @brief Links vertex and fragment shaders into a program
     * @param vertexShader The compiled vertex shader ID
     * @param fragmentShader The compiled fragment shader ID
     * @return The linked shader program ID
     */
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) const;
};

#endif // SHADER_H
