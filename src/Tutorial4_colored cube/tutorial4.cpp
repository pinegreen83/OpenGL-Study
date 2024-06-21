// 기본 c++ 라이브러리
#include <iostream>

// GLEW 추가
#include <GL/glew.h>

// GLFW 포함 및 window 변수 선언
#include <GLFW/glfw3.h>
GLFWwindow* window;

// glm 포함
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    // 파란색 화면
    glClearColor(0.0f, .0f, .4f, .0f);

    // Vertex Array Object 생성
    GLuint VertexArrayId;
    glGenVertexArrays(1, &VertexArrayId);
    glBindVertexArray(VertexArrayId);

    // 쉐이더들에서 GLSL 프로그램 생성 및 컴파일
    GLuint programId = LoadShaders("src/CubeVertexShader.vert", "src/ColorFragmentShader.frag");

    // 프로젝션 매트릭스(행렬)
    glm::mat4 Projection = glm::perspective(
        glm::radians(45.0f), // 수직방향 시야각 : 카메라"줌"과 같은 
        4.0f / 3.0f,         // 화면 비 : 윈도우 크기에 의존
        0.1f,                // Near Clipping plane(근거리 잘라내기 평면)
        100.0f               // Far Clipping plane(원거리 잘라내기 평면)
    );

    // 카메라 매트릭스(행렬)
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, -3), // cameraPosition : 카메라 위치
        glm::vec3(0, 0, 0), // cameraTarget : 카메라가 바라보는 곳
        glm::vec3(0, 1, 0)  // upVector : 원래 우리가 바라보는 방향, (0, -1, 0)이면 화면이 뒤집힘.
    );

    // 모델 매트릭스(행렬)
    glm::mat4 Model = glm::mat4(1.0f);

    // 매트릭스 계산
    glm::mat4 MVP = Projection * View * Model;

    // MVP 매트릭스(행렬)에 대한 참조
    GLuint MatrixId = glGetUniformLocation(programId, "MVP");

    // Vertex Buffer Object 생성
    // 3개의 연속된 float 숫자가 하나의 3차원 vertex.
    // 하나의 육면체는 2개의 삼각형으로 이뤄진 면 6개를 가지고 있기 때문에
    // 총 6 * 2 = 12개의 삼각형, 12 * 3 = 36개의 vertex를 가지고 있음.
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f, // 첫번째 삼각형 : 시작
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f, // 첫번째 삼각형 : 끝
        1.0f, 1.0f, -1.0f, // 두번째 삼각형 : 시작
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, // 두번째 삼각형 : 끝
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // 각 vertex들에 대한 색상으로 랜덤하게 구했음.
    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // 깊이 테스트 활성화
    glEnable(GL_DEPTH_TEST);
    // 이전 fragment보다 카메라에 더 가까우면 해당 fragment를 그림.
    glDepthFunc(GL_LESS);

    do
    {
        // 화면 지우기
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 화면 그리기
        // 만든 쉐이더 프로그램 사용
        glUseProgram(programId);

        // 위에서 생성한 MVP 매트릭스(행렬)에 대한 참조를 현재 바인딩된 쉐이더에 변환한 매트릭스 전송.
        // 각각의 모델마다 다른 MVP 매트릭스(행렬)을 갖고 있으니, 메인 루프에서 적용.
        glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &MVP[0][0]);

        // 첫 번째 Attribute 버퍼 설정 : verteices(버텍스들)
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,           // attribute, 0인 이유가 있진 않지만, 쉐이더의 레이아웃과 일치해야 함.
            3,           // 크기
            GL_FLOAT,    // 타입
            GL_FALSE,    // 정규화되었는지
            0,           // 폭
            (void*)0     // array buffer offset
        );

        // 두 번째 Attribute 버퍼 설정 : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,           // attribute, 1인 이유가 있진 않지만, 쉐이더의 레이아웃과 일치해야 함.
            3,           // 크기
            GL_FLOAT,    // 타입
            GL_FALSE,    // 정규화되었는지
            0,           // 폭
            (void*)0     // array buffer offset
        );

        // 삼각형 그리기
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3의 0에서 시작하는 인덱스로 0 -> 12개의 삼각형 -> 6개의 사각형

        // Vertex Attribute 비활성화
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // 버퍼 스왑
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

    // 자원 해제
    glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programId);
    glDeleteVertexArrays(1, &VertexArrayId);

    // OpenGL 화면 닫고 GLFW 종료
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}