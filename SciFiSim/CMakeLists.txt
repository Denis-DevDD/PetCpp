cmake_minimum_required(VERSION 3.20)
project(scifisim CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG ae7a13539fb71f270b87eb2e874fbac80bc8dda2
)
FetchContent_MakeAvailable(doctest)

add_library(sim STATIC
    src/sim/world.cpp
    src/sim/contentloader.cpp
    src/sim/save.cpp
)
target_include_directories(sim PUBLIC include)
target_link_libraries(sim PUBLIC nlohmann_json::nlohmann_json)

# Raylib front
add_executable(game
    apps/game/main.cpp
)
target_link_libraries(game PRIVATE sim raylib)

add_executable(balance
    apps/balance/main.cpp
)
target_link_libraries(balance PRIVATE sim)

enable_testing()
add_executable(tests
    tests/test_main.cpp
)
target_link_libraries(tests PRIVATE sim doctest::doctest)
add_test(NAME tests COMMAND tests)
