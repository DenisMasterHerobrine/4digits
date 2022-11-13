#pragma once
#include <string>
#include <vector>

void playMainMenuMusic();

void stopMusic();

void setWindowName(const std::string&);

void executeLink(const std::string&);

void setEnglishLocale();

bool contains(const std::string&, const std::vector<std::string>&);

bool contains(const char&, const std::vector<char>&);

bool contains(const char&, const std::string&);