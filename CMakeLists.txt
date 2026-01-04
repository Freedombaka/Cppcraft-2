cmake_minimum_required(VERSION 3.12)
project(Cppcraft-2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

# Add source files
set(SOURCES
    src/main.cpp
    # Add more source files here as needed
)

# Create the executable
add_executable(cppcraft-2 ${SOURCES})

# Link libraries
target_link_libraries(cppcraft-2 
    ${OPENGL_LIBRARIES}
    glfw
    glm::glm
)

# Compiler-specific settings
if(MSVC)
    # Visual Studio
    target_compile_options(cppcraft-2 PRIVATE /W4)
else()
    # GCC/Clang
    target_compile_options(cppcraft-2 PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: Add a debug mode
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_MODE)
endif()
