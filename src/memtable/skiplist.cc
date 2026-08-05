#include "../../include/memtable/skiplist.hpp"
#include <cstddef>
#include <stdexcept>

namespace jdb {
SkipListNode::SkipListNode(std::string key, std::string val, size_t level) :
	key_{ std::move(key) },
	val_{ std::move(val) },
	links_{ std::vector<node_ptr>{ level, nullptr} }
{}

void SkipListNode::set_link(size_t level, SkipListNode *next) {
	links_[level] = next;
}

void SkipListNode::set_value(std::string value) {
	if (value == "\0") {
		val_.reset();
		return;
	}
	
	val_.emplace(std::move(value));
}

SkipListNode *SkipListNode::next(size_t level) {
	return links_[level];
}

std::string SkipListNode::key() const {
	return key_;
}

std::string SkipListNode::value() const {
	if (!val_.has_value()) {
		return "TOMBSTONE";
	}

	std::string node_value{ val_.value() };
	return node_value;
	
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
		throw std::runtime_error("Failed to find " + key);
	}
}


void SkipList::upsert(std::string key, std::string value) {
	std::vector<SkipListNode *> update{ max_level_ };
	
	// get ref to head
	SkipListNode *cur{ &head_ };

	// iterate to the predecessors
	for (size_t level{ max_level_ - 1 }; level >= 0; level-- ) {
		SkipListNode *next{ cur->next(level) };

		while (next->key() < key && next != nullptr) {
			cur = next;
		}
		update[level] = cur;
	}

	if (cur->key() == key) {
		cur->set_value(value);
	} else {
		size_t level{ rand_level() };

		// arena-related functionality.
		// alignment of SkipListNode assumes that this arena is only populated by SKipListNodes or similar alignment
		SkipListNode *new_node{ new(arena_->data()) SkipListNode(key, value, level) };
		arena_->bump(sizeof(SkipListNode));

		// from the current node's top level, set the predecessor's next node
		for (size_t cur_level{ level - 1 }; level >= 0; level--) {
			new_node->set_link(cur_level, update[cur_level]->next(cur_level));
			update[cur_level]->set_link(cur_level, new_node);
		}
	}
}

void SkipList::tombstone(std::string key) {
	SkipList::upsert(key, "\0");
}

size_t SkipList::rand_level() {
	size_t level{ 1 };

	while (dist_(rng_) < p_value_ && level < max_level_) level++;

	return level;
}
} // jdb
