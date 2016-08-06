#include "MyGL.h"
#include "JPEG_PNG.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <GL/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include "ChunkRender.h"

using namespace std;

float* Vmat;
ChunkRender* tile1;

void initData() {
	ChunkRender::init();
	cout << endl << "initialized ChunkRender System" << endl;
	Vmat = new float[16] {1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
	cout << endl << "created Vmat" << endl;
	unsigned char* tmp = new unsigned char[64];
	for (int i = 0; i < 64; i++)	tmp[i] = i;
	cout << endl << "hello" << endl;
	tile1 = new ChunkRender(0, 0, tmp);
	delete[] tmp;
	cout << endl << "Created ChunkRender Object" << endl;
}
void display() {
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	cout << "coming" << endl;
	ChunkRender::bind(Vmat);
	cout << "hello" << endl;
	tile1->draw();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GL_RGBA | GL_DOUBLEBUFFER);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Testing Phase 1");

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cerr << "unable to initialize glew. Closing program.\n";
		exit(EXIT_FAILURE);
	}

	initData();

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}

/**** SHADER COMPILING FUNCTIONS *****/
/*-----------------------------------*/
GLuint	loadShaderProgram(ShaderInfo* shaders)	{
	GLuint shader, program;
	program = glCreateProgram();
	int index = 0;

	while(shaders[index].type != GL_NONE)	{
		shader = compileShader(shaders[index]);
		glAttachShader(program, shader);
		index++;	}
	linkProgram(program);
	glUseProgram(program);
	return program;		}
/*-----------------------------------*/
GLuint	compileShader(ShaderInfo shaders)	{
		//create shader object
	GLuint shader = glCreateShader(shaders.type);
	if(shader == 0)	{
		fprintf(stderr, "Could not create shader object of type ");
		switch(shaders.type)	{
			case GL_VERTEX_SHADER: fprintf(stderr, "GL_VERTEX_SHADER:\n"); break;
			case GL_FRAGMENT_SHADER: fprintf(stderr, "GL_FRAGMENT_SHADER:\n"); break;
			case GL_TESS_CONTROL_SHADER: fprintf(stderr, "GL_TESS_CONTROL_SHADER:\n"); break;
			case GL_TESS_EVALUATION_SHADER: fprintf(stderr, "GL_TESS_EVALUATION_SHADER:\n"); break;
			case GL_GEOMETRY_SHADER: fprintf(stderr, "GL_GEOMETRY_SHADER:\n"); break;
			default: fprintf(stderr, "%i", shaders.type);	}
		exit(EXIT_FAILURE);	}

		//get shader source code from file
	fstream fin(shaders.file, ios::in | ios::binary);
	if(!fin)	{
		fprintf(stderr, "Could not open file '%s':\n", shaders.file);
		exit(EXIT_FAILURE);	}
	fin.seekg(0, ios::end);
	GLint length = fin.tellg();
	fin.seekg(0, ios::beg);
	GLchar* text = new GLchar[length];
	fin.read(text,length);
	fin.close();
	
		//put data into shader and compile
	glShaderSource(shader, 1, (const GLchar**)&text, &length);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success != GL_TRUE)	{
		GLint infoLogLength;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLogLength);
		GLchar *InfoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type ");
		switch(shaders.type)	{
			case GL_VERTEX_SHADER: fprintf(stderr, "GL_VERTEX_SHADER:\n"); break;
			case GL_FRAGMENT_SHADER: fprintf(stderr, "GL_FRAGMENT_SHADER:\n"); break;
			case GL_TESS_CONTROL_SHADER: fprintf(stderr, "GL_TESS_CONTROL_SHADER:\n"); break;
			case GL_TESS_EVALUATION_SHADER: fprintf(stderr, "GL_TESS_EVALUATION_SHADER:\n"); break;
			case GL_GEOMETRY_SHADER: fprintf(stderr, "GL_GEOMETRY_SHADER:\n"); break;
			default: fprintf(stderr, "%i", shaders.type);	}
		fprintf(stderr, ":\n'%s'\n", InfoLog);
		exit(EXIT_FAILURE);	}

	//clean up
	delete[] text;

	return shader;		}
