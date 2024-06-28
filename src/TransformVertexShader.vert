#version 330 core

// vertex 데이터 입력 값, 쉐이더의 실행때마다 값이 다름.
layout(location = 0) in vec3 vertexPosition_modelspace;
// 여기서의 1이 앞서 작성한 glVertexAttribPointer의 1과 일치함.
layout(location = 1) in vec2 vertexUV;

// 출력할 값 : 각 픽셀마다 알아서 보간.
out vec2 UV;

// 이 변수는 '메쉬당' 상수.
uniform mat4 MVP;

void main()
{
    // 정점의 출력 좌표, clip space에서 MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    // 정점의 UV. 특별한 건 없음.
    UV = vertexUV;
}
