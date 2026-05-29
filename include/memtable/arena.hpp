#include "./node.hpp"
#include "./allocator.hpp"
#include <vector>

namespace jstore {
/**
 * @brief A block bump allocator for memory-resident structures
 */
class Arena {
	using block = std::byte[];
	using block_ptr = std::unique_ptr<block>;

public:
	static const size_t kMinBlockSize;
	static const size_t kMaxBlockSize;
	Arena(size_t block_size);

	Arena(const Arena &) = delete;
	Arena &operator=(const Arena &) = delete;

	/**
	 * @brief Simply moves the block offset to the right.
		*
		* If the next free space is outside of the block, allocate a new block
		* and set the offset to 0
		*
		* @param move bytes to move
	 */
	void bump(size_t move);

	/**
	 * @brief Retrieves next free space in active block
		*
		* @returns A non-owning ptr to the next free space in the active block
	 */
	std::byte *data();
private:
	std::vector<block_ptr> blocks_;
	size_t offset_;
	size_t block_size_;

	void allocate(size_t size);
	size_t optimize_block_size(size_t block_size);
};
} // jstore
