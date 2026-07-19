cmake_minimum_required(VERSION 3.20)
project(navmesh2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options(/utf-8)
endif()

include(FetchContent)

# ---- nlohmann/json ----
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

# ---- doctest ----
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG master
)

# ---- raylib ----
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)

# ---- Clipper2 ----
set(CLIPPER2_TESTS OFF CACHE BOOL "" FORCE)
set(CLIPPER2_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLIPPER2_UTILS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  clipper2
  GIT_REPOSITORY https://github.com/AngusJohnson/Clipper2.git
  GIT_TAG main
  SOURCE_SUBDIR CPP
)

# ---- raygui (viewer UI: sliders, checkboxes) ----
# Header-only; no CMakeLists at the repo root worth building, so we just
# populate the source and point the viewer target at src/raygui.h.
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui.git
  GIT_TAG 4.0
)

FetchContent_MakeAvailable(json doctest raylib clipper2 raygui)

# ---- nav: headless core library ----
add_library(nav STATIC
  src/nav/geometry.cpp
  src/nav/level.cpp
  src/nav/offset.cpp
  src/nav/triangulate.cpp
  src/nav/mesh.cpp
  src/nav/astar.cpp
  src/nav/funnel.cpp
  src/nav/builder.cpp
  src/nav/query.cpp
)
target_include_directories(nav PUBLIC include)
target_include_directories(nav PRIVATE src)
target_link_libraries(nav PUBLIC nlohmann_json::nlohmann_json PRIVATE Clipper2)

# ---- viewer: raylib visualizer ----
add_executable(viewer
  apps/viewer/main.cpp
  apps/viewer/draw.cpp
  apps/viewer/ui.cpp
)
target_include_directories(viewer PRIVATE ${raygui_SOURCE_DIR}/src)
target_link_libraries(viewer PRIVATE nav raylib)

# ---- bench: headless timing ----
add_executable(bench apps/bench/main.cpp)
target_link_libraries(bench PRIVATE nav)

# ---- tests ----
add_executable(tests
  tests/test_main.cpp
  tests/test_geometry.cpp
  tests/test_triangulate.cpp
  tests/test_mesh.cpp
  tests/test_astar.cpp
  tests/test_funnel.cpp
)
target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE nav doctest::doctest)

enable_testing()
add_test(NAME tests COMMAND tests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
