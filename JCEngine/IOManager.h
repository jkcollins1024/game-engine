#pragma once

#include <vector>
#include <string>

namespace JCEngine {
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string path, std::vector<unsigned char>& buffer);
		static bool readFileToString(std::string path, std::string& buffer);
	};
}


