#pragma once
#include <string>
#include <vector>

// Generates a hidden code using "Memory and Time Salting" (MaTS) algorithm.
const char* generateHiddenCode();

char* isUniqueHiddenCode(const char*);

char* isValidCode(const char*);

std::string turnCodesDecorator(std::vector<std::string>);