/*-----------------------------------*/
void	linkProgram(GLuint program)		{
	glLinkProgram(program);
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(success != GL_TRUE)	{
		GLint infoLogLength;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&infoLogLength);
		GLchar *InfoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, NULL, InfoLog);
		fprintf(stderr, "Error linking shader program:\n'%s'\n", InfoLog);
		exit(EXIT_FAILURE);	}	}
/*-----------------------------------*/

/******** AUXILIARY FUNCTIONS ********/
/*-----------------------------------*/
void	glOutputError()		{
	GLenum error = glGetError();
	switch(error)	{
		case GL_NO_ERROR:
			//fprintf(stdout,"GL_NO_ERROR");
			cout << "GL_NO_ERROR" << endl;
			break;
		case GL_INVALID_ENUM:
			//fprintf(stderr,"GL_INVALID_ENUM");
			cout << "GL_INVALID_ENUM" << endl;
			break;
		case GL_INVALID_VALUE:
			//fprintf(stderr,"GL_INVALID_VALUE");
			cout << "GL_INVALID_VALUE" << endl;
			break;
		case GL_INVALID_OPERATION:
			//fprintf(stderr,"GL_INVALID_OPERATION");
			cout << "GL_INVALID_OPERATION" << endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			//fprintf(stderr,"GL_INVALID_FRAMEBUFFER_OPERATION");
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl;
			break;
		case GL_OUT_OF_MEMORY:
			//fprintf(stderr,"GL_OUT_OF_MEMORY");
			cout << "GL_OUT_OF_MEMORY" << endl;
			break;
}	}
/*-----------------------------------*/
void	glOutputFramebufferStatus()		{
	switch(glCheckFramebufferStatus(GL_FRAMEBUFFER))	{
		case GL_FRAMEBUFFER_COMPLETE:
			cout << "GL_FRAMEBUFFER_COMPLETE" << endl;	break;
		case GL_FRAMEBUFFER_UNDEFINED:
			cout << "GL_FRAMEBUFFER_UNDEFINED" << endl;	break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << endl;	break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << endl;	break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << endl;	break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << endl;	break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			cout << "GL_FRAMEBUFFER_UNSUPPORTED" << endl;	break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << endl;	break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << endl;	break;
		case 0:
			cout << "framebuffer is in error" << endl;	break;
}	}
/*-----------------------------------*/
size_t	TypeSize(GLenum type)	{
	size_t size;

	#define CASE(Enum, Count, Type) \
	case Enum: size = Count * sizeof(Type); break
	switch(type)	{
		CASE(GL_FLOAT,				1,	GLfloat);
		CASE(GL_FLOAT_VEC2,			2,	GLfloat);
		CASE(GL_FLOAT_VEC3,			3,	GLfloat);
		CASE(GL_FLOAT_VEC4,			4,	GLfloat);
		CASE(GL_INT, 1, GLint);
		CASE(GL_INT_VEC2, 2, GLint);
		CASE(GL_INT_VEC3, 3, GLint);
		CASE(GL_INT_VEC4, 4, GLint);
		CASE(GL_UNSIGNED_INT, 1, GLuint);
		CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
		CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
		CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
		CASE(GL_BOOL, 1, GLboolean);
		CASE(GL_BOOL_VEC2, 2, GLboolean);
		CASE(GL_BOOL_VEC3, 3, GLboolean);
		CASE(GL_BOOL_VEC4, 4, GLboolean);
		CASE(GL_FLOAT_MAT2, 4, GLfloat);
		CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
		CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
		CASE(GL_FLOAT_MAT3, 9, GLfloat);
		CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
		CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
		CASE(GL_FLOAT_MAT4, 16, GLfloat);
		CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
		CASE(GL_FLOAT_MAT4x3, 12, GLfloat);
		#undef CASE

		default:
			fprintf(stderr, "Unknown type: 0x%x\n", type);
			exit(EXIT_FAILURE);
			break;
	}
	return size;	}
/*-----------------------------------*/

