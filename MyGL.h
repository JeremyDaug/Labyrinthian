#ifndef MY_GL_H
#define MY_GL_H
#include <fstream>
#include <GL/glew.h>

struct	ShaderInfo	{
	GLenum			type;
	const char*	file;	};

/* SHADER COMPILING FUNCTIONS */
GLuint	loadShaderProgram(ShaderInfo* shaders);
GLuint	compileShader(ShaderInfo shader);
void	linkProgram(GLuint program);

/* AUXILIARY FUNCTIONS */
void	glOutputError();
void	glOutputFramebufferStatus();
size_t	TypeSize(GLenum type);

/* TEXTURE GENERATORS */
GLuint	genTxt2D(const char* txtFile);
GLuint	genTxt2DArray(const char** txtFiles, int numTxt);
GLuint	genCubeMap(const char** txtFiles);
GLuint	genShadowMapArray(int numTxt);
GLuint	genRandTxt(int x, int y);
GLuint	genRectTxt(const char* txtFile);

#endif
