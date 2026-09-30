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

void WriteAheadLogParser::to_beginning(std::istream &is) {
	is.clear();
	is.seekg(0);
}

std::vector<std::string> WriteAheadLogParser::get_lines(std::istream &is) {
	to_beginning(is);

	std::vector<std::string> lines{};

	while (is.eof()) {
		char *c_line;
		is.getline(c_line, kMaxBytes);
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

WriteAheadLog::WriteAheadLog(std::string filename) {
	fs_.open(
		filename, 
		std::ios_base::in | std::ios_base::out | std::ios_base::ate
	);

	if (fs_.fail()) {
		throw std::runtime_error("File open error: failbit was set following fs.open()");
	}
}

WriteAheadLog::~WriteAheadLog() {
	fs_.close();
}

void WriteAheadLog::append(std::string &&line_to_append) {
	fs_.seekp(0, fs_.end);

	fs_ << line_to_append << '\n';
}

std::vector<ParsedInstruction> WriteAheadLog::instructions() {
	std::vector<ParsedInstruction> parsed_instructions{};
	
	std::vector<std::string> lines{ WriteAheadLogParser::get_lines(fs_) };
	
	for (const std::string &line : lines) {
		parsed_instructions.push_back(
			WriteAheadLogParser::parse_instruction(line)
		);
	}

	return parsed_instructions;
}
} //jdb

