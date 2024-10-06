cmake_minimum_required(VERSION 3.15)

set(OPENGL_UI "opengl-ui")
project(${OPENGL_UI})

# Warning flags
set(FLAGS "-Wall -Wextra")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Build type
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${FLAGS} -g3 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS} -O3")

# Debug option
option(DEBUG "Enable debug mode" OFF)
if (DEBUG)
    message("DEBUG MODE ON")
    add_definitions(-DDEBUG)
else()
    message("DEBUG MODE OFF")
endif()

# Find packages
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Library output
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Engine source files
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(
    ENGINE_SOURCE_FILES

    ${SOURCE_DIR}/application.cpp
    ${SOURCE_DIR}/border_radius.cpp
    ${SOURCE_DIR}/debug.cpp
    ${SOURCE_DIR}/dim.cpp
    ${SOURCE_DIR}/font.cpp
    ${SOURCE_DIR}/quad.cpp
    ${SOURCE_DIR}/shader.cpp

    # External resources
    ${CMAKE_SOURCE_DIR}/external/glad/glad.c
    ${CMAKE_SOURCE_DIR}/external/stb/stb_image.c
)

# Adding engine shared library
add_library(${OPENGL_UI} SHARED ${ENGINE_SOURCE_FILES})

# Libraries
target_link_directories(
    ${OPENGL_UI}
    PRIVATE
    ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(
    ${OPENGL_UI}
    PRIVATE
    ${OPENGL_LIBRARIES} glfw
)

# Header files
target_include_directories(
    ${OPENGL_UI}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/include
    ${CMAKE_SOURCE_DIR}/external/include/freetype
)

# Tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#######################################################
#######################################################
# Main test

# Source files
set(
    TEST_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
)

# Adding executables for testing
add_executable(main-test ${TEST_SOURCE_FILES})
add_dependencies(main-test ${OPENGL_UI})
target_compile_definitions(main-test PUBLIC PROJECT_ROOT_FOLDER="${CMAKE_SOURCE_DIR}")

# Libraries
target_link_directories(
    main-test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/external/freetype
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)
target_link_libraries(
    main-test
    PRIVATE
    ${OPENGL_UI}
    ${OPENGL_LIBRARIES} glfw freetype
)

# Header files
target_include_directories(
    main-test
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/include
    ${CMAKE_SOURCE_DIR}/external/include/freetype
)
