#pragma once

#include <windows.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
#include <shellapi.h>

#include "Utilities.h"
#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>


// Plays a "Four Digits" Main Menu Theme - "Dread Factory" by MFG38, looped and working asynchronously. 
// Called without any parameters, so just call it and it will start playing until you call Utilties#stopMusic(). 
// Проигрывает главную тему игры - "Dread Factory" от MFG38 в асинхронном потоке и зацикливается до тех пор, пока не будет вызван метод Utilties#stopMusic(). 
// Вызывается без всяких параметров.
void playMainMenuMusic()
{
    PlaySound("MAIN_MUSIC", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

// Stops any music is currently playing asynchronously in the game. 
// Called without any parameters, so just call it and it will stop playing any music resource. 
// Проигрывает главную тему игры - "Dread Factory" от MFG38 в асинхронном потоке и зацикливается до тех пор, пока не будет вызван метод Utilties#stopMusic(). 
// Вызывается без всяких параметров.
void stopMusic()
{
    PlaySound(NULL, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

// Sets the volume based on volume parameter.
// Выставляет громкость приложения в зависимости от параметра volume. 
void setVolume(int volume) {
    switch (volume) {
    case 0: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x0000, 0x0000)); break;
    }
    case 6: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x1111, 0x1111)); break;
    }
    case 12: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x2222, 0x2222)); break;
    }
    case 18: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x3333, 0x3333)); break;
    }
    case 24: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x4444, 0x4444)); break;
    }
    case 30: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x5555, 0x5555)); break;
    }
    case 36: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x6666, 0x6666)); break;
    }
    case 42: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x7777, 0x7777)); break;
    }
    case 48: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x8888, 0x8888)); break;
    }
    case 54: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0x9999, 0x9999)); break;
    }
    case 60: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0xAAAA, 0xAAAA)); break;
    }
    case 66: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0xBBBB, 0xBBBB)); break;
    }
    case 72: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0xCCCC, 0xCCCC)); break;
    }
    case 78: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0xDDDD, 0xDDDD)); break;
    }
    case 84: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0xEEEE, 0xEEEE)); break;
    }
    case 90: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0xFFFF, 0xFFFF)); break;
    }
    case 96: {
        waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, MAKELONG(0xFFFF, 0xFFFF)); break;
    }
    }
}

// Sets a current window name. 
// Выставляет название окну консоли.
// Calls with a constant string or const char* array where the name of the window is placed. 
// Вызывается с параметров типа string или const char*, который и задаётся в качестве имени окна.
// 
// Usage: 
// setWindowName("Test");
// OR
// const std::string& name = "Four Digits";
// setWindowName(name);
// 
void setWindowName(const std::string& name) {
    SetConsoleTitleA(name.c_str());
}

// Opens an URI link in a user's preffered browser or Microsoft Edge if no preffered browser is selected in user's environment. 
// Открывает ссылку в предпочтённом пользователем браузере или Microsoft Edge по умолчанию.
// Calls with a constant string or const char* array where the link is placed. 
// Вызывается с параметром типа string или const char*, где находится сама ссылка на страницу.
// 
// Usage: 
// executeLink("https://google.com");
// OR
// const std::string& name = "https://google.com";
// executeLink(name);
void executeLink(const std::string& link) {
    ShellExecute(0, 0, link.c_str(), 0, 0, SW_SHOW);
}

// Sets an English (US) default keyboard layout if the system is Windows, since we cannot support multiple languages for menu navigation in the game window. 
// Called without any parameters, so just call it using asynchronous callbacks or any other multithreading way and you're ready to go. 
// Выставляет пользователю принудительно раскладку клавиатуры English (US) внутри приложения.
// Вызывается без параметров. Необходимо вызывать в отдельном асинхронном потоке.
void setEnglishLocale() {
    // WARNING: YOU NEED AN ANOTHER THREAD APPROACH FOR THIS. IF YOU CALL IT IN MAIN THREAD, YOU'LL GET STUCK.
    SendMessageW(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, reinterpret_cast<LPARAM>(LoadKeyboardLayoutA("00000409", KLF_ACTIVATE | KLF_SETFORPROCESS)));
}

// Checks if an array contains a specified value. Alternative to std::find (which is preffered), since std::find returns last element, while it is not expected in game code.
// Проверяет, находится ли значение value в векторе array.
bool contains(const std::string& value, const std::vector<std::string>& array)
{
    for (int i = 0; i < array.size(); i++) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

// Checks if an array contains a specified value. Alternative to std::find (which is preffered), since std::find returns last element, while it is not expected in game code.
// Проверяет, находится ли значение value в векторе array.
bool contains(const char& value, const std::vector<char>& array)
{
    for (int i = 0; i < array.size(); i++) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

// Checks if an array contains a specified value. Alternative to std::find (which is preffered), since std::find returns last element, while it is not expected in game code.
// Проверяет, находится ли значение value в векторе array.
bool contains(const char& value, const std::string& array)
{
    for (int i = 0; i < array.size(); i++) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}