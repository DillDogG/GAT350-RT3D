#pragma once
#include <string>

namespace nc {
	//class StringUtils {
	//public:
		std::string ToUpper(std::string lower);
		std::string ToLower(std::string upper);
		bool IsEqualIgnoreCase(std::string firstS, std::string secondS);
		std::string CreateUnique(const std::string& str);
	//};
}