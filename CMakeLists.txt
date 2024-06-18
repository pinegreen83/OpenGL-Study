cmake_minimum_required(VERSION 3.10)
project(OpenGLStudy)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/external/include ${PROJECT_SOURCE_DIR}/common)

# Link directories
link_directories(${PROJECT_SOURCE_DIR}/external/lib)

# 공통 소스 파일
set(COMMON_SOURCES
    ${PROJECT_SOURCE_DIR}/common/shader.cpp
)

# 메인 파일
set(MAIN_SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# 실행 파일 설정
add_executable(main ${MAIN_SOURCES} ${COMMON_SOURCES})

# Link libraries
if(APPLE)
    target_link_libraries(main GLEW glfw3 "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
else()
    target_link_libraries(main GLEW glfw3 GL)
endif()