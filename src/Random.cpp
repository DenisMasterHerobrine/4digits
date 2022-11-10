/*
MIT License

Copyright (c) 2022 DenisMasterHerobrine

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define _CRT_SECURE_NO_WARNINGS

#include "chrono"
#include "string"
#include "ctime"
#include "iomanip"
#include "Generator.h"
#include "stdlib.h"
#include "regex"
#include "iostream"
#include "algorithm"

/*
*	Random.cpp | Password Generator's Random API | Made by DMHDev | v1.0.0
*	Memory and Time Salting Randomness algorithm. 
*	Generates a random seed based on corrupted memory blocks and time.
*	In most cases the most easiest and fastest way to randomize the output no matter what circumstances are happening, requires small amount of memory
*/

unsigned int getSeed()
{
	// A Pseudo-Random based on Memory and Time Salting Method

	int* arr[1]{0};

	int* abstractMemorySalt = new int{};
	std::string currentTimeSalt = std::to_string(time(0));
	std::string abstractMemorySaltString{};

	srand(time(0));

	for (int i = 1; i < rand() % 25; i++) {
		abstractMemorySaltString += std::to_string(abstractMemorySalt[i]); // Ignore the warning, we need total randomness!
	}

	std::replace(abstractMemorySaltString.begin(), abstractMemorySaltString.end(), '-', '5');

	std::string seed = currentTimeSalt + abstractMemorySaltString;
	std::string prepSeed{};
	int intSeed{};
	for (int j = 0; j < 9; j++) {
		prepSeed += seed[rand() % seed.size()];
	}
	intSeed = std::stoi(prepSeed);

	unsigned int saltedRandom = intSeed;

	return saltedRandom;
}
