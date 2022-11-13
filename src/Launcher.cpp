#include <iostream>  // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
#include <memory>    // for shared_ptr, __shared_ptr_access
#include <string>    // for to_string, allocator

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for MenuEntryAnimated, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/component_options.hpp"   // for MenuEntryAnimated
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, separator, Element, Decorator, color, text, hbox, size, bold, frame, inverted, vbox, HEIGHT, LESS_THAN, border
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Cyan, Color::Green, Color::Red, Color::Yellow
#include <wtypes.h>

// Four Digits headers.
#include <Utilities.h> // for in-game music interaction, playing/disabling sounds, doing some abstract stuff not related to the game.
#include <4DigitsAPI.h> // for game entrypoint callback and the whole game logic being processed on launcher's state.

using namespace ftxui;

const std::string& name = "Four Digits";

auto screen = ScreenInteractive::Fullscreen();

std::string code{};
std::string computerCode{};

std::string userTurnCode{};
std::string computerTurnCode{};
std::string errorCode{}, errorCodeTurn{};

std::vector<std::string> computerTurnCodes{};
std::vector<std::string> userTurnCodes{};

std::vector<std::string> encryptedComputerTurnCodes{};
std::vector<std::string> encryptedUserTurnCodes{};

Component inputUserCode = Input(&code, "Guess the code for the computer.");
Component inputUserTurnCode = Input(&userTurnCode, "Enter your code to try to guess computer's code.");

auto componentInput = Container::Vertical({
  inputUserCode,
});

auto componentInputTurn = Container::Vertical({
  inputUserTurnCode
});

int selected = 0;
auto menu = Container::Vertical(
    {
        MenuEntry("Start  Game  ") | center,
        MenuEntry("How to Play  ") | center,
        MenuEntry("Options  ") | center,
        MenuEntry("Credits  ") | center,
        MenuEntry("Quit?  ") | center,
    },
    &selected);

// Display together the menu with a border
auto renderer = Renderer(menu, [&] {
    return vbox({
           text("Four Digits (v0.6.0)") | center | color(Color::LightSkyBlue1),
           separator(),
           menu->Render() | frame,
        }) |
        border | bgcolor(Color::MediumPurple4);
    });

// Display together the menu with a border
auto rendererGame = Renderer(componentInput, [&] {
    return vbox({
           text("Four Digits - Starting a new game...") | center | color(Color::LightSkyBlue1),
           separator(),
           hbox(text(" Your Code: "), inputUserCode->Render()) | frame,
           separator(),
           hbox(text(" Debug: "), text(code)) | frame,
           hbox(text(" Debug: ERROR_CODE = "), text(errorCode)) | frame,
        }) |
        border | bgcolor(Color::MediumPurple4);
    });

// Display together the menu with a border
auto rendererGuessing = Renderer(componentInputTurn, [&] {
    return vbox({
           text("Four Digits - Game") | center | color(Color::LightSkyBlue1),
           separator(),
           vbox(text(" Your Turn: "), inputUserTurnCode->Render()) | frame,
           separator(),
           hbox({
           vbox(text(" User's Output: "), paragraph(turnCodesDecorator(userTurnCodes, computerCode, encryptedUserTurnCodes))),
           separator(),
           vbox(text(" Computer's Output: "), paragraph(turnCodesDecorator(computerTurnCodes, code, encryptedComputerTurnCodes))),
           }),
        }) |
        border | bgcolor(Color::MediumPurple4);
    });

auto rendererCredits = Renderer([&] {
    return vbox({
             text("Four Digits (v0.6.0)") | center | color(Color::LightSkyBlue1),
             separator(),
             text("Coded by DenisMasterHerobrine (also known as DMHDev, Denis Kalashnikov). Licensed under MIT License.") | center | color(Color::White),
             text("") | center,
             text("Music: Dread Factory by MFG38 from DOOMWorld's Ultimate MIDI Pack by northivanastan") | center | color(Color::DarkRedBis),
             text("") | center,
             text("Sources: https://github.com/DenisMasterHerobrine/4digits") | center | color(Color::GrayLight),
             text("") | center,
             text("") | center,
             text("") | center,
             text("Press G to go to GitHub page.") | center | color(Color::GrayLight),
             text("Press M to go to music's author page.") | center | color(Color::DarkRedBis),
             text("Press ENTER to leave Credits menu.") | center | color(Color::White),
        }) | border | bgcolor(Color::MediumPurple4);
    });

auto rendererWin = Renderer([&] {
    return vbox({
             text("Four Digits - Victory!") | center | color(Color::LightSkyBlue1),
             separator(),
             text("You win!") | center | color(Color::Green3Bis),
             text("Press ENTER to go back to Main Menu.") | center | color(Color::White),
        }) | border | bgcolor(Color::MediumPurple4);
    });

auto rendererLose = Renderer([&] {
    return vbox({
             text("Four Digits - Loss!") | center | color(Color::LightSkyBlue1),
             separator(),
             text("You lost! Computer solved your code faster than you...") | center | color(Color::DarkRedBis),
             text("Press ENTER to go back to Main Menu.") | center | color(Color::White),
        }) | border | bgcolor(Color::MediumPurple4);
    });

