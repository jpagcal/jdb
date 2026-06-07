#include <memory>
#include <random>
#include <vector>
#include "./arena.hpp"

namespace jdb {
class SkipListNode {
	using node_ptr = SkipListNode *;
public:
	SkipListNode(std::string key, std::string val, size_t level);

	void set_link(size_t level, SkipListNode *next);
	SkipListNode *next(size_t level);
	std::string key() const;
	std::string value() const;

private:
	std::vector<node_ptr> links_;
	std::string key_;
	std::string val_;
};

/**
 * @brief Append-only skiplist
 */
class SkipList {
public:
	SkipList(size_t maxLevel, std::unique_ptr<Arena> arena);

	void insert(std::string key, std::string value);
	void invalidate(std::string key);
	std::string search(std::string key);
private:
	size_t const max_level_;
	std::unique_ptr<Arena> const arena_;
 	SkipListNode head_;
   	SkipListNode end_;

   std::mt19937 rng_{ std::random_device{}() };
   std::uniform_real_distribution<double> dist_{ 0.0, 1.0 };
   double p_value_{ 0.5 };

  	SkipListNode *search_prev(std::string key);
   	size_t rand_level();
};
} // jdb
