#include "IOManager.h"

#include <fstream>

namespace JCEngine {
	bool IOManager::readFileToBuffer(std::string path, std::vector<unsigned char>& buffer) {
		std::ifstream file(path, std::ios::binary);

		if (file.fail()) {
			perror(path.c_str());
			return false;
		}

		//seek to end
		file.seekg(0, std::ios::end);

		//get size of file
		int fileSize = file.tellg();

		//back to beginning for read
		file.seekg(0, std::ios::beg);

		//reduce file size by header bytes
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&buffer[0], fileSize);
		file.close();

		return true;
	}
	bool IOManager::readFileToString(std::string path, std::string& buffer)
	{
		std::ifstream file(path, std::ios::binary);

		if (file.fail()) {
			perror(path.c_str());
			return false;
		}

		//seek to end
		file.seekg(0, std::ios::end);

		//get size of file
		int fileSize = file.tellg();

		//back to beginning for read
		file.seekg(0, std::ios::beg);

		//reduce file size by header bytes
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&buffer[0], fileSize);
		file.close();

		return true;
	}
}
