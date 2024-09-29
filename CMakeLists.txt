cmake_minimum_required(VERSION 3.10)

#Project name
project(gameboy)

#Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Enable all warnings and treat them as errors
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=undefined)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

# Find SDL2 and SDL2_ttf
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/include)

# Add executable
add_executable(gameboy
    src/main.cpp
    src/lib/gameboy.cpp
    src/lib/window.cpp
    src/lib/bus.cpp
    src/lib/cpu.cpp
    src/lib/instructions.cpp
    src/lib/registers.cpp
    src/lib/gpu.cpp
    src/lib/cart.cpp
)

# Link libraries
target_link_libraries(gameboy ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})