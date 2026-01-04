#ifndef SRC_WORLD_BLOCK_H_
#define SRC_WORLD_BLOCK_H_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace cppcraft {
namespace world {

/**
 * @enum BlockType
 * @brief Enumeration of all block types available in the game
 */
enum class BlockType : uint16_t {
    // Special blocks
    AIR = 0,
    BEDROCK = 1,

    // Stone variants
    STONE = 2,
    COBBLESTONE = 3,
    STONE_BRICKS = 4,
    MOSSY_STONE_BRICKS = 5,
    CRACKED_STONE_BRICKS = 6,
    CHISELED_STONE_BRICKS = 7,
    POLISHED_ANDESITE = 8,
    POLISHED_DIORITE = 9,
    POLISHED_GRANITE = 10,

    // Dirt and grass
    DIRT = 11,
    GRASS_BLOCK = 12,
    GRASS = 13,
    TALL_GRASS = 14,

    // Wood and leaves
    OAK_LOG = 15,
    BIRCH_LOG = 16,
    SPRUCE_LOG = 17,
    JUNGLE_LOG = 18,
    ACACIA_LOG = 19,
    DARK_OAK_LOG = 20,
    OAK_LEAVES = 21,
    BIRCH_LEAVES = 22,
    SPRUCE_LEAVES = 23,
    JUNGLE_LEAVES = 24,
    ACACIA_LEAVES = 25,
    DARK_OAK_LEAVES = 26,
    OAK_PLANKS = 27,
    BIRCH_PLANKS = 28,
    SPRUCE_PLANKS = 29,
    JUNGLE_PLANKS = 30,
    ACACIA_PLANKS = 31,
    DARK_OAK_PLANKS = 32,

    // Ores
    COAL_ORE = 33,
    IRON_ORE = 34,
    GOLD_ORE = 35,
    DIAMOND_ORE = 36,
    EMERALD_ORE = 37,
    LAPIS_ORE = 38,
    REDSTONE_ORE = 39,

    // Sand and gravel
    SAND = 40,
    RED_SAND = 41,
    GRAVEL = 42,

    // Water and lava
    WATER = 43,
    LAVA = 44,

    // Decorative blocks
    SNOW = 45,
    ICE = 46,
    PACKED_ICE = 47,
    OBSIDIAN = 48,
    BOOKSHELF = 49,
    MOSS_BLOCK = 50,
    CLAY = 51,
    SAND_STONE = 52,

    // Manufactured blocks
    BRICKS = 53,
    CRAFTING_TABLE = 54,
    FURNACE = 55,
    CHEST = 56,
    LADDER = 57,
    FENCE = 58,
    FENCE_GATE = 59,
    DOOR = 60,
    PRESSURE_PLATE = 61,
    BUTTON = 62,
    LEVER = 63,

    // Unknown/Invalid
    UNKNOWN = 65535
};

/**
 * @struct Block
 * @brief Represents a single block in the world
 * 
 * A block is the basic building unit of the world. Each block has a type,
 * and can have additional properties like metadata (rotation, orientation, etc).
 */
struct Block {
    /**
     * @var type
     * @brief The type of this block
     */
    BlockType type;

    /**
     * @var metadata
     * @brief Additional data about the block (rotation, orientation, etc.)
     * 
     * The interpretation of metadata depends on the block type:
     * - For logs: rotation (0-2)
     * - For stairs: direction and half (upper/lower)
     * - For doors: hinge side, orientation, etc.
     * - For redstone: powered state, direction, etc.
     */
    uint8_t metadata;

    /**
     * @var light_level
     * @brief The light level of this block (0-15)
     * 
     * 0 = no light, 15 = full brightness
     */
    uint8_t light_level;

    /**
     * @var is_solid
     * @brief Whether this block is solid and can be walked on
     */
    bool is_solid;

    /**
     * @brief Default constructor
     * Creates an air block with no metadata
     */
    Block();

    /**
     * @brief Constructor with type
     * @param block_type The type of block to create
     */
    explicit Block(BlockType block_type);

    /**
     * @brief Constructor with type and metadata
     * @param block_type The type of block to create
     * @param block_metadata Additional metadata for the block
     */
    Block(BlockType block_type, uint8_t block_metadata);

    /**
     * @brief Check if this block is air
     * @return true if this block is air, false otherwise
     */
    bool is_air() const;

    /**
     * @brief Check if this block is liquid (water or lava)
     * @return true if this block is a liquid, false otherwise
     */
    bool is_liquid() const;

    /**
     * @brief Check if this block is transparent
     * @return true if light can pass through this block, false otherwise
     */
    bool is_transparent() const;

    /**
     * @brief Get the display name of this block type
     * @return A string representation of the block type
     */
    std::string get_name() const;

    /**
     * @brief Check equality of two blocks
     * @param other The block to compare with
     * @return true if both blocks have the same type and metadata
     */
    bool operator==(const Block& other) const;

    /**
     * @brief Check inequality of two blocks
     * @param other The block to compare with
     * @return true if blocks differ in type or metadata
     */
    bool operator!=(const Block& other) const;
};

/**
 * @class BlockRegistry
 * @brief Registry for block types and their properties
 */
class BlockRegistry {
public:
    /**
     * @brief Get the singleton instance of BlockRegistry
     * @return Reference to the BlockRegistry instance
     */
    static BlockRegistry& get_instance();

    /**
     * @brief Check if a block type is solid
     * @param type The block type to check
     * @return true if the block is solid, false otherwise
     */
    bool is_solid(BlockType type) const;

    /**
     * @brief Check if a block type is liquid
     * @param type The block type to check
     * @return true if the block is a liquid, false otherwise
     */
    bool is_liquid(BlockType type) const;

    /**
     * @brief Check if a block type is transparent
     * @param type The block type to check
     * @return true if light can pass through this block type
     */
    bool is_transparent(BlockType type) const;

    /**
     * @brief Get the display name for a block type
     * @param type The block type
     * @return A string representation of the block type
     */
    std::string get_block_name(BlockType type) const;

private:
    BlockRegistry();
    ~BlockRegistry() = default;

    // Prevent copying
    BlockRegistry(const BlockRegistry&) = delete;
    BlockRegistry& operator=(const BlockRegistry&) = delete;

    std::unordered_map<uint16_t, std::string> block_names_;
    std::unordered_map<uint16_t, bool> solid_blocks_;
    std::unordered_map<uint16_t, bool> liquid_blocks_;
    std::unordered_map<uint16_t, bool> transparent_blocks_;
};

}  // namespace world
}  // namespace cppcraft

#endif  // SRC_WORLD_BLOCK_H_
