#pragma once
#include <string>
#include <cstdlib>
#include <GL/glew.h>
#include "MyGL.h"

class ChunkRender
{
private:
	static const ShaderInfo ChunkRenderShaders[];
	static GLuint ShaderProgram;
	static GLuint VBO, EBO, UNI[3], TXT, SMP;	//standard 8x8 grid data
	enum { CR_MMAT, CR_VMAT, CR_TXT };

	float* Mmat;			//model matrix for each chunk
	GLuint DBO, VAO;		//contains values for tile type for specific chunk objects
	int X, Y;				//coordinates of chunk
public:
	ChunkRender();
	ChunkRender(int X, int Y, unsigned char* tile64);
	void draw();
	~ChunkRender();
	static void init();		//compiles the shaders at runtime, and sets up static content.
	static void bind(float* Vmat);		//binds the shader to the opengl context. 
private:
	static void initBuffers();
	static void initProgram();
};

