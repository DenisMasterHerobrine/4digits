#pragma once
#include <string>

// Generates a hidden code using "Memory and Time Salting" (MaTS) algorithm.
const char* generateHiddenCode();

char* isUniqueHiddenCode(const char*);