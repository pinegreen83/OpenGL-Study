#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "texture.hpp"

GLuint loadBMP_custom(const char * imagepath)
{
    printf("Reading image %s\n", imagepath);

    // BMP 파일의 헤더부터 데이터을 읽어옴.
    unsigned char header[54];   // 모든 BMP 파일은 54바이트의 헤더로 시작함.
    unsigned int dataPos;       // 실제로 데이터가 시작하는 위치.
    unsigned int width, height;
    unsigned int imageSize;     // = width * height * 3
    // 실제 RGB 데이터
    unsigned char * data;

    // 진짜로 파일을 여는 부분.
    FILE * file = fopen(imagepath, "rb");
    if(!file)
    {
        printf("%s could not be opened. Are you in the right directory? Don't forget to read the FAQ!\n", imagepath);
        getchar();
        return 0;
    }

    // 파일의 헤더인 처음 54바이트부터 읽기 시작.

    // 54바이트보다 적게 읽는다면 문제 발생.
    if(fread(header, 1, 54, file) != 54)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    // BMP 파일은 항상 "BM"으로 시작.
    if(header[0] != 'B' || header[1] != 'M')
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    // 24비트 비트맵으로 압축된 파일인지 확인하기.	
    if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}

    // 바이트 배열에서 int 변수읽기.
    dataPos     = *(int*)&(header[0x0A]);
    imageSize   = *(int*)&(header[0x22]);
    width       = *(int*)&(header[0x12]);
    height      = *(int*)&(header[0x16]);

    // 몇몇 BMP 파일들은 포맷이 잘못됨. 정보가 누락됐는지 확인하기.
    if(imageSize==0)    imageSize = width * height * 3; // 3 : 각각 Red, Green, Blue를 나타내는 바이트를 의미.
    if(dataPos==0)      dataPos = 54; // BMP 헤더가 종료되는 시점.

    // 버퍼 생성
    data = new unsigned char [imageSize];

    // 파일에서 버퍼로 실제 데이터 넣기.
    fread(data, 1, imageSize, file);

    // 버퍼로 데이터를 모두 옮겼으므로 파일 닫기.
    fclose(file);

    // OpenGL 텍스쳐 생성.
    GLuint textureID;
    glGenTextures(1, &textureID);

    // 새로운 텍스쳐에 "Bind"하기. 이제 모든 텍스쳐 함수들은 이 텍스쳐를 수정.
    glBindTexture(GL_TEXTURE_2D, textureID);

    // OpenGL에게 이미지 넘겨주기.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL이 데이터 버퍼를 복사했으므로 버퍼 해제
    delete [] data;

    // Poor filtering, or ...
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering ...
    // 괜찮은 3중 필터링
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 이미지를 확대할땐(영어로 확대는 MAGnifying. 앞글자 MAG를 따왔네요.), 선형 필터링을 사용. 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 이미지를 축소할땐(영어로 축소는 MINifying. 앞글자 MIN을 따왔네요.), 두개의 밉맵을 선형으로 블랜드하고, 선형으로 필터링.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
    // 밉맵 생성
	glGenerateMipmap(GL_TEXTURE_2D);

    // 우리가 만든 텍스쳐의 ID를 반환
    return textureID;
}

// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
// or do it yourself (just like loadBMP_custom and loadDDS)
//GLuint loadTGA_glfw(const char * imagepath){
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// Read the file, call glTexImage2D with the right parameters
//	glfwLoadTexture2D(imagepath, 0);
//
//	// Nice trilinear filtering.
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Return the ID of the texture we just created
//	return textureID;
//}



#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath)
{

    unsigned char header[124];

    FILE *fp;

    /* 파일 열기 시도 */
    fp = fopen(imagepath, "rb");
    if(fp == nullptr)
    {
        printf("%s could not be opened. Are you in the right directory? Don't forget to read the FAQ!\n", imagepath);
        getchar();
        return 0;
    }

    /* 파일 타입 체크 */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if(strncmp(filecode, "DDS ", 4) != 0)
    {
        fclose(fp);
        return 0;
    }

    /* 이미지의 정보를 가져오기. */
    fread(&header, 124, 1, fp);

    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width       = *(unsigned int*)&(header[12]);
    unsigned int linearSize  = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);

    unsigned char * buffer;
    unsigned int bufsize;
    /* 모든 밉맵을 포함하면 얼마나 큰가 */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* 파일 포인터 닫기 */
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            return 0;
    }

    // OpenGL 텍스쳐 생성
    GLuint textureID;
    glGenTextures(1, &textureID);

    // 새롭게 생성된 텍스쳐를 "Bind"함. : 모든 텍스쳐 관련 함수는 이 함수를 사용.
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* 밉맵 로드*/
    for(unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*(blockSize);
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer+offset);
        offset  += size;
        width   /= 2;
        height  /= 2;
    }
    free(buffer);

    return textureID;
}