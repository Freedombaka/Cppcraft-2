#ifndef SRC_WORLD_CHUNK_H_
#define SRC_WORLD_CHUNK_H_

#include <array>
#include <cstdint>
#include <vector>

namespace cppcraft {
namespace world {

// Chunk dimensions
constexpr int CHUNK_SIZE_X = 16;
constexpr int CHUNK_SIZE_Y = 256;  // Height
constexpr int CHUNK_SIZE_Z = 16;
constexpr int CHUNK_VOLUME = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

/**
 * @brief Represents a 16x256x16 chunk of blocks in the world
 * 
 * The Chunk class manages all blocks within a chunk section.
 * Chunks are the fundamental building blocks of the world structure,
 * containing 16x256x16 (65536) block positions.
 */
class Chunk {
 public:
  /**
   * @brief Construct a new Chunk object
   * @param chunk_x X coordinate of the chunk in chunk space
   * @param chunk_z Z coordinate of the chunk in chunk space
   */
  Chunk(int chunk_x, int chunk_z);

  /**
   * @brief Destroy the Chunk object
   */
  ~Chunk() = default;

  /**
   * @brief Get the block ID at the specified local coordinates
   * @param x Local X coordinate (0-15)
   * @param y Local Y coordinate (0-255)
   * @param z Local Z coordinate (0-15)
   * @return The block ID at the position
   */
  uint16_t GetBlock(int x, int y, int z) const;

  /**
   * @brief Set the block ID at the specified local coordinates
   * @param x Local X coordinate (0-15)
   * @param y Local Y coordinate (0-255)
   * @param z Local Z coordinate (0-15)
   * @param block_id The block ID to set
   */
  void SetBlock(int x, int y, int z, uint16_t block_id);

  /**
   * @brief Get the chunk's X coordinate in chunk space
   * @return Chunk X coordinate
   */
  int GetChunkX() const { return chunk_x_; }

  /**
   * @brief Get the chunk's Z coordinate in chunk space
   * @return Chunk Z coordinate
   */
  int GetChunkZ() const { return chunk_z_; }

  /**
   * @brief Check if the chunk has been modified since last save
   * @return True if the chunk is dirty, false otherwise
   */
  bool IsDirty() const { return is_dirty_; }

  /**
   * @brief Mark the chunk as clean (not modified)
   */
  void MarkClean() { is_dirty_ = false; }

  /**
   * @brief Mark the chunk as dirty (modified)
   */
  void MarkDirty() { is_dirty_ = true; }

  /**
   * @brief Fill the entire chunk with a single block type
   * @param block_id The block ID to fill with
   */
  void Fill(uint16_t block_id);

  /**
   * @brief Get the total number of blocks in this chunk
   * @return Number of blocks (always CHUNK_VOLUME)
   */
  static constexpr int GetVolume() { return CHUNK_VOLUME; }

  /**
   * @brief Calculate the linear index for a block position
   * @param x Local X coordinate (0-15)
   * @param y Local Y coordinate (0-255)
   * @param z Local Z coordinate (0-15)
   * @return Linear index into the block array
   */
  static constexpr int GetIndex(int x, int y, int z) {
    return y * (CHUNK_SIZE_X * CHUNK_SIZE_Z) + z * CHUNK_SIZE_X + x;
  }

  /**
   * @brief Check if the given local coordinates are within chunk bounds
   * @param x Local X coordinate
   * @param y Local Y coordinate
   * @param z Local Z coordinate
   * @return True if coordinates are within bounds
   */
  static bool IsInBounds(int x, int y, int z) {
    return x >= 0 && x < CHUNK_SIZE_X &&
           y >= 0 && y < CHUNK_SIZE_Y &&
           z >= 0 && z < CHUNK_SIZE_Z;
  }

  /**
   * @brief Get a reference to the raw block data
   * @return Reference to the block data array
   */
  std::array<uint16_t, CHUNK_VOLUME>& GetBlockData() { return blocks_; }

  /**
   * @brief Get a const reference to the raw block data
   * @return Const reference to the block data array
   */
  const std::array<uint16_t, CHUNK_VOLUME>& GetBlockData() const {
    return blocks_;
  }

 private:
  // Chunk coordinates in chunk space
  int chunk_x_;
  int chunk_z_;

  // Block storage (16x256x16 = 65536 blocks)
  // Indexed as: blocks_[y * (16 * 16) + z * 16 + x]
  std::array<uint16_t, CHUNK_VOLUME> blocks_;

  // Flag indicating if this chunk has been modified
  bool is_dirty_;
};

}  // namespace world
}  // namespace cppcraft

#endif  // SRC_WORLD_CHUNK_H_
