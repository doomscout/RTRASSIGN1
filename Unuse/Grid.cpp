#include "Grid.hpp"

//Construction class

Grid::Grid(int r, int c)
{
	if(r <= 1)
	{
		r = 2;
	}
	
	if(c <= 2)
	{
		c = 2;
	}
	
	rows = r;
	cols = c;
	
	#if 0
	sizeOfCols = c - 1;
	sizeOfRows = 2 * r;
	indicesCounts = new GLsizei[(int)sizeOfCols];
	for(int i = 0; i < c - 1; i++)
	{
		indicesCounts[i] = sizeOfRows;
	}
	#endif
	Grid::initSize(rows, cols);
	Grid::initGrid();
}
#if 0
Grid::~Grid()
{
	delete[] vertices;
	delete[] indices;
	//delete[] indicesCounts;
}
#endif
//initialization grid's size
void Grid::initSize(int r, int c)
{
	size = r * c;
}

//initialization vertices and indices for grid 

void Grid::initGrid()
{
	
	vertices = new Point3f[size];
	GLfloat rowStep = 2.0f / (float)(rows - 1);
	GLfloat colStep = 2.0f / (float)(cols - 1);
	
	int loop = 0;
	for (int i = 0; i < cols; i++)
	{
		GLfloat x = i * colStep - 1;

		for (int j = 0; j < rows; j++)
		{
			GLfloat y = j * rowStep - 1;

			vertices[loop].setX(x);
			vertices[loop].setY(y);
			vertices[loop].setZ(0);
			loop++;
		}
	}
	
	#if 1
	sizeOfIndices = 2 * rows * (cols - 1);
	indices = new GLuint[sizeOfIndices];
	
	
	int count = 0;

	for (int i = 0; i < cols - 1; i++)
	{
		int ii = i * (rows);
		for (int j = 0; j < rows; j++)
		{
			indices[count] = ii + j;
			count++;
			indices[count] = rows + j + ii;
			count++;
		}
	}
	#endif
	
	#if 0 
	//int numStripRequired = cols - 1;
	//int numDegensRequired = 2 * (numStripRequired - 1);
	//int verticesPerStrip = 2 * cols;
	//sizeOfIndices = numStripRequired * numDegensRequired + verticesPerStrip;
	sizeOfIndices = 2 * rows * (cols - 1);
	indices = new GLuint[sizeOfIndices];
	int offset = 0;
	int lt = 1;
	for(int i = 0; i < cols; i++)
	{
		//indices[]
		for(int j = 0; j < rows; j++)
		{
			indices[offset++] = i * cols + j;
			indices[offset++] = (i+1) * cols + j;
		}
	}
	#endif 
	//Grid::debugPrint();
}

//printout for console for debugging
void Grid::debugPrint()
{
	std::cout << "Grid size :" << size << std::endl;
	std::cout << "Grid's indices size :" << sizeOfIndices << std::endl;
	std::cout << "Rows :" << rows << std::endl;
	std::cout << "cols :" << cols << std::endl;
	/*
	for (int i = 0; i < size; i++)
	{
		std::cout << "arrayOfVertex[" << i << "]s value is " << vertices[i].getX()  << " "<<
			vertices[i].getY() << " " << vertices[i].getZ() << " " << std::endl;
	}
	
	for (int i = 0; i < sizeOfIndices; i++)
	{
		std::cout << "indices[" << i << "]'s value : " << indices[i] << std::endl;
	}
	
	
	for(int i = 0; i < (int )sizeOfCols; i++)
	{
		std::cout << "indicesCounts[" << i <<"]'s " <<  indicesCounts[i] << std::endl;
	}
	
	std::cout <<" sizeOfRows: " << sizeOfRows << std::endl;
	std::cout <<" sizeOfCols: " << sizeOfCols << std::endl;
	* */
}

//init VBO buffer
void Grid::initGlenBuffers()
{
	    /* Clear old buffers if necessary */
    if (buffers != NULL) {
        glDeleteBuffers(2, buffers);
    }
    
    glGenBuffers(2, buffers);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(Point3f), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
    
    Grid::unBindBuffers();
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Grid::unBindBuffers()
{
  int buffer;

  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffer);
  if (buffer != 0)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffer);
  if (buffer != 0)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Grid::renderingWithVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, sizeof(Point3f), BUFFER_OFFSET(0));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	for (int i = 0; i < cols - 1; i++)
	{
		//int indexa = i * (rows + 1) * 2;
		//cout << "Draw index with VA : " << indexa << endl; 
		glDrawElements(GL_TRIANGLE_STRIP, (rows) * 2, GL_UNSIGNED_INT,  BUFFER_OFFSET(i * (rows) * 2 * sizeof(GLuint)));
	}
	//glVertexPointer(3, GL_FLOAT, 0, vertices, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	Grid::unBindBuffers();
}

void Grid::renderingWithVertexArray()
{
	#if 1
	int c = cols - 1;
	int r = rows - 1;
	//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), structVert);
	glVertexPointer(3, GL_FLOAT, sizeof(Point3f), vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	//from following lab 2
	for (int i = 0; i < c; i++)
	{
		int indexa = i * (r + 1) * 2;
		//cout << "Draw index with VA : " << indexa << endl; 
		glDrawElements(GL_TRIANGLE_STRIP, (r + 1) * 2, GL_UNSIGNED_INT, &indices[indexa]);
			
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	#endif
}

//Clean up memory
void Grid::cleanUP()
{
	if(indices != NULL)
		delete[] indices;
	if(vertices != NULL)
		delete[] vertices;
  //  delete[] indicesCounts;
}
