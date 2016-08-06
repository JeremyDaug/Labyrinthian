#include "ChunkRender.h"
#include <iostream>

using namespace std;

GLuint ChunkRender::EBO, ChunkRender::VBO, ChunkRender::UNI[3], ChunkRender::ShaderProgram, ChunkRender::TXT;
const ShaderInfo ChunkRender::ChunkRenderShaders[] = { { GL_VERTEX_SHADER, "../ChunkRender.vert" },
{ GL_FRAGMENT_SHADER, "../ChunkRender.frag" },
{ GL_NONE, "" } };

ChunkRender::ChunkRender()
{
}

ChunkRender::ChunkRender(int X, int Y, unsigned char* tile64) {
	cout << "Mmat started" << endl;
	Mmat = new float[16]{ 1.0f,0.0f,0.0f,1.0f*X,
						0.0f,1.0f,0.0f,1.0f*Y,
						0.0f,0.0f,1.0f,0.0f,
						0.0f,0.0f,0.0f,1.0f };
	cout << "Mmat done" << endl;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	cout << "starting arrays" << endl;

	glGenBuffers(1, &DBO);
	glBindBuffer(GL_ARRAY_BUFFER, DBO);
		//there are 9x9 vertices to create 8x8 tiles. This spaces out the values to the right positions for reference
	unsigned char* cpy = new unsigned char[81];
	for(int i = 0; i < 8; i++)	{
		for (int j = 0; j < 8; j++) {
			cpy[9 * i + j] = tile64[8 * i + j];
		}
	}
	cout << "reallocated data" << endl;
	glBufferData(GL_ARRAY_BUFFER, 81 * sizeof(unsigned char), cpy, GL_STATIC_DRAW);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
		//clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] cpy;
}

void ChunkRender::draw() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glUniformMatrix4fv(UNI[CR_MMAT], 1, GL_FALSE, Mmat);
	glDrawElements(GL_TRIANGLES, 6 * 64, GL_UNSIGNED_SHORT, NULL);	//type must be unsigned.
}


ChunkRender::~ChunkRender()
{
	glDeleteBuffers(1, &DBO);
	glDeleteVertexArrays(1, &VAO);
	delete[] Mmat;
}

void ChunkRender::bind(float* Vmat) {
	glUseProgram(ShaderProgram);
	glUniformMatrix4fv(UNI[CR_VMAT], 1, GL_FALSE, Vmat);
	glUniform1i(UNI[CR_TXT], 0);	//link to sampler at texture unit zero
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TXT);
}

void ChunkRender::init() {
	initProgram();
	cout << "initializing buffers";
	initBuffers();
	cout << "initialized buffers";
}
void ChunkRender::initProgram()
{
	ShaderProgram = loadShaderProgram((ShaderInfo*)ChunkRenderShaders);
	UNI[CR_MMAT] = glGetUniformLocation(ShaderProgram, "Mmat");
	UNI[CR_VMAT] = glGetUniformLocation(ShaderProgram, "Vmat");
	UNI[CR_TXT] = glGetUniformLocation(ShaderProgram, "tileMap");

	cout << "does this work?" << endl;
	TXT = genTxt2D("../KirbyTiles.png");
	cout << "yes" << endl;

}
void ChunkRender::initBuffers()
{
	float* Vdata = new float[9 * 9 * 2];	//8x8 quads is defined by 9x9 grid of 2 dimensional vectors
	int idx = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			Vdata[idx++] = 0.125f*j;
			Vdata[idx++] = 0.125f*i;
		}
	}
	short* Edata = new short[6*64];	//indexed rendering mode utilized.
	idx = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Edata[idx++] = 9 * i + j;
			Edata[idx++] = 9 * i + 1 + j;
			Edata[idx++] = 9 * i + 10 + j;
			Edata[idx++] = 9 * i + j;
			Edata[idx++] = 9 * i + 10 + j;
			Edata[idx++] = 9 * i + 9 + j;
		}
	}

	//place data in GL buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * 9 * 2 * sizeof(float), Vdata, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 19 * 8 * sizeof(short), Edata, GL_STATIC_DRAW);

	//clear bindings (unlinks from GL engine state) and clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] Vdata;
	delete[] Edata;
}
