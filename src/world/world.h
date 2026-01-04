#ifndef CPPCRAFT_WORLD_H
#define CPPCRAFT_WORLD_H

#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include "chunk.h"

namespace cppcraft {
namespace world {

/**
 * @class World
 * @brief Manages the world and all its chunks
 * 
 * The World class is responsible for:
 * - Loading and unloading chunks
 * - Managing chunk positions and data
 * - Providing access to blocks across chunk boundaries
 * - Updating chunks and their states
 */
class World {
public:
    /**
     * @brief Constructor for World
     */
    World();

    /**
     * @brief Destructor for World
     */
    ~World();

    /**
     * @brief Get or create a chunk at the specified chunk coordinates
     * @param chunkX The X coordinate of the chunk
     * @param chunkZ The Z coordinate of the chunk
     * @return Pointer to the chunk
     */
    Chunk* getChunk(int chunkX, int chunkZ);

    /**
     * @brief Check if a chunk exists at the specified coordinates
     * @param chunkX The X coordinate of the chunk
     * @param chunkZ The Z coordinate of the chunk
     * @return True if chunk exists, false otherwise
     */
    bool hasChunk(int chunkX, int chunkZ) const;

    /**
     * @brief Load a chunk at the specified coordinates
     * @param chunkX The X coordinate of the chunk
     * @param chunkZ The Z coordinate of the chunk
     * @return Pointer to the loaded chunk
     */
    Chunk* loadChunk(int chunkX, int chunkZ);

    /**
     * @brief Unload a chunk at the specified coordinates
     * @param chunkX The X coordinate of the chunk
     * @param chunkZ The Z coordinate of the chunk
     * @return True if chunk was unloaded, false if it didn't exist
     */
    bool unloadChunk(int chunkX, int chunkZ);

    /**
     * @brief Get a block at world coordinates
     * @param x The world X coordinate
     * @param y The world Y coordinate
     * @param z The world Z coordinate
     * @return The block ID at the specified location
     */
    uint16_t getBlock(int x, int y, int z) const;

    /**
     * @brief Set a block at world coordinates
     * @param x The world X coordinate
     * @param y The world Y coordinate
     * @param z The world Z coordinate
     * @param blockID The block ID to set
     */
    void setBlock(int x, int y, int z, uint16_t blockID);

    /**
     * @brief Update all loaded chunks
     * @param deltaTime The time elapsed since last update in seconds
     */
    void update(float deltaTime);

    /**
     * @brief Get the number of loaded chunks
     * @return The count of loaded chunks
     */
    size_t getLoadedChunkCount() const;

    /**
     * @brief Unload all chunks from the world
     */
    void unloadAll();

    /**
     * @brief Convert world coordinates to chunk coordinates
     * @param worldCoord The world coordinate
     * @return The chunk coordinate
     */
    static int worldToChunkCoord(int worldCoord);

    /**
     * @brief Convert world coordinates to local chunk coordinates
     * @param worldCoord The world coordinate
     * @return The local coordinate within a chunk
     */
    static int worldToLocalCoord(int worldCoord);

private:
    // Chunk storage using a hash map for O(1) access
    // Key format: (chunkX << 16) | (chunkZ & 0xFFFF)
    std::unordered_map<long long, std::unique_ptr<Chunk>> chunks;

    /**
     * @brief Generate a key for chunk coordinates
     * @param chunkX The X coordinate of the chunk
     * @param chunkZ The Z coordinate of the chunk
     * @return Hash key for the chunk
     */
    static long long chunkKey(int chunkX, int chunkZ);
};

} // namespace world
} // namespace cppcraft

#endif // CPPCRAFT_WORLD_H
