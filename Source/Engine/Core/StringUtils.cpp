#include "StringUtils.h"

namespace nc {
	std::string ToUpper(std::string lower) {
		std::string upper;
		for (int i = 0; i < lower.length(); i++) {
			// goes through each letter and adds the upper case version of it to a new variable
			upper += toupper(lower[i]);
		}
		return upper;
	}

	std::string ToLower(std::string upper) {
		std::string lower;
		for (int i = 0; i < upper.length(); i++) {
			// goes through each letter and adds the lower case version of it to a new variable
			lower += tolower(upper[i]);
		}
		return lower;
	}

	bool IsEqualIgnoreCase(std::string firstS, std::string secondS) {
		if (firstS == secondS) return true;
		if (firstS.length() != secondS.length()) return false;
		for (int i = 0; i < firstS.length(); i++) {
			// first checks if both are the same, then checks for the same length, then checks if each letter is the same assuming both are lowercase
			if (tolower(firstS[i]) != tolower(secondS[i])) return false;
		}
		return true;
	}

	std::string CreateUnique(const std::string& str) {
		static uint32_t unique = 0;

		return str + std::to_string(unique++);
	}

}