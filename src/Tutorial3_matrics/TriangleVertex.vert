#version 330 core

// vertex 데이터 입력 값, 쉐이더의 실행때마다 값이 다름.
layout(location = 0) in vec3 vertexPosition_modelspace;

// 한번의 실행에서는 상수적.
uniform mat4 MVP;

void main()
{
    // 정점의 출력 좌표, clip space에서 MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
}