#version 330 core

// vertex 데이터 입력 값, 쉐이더의 실행때마다 값이 다름.
layout(location = 0) in vec3 vertexPosition_modelspace;
// 여기서의 1이 앞서 작성한 glVertexAttribPointer의 1과 일치함.
layout(location = 1) in vec3 vertexColor;

// 출력할 값 : 모든 fragment에 대해서 보간할 예정.
out vec3 fargmentColor;
// 한번의 실행에서는 상수적.
uniform mat4 MVP;

void main()
{
    // 정점의 출력 좌표, clip space에서 MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    // 모든 버텍스의 색은 보간되어 각 fragment에서 보여질 예정.
    fargmentColor = vertexColor;
}
