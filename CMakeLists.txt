cmake_minimum_required(VERSION 3.10)
project(OpenGLStudy)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/external/include)

# Link directories
link_directories(${PROJECT_SOURCE_DIR}/external/lib)

# Add the executable
add_executable(main src/main.cpp)

# Link libraries
if(APPLE)
    target_link_libraries(main GLEW glfw3 "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
else()
    target_link_libraries(main GLEW glfw3 GL)
endif()