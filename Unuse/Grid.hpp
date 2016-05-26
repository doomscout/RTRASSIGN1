#pragma once
#define GL_GLEXT_PROTOTYPES
#include "Point3f.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
//#include "Point3f.hpp"
#define BUFFER_OFFSET(i) ((void*)(i))
class Grid
{
public:
	Point3f *vertices;
	GLuint *indices;
//	GLsizei *indicesCounts;
	int size;
	int sizeOfIndices;
	int rows;
	int cols;
//	GLuint sizeOfRows; // for Rendering Object with glMultiDrawElements
//	GLuint sizeOfCols; // for Rendering Object with glMultiDrawElements
	GLuint buffers[2];
	
	Grid(int r, int c);
	//~Grid();
	void initSize(int r, int c);
	void initGrid();
	void initGlenBuffers();
	void unBindBuffers();
	void renderingWithVBO();
	void renderingWithVertexArray();
	void debugPrint();
	void cleanUP();
};
