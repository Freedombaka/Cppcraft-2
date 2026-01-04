#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

class Shader;
class Texture;

/**
 * @class Renderer
 * @brief Handles all OpenGL rendering operations for the game engine.
 * 
 * The Renderer class manages the rendering pipeline, including shader programs,
 * vertex buffers, textures, and drawing operations. It provides a clean interface
 * for rendering 2D and 3D graphics using OpenGL.
 */
class Renderer {
public:
    /**
     * @brief Constructor for the Renderer class.
     */
    Renderer();

    /**
     * @brief Destructor for the Renderer class.
     */
    ~Renderer();

    /**
     * @brief Initialize the renderer and OpenGL context.
     * @return true if initialization was successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Shutdown the renderer and clean up resources.
     */
    void shutdown();

    /**
     * @brief Begin a new frame for rendering.
     */
    void beginFrame();

    /**
     * @brief End the current frame and present to screen.
     */
    void endFrame();

    /**
     * @brief Clear the screen with the specified color.
     * @param r Red component (0.0 - 1.0)
     * @param g Green component (0.0 - 1.0)
     * @param b Blue component (0.0 - 1.0)
     * @param a Alpha component (0.0 - 1.0)
     */
    void clearScreen(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Set the viewport dimensions.
     * @param width Width in pixels
     * @param height Height in pixels
     */
    void setViewport(int width, int height);

    /**
     * @brief Set the projection matrix.
     * @param projection The projection matrix
     */
    void setProjectionMatrix(const glm::mat4& projection);

    /**
     * @brief Set the view matrix.
     * @param view The view matrix
     */
    void setViewMatrix(const glm::mat4& view);

    /**
     * @brief Set the model matrix.
     * @param model The model matrix
     */
    void setModelMatrix(const glm::mat4& model);

    /**
     * @brief Get the current projection matrix.
     * @return The projection matrix
     */
    const glm::mat4& getProjectionMatrix() const;

    /**
     * @brief Get the current view matrix.
     * @return The view matrix
     */
    const glm::mat4& getViewMatrix() const;

    /**
     * @brief Get the current model matrix.
     * @return The model matrix
     */
    const glm::mat4& getModelMatrix() const;

    /**
     * @brief Use a shader program for subsequent rendering operations.
     * @param shader The shader program to use
     */
    void useShader(std::shared_ptr<Shader> shader);

    /**
     * @brief Get the currently active shader.
     * @return Pointer to the active shader, or nullptr if none
     */
    std::shared_ptr<Shader> getCurrentShader() const;

    /**
     * @brief Bind a texture for rendering.
     * @param texture The texture to bind
     * @param unit The texture unit to bind to (default: 0)
     */
    void bindTexture(std::shared_ptr<Texture> texture, unsigned int unit = 0);

    /**
     * @brief Draw a vertex array.
     * @param vao Vertex Array Object ID
     * @param vertexCount Number of vertices to draw
     * @param indexCount Number of indices to draw (if 0, uses vertexCount)
     * @param offset Offset into the buffer (default: 0)
     */
    void drawVertexArray(unsigned int vao, unsigned int vertexCount, unsigned int indexCount = 0, unsigned int offset = 0);

    /**
     * @brief Draw arrays without using indices.
     * @param vao Vertex Array Object ID
     * @param vertexCount Number of vertices to draw
     * @param offset Offset into the buffer (default: 0)
     */
    void drawArrays(unsigned int vao, unsigned int vertexCount, unsigned int offset = 0);

    /**
     * @brief Draw indexed geometry.
     * @param vao Vertex Array Object ID
     * @param indexCount Number of indices to draw
     * @param offset Offset into the index buffer (default: 0)
     */
    void drawIndexed(unsigned int vao, unsigned int indexCount, unsigned int offset = 0);

    /**
     * @brief Enable/disable depth testing.
     * @param enabled true to enable, false to disable
     */
    void setDepthTesting(bool enabled);

    /**
     * @brief Enable/disable blending.
     * @param enabled true to enable, false to disable
     */
    void setBlending(bool enabled);

    /**
     * @brief Set the blend function.
     * @param srcFactor Source blend factor
     * @param dstFactor Destination blend factor
     */
    void setBlendFunction(unsigned int srcFactor, unsigned int dstFactor);

    /**
     * @brief Enable/disable face culling.
     * @param enabled true to enable, false to disable
     */
    void setFaceCulling(bool enabled);

    /**
     * @brief Set the front face winding order.
     * @param clockwise true for clockwise, false for counter-clockwise
     */
    void setFrontFace(bool clockwise);

    /**
     * @brief Enable/disable wireframe mode.
     * @param enabled true for wireframe, false for solid
     */
    void setWireframeMode(bool enabled);

    /**
     * @brief Get the renderer version string.
     * @return OpenGL version string
     */
    const char* getVersionString() const;

    /**
     * @brief Get available OpenGL features and capabilities.
     * @return Information about supported features
     */
    std::string getCapabilitiesInfo() const;

private:
    // Matrix storage
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_modelMatrix;

    // Shader and texture management
    std::shared_ptr<Shader> m_currentShader;
    std::shared_ptr<Texture> m_currentTexture;

    // Render state
    bool m_depthTestingEnabled;
    bool m_blendingEnabled;
    bool m_faceCullingEnabled;
    bool m_wireframeMode;

    // Viewport dimensions
    int m_viewportWidth;
    int m_viewportHeight;

    /**
     * @brief Internal method to apply the current matrix stack to the shader.
     */
    void applyMatrices();

    /**
     * @brief Internal method to validate the current OpenGL state.
     * @return true if state is valid, false otherwise
     */
    bool validateRenderState() const;
};

#endif // RENDERER_H