auto componentWin = CatchEvent(rendererWin, [&](Event event) {
    if (event == Event::Character('\n')) {
        screen.ExitLoopClosure()();
        return true;
    }

    return false;
    }
);

auto componentLose = CatchEvent(rendererWin, [&](Event event) {
    if (event == Event::Character('\n')) {
        screen.ExitLoopClosure()();
        return true;
    }

    return false;
    }
);

auto componentGuessing = CatchEvent(rendererGuessing, [&](Event event) {
    if (event == Event::Character('\n')) {
        errorCodeTurn = "BEGIN_CHECK";
        // Check User's code to check if it's valid or not.
        char* arrayCode = new char{};
        strcpy(arrayCode, userTurnCode.c_str());
        char* errorCodeType = isValidCode(arrayCode);
        errorCodeTurn = errorCodeType;

        // If it's valid -> generate a computer's turn and do checks.
        if (errorCodeTurn == "") {
            // Generate a computer's one.
            computerTurnCode = generateHiddenCode();
            char* arrayComputerCode = new char{};
            strcpy(arrayComputerCode, computerTurnCode.c_str());

            userTurnCodes.push_back(userTurnCode);
            turnCodesDecorator(userTurnCodes, computerCode, encryptedUserTurnCodes);
            if (contains("4B0C", encryptedUserTurnCodes)) {
                screen.ExitLoopClosure()();
                screen.Loop(componentWin);
            }
            userTurnCode = "";

            computerTurnCodes.push_back(computerTurnCode);
            turnCodesDecorator(computerTurnCodes, code, encryptedComputerTurnCodes);
            std::vector<std::string> s = encryptedComputerTurnCodes;
            if (contains("4B0C", encryptedComputerTurnCodes)) {
                screen.ExitLoopClosure()();
                screen.Loop(componentLose);
            }
            computerTurnCode = ""; 

        }
        else {
            userTurnCode = "ERROR: " + std::string(errorCodeType);
        }
        return true;
    }

    return false;
    }
);

auto componentGame = CatchEvent(rendererGame, [&](Event event) {
    if (event == Event::Character('\n')) {
        errorCode = "BEGIN_CHECK";
        char* arrayCode = new char{};
        strcpy(arrayCode, code.c_str());
        char* errorCodeType = isUniqueHiddenCode(arrayCode);
        errorCode = errorCodeType;

        if (errorCode == "") {
            if (userTurnCodes.size() > 0) userTurnCodes.clear();
            if (computerTurnCodes.size() > 0) computerTurnCodes.clear();

            computerCode = generateHiddenCode(); // Generate a hidden code that player needs to find out.

            screen.ExitLoopClosure()();
            screen.Loop(componentGuessing);
        }
        return true;
    }

    return false;
    }
);

auto componentCredits = CatchEvent(rendererCredits, [&](Event event) {
    if (event == Event::Character('\n')) {
        screen.ExitLoopClosure()();
        return true;
    }

    if (event == Event::Character('G') || event == Event::Character('g')) {
        const std::string& link = "https://github.com/DenisMasterHerobrine/4digits";
        executeLink(link);
        return true;
    }

    if (event == Event::Character('M') || event == Event::Character('m')) {
        const std::string& link = "https://www.doomworld.com/forum/topic/120788-released-ultimate-midi-pack-a-community-music-replacement-for-the-original-doom/";
        executeLink(link);
        return true;
    }
    return false;
    }
);

auto component = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('\n') && selected == 0) {
        screen.ExitLoopClosure()();
        userTurnCode = "";
        screen.Loop(componentGame);

        return true;
    }

    if (event == Event::Character('\n') && selected == 1) {
        screen.ExitLoopClosure()();
        screen.Loop(componentGame);

        return true;
    }

    if (event == Event::Character('\n') && selected == 3) {
        screen.ExitLoopClosure()();
        screen.Loop(componentCredits);

        return true;
    }

    if (event == Event::Character('\n') && selected == 4) {
        exit(EXIT_SUCCESS);
        return true;
    }
    return false;
    }
);

int main(int argc, const char* argv[]) {
  // Remove alert() dialog box on Windows, since we don't need it whatsoever. If something really bad happens, create a .log file and write the error.
  _set_abort_behavior(0, _WRITE_ABORT_MSG); 

  // Setting command line box name here, it's "Four Digits".
  setWindowName(name);
  
  // Do asynchronous task to Windows API to change our keyboard's language layout to English (US) in case we have non-English layout to work properly.
  std::thread AsyncLanguageLayoutChangeThread{
      setEnglishLocale
  };
  AsyncLanguageLayoutChangeThread.native_handle(); 

  playMainMenuMusic(); // Start a main menu music loop, initialize the sound engine.

  componentWin = CatchEvent(rendererWin, [&](Event event) {
      if (event == Event::Character('\n')) {
          screen.ExitLoopClosure()();
          screen.Loop(component);
          return true;
      }

      return false;
      }
  );

  componentLose = CatchEvent(rendererLose, [&](Event event) {
      if (event == Event::Character('\n')) {
          screen.ExitLoopClosure()();
          screen.Loop(component);
          return true;
      }

      return false;
      }
  );

  screen.Loop(component);

  if (selected == 3) {
      AsyncLanguageLayoutChangeThread.detach();
      return EXIT_SUCCESS;
  }
}