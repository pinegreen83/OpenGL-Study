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

#include "../common/shader.hpp"

using namespace std;

int main()
{
    // GLFW 실행
    if(!glfwInit())
    {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window 생성
    window = glfwCreateWindow(1024, 768, "Hello World", nullptr, nullptr);
    if(window == nullptr)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental=true; // 코어 프로파일용
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        cerr << "Failed to initialize GLEW : " << glewGetErrorString(err) << endl;
        return -1;
    }

    // 키 입력 받기 위함.
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Vertex Array Object 생성
    GLuint VertexArrayId;
    glGenVertexArrays(1, &VertexArrayId);
    glBindVertexArray(VertexArrayId);

    // 쉐이더들에서 GLSL 프로그램 생성 및 컴파일
    GLuint programId = LoadShaders("src/TriangleVertex.vert", "src/TriangleFragment.frag");

    // Vertex Buffer Object 생성 및 데이터 설정
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // 파란색 화면
    glClearColor(0.0f, .0f, .4f, .0f);

    do
    {
        // 화면 지우기
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 화면 그리기
        // 만든 쉐이더 프로그램 사용
        glUseProgram(programId);

        // Vertex Attribute 설정
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        // 삼각형 그리기
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Vertex Attribute 비활성화
        glDisableVertexAttribArray(0);

        // 버퍼 스왑
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

    // 자원 해제
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayId);

    // OpenGL 화면 닫고 GLFW 종료
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}