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

// Define a special style for some menu entry.
MenuEntryOption Colored(ftxui::Color c) {
  MenuEntryOption option;
  option.animated_colors.foreground.enabled = true;
  option.animated_colors.background.enabled = true;
  option.animated_colors.background.active = c;
  option.animated_colors.background.inactive = Color::Black;
  option.animated_colors.foreground.active = Color::White;
  option.animated_colors.foreground.inactive = c;
  return option;
}

void PlayMusic()
{
    PlaySound("MAIN_MUSIC", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

void StopMusic()
{
    PlaySound("MAIN_MUSIC", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

int main(int argc, const char* argv[]) {
  SetConsoleTitleA("Four Digits");

  auto screen = ScreenInteractive::Fullscreen();

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

  PlayMusic();
  screen.Loop(renderer);

  std::cout << "Selected element = " << selected << std::endl;
}