#pragma once
#include "Random.cpp"

/*
*	Random.cpp | Password Generator's Random API | Made by DMHDev | v1.0.0
*	Memory and Time Salting Randomness algorithm.
*	Generates a random seed based on corrupted memory blocks and time.
*	In most cases the most easiest and fastest way to randomize the output no matter what circumstances are happening, requires small amount of memory
*/

unsigned int getSeed(); // Gets a random seed for future generation tasks.