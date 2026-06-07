#include "../../include/memtable/skiplist.hpp"

namespace jdb {
SkipListNode::SkipListNode(std::string key, std::string val, size_t level) :
	key_{ std::move(key) },
	val_{ std::move(val) },
	links_{ std::vector<node_ptr>{ level, nullptr} }
{}

void SkipListNode::set_link(size_t level, SkipListNode *next) {
	links_[level] = next;
}

SkipListNode *SkipListNode::next(size_t level) {
	return links_[level];
}

std::string SkipListNode::key() const {
	return key_;
}

std::string SkipListNode::value() const {
	return val_;
}

SkipList::SkipList(size_t max_level, std::unique_ptr<Arena> arena) :
	max_level_{ max_level },
	arena_{ std::move(arena) },
	head_{ SkipListNode("-1", "", max_level) },
	end_{ SkipListNode("-1", "", max_level) }
{
	for (size_t level{ max_level - 1 }; level >= 0; --level) {
		head_.set_link(level, &end_);
	}
}

std::string SkipList::search(std::string key) {
	// start at the header
	SkipListNode *cur = &head_;
	// from the top level, to the bottom iterate till 0
	for (size_t level{ max_level_ - 1 }; level >= 0; level--) {
		SkipListNode *next{ cur->next(level) };
		while (next->key() < key && next != nullptr) {
			cur = next;
		}
	}

	cur = cur->next(1);
	
	if (cur->key() == key) {
		return cur->value();
	} else {
		// some failure
	}

	// if cur->key == key return cur->value
	// return some failure condition
}

size_t SkipList::rand_level() {
	size_t level{ 1 };

	while (dist_(rng_) < p_value_ && level < max_level_) level++;

	return level;
}
} // jdb
