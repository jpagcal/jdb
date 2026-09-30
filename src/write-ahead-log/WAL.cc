#include "../../include/write-ahead-log/wal.hpp"
#include <sstream>

namespace jdb {
INST WriteAheadLogParser::to_instruction(const std::string &inst) {
	if (!inst.compare("ups")) {
		return INST::ups;
	} else if (!inst.compare("del")) {
		return INST::del;
	}

	throw std::runtime_error("Instruction parse error: check thew instruction to see if it is malformed");
}

void WriteAheadLogParser::to_beginning(std::fstream &fs) {
	fs.clear();
	fs.seekp(0);
}

std::vector<std::string> WriteAheadLogParser::get_lines(std::fstream &fs) {
	to_beginning(fs);

	std::vector<std::string> lines{};

	while (fs.eof()) {
		char *c_line;
		fs.getline(c_line, kMaxBytes);
		lines.push_back(c_line);
	}

	// return the vector
	return lines;
}

ParsedInstruction parse_instruction(const std::string &line) {
	std::vector<std::string> tokens;
	std::stringstream ss{ line };
	std::string tmp;
	
	while (!std::getline(ss, tmp, ' ').eof()) {
		tokens.push_back(tmp);
	}
	
	std::pair<std::string, std::string> key_value{ tokens[1], tokens[2] };
	ParsedInstruction parsed_instruction{ 
		WriteAheadLogParser::to_instruction(tokens[0]),
		key_value
	};

	return parsed_instruction;
}


} //jdb

