#version 330 core

// 정점 셰이더에서 넘겨준 보간 값.
in vec2 UV;

// 결과 값
out vec3 color;

// 한 메쉬를 그리는 동안 일정하게 유지되는 값.
uniform sampler2D myTextureSampler;

void main()
{
    // 출력할 색 = 버텍스 쉐이더에서 정해진 색
    // 3개의 둘러싸고 있는 쉐이더의 사이에서 보간됨.
    color = texture(myTextureSampler, UV).rgb;
}