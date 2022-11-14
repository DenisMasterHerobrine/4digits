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
using namespace ftxui; // Initialize FTXUI library.

const std::string& name = "Four Digits"; // Имя окна консоли.

auto screen = ScreenInteractive::Fullscreen(); // Придаём нашему окну консоли возможность разворачиваться и подстраиваться под любой экран, делая приложение "полноэкранным". (ftxui)

// Объявление глобальных переменных для обработки чисел в игре.
std::string code{};
std::string computerCode{};

std::string userTurnCode{};
std::string computerTurnCode{};
std::string errorCode{}, errorCodeTurn{};

std::vector<std::string> computerTurnCodes{};
std::vector<std::string> userTurnCodes{};

std::vector<char> excludedErastophenChars{};

std::vector<std::string> encryptedComputerTurnCodes{};
std::vector<std::string> encryptedUserTurnCodes{};

// Объявление глобальных полей ввода для обработки чисел в игре.
Component inputUserCode = Input(&code, "Enter the code for the computer.");
Component inputUserTurnCode = Input(&userTurnCode, "Enter your code to try to guess computer's code.");

auto componentInput = Container::Vertical({
  inputUserCode,
});

auto componentInputTurn = Container::Vertical({
  inputUserTurnCode
});

// Инициализатор главного меню:
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

// Инициализатор громкости звука в игре:
int musicVolume = 96;
auto sliderVolume = Container::Vertical({
    Slider("Music Volume:", &musicVolume, 0, 96, 6),
    });

// Display together the menu with a border
// Отображение меню вместе с границами из ftxui:
auto renderer = Renderer(menu, [&] {
    return vbox({
           text("Four Digits (v0.6.0)") | center | color(Color::LightSkyBlue1),
           separator(),
           menu->Render() | frame,
        }) |
        border | bgcolor(Color::MediumPurple4);
    });

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

auto rendererHowToPlay = Renderer([&] {
    return vbox({
             text("Four Digits - How to Play?") | center | color(Color::LightSkyBlue1),
             separator(),
             paragraph("There are 2 players. You and Computer. You and Computer create a 4-digit code that the opponent needs to solve.") | center | color(Color::White),
             paragraph("After entering the code to guess, the game begins. Now you need to try to guess the opponent's code by entering codes into the field.") | center | color(Color::White),
             paragraph("Once you've entered it, the result appear in the bottom, you'll get an output as `Bulls (B) and Cows (C)` encoded code with original code in brackets.") | center | color(Color::White),
             paragraph("Bull (B) counter means that the N numbers are correct and staying on the same place as in the opponent's code.") | center | color(Color::White),
             paragraph("Cows (C) counter means that the N numbers are containing in the opponent's code, but they're are not on the same place as in the opponent's code.") | center | color(Color::White),
             paragraph("The first player, who solves the opponent's code faster, is won.") | center | color(Color::White),
             text("") | center,
             text("Press ENTER to leave Credits menu.") | center | color(Color::White),
        }) | border | bgcolor(Color::MediumPurple4);
    });

auto rendererOptions = Renderer([&] {
    return vbox({
             text("Four Digits - Settings") | center | color(Color::LightSkyBlue1),
             separator(),
             text("Settings:") | center,
             text(""),
             text(""),
             sliderVolume->Render(),
             text(""),
             text("Press ENTER to leave Credits menu. Use A or D to scroll music volume.") | center | color(Color::White),
        }) | border | bgcolor(Color::MediumPurple4);
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

// Инициализатор всех компонентов и доступных действий для пользователя, алгоритмов игры:
auto componentWin = CatchEvent(rendererWin, [&](Event event) {
    if (event == Event::Character('\n')) {
        // Нажали Enter -> Вышли в главное меню. 
        // Конкретно в этом месте задан лишь "прототип" этого события, этот компонент просто возвращает на пустую консоль. 
        // Позднее, в main мы переопределяем этот компонент с выходом в Главное Меню.
        screen.ExitLoopClosure()();
        return true;
    }

    return false;
    }
);

auto componentLose = CatchEvent(rendererWin, [&](Event event) {
    if (event == Event::Character('\n')) {
        // Нажали Enter -> Вышли в главное меню. 
        // Конкретно в этом месте задан лишь "прототип" этого события, этот компонент просто возвращает на пустую консоль. 
        // Позднее, в main мы переопределяем этот компонент с выходом в Главное Меню.
        screen.ExitLoopClosure()();
        return true;
    }

    return false;
    }
);

// Этап "Игра": Пользователь пытается угадать число Компьютера, а Компьютер - пользователя.
auto componentGuessing = CatchEvent(rendererGuessing, [&](Event event) {
    if (event == Event::Character('\n')) {
        errorCodeTurn = "BEGIN_CHECK";
        // Нажали Enter на этапе "игра" -> Отправили число на проверку. 
        // Check User's code to check if it's valid or not.
        char* errorCodeType = isValidCode(userTurnCode.c_str());
        errorCodeTurn = errorCodeType;

        // Если оно валидное -> сгененрировать ход Компьютера и сделать все проверки для получения кода в виде "быков и коров". 
        // If it's valid -> generate a computer's turn and do checks.
        if (errorCodeTurn == "") {
            
            // Сгенерировать ход Компьютера с минимальным использованием цикла и сложностью алгоритма O(n) = max(n, n+1).
            // n+1 прогон алгоритма может быть в том случае, если generateHiddenCode возвращает пятизначное число в связи с особенностью нашего рандома.
            // Generate a computer's one.
            while (computerTurnCode.size() != 4) {
                if (excludedErastophenChars.size() == 6) {
                    // Сгенерировать код с учётом чисел, у которых есть вероятность не оказаться в числе оппонента.
                    computerTurnCode = generateErastophenCode(excludedErastophenChars);
                }
                else {
                    // Сгенерировать произвольный код.
                    computerTurnCode = generateHiddenCode();
                }
            }

            // Добавить в вектор userTurnCodes число пользователя и обработать методом turnCodesDecorator для "красивого вывода в консоль".
            userTurnCodes.push_back(userTurnCode);
            turnCodesDecorator(userTurnCodes, computerCode, encryptedUserTurnCodes);
            if (contains("4B0C", encryptedUserTurnCodes)) {
                // Пользователь выиграл -> Уничтожаем все векторы и данные текущей игры для освобождения памяти (ручной Garbage Collector) -> Переход на экран "выигрыша"
                userTurnCodes.clear();
                userTurnCodes.shrink_to_fit();
                computerTurnCodes.clear();
                computerTurnCodes.shrink_to_fit();
                excludedErastophenChars.clear();
                excludedErastophenChars.shrink_to_fit();

                screen.ExitLoopClosure()();
                screen.Loop(componentWin);
            }
            userTurnCode = ""; // Стереть текущее значение поля ввода.

            // Добавить в вектор computerTurnCodes число Компьютера и обработать методом turnCodesDecorator для "красивого вывода в консоль".
            computerTurnCodes.push_back(computerTurnCode);
            turnCodesDecorator(computerTurnCodes, code, encryptedComputerTurnCodes);

            // Модуль стратегии Компьютера: если мы в turnCodesDecorator получили значение типа nBnC, то отправляем его на проверку "упрощённым решетом Эрастофена" (выборка заведомо неподходящих чисел)
            // Таким образом, Компьютер запомнит после этого хода, какие числа скорее всего не находятся в числе оппонента и учтёт это в следующем ходу.
            if (excludedErastophenChars.size() < 6) updateErastophenVector(code, computerTurnCode, excludedErastophenChars);

            if (contains("4B0C", encryptedComputerTurnCodes)) {
                // Пользователь проиграл -> Уничтожаем все векторы и данные текущей игры для освобождения памяти -> Переход на экран "выигрыша"
                userTurnCodes.clear();
                userTurnCodes.shrink_to_fit();
                computerTurnCodes.clear();
                computerTurnCodes.shrink_to_fit();
                excludedErastophenChars.clear();
                excludedErastophenChars.shrink_to_fit();

                screen.ExitLoopClosure()();
                screen.Loop(componentLose);
            }
            computerTurnCode = ""; 

        }
        else {
            // Если же число не прошёл одну из проверок - возвращаем пользователю код ошибки, чтобы он мог задать другой код, который уже не имеет этой ошибки.
            userTurnCode = "ERROR: " + std::string(errorCodeTurn);
        }
        return true;
    }
    return false;
    }
);

// Этап "Ввода числа": Пользователь загадывает число для Компьютера.
auto componentGame = CatchEvent(rendererGame, [&](Event event) {
    if (event == Event::Character('\n')) {
        errorCode = "BEGIN_CHECK";
        
        // Проверяем, есть ли в числе пользователя неуникальные цифры, соответствует ли число четырёзначному числу и т.п...
        char* errorCodeType = isUniqueHiddenCode(code.c_str());
        errorCode = errorCodeType;

        // Ошибок нет -> удаляем предыдущие ходы, если их не подчистил Garbage Collector и начинаем игру.
        if (errorCode == "") {
            if (userTurnCodes.size() > 0) userTurnCodes.clear();
            if (computerTurnCodes.size() > 0) computerTurnCodes.clear();

            // Генерируем произвольное четырёхзначное число.
            computerCode = generateHiddenCode();

            // Начинаем этап "Игра".
            screen.ExitLoopClosure()();
            screen.Loop(componentGuessing);
        }
        return true;
    }

    return false;
    }
);

// Информационное меню: "Как играть?". Если нажать Enter -> Выходим из меню.
auto componentHowToPlay = CatchEvent(rendererHowToPlay, [&](Event event) {
    if (event == Event::Character('\n')) {
        screen.ExitLoopClosure()();
        return true;
    }

    return false;
    }
);

// Меню настроек. В данный момент только одна настройка - громкость фоновой музыки.
auto componentOptions = CatchEvent(rendererOptions, [&](Event event) {
    if (event == Event::Character('\n')) {
        // Закрыть настройки.
        screen.ExitLoopClosure()();
        return true;
    }

    if (event == Event::Character('A') || event == Event::Character('a')) {
        // Уменьшить громкость на 1 шаг ползунка.
        musicVolume -= 6;
        // Устанавливаем громкость нашей музыки, используя звуковой движок Windows/Linux (Linux в случае, если вы запускаете игру через Wine)
        setVolume(musicVolume);
        return true;
    }

    if (event == Event::Character('D') || event == Event::Character('d')) {
        // Увеличить громкость на 1 шаг ползунка.
        musicVolume += 6;
        // Устанавливаем громкость нашей музыки, используя звуковой движок Windows/Linux (Linux в случае, если вы запускаете игру через Wine)
        setVolume(musicVolume);
        return true;
    }
    
    return false;
    }
);

// Меню авторских прав и авторов игры, музыки.
auto componentCredits = CatchEvent(rendererCredits, [&](Event event) {
    if (event == Event::Character('\n')) {
        // Закрыть меню.
        screen.ExitLoopClosure()();
        return true;
    }

    if (event == Event::Character('G') || event == Event::Character('g')) {
        const std::string& link = "https://github.com/DenisMasterHerobrine/4digits";
        // Этот метод открывает ссылку, которая будет указана в параметре link в любом браузере, будь то это Internet Explorer или любой другой современный браузер, ссылка откроется в браузере по умолчанию.
        // Конкретно эта ссылка открывается при нажатии на G (П) и открывает онлайн-исходники этого проекта. 
        executeLink(link);
        return true;
    }

    if (event == Event::Character('M') || event == Event::Character('m')) {
        const std::string& link = "https://www.doomworld.com/forum/topic/120788-released-ultimate-midi-pack-a-community-music-replacement-for-the-original-doom/";
        // Этот метод открывает ссылку, которая будет указана в параметре link в любом браузере, будь то это Internet Explorer или любой другой современный браузер, ссылка откроется в браузере по умолчанию.
        // Конкретно эта ссылка открывается при нажатии на M (ь) и открывает оригинальную страницу форума со сборником любительской музыки для использования в подобных проектах. 
        executeLink(link);
        return true;
    }
    return false;
    }
);

// Главное меню игры. При нажатии Enter на одной из кнопок - откроет соответствующее меню.
auto component = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('\n') && selected == 0) {
        screen.ExitLoopClosure()();
        userTurnCode = "";
        screen.Loop(componentGame);

        return true;
    }

    if (event == Event::Character('\n') && selected == 1) {
        screen.ExitLoopClosure()();
        screen.Loop(componentHowToPlay);

        return true;
    }

    if (event == Event::Character('\n') && selected == 2) {
        screen.ExitLoopClosure()();
        screen.Loop(componentOptions);

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

// Тело программы. Основное тело игры было описано выше, здесь по большей части вызовы нестандартных функций для корректной работы самописного игрового движка.
int main(int argc, const char* argv[]) {
  // Remove alert() dialog box on Windows, since we don't need it whatsoever. If something really bad happens, create a .log file and write the error.
  // Этот метод убирает окно, вызываемое командой abort(). Нам оно не нужно из-за того, что оно вызывается только при выходе из игры при помощи exit(EXIT_SUCCESS). 
  // Если произойдёт что-то дейстельное плохое, то создастся .log файл с логами программы с текстом ошибки. Пользователю не нужно знать, что мы использовали abort().
  _set_abort_behavior(0, _WRITE_ABORT_MSG); 

  // Setting command line box name here, it's "Four Digits".
  // Этот метод изменяет название консольного огня на "Four Digits". Чисто декоративный вызов функции.
  setWindowName(name);
  
  // Do asynchronous task to Windows API to change our keyboard's language layout to English (US) in case we have non-English layout to work properly.
  // Этот код - специфичное создание потока и вызов асинхронной задачи на смену языка клавиатуры на English (US), если у нас таковой не выставлен, 
  // поскольку игровой движок не умеет работать с неанглийскими символами с клавиатуры в меню, хотя отображение символов работает корректно. 
  // (см. Utilities.cpp)
  std::thread AsyncLanguageLayoutChangeThread{
      setEnglishLocale
  };
  AsyncLanguageLayoutChangeThread.native_handle(); 

  // Start a main menu music loop, initialize the sound engine.
  // Запуск главной темы нашей игры, зацикливаем и запускаем в асинхронном потоке, инициализируем WASAPI (Windows Sound Engine API)
  // (см. Utilities.cpp)
  playMainMenuMusic(); 

  // Помните componentWin и componentLose? Конкретно здесь мы переопределяем эти 2 компонента для того, чтобы мы возвращались в Главное меню, а не куда-то в пустоту.
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

  // Запускаем игру после инициализации всего необходимого! 🚀
  screen.Loop(component);

  // Если мы вышли из игры -> Выходим из всех потоков и уничтожаем нами созданный поток из Utilities.cpp со сменой языка, также уничтожаем все асинхронные задачи и выходим из игры, заканчиваем процесс игры.
  if (selected == 4) {
      AsyncLanguageLayoutChangeThread.detach();
      return EXIT_SUCCESS;
  }

  // Залицензировано под лицензией MIT License. (https://github.com/DenisMasterHerobrine/4digits/blob/master/LICENSE) 
  // Исходники проекта: https://github.com/DenisMasterHerobrine/4digits
}