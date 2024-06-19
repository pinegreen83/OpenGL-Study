// 기본 c++ 라이브러리
#include <iostream>

// GLEW 추가
#include <GL/glew.h>

// GLFW 포함 및 window 변수 선언
#include <GLFW/glfw3.h>
GLFWwindow* window;

// glm 포함
#include <glm/glm.hpp>
using namespace glm;

using namespace std;

int main()
{
    // GLFW 실행
    if(!glfwInit())
    {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x 안티엘리어싱
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3 사용
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS를 위함.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 옛날 OpenGL을 사용하지 않음.

    // window 생성
    window = glfwCreateWindow(1024, 768, "Hello World", nullptr, nullptr);
    if(window == nullptr)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // GLEW 초기화
    glewExperimental=true; // 코어 프로파일용
    
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        cerr << "Failed to initialize GLEW : " << glewGetErrorString(err) << endl;
        return -1;
    }

    // 키 입력 받기 위함.
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // 파란색 화면
    glClearColor(0.0f, .0f, .4f, .0f);

    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
    {
        // 화면 지우기
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 추후에 화면에 그릴 것을 놓을 영역

        // 버퍼 스왑
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // OpenGL 화면 닫고 GLFW 종료
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}