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

#include <windows.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
#include <string>

using namespace ftxui;

void PlayMusic()
{
    PlaySound("MAIN_MUSIC", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

void StopMusic()
{
    PlaySound(NULL, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

int main(int argc, const char* argv[]) {
  SetConsoleTitleA("Four Digits");

  auto screen = ScreenInteractive::Fullscreen();
  auto screenCredits = ScreenInteractive::Fullscreen();

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
               text("4 Digits (v0.6.0)") | center | color(Color::BlueLight),
               separator(),
               menu->Render() | frame,
           }) |
           border | bgcolor(Color::DeepSkyBlue4);
  });

  auto rendererCredits = Renderer([&] {
      return vbox({
                 text("4 Digits (v0.6.0)") | center | color(Color::BlueLight),
                 separator(),
                 text("Coded by DenisMasterHerobrine (also known as DMHDev, Denis Kalashnikov). Licensed under MIT License.") | center | color(Color::White),
                 text("Music: Dread Factory by MFG38 from DOOMWorld's Ultimate MIDI Pack by northivanastan") | center | color(Color::Red1),
          }) |
          border | bgcolor(Color::DeepSkyBlue4);
      });

  auto componentCredits = CatchEvent(rendererCredits, [&](Event event) {
      if (event == Event::Character('\n')) {
          screen.ExitLoopClosure()();
          return true;
      }
      return false;
      }
  );

  auto component = CatchEvent(renderer, [&](Event event) {
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

  PlayMusic();

  screen.Loop(component);

  if (selected == 3) {
      return EXIT_SUCCESS;
  }
}