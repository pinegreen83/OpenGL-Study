#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "controls.hpp"

mat4 ViewMatrix;
mat4 ProjectionMatrix;

mat4 getViewMatrix()
{
    return ViewMatrix;
}

mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}


// 초기 좌표 : +Z 방향
vec3 position = vec3(0, 0, 5);
// 수평각 : -Z 방향으로
float horizontalAngle = 3.14f;
// 수직각 : 0, 수직을 바라봄.
float verticalAngle = 0.0f;
// 초기 FOV값.
float initialFoV = 45.0f;

float speed = 3.0f; // 3 unit / second
float mouseSpeed = 0.001f;


void computeMatricesFromInputs(GLFWwindow* window)
{
    // 이 변수가 선언될 때 불리는 glfwGetTime 함수는 한번만 불림.
    static double lastTime = glfwGetTime();

    // 현재 시간과 지난 시간 사이의 시간을 계산.
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // 마우스 위치 가져오기.
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // 매 프레임마다, 화면의 중앙으로 마우스 커서를 움직임.
    glfwSetCursorPos(window, 1024/2, 768/2);

    // 새로운 방향 계산
    horizontalAngle += mouseSpeed * deltaTime * (1024/2 - xpos);
    verticalAngle   += mouseSpeed * deltaTime * ( 768/2 - ypos);

    // 방향 벡터 : 구면 좌표계에서 데카르트(직교) 좌표계로 변환
    vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // 오른쪽 벡터
    vec3 right = vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    // 방향 벡터와 오른쪽 벡터의 외적으로 위쪽 벡터 계산
    vec3 up = cross(right, direction);

    // 키보드 입력 처리
    // 앞으로 이동
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += direction * deltaTime * speed;
    }
    // 뒤로 이동
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= direction * deltaTime * speed;
    }
    // 오른쪽으로 이동
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += right * deltaTime * speed;
    }
    // 왼쪽으로 이동
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position -= right * deltaTime * speed;
    }

    float FoV = initialFoV; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection Matrix : 45도(각도)의 FoV, 4:3 비율, 화면 표시 범위 : 0.1unit <-> 100unit
    ProjectionMatrix = perspective(radians(FoV), 4.0f/3.0f, 0.1f, 100.0f);
    // Camera Matrix
    ViewMatrix       = lookAt(
        position,            // 카메라 위치
        position+direction,  // 바라보는 위치 : 동일한 위치에서 방향만 더함.
        up                   // 머리가 위를 향함.
    );
}