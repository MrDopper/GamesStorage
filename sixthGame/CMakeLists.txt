
#Checking requirement for cmake
cmake_minimum_required(VERSION 3.26)

project(Game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect Homebrew on macOS
if(APPLE)
    find_program(BREW_EXECUTABLE brew
        PATHS /opt/homebrew/bin /usr/local/bin
    )

    if(BREW_EXECUTABLE)
        execute_process(
            COMMAND ${BREW_EXECUTABLE} --prefix sdl3
            OUTPUT_VARIABLE SDL3_BREW
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE SDL3_RESULT
        )

        execute_process(
            COMMAND ${BREW_EXECUTABLE} --prefix sdl3_image
            OUTPUT_VARIABLE IMAGE_BREW
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE IMAGE_RESULT
        )

        if(SDL3_RESULT EQUAL 0 AND IMAGE_RESULT EQUAL 0)
            set(USE_BREW_SDL TRUE)

            find_package(SDL3 REQUIRED CONFIG
                PATHS "${SDL3_BREW}"
                NO_DEFAULT_PATH
            )

            find_package(SDL3_image REQUIRED CONFIG
                PATHS ${IMAGE_BREW}
                NO_DEFAULT_PATH
            )
        endif()
    endif()
endif()

# Windows, Linux, or Mac without Homebrew
if(NOT USE_BREW_SDL)
    find_package(SDL3 REQUIRED CONFIG)
    find_package(SDL3_image REQUIRED CONFIG)
endif()

# Source files
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
#building executable file
add_executable(game main.cpp ${SOURCES})
#include include folder
target_include_directories(game PRIVATE include)

# Prioritize the correct SDL3 headers on Homebrew
if(USE_BREW_SDL)
    target_include_directories(game BEFORE PRIVATE
        ${SDL3_BREW}/include
    )
endif()

target_link_libraries(game PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

#copy assets folder into the executable directory
#VERBATIM handle paths containing spaces or special characters for cross-platform.
add_custom_target(copy_assets 
COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
${CMAKE_CURRENT_LIST_DIR}/assets 
${CMAKE_CURRENT_BINARY_DIR}/assets
COMMENT "Copying assets into build folder"
VERBATIM
)

add_dependencies(game copy_assets)