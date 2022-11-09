cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v3.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(4digits-launcher
  LANGUAGES CXX
  VERSION 0.6.0
)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/4digits.rc")
add_executable(4digits-launcher "src/Launcher.cpp" ${APP_ICON_RESOURCE_WINDOWS}  "src/Utilities.h" "src/4DigitsAPI.h" "src/4DigitsAPI.cpp" "src/Utilities.cpp")
target_include_directories(4digits-launcher PRIVATE src)

target_link_libraries(4digits-launcher
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)