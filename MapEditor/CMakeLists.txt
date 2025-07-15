cmake_minimum_required(VERSION 3.15)

project(mapeditor CXX)

include(FetchContent)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
  Thor
  GIT_REPOSITORY https://github.com/Bromeon/Thor.git
  PATCH_COMMAND git apply ${CMAKE_CURRENT_SOURCE_DIR}/Thor.patch
  UPDATE_DISCONNECTED 1
)
FetchContent_MakeAvailable(Thor)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG 35b1148efb839381b84de9290d9caf0b66ad7d03
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG 82dc2033e51b8323857c3ae1cf1f458b3a933c35
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_MakeAvailable(imgui-sfml)

add_executable(mapeditor
  Camera.cpp
  Editor.cpp
  main.cpp
  Utils.cpp
)

target_link_libraries(christapheno
  ImGui-SFML::ImGui-SFML
  thor
)
