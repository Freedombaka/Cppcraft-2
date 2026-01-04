#include "chunk.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

// Constructor
Chunk::Chunk(int chunkX, int chunkY, int chunkZ, World* world)
    : chunkX(chunkX), chunkY(chunkY), chunkZ(chunkZ), world(world),
      blocks(CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT, BlockType::Air),
      meshDirty(true), meshBuilt(false), vao(0), vbo(0), ebo(0),
      vertexCount(0), indexCount(0) {}

// Destructor
Chunk::~Chunk() {
    cleanup();
}

// Get block at local coordinates
BlockType Chunk::getBlock(int x, int y, int z) const {
    if (isInBounds(x, y, z)) {
        return blocks[getBlockIndex(x, y, z)];
    }
    return BlockType::Air;
}

// Set block at local coordinates
void Chunk::setBlock(int x, int y, int z, BlockType type) {
    if (isInBounds(x, y, z)) {
        blocks[getBlockIndex(x, y, z)] = type;
        meshDirty = true;
    }
}

// Check if coordinates are within chunk bounds
bool Chunk::isInBounds(int x, int y, int z) const {
    return x >= 0 && x < CHUNK_SIZE &&
           y >= 0 && y < CHUNK_HEIGHT &&
           z >= 0 && z < CHUNK_SIZE;
}

// Calculate linear index for block array
int Chunk::getBlockIndex(int x, int y, int z) const {
    return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
}

// Generate chunk terrain using Perlin noise
void Chunk::generate() {
    // Simple terrain generation using height-based noise
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            // Calculate world coordinates
            float worldX = chunkX * CHUNK_SIZE + x;
            float worldZ = chunkZ * CHUNK_SIZE + z;
            
            // Generate height using noise function
            int height = generateHeight(worldX, worldZ);
            
            // Fill blocks from ground up to height
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                BlockType blockType = BlockType::Air;
                
                if (y < height - 3) {
                    blockType = BlockType::Stone;
                } else if (y < height) {
                    blockType = BlockType::Dirt;
                } else if (y == height) {
                    blockType = BlockType::Grass;
                }
                
                setBlock(x, y, z, blockType);
            }
        }
    }
    
    meshDirty = true;
}

// Simplified Perlin noise height generation
int Chunk::generateHeight(float x, float z) const {
    // Using a simple sine/cosine based height function
    float scale1 = 50.0f;
    float scale2 = 100.0f;
    
    float height = 40.0f;
    height += std::sin(x / scale1) * 15.0f;
    height += std::cos(z / scale2) * 15.0f;
    height += std::sin((x + z) / 75.0f) * 10.0f;
    
    return std::clamp(static_cast<int>(height), 1, CHUNK_HEIGHT - 1);
}

// Build mesh for rendering
void Chunk::buildMesh() {
    if (!meshDirty && meshBuilt) {
        return;
    }
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Iterate through all blocks in the chunk
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                BlockType blockType = getBlock(x, y, z);
                
                if (blockType == BlockType::Air) {
                    continue;
                }
                
                // Check each face and add if exposed
                addFaceIfExposed(x, y, z, blockType, 0, vertices, indices); // Front
                addFaceIfExposed(x, y, z, blockType, 1, vertices, indices); // Back
                addFaceIfExposed(x, y, z, blockType, 2, vertices, indices); // Left
                addFaceIfExposed(x, y, z, blockType, 3, vertices, indices); // Right
                addFaceIfExposed(x, y, z, blockType, 4, vertices, indices); // Bottom
                addFaceIfExposed(x, y, z, blockType, 5, vertices, indices); // Top
            }
        }
    }
    
    vertexCount = vertices.size() / 8; // 8 floats per vertex (pos + tex)
    indexCount = indices.size();
    
    // Create or update OpenGL buffers
    if (!meshBuilt) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }
    
    glBindVertexArray(vao);
    
    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Vertex attribute pointers
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    meshDirty = false;
    meshBuilt = true;
}

// Add a face if it's exposed (adjacent block is air)
void Chunk::addFaceIfExposed(int x, int y, int z, BlockType blockType, int face,
                              std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    int nx = x, ny = y, nz = z;
    
    // Determine neighbor coordinates based on face
    switch (face) {
        case 0: nz++; break; // Front
        case 1: nz--; break; // Back
        case 2: nx--; break; // Left
        case 3: nx++; break; // Right
        case 4: ny--; break; // Bottom
        case 5: ny++; break; // Top
    }
    
    // Check if neighbor is air (exposed)
    BlockType neighborBlock = getBlock(nx, ny, nz);
    if (neighborBlock != BlockType::Air) {
        return;
    }
    
    // Add face vertices and indices
    addFace(x, y, z, face, blockType, vertices, indices);
}

