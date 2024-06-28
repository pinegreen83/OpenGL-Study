#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>

// .BMP 파일을 사용하는 커스텀 텍스쳐 로더 함수 정의
GLuint loadBMP_custom(const char * imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

#endif