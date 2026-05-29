#include <cstddef>
#include <memory>


namespace jstore {
/**
 * General purpose allocator inspired by RocksDB's allocator
 * defines abstract method allocate for unit-aligned allocations
 */
class Allocator {
public:
	virtual ~Allocator() {}
private:
	virtual void allocate(size_t bytes);
};
} //jstore
