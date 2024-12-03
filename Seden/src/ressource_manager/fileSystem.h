#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/logger.h"

namespace fs = std::filesystem;

class FileSystem {
public:
	enum class Type {
		FILE_PATH,
		STRING_FILE
	};

	FileSystem() = default;

	FileSystem(const std::string& str, Type type) {
		switch (type) {
		case Type::FILE_PATH:
			if (fs::exists(str) && fs::is_regular_file(str)) {
				std::ifstream fileStream(str);
				if (fileStream) {
					std::stringstream buffer;
					buffer << fileStream.rdbuf();
					file = buffer.str();
				}
				else {
					DEBUG_MSG("Failed to open file");
				}
			}
			else {
				DEBUG_MSG("File path does not exist or is not a regular file");
			}
			break;

		case Type::STRING_FILE:
			file = str;
			break;

		default:
			DEBUG_MSG("Type not recognized");
			break;
		}
	}

	void replace(const std::string& toReplace, const std::string& newSubstring) {
		size_t pos = file.find(toReplace);
		if (pos != std::string::npos) {
			file.replace(pos, toReplace.length(), newSubstring);
		}
	}

	void addAfter(const std::string& targetLine, const std::string& newLine) {
		size_t pos = file.find(targetLine);
		if (pos != std::string::npos) {
			pos += targetLine.length();

			file.insert(pos, newLine);
		}
	}

	bool find(const std::string& str) {
		if (file.find(str) != std::string::npos) 
			return true;
		return false;
	}

	void add(const std::string& str ) {
		file += str;
	}

	std::string& getFile() {
		return file;
	}

private:
	std::string file;
};