/********* TEXTURE GENERATORS ********/
/*-----------------------------------*/
GLuint	genTxt2D(const char* txtFile) {
	//STEP 0: Read in texture file
	cout << "um..." << endl;
	imageData txt = loadPNG(txtFile);
	cout << "loaded image data" << endl;
	//STEP 1: generate texture buffer object storage
	GLuint TXT;
	glGenTextures(1, &TXT);
	glBindTexture(GL_TEXTURE_2D, TXT);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		txt.width,
		txt.height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		txt.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return TXT;
}
/*-----------------------------------*/
GLuint	genTxt2DArray(const char** txtFiles, int numTxt)	{
	//STEP 1: generate texture buffer object storage
	GLuint	TXT;
	glGenTextures(1, &TXT);
	glBindTexture(GL_TEXTURE_2D_ARRAY, TXT);
	glTexImage3D(GL_TEXTURE_2D_ARRAY,	//type
				0,						//level
				GL_RGBA8,				//internal format
				64,64,numTxt,		//width, height, depth
				0,						//border (must be zero)
				GL_RGBA,				//format (of data going in)
				GL_UNSIGNED_BYTE,		//data type (of data going in)
				NULL);					//data pointer (of data going in)
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//STEP 2:	load texture buffer storage object with texture files
	imageData txt;
	for(int i = 0; i < numTxt; i++)	{
		txt = loadPNG(txtFiles[i]);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,	//type
						0,					//level
						0,0,i,				//(x,y,z) offset
						64,64,1,			//x,y,z size
						GL_RGBA,			//format
						GL_UNSIGNED_BYTE,	//data type
						txt.data);			//data pointer
	}
	return TXT;	}	
/*-----------------------------------*/
GLuint	genCubeMap(const char** txtFiles)	{
	//STEP 1: Load the first image to get the base size
	imageData	txt = loadPNG(txtFiles[0]);
	int width = txt.width, height = txt.height;
	//STEP 2: Generate the texture buffer object storage
	GLuint	TXT;
	glGenTextures(1, &TXT);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TXT);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, txt.data);
	txt = loadPNG(txtFiles[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, txt.data);
	txt = loadPNG(txtFiles[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, txt.data);
	txt = loadPNG(txtFiles[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, txt.data);
	txt = loadPNG(txtFiles[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, txt.data);
	txt = loadPNG(txtFiles[5]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, txt.data);

	return TXT;		}
/*-----------------------------------*/
GLuint	genShadowMapArray(int numTxt)	{
	GLuint	TXT;
	glGenTextures(1, &TXT);
	glBindTexture(GL_TEXTURE_2D_ARRAY, TXT);
	glTexImage3D(GL_TEXTURE_2D_ARRAY,	//type
				0,						//level
				GL_DEPTH_COMPONENT16,	//internal format
				1024,1024,numTxt,			//width, height, depth
				0,						//border (must be zero)
				GL_DEPTH_COMPONENT,		//format (of data going in)
				GL_FLOAT,				//data type (of data going in)
				NULL);					//data pointer (of data going in)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return TXT;		}
/*-----------------------------------*/
GLuint	genRandTxt(int x, int y)	{
	GLuint TXT;
	glGenTextures(1, &TXT);
	glBindTexture(GL_TEXTURE_2D, TXT);
	int numPixels = x*y;
	unsigned char* data = new unsigned char[4*numPixels];
	for(int i = 0; i < 4*numPixels; i++)	{
		data[i] = rand() % 256;	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete[] data;
	return TXT;	}
/*-----------------------------------*/
GLuint	genRectTxt(const char* txtFile)	{
	GLuint TXT;
	glGenTextures(1, &TXT);
	glBindTexture(GL_TEXTURE_RECTANGLE, TXT);
	imageData txt;
	txt = loadPNG(txtFile);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 		//type
				0, 							//level
				GL_RGBA8, 					//internalFormat
				txt.width, txt.height, 0, 	//width, height, border
				GL_RGBA,					//format
				GL_UNSIGNED_BYTE,			//type
				txt.data);					//data
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return TXT;	}
/*-----------------------------------*/
