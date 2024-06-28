#version 330 core

// 버텍스 쉐이서의 보간된 값
in vec3 fargmentColor;

// 결과 값
out vec3 color;

void main()
{
    // 출력할 색 = 버텍스 쉐이더에서 정해진 색
    // 3개의 둘러싸고 있는 쉐이더의 사이에서 보간됨.
    color = fargmentColor;
}