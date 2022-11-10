#include "4DigitsAPI.h" // for initial functions prototypes.

#include "Random.h" // for getSeed() method from Random API.

#include "string" // for char* and strcpy()/std::to_string conversion
#include <algorithm>
#include <set>
#include <time.h>
#include <vector>
#include <unordered_set>

// Generates a hidden code using "Memory and Time Salting" (MaTS) algorithm.
const char* generateHiddenCode() {
	srand(time(0));
	int code = 1000 + rand() % 10000;
	char* arrayCode = new char{};
	strcpy(arrayCode, std::to_string(code).c_str());
	return arrayCode;
	delete[] arrayCode;
}

// Checks if player's hidden code is unique and all numbers in the code are not the same.
char* isUniqueHiddenCode(const char* code) {
	size_t length = strlen(code);
	char* errorCode = "INITIALIZED_SUCCESSFULLY";

	if (length != 4) {
		errorCode = "NOT_VALID_CODE_LENGTH"; // Not a valid code length, entered less or more symbols by user.
		return errorCode;
	}

	int numberCounter{};
	for (int cn = 0; cn < 4; cn++) {
		if (int(code[cn]) >= '0' && int(code[cn] <= '9')) numberCounter++;
	}

	if (numberCounter != 4) {
		errorCode = "NOT_VALID_CODE"; // Not a valid code, but 4 letters.
		return errorCode;
	}

	std::vector<char> data(code, code + 4);
	// Put all array elements in a map
	int n = data.size();

	std::unordered_set<int> s;
	for (int i = 0; i < n; i++) {
		s.insert(data[i]);
	}

	// If all elements are distinct, size of
	// set should be same array.
	if (s.size() == data.size()) {
		errorCode = ""; // Valid code, continue!
		return errorCode;
	}
	else {
		errorCode = "NOT_UNIQUE_NUMBERS_IN_CODE"; // Not a valid code, but our numbers are not unique and distinct.
		return errorCode;
	}

	return "SKIPPED_CHECKS";
	//set.erase(set.begin(), set.end()); // Delete the set to save memory.
}