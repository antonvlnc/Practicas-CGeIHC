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
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	void setEnciende(GLboolean resultado) { Enciende = resultado; }
	GLboolean getEnciende() { return Enciende; }

	GLfloat getarticulacion_capo() { return articulacion_capo; }
	GLfloat getarticulacion_puerta() { return articulacion_puerta; }
	GLfloat getarticulacion_avanza() { return articulacion_avanza; }
	GLfloat getarticulacion_reversa() { return articulacion_reversa; }
	GLfloat getarticulacion_gira_der() { return articulacion_gira_der; }
	GLfloat getarticulacion_gira_izq() { return articulacion_gira_izq; }
	GLfloat getmovimiento_avanza_retrocede() { return avanza_retrocede; }

	GLfloat getmovimiento_helicoptero() { return avanza_retrocede_helicoptero; }
	
	GLfloat getanguK() { return anguK; }


	GLint getlight() { return K; }


	//PARA EL CONTROL DE LAS LUCES CUANDO EL AUTO AVANCE/RETROCEDA
	GLint getLuzDel() { return luz_del; }
	GLint getLuzTras() { return luz_tras; }


	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLint K = 0;
	GLint luz_del = 1;
	GLint luz_tras = 0;
	GLfloat anguK = 0.0f;
	GLfloat muevex, articulacion_capo, articulacion_puerta, articulacion_avanza, articulacion_reversa, articulacion_gira_der, articulacion_gira_izq, avanza_retrocede, avanza_retrocede_helicoptero, avanza_helicoptero, retrocede_helicoptero;
	bool mouseFirstMoved, Enciende;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

