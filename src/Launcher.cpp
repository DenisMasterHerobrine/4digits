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

int main(int argc, const char* argv[]) {
    
    std::thread AsyncLanguageLayoutChangeThread{
        setEnglishLocale
    };
    AsyncLanguageLayoutChangeThread.native_handle();

    const std::string& name = "Four Digits";
    setWindowName(name);

    auto screen = ScreenInteractive::Fullscreen();
    auto creditsScreen = ScreenInteractive::Fullscreen();

    std::string code{};
    std::string errorCode{};
    Component inputUserCode = Input(&code, "Guess the code for the computer.");

    auto componentInput = Container::Vertical({
      inputUserCode,
    });

    int selected = 0;
    auto menu = Container::Vertical(
      {
          MenuEntry("Start  Game  ") | center,
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

    auto rendererCredits = Renderer([&] {
        return vbox({
                 text("Four Digits (v0.6.0)") | center | color(Color::LightSkyBlue1),
                 separator(),
                 text("Coded by DenisMasterHerobrine (also known as DMHDev, Denis Kalashnikov). Licensed under MIT License.") | center | color(Color::White),
                 text("") | center,
                 text("Music: Dread Factory by MFG38 from DOOMWorld's Ultimate MIDI Pack by northivanastan") | center | color(Color::DarkRedBis),
                 text("") | center,
                 text("https://github.com/DenisMasterHerobrine/4digits") | center | color(Color::GrayLight),
                 text("") | center,
                 text("") | center,
                 text("") | center,
                 text("Press G to go to GitHub page.") | center | color(Color::GrayLight),
                 text("Press M to go to music's author page.") | center | color(Color::DarkRedBis),
                 text("Press ENTER to leave Credits menu.") | center | color(Color::White),
        }) | border | bgcolor(Color::MediumPurple4);
    });

    auto componentGame = CatchEvent(rendererGame, [&](Event event) {
        if (event == Event::Character('\n')) {
            char* arrayCode = new char{};
            strcpy(arrayCode, code.c_str());
            char* errorCodeChar = new char{};
            strcpy(arrayCode, code.c_str());
            char* errorCodeType = isUniqueHiddenCode(arrayCode);
            errorCode = errorCodeType;
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
          screen.Loop(componentGame);

          return true;
      }

      if (event == Event::Character('\n') && selected == 2) {
          screen.ExitLoopClosure()();
          screen.Loop(componentCredits);

          return true;
      }

      if (event == Event::Character('\n') && selected == 3) {
          screen.ExitLoopClosure()();
          return true;
      }
      return false;
      }
  );

  playMainMenuMusic(); // Start a main menu music loop, initialize the sound engine.

  screen.Loop(component);

  if (selected == 3) {
      AsyncLanguageLayoutChangeThread.detach();
      return EXIT_SUCCESS;
  }
}