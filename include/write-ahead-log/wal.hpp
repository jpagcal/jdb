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
	void to_beginning(std::fstream &fs);
	INST to_instruction(const std::string &inst);
	ParsedInstruction parse_instruction(const std::string &line);
	std::vector<std::string> get_lines(std::fstream &fs);
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
