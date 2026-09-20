#include <memory>
#include <random>
#include <vector>
#include "./arena.hpp"

namespace jdb {
/**
 * @brief A unit within the skip list; holds links to multiple nodes
 */
class SkipListNode {
	using node_ptr = SkipListNode *;
public:
	SkipListNode(std::string key, std::string val, size_t level);

	/**
	 * @brief sets the ptr at a given level to a SkipListNode ptr
	 */
	void set_link(size_t level, SkipListNode *next);

	/**
	 * @brief sets the value the node
	 */
	void set_value(std::string value);

	/**
	 * @brief gets the next node at a given level
	 */
	SkipListNode *next(size_t level);

	/**
	 * @brief key getter
	 */
	std::string key() const;

	/**
	 * @brief val getter
	 */
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

	void upsert(std::string key, std::string value);
	void tombstone(std::string key);
	void invalidate(std::string key);
	std::string search(std::string key);
	std::vector<SkipListNode *> predecessors(std::string key);
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
