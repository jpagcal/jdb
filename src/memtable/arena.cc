#include "../../include/memtable/arena.hpp"
#include <algorithm>
#include <cassert>
#include <memory>

namespace jstore {
	const size_t Arena::kMinBlockSize{ 4096 };
	const size_t Arena::kMaxBlockSize = 64 * 1024 * 1024;
	const size_t kUnitSize = sizeof(std::byte *);

	Arena::Arena(size_t block_size) :
		block_size_{optimize_block_size(block_size)},
		offset_{ } {
			assert(
				block_size_ > kMinBlockSize &&
				block_size_ < kMaxBlockSize &&
				block_size % kUnitSize == 0
			);

			blocks_.emplace_back(std::make_unique<Arena::block>(block_size_));
			offset_ = 0;
		}

	size_t Arena::optimize_block_size(size_t block_size) {
		block_size = std::max(kMinBlockSize, block_size);
		block_size = std::min(kMaxBlockSize, block_size);

		// round to the nearest lower multiple
		block_size =
			((block_size + kUnitSize - 1) / kUnitSize) * kUnitSize;

		return block_size;
	}

	void Arena::allocate(size_t size) {
		blocks_.emplace_back(std::make_unique<block>(size));
	}

	void Arena::bump(size_t move) {
		if ((offset_ + move) > block_size_) {
			this->allocate(block_size_);
			offset_ = 0;
			return;
		}

		offset_ += move;
	}

	std::byte *Arena::data() {
		size_t active_block{ blocks_.size() - 1 };
		return (this->blocks_)[active_block].get() + offset_;
	}
} // jstore