// Add a face to the mesh
void Chunk::addFace(int x, int y, int z, int face, BlockType blockType,
                     std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    unsigned int baseIndex = vertices.size() / 8;
    
    float texX = getTextureCoord(blockType, face).x;
    float texY = getTextureCoord(blockType, face).y;
    
    // Define face vertices (position, texture coordinates, normal)
    std::vector<float> faceVertices;
    
    switch (face) {
        case 0: // Front (+Z)
            faceVertices = {
                x,     y,     z + 1, texX,     texY,     0, 0, 1,
                x + 1, y,     z + 1, texX + 1, texY,     0, 0, 1,
                x + 1, y + 1, z + 1, texX + 1, texY + 1, 0, 0, 1,
                x,     y + 1, z + 1, texX,     texY + 1, 0, 0, 1,
            };
            break;
        case 1: // Back (-Z)
            faceVertices = {
                x + 1, y,     z,     texX,     texY,     0, 0, -1,
                x,     y,     z,     texX + 1, texY,     0, 0, -1,
                x,     y + 1, z,     texX + 1, texY + 1, 0, 0, -1,
                x + 1, y + 1, z,     texX,     texY + 1, 0, 0, -1,
            };
            break;
        case 2: // Left (-X)
            faceVertices = {
                x,     y,     z,     texX,     texY,     -1, 0, 0,
                x,     y,     z + 1, texX + 1, texY,     -1, 0, 0,
                x,     y + 1, z + 1, texX + 1, texY + 1, -1, 0, 0,
                x,     y + 1, z,     texX,     texY + 1, -1, 0, 0,
            };
            break;
        case 3: // Right (+X)
            faceVertices = {
                x + 1, y,     z + 1, texX,     texY,     1, 0, 0,
                x + 1, y,     z,     texX + 1, texY,     1, 0, 0,
                x + 1, y + 1, z,     texX + 1, texY + 1, 1, 0, 0,
                x + 1, y + 1, z + 1, texX,     texY + 1, 1, 0, 0,
            };
            break;
        case 4: // Bottom (-Y)
            faceVertices = {
                x,     y,     z + 1, texX,     texY,     0, -1, 0,
                x,     y,     z,     texX + 1, texY,     0, -1, 0,
                x + 1, y,     z,     texX + 1, texY + 1, 0, -1, 0,
                x + 1, y,     z + 1, texX,     texY + 1, 0, -1, 0,
            };
            break;
        case 5: // Top (+Y)
            faceVertices = {
                x,     y + 1, z,     texX,     texY,     0, 1, 0,
                x,     y + 1, z + 1, texX + 1, texY,     0, 1, 0,
                x + 1, y + 1, z + 1, texX + 1, texY + 1, 0, 1, 0,
                x + 1, y + 1, z,     texX,     texY + 1, 0, 1, 0,
            };
            break;
    }
    
    // Add vertices
    vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());
    
    // Add indices (two triangles per face)
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
    
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}

// Get texture coordinates for a block type
glm::vec2 Chunk::getTextureCoord(BlockType blockType, int face) const {
    // Simple texture atlas mapping (assuming 16x16 grid)
    float texSize = 1.0f / 16.0f;
    glm::vec2 texCoord(0.0f);
    
    switch (blockType) {
        case BlockType::Grass:
            texCoord = glm::vec2(0, 0) * texSize;
            break;
        case BlockType::Dirt:
            texCoord = glm::vec2(2, 0) * texSize;
            break;
        case BlockType::Stone:
            texCoord = glm::vec2(1, 0) * texSize;
            break;
        default:
            texCoord = glm::vec2(0, 0) * texSize;
            break;
    }
    
    return texCoord;
}

// Render the chunk
void Chunk::render() {
    if (!meshBuilt) {
        return;
    }
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Update mesh if dirty
void Chunk::update() {
    if (meshDirty) {
        buildMesh();
    }
}

// Clean up OpenGL resources
void Chunk::cleanup() {
    if (meshBuilt) {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
        meshBuilt = false;
    }
}

// Get chunk world position
glm::vec3 Chunk::getWorldPosition() const {
    return glm::vec3(chunkX * CHUNK_SIZE, 0, chunkZ * CHUNK_SIZE);
}

// Check if chunk is loaded
bool Chunk::isLoaded() const {
    return meshBuilt;
}

// Mark mesh as dirty (needs rebuilding)
void Chunk::markDirty() {
    meshDirty = true;
}
