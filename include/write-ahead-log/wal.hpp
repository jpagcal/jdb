#include <string>
#include <vector>

namespace jdb {
class WriteAheadLog {
public:
	// we want to assume we already have a given file open, which is why I want the WAL to take in 
	// some path to some file, but what if it isnt?
	// we can let the parent program specify the path (it needs to be a valid path) and then pass it in as a parameter
	// invalid paths throw a runtime error; if there is no file at that path then the constructor creates one 
	// a WALs job is just to append instructions to a log and to replay instructions. 
	// instructions are in the form of sometihng along the lines of 'INST key[.val]'
	// qs:
	// 	how should we handle malformed logs?
	// 	failure during replay?
	void append(std::string inst) const;
	void replay(std::vector<std::string> insts);
	
private:
	int fd_;
	// needs a parser
};
} //jdb
