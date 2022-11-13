#include "4DigitsAPI.h" // for initial functions prototypes.

#include "Random.h" // for getSeed() method from Random API.

#include "string" // for char* and strcpy()/std::to_string conversion
#include <algorithm>
#include <set>
#include <time.h>
#include <vector>
#include <unordered_set>
#include <Utilities.h>

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
}

// Checks if player's hidden code is correct to make a turn.
char* isValidCode(const char* code) {
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

	errorCode = ""; // Return nothing if we're fine.

	return errorCode;
}

// Encodes a code into a "bulls and cows" based code.
std::vector<std::string> encodeVector(std::vector<std::string> v, std::string encryptCode, std::string errorEncryptionCode) {
	std::vector<std::string> encodedVector;
	if (encryptCode.size() != 4 && !v.empty()) {
		encodedVector = v;
		return encodedVector; // Do not encrypt, since this method supports only 4-bit encryption.
	}

	int bulls{};
	int cows{};

	std::string encrypted{};

	// Get hidden code keys;
	char firstKey = encryptCode[0];
	char secondKey = encryptCode[1];
	char thirdKey = encryptCode[2];
	char fourthKey = encryptCode[3];

	for (int codes = 0; codes < v.size(); codes++) {
		// Check first character:
		if (v[codes].find(firstKey) != std::string::npos) ++cows;
		if (v[codes][0] == firstKey) { ++bulls; --cows; }

		// Check second character:
		if (v[codes].find(secondKey) != std::string::npos) ++cows;
		if (v[codes][1] == secondKey) { ++bulls; --cows; }

		// Check third character:
		if (v[codes].find(thirdKey) != std::string::npos) ++cows;
		if (v[codes][2] == thirdKey) { ++bulls; --cows; }

		// Check fourth character:
		if (v[codes].find(fourthKey) != std::string::npos) ++cows;
		if (v[codes][3] == fourthKey) { ++bulls; --cows; }

		encrypted = std::to_string(bulls) + "B" + std::to_string(cows) + "C";

		encodedVector.push_back(encrypted);
		encrypted = "";
		cows = 0; bulls = 0;
	}

	return encodedVector;
}

// Decorates a vector<string> codes array into something cool.
// Turns an array ["1234", "4321"] into stylized array:
// Turn 1: 1234; 
// Turn 2: 4321.
std::string turnCodesDecorator(std::vector<std::string> v, std::string keyCode, std::vector<std::string>& v2) {
	int n = v.size();
	std::string errorCode{};
	v = encodeVector(v, keyCode, errorCode);
	v2 = v;

	std::string res{};
	for (int i = 0; i < n; i++) {
		if (!(i == n - 1)) res += "Turn " + std::to_string(i + 1) + ": " + v[i] + "; \n";
		else {
			res += "Turn " + std::to_string(i + 1) + ": " + v[i] + ".";
		}
	}

	return res;
}

bool updateErastophenVector(std::string key, std::string code, std::vector<char>& erastophenVector) {
	if (key.size() == 4 && code.size() == 4) {
		// Get hidden code keys;
		char firstKey = code[0];
		char secondKey = code[1];
		char thirdKey = code[2];
		char fourthKey = code[3];

		if (!contains(firstKey, key) && !contains(firstKey, erastophenVector)) erastophenVector.push_back(firstKey);
		if (!contains(secondKey, key) && !contains(secondKey, erastophenVector)) erastophenVector.push_back(secondKey);
		if (!contains(thirdKey, key) && !contains(thirdKey, erastophenVector)) erastophenVector.push_back(thirdKey);
		if (!contains(fourthKey, key) && !contains(fourthKey, erastophenVector)) erastophenVector.push_back(fourthKey);
		
		return true;
	}
	else return false;
}

// Checks if in generated code all chars are unique (do not misuse with unordered_set's unification)
bool uniqueCharacters(std::string str)
{
	// If at any time we encounter 2
	// same characters, return false
	for (int i = 0; i < str.length() - 1; i++) {
		for (int j = i + 1; j < str.length(); j++) {
			if (str[i] == str[j]) {
				return false;
			}
		}
	}

	// If no duplicate characters encountered,
	// return true
	return true;
}

// Generates a hidden code using "Memory and Time Salting" (MaTS) algorithm.
const char* generateHiddenCode() {
	srand(static_cast<int>(getSeed())); // time(0) causes here a huge memory leak, wtf.
	char* arrayCode = new char{};
	bool unique = false;

	do {
		int code = 1000 + rand() % 10000;
		strcpy(arrayCode, std::to_string(code).c_str());
		unique = uniqueCharacters(std::to_string(code));
	} while (!unique);

	return arrayCode;
	delete arrayCode;
}