#include <fstream>
#include <string>
#include <vector>

namespace jdb {
enum class INST {
	ups,
	del,
};

using ParsedInstruction = std::pair<INST, std::string>;

namespace WriteAheadLogParser {
	static ParsedInstruction parse_instruction(std::string line);
	static std::vector<std::string> get_lines(std::fstream fs);
} // WriteAheadLogParser

class WriteAheadLog {
public:
	WriteAheadLog();
	~WriteAheadLog();

	void append(std::string line_to_append) const;
	std::vector<ParsedInstruction> instructions() const;
	
private:
	std::fstream fs_;
	// needs a parser
};
} //jdb
