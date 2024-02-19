#pragma once
#include<glew.h>
class Mesh
{
public:
	Mesh(); //clase tipo mesh
	void CreateMesh(GLfloat *vertices,unsigned int *indices, unsigned int numOfVertices, unsigned int numberOfIndices);
	void RenderMesh();
	void ClearMesh();
	~Mesh();
private:
	GLuint VAO, VBO, IBO; //esto es lo que recibe para nuestro arreglo de índices
	GLsizei indexCount;

};
class MeshColor
{
public:
	MeshColor(); //Clase tipo mesh color
	void CreateMeshColor(GLfloat *vertices, unsigned int numOfVertices);
	void RenderMeshColor();
	void ClearMeshColor();
	~MeshColor();
private:
	GLuint VAO, VBO; //se borró IBO
	GLsizei  vertexCount;
};

