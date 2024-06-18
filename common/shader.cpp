#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.hpp"

using namespace std;

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
    // 쉐이더 생성
    GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // 버텍스 쉐이더 코드를 파일에서 읽기
    string VertexShaderCode;
    ifstream VertexShaderStream(vertex_file_path, ios::in);
    if(VertexShaderStream.is_open())
    {
        stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else
    {
        printf("파일 %s를 읽을 수 없음. 디렉토리 확인 필요.", vertex_file_path);
        getchar();
        return 0;
    }

    // 프래그먼트 쉐이더 코드를 파일에서 읽기
    string FragmentShaderCode;
    ifstream FragmentShaderStream(fragment_file_path, ios::in);
    if(FragmentShaderStream.is_open())
    {
        stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;

    // 버텍스 쉐이더를 컴파일
    printf("Compiling shader : %s\n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderId, 1, &VertexSourcePointer, nullptr);
    glCompileShader(VertexShaderId);

    // 버텍스 쉐이더를 검사
    glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0)
    {
        vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderId, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // 프래그먼트 쉐이더를 컴파일
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderId, 1, & FragmentSourcePointer, nullptr);
    glCompileShader(FragmentShaderId);

    // 프래그먼트 쉐이더를 검사
    glGetShaderiv(FragmentShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0)
    {
        vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderId, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // 프로그램에 링크
    printf("Linking program\n");
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    glLinkProgram(ProgramId);

    // 프로그램 검사
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0)
    {
        vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramId, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteShader(VertexShaderId);
    glDeleteShader(FragmentShaderId);

    return ProgramId;
}