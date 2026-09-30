#include <fstream>
#include <string>
#include <vector>

namespace jdb {
const size_t kMaxBytes = INT_MAX;

enum class INST {
	ups,
	del,
};

using ParsedInstruction = std::pair<INST, std::pair<std::string, std::string>>;

namespace WriteAheadLogParser {
	void to_beginning(std::istream &is);
	INST to_instruction(const std::string &inst);
	ParsedInstruction parse_instruction(const std::string &line);
	std::vector<std::string> get_lines(std::istream &is);
} // WriteAheadLogParser

class WriteAheadLog {
public:
	WriteAheadLog(std::string filename);
	~WriteAheadLog();

	void append(std::string &&line_to_append);
	std::vector<ParsedInstruction> instructions();
	
private:
	std::fstream fs_;
	// needs a parser
};
} //jdb
