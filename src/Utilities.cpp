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
void playMainMenuMusic()
{
    PlaySound("MAIN_MUSIC", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

// Stops any music is currently playing asynchronously in the game. 
// Called without any parameters, so just call it and it will stop playing any music resource. 
void stopMusic()
{
    PlaySound(NULL, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

// Sets a current window name. 
// Calls with a constant string or const char* array where the name of the window is placed. 
// 
// Usage: 
// setWindowName("Test");
// OR
// const std::string& name = "Four Digits";
// setWindowName(name);
void setWindowName(const std::string& name) {
    SetConsoleTitleA(name.c_str());
}

// Opens an URI link in a user's preffered browser or Microsoft Edge if no preffered browser is selected in user's environment. 
// Calls with a constant string or const char* array where the link is placed. 
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
void setEnglishLocale() {
    // WARNING: YOU NEED AN ANOTHER THREAD APPROACH FOR THIS. IF YOU CALL IT IN MAIN THREAD, YOU'LL GET STUCK.
    SendMessageW(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, reinterpret_cast<LPARAM>(LoadKeyboardLayoutA("00000409", KLF_ACTIVATE | KLF_SETFORPROCESS)));
}

// Checks if an array contains a specified value. Alternative to std::find (which is preffered), since std::find returns last element, while it is not expected in game code.
bool contains(const std::string& value, const std::vector<std::string>& array)
{
    for (int i = 0; i < array.size(); ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

// Checks if an array contains a specified value. Alternative to std::find (which is preffered), since std::find returns last element, while it is not expected in game code.
bool contains(const char& value, const std::vector<char>& array)
{
    for (int i = 0; i < array.size(); ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

// Checks if an array contains a specified value. Alternative to std::find (which is preffered), since std::find returns last element, while it is not expected in game code.
bool contains(const char& value, const std::string& array)
{
    for (int i = 0; i < array.size(); ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}