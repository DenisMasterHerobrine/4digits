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
#include "stdlib.h"
#include "regex"
#include "iostream"
#include "algorithm"

#include "Random.h"

/*
*	Random.cpp | Password Generator's Random API | Made by DMHDev | v1.0.0
*	Memory and Time Salting Randomness algorithm. 
*	Generates a random seed based on corrupted memory blocks and time.
*	In most cases the most easiest and fastest way to randomize the output no matter what circumstances are happening, requires small amount of memory.
*/

unsigned int getSeed()
{
	// A Pseudo-Random based on Memory and Time Salting Method

	// Создаём пустую и абстрактную ссылку на массив-деструктор типа int.
	int* abstractMemorySalt = new int{};
	std::string currentTimeSalt = std::to_string(time(0)); // Получаем текущее время в UNIX формате.
	std::string abstractMemorySaltString{}; // Создаём строку для получения недопустимых данных.

	srand(time(0)); // Задаём сид в виде времени.

	for (int i = 1; i < rand() % 5; i++) {
		// Несмотря на предупреждения компилятора о том, что так делать не стоит, но мы же смелые настолько, что настало время улететь в случайную ячейку памяти и забрать оттуда данные.
		// Собираем случайные значения до того, как итератор перегруженного "рандома" превысит значение rand() % 5 (т.е. делаем 1-3 несанкционированных вызовов из стеков памяти)
		abstractMemorySaltString += std::to_string(abstractMemorySalt[i]); // Ignore the warning, we need total randomness!
	}

	// Если мы в конце перегрузки получили отрицательные числа - заменяем их на произвольную цифру. 
	// В более простой вариации их можно вообще или убрать, или перегрузить ещё одной ячейкой памяти, если брать более "серьёзный" алгоритм.
	std::replace(abstractMemorySaltString.begin(), abstractMemorySaltString.end(), '-', '5');

	// Делаем преобразования сида в нечто простое, чтобы мы могли это использовать. Берём Integer значение сида -> берём произвольные 9 последовательных элементов из несанкционированных значений.
	std::string seed = currentTimeSalt + abstractMemorySaltString;
	std::string prepSeed{};
	int intSeed{};
	for (int j = 0; j < 9; j++) {
		prepSeed += seed[rand() % seed.size()];
	}
	intSeed = std::stoi(prepSeed);

	// Превращаем наше значение в беззнаковое представление сида для использования в srand() или иных функциях рандома.
	unsigned int saltedRandom = intSeed;

	// Чистим память там, где намусорили.
	delete abstractMemorySalt;
	seed = "";
	prepSeed = seed;

	// Возвращаем произвольный сид, который повторится только в случае ограничения элементов (в нашем случае это 8-значный сид, т.е. 99 999 999 вариаций кодов может придумать природа компьютера) или нарушении "Теории Всего",
	// когда все существующие компоненты компьютера и данные вплоть до состояний атома будут одинаковы. 
	// (Чтобы повторить последовательность в оригинальном алгоритме - необходимо,
	// чтобы оба объекта были сгенерированы в один и тот же момент времени, 
	// сохраняя одно и то же состояние всех компонентов и природных факторов "железной" части компьютера, 
	// что сводит повторение к бесконечно минимальному значению, НО не равному 0).
	return saltedRandom;
}
