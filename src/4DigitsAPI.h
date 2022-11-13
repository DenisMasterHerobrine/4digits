#pragma once
#include <string>
#include <vector>

// Generates a hidden code using "Memory and Time Salting" (MaTS) algorithm.
std::string generateHiddenCode();

std::string generateErastophenCode(std::vector<char>&);

char* isUniqueHiddenCode(const char*);

char* isValidCode(const char*);

std::string turnCodesDecorator(std::vector<std::string> v, std::string keyCode, std::vector<std::string>& v2);

std::vector<std::string> encodeVector(std::vector<std::string>, std::string, std::string);

void updateErastophenVector(std::string, std::string, std::vector<char>&);

bool uniqueCharacters(std::string);