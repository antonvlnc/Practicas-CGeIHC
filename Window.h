#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getarticulacion_IzqAd() { return articulacion_IzqAd; }
	GLfloat getarticulacion_DerAd() { return articulacion_DerAd; }
	GLfloat getarticulacion_IzqAt() { return articulacion_IzqAt; }
	GLfloat getarticulacion_DerAt() { return articulacion_DerAt; }
	GLfloat getarticulacion_cuello() { return articulacion_cuello; }
	GLfloat getarticulacion_muslo_IzqAd() { return articulacion_muslo_IzqAd; }
	GLfloat getarticulacion_muslo_DerAd() { return articulacion_muslo_DerAd; }
	GLfloat getarticulacion_muslo_IzqAt() { return articulacion_muslo_IzqAt; }
	GLfloat getarticulacion_muslo_DerAt() { return articulacion_muslo_DerAt; }
	GLfloat getarticulacion_pata_IzqAd() { return articulacion_pata_IzqAd; }
	GLfloat getarticulacion_pata_DerAd() { return articulacion_pata_DerAd; }
	GLfloat getarticulacion_pata_IzqAt() { return articulacion_pata_IzqAt; }
	GLfloat getarticulacion_pata_DerAt() { return articulacion_pata_DerAt; }
	GLfloat getarticulacion_colaCerca() { return articulacion_colaCerca; }
	GLfloat getarticulacion_colaLejos() { return articulacion_colaLejos; }




	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion_IzqAd, articulacion_DerAd, articulacion_IzqAt, articulacion_DerAt, articulacion_cuello, articulacion_colaCerca, articulacion_colaLejos, articulacion_muslo_DerAd,articulacion_muslo_DerAt, articulacion_muslo_IzqAd,articulacion_muslo_IzqAt, articulacion_pata_DerAd, articulacion_pata_DerAt, articulacion_pata_IzqAd, articulacion_pata_IzqAt;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

