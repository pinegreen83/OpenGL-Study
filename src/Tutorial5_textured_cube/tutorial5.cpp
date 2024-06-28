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
#include "../common/texture.hpp"

using namespace std;

int main( void )
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
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

	// 파란색 배경
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Vertex Array Object 생성
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// 쉐이더들에서 GLSL 프로그램 생성 및 컴파일
	GLuint programID = LoadShaders( "src/TransformVertexShader.vert", "src/TextureFragmentShader.frag" );

	// MVP 매트릭스(행렬)에 대한 참조
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

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

	// Load the texture using any two methods
	//GLuint Texture = loadBMP_custom("src/uvtemplate.bmp");
	GLuint Texture = loadDDS("src/uvtemplate.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Vertex Buffer Object 생성
    // 3개의 연속된 float 숫자가 하나의 3차원 vertex.
    // 하나의 육면체는 2개의 삼각형으로 이뤄진 면 6개를 가지고 있기 때문에
    // 총 6 * 2 = 12개의 삼각형, 12 * 3 = 36개의 vertex를 가지고 있음.
    static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
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

	// 정점 당 두 개의 UV 좌표. Blender에서 생성.
	static const GLfloat g_uv_buffer_data[] = { 
		0.000059f, 1.0f-0.000004f, 
		0.000103f, 1.0f-0.336048f, 
		0.335973f, 1.0f-0.335903f, 
		1.000023f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.336024f, 1.0f-0.671877f, 
		0.667969f, 1.0f-0.671889f, 
		1.000023f, 1.0f-0.000013f, 
		0.668104f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.000059f, 1.0f-0.000004f, 
		0.335973f, 1.0f-0.335903f, 
		0.336098f, 1.0f-0.000071f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.336024f, 1.0f-0.671877f, 
		1.000004f, 1.0f-0.671847f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.668104f, 1.0f-0.000013f, 
		0.335973f, 1.0f-0.335903f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.668104f, 1.0f-0.000013f, 
		0.336098f, 1.0f-0.000071f, 
		0.000103f, 1.0f-0.336048f, 
		0.000004f, 1.0f-0.671870f, 
		0.336024f, 1.0f-0.671877f, 
		0.000103f, 1.0f-0.336048f, 
		0.336024f, 1.0f-0.671877f, 
		0.335973f, 1.0f-0.335903f, 
		0.667969f, 1.0f-0.671889f, 
		1.000004f, 1.0f-0.671847f, 
		0.667979f, 1.0f-0.335851f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    // 깊이 테스트 활성화
    glEnable(GL_DEPTH_TEST);
    // 이전 fragment보다 카메라에 더 가까우면 해당 fragment를 그림.
    glDepthFunc(GL_LESS);

	do{
		// 화면 지우기
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 화면 그리기
        // 만든 쉐이더 프로그램 사용
		glUseProgram(programID);

		// 위에서 생성한 MVP 매트릭스(행렬)에 대한 참조를 현재 바인딩된 쉐이더에 변환한 매트릭스 전송.
        // 각각의 모델마다 다른 MVP 매트릭스(행렬)을 갖고 있으니, 메인 루프에서 적용.
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 우리의 텍스쳐를 텍스쳐 유닛 0에 Bind.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
        // "myTextureSampler" 샘플러를 사용하기 위해 텍스쳐 유닛 0에 설정.
		glUniform1i(TextureID, 0);

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

		// 두 번째 Attribute 버퍼 설정 : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
            1,           // attribute, 1인 이유가 있진 않지만, 쉐이더의 레이아웃과 일치해야 함.
            2,           // 크기 : U+V => 2
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

	} // ESC가 눌리거나 윈도우가 닫혔는지 확인
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// 자원 해제
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// OpenGL 화면 닫고 GLFW 종료
    glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

