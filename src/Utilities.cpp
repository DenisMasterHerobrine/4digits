#pragma once

#include <windows.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
#include <shellapi.h>

#include "Utilities.h"
#include <string>
#include <iostream>

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

void setEnglishLocale() {
    SendMessageW(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, reinterpret_cast<LPARAM>(LoadKeyboardLayoutA("00000409", KLF_ACTIVATE | KLF_SETFORPROCESS)));

}