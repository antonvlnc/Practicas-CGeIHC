#include <stdio.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h> //para poder usar rand()
#include <iostream> //para poder usar cout
#include <Windows.h> //para poder usar sleep()

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
int colorAleatorio; //Declaraci�n de la variable para el uso del n�mero aleatorio
float rojo, verde, azul;

int main()
{

	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Ejercicio en clase 1", NULL, NULL); //Las 2 �ltimas banderas siempre se usar�n en NULL, tiene que ver con m�ltiples monitores

	if (!mainWindow) //Si falla la ventana
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	int BufferWidth, BufferHeight; //reciben los 800 por 600 pixeles
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow); //se declara la ventana activa

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}


	//A partir de aqu� comienza la parte de OpenGL

	// Asignar valores de la ventana y coordenadas
	
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);
	printf("Version de Opengl: %s \n",glGetString(GL_VERSION));
	printf("Marca: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("Shaders: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana

		//EJERCICIO EN CLASE 1.1 (modificado)

		Sleep(2000); //Para que el cambio de color no sea tan r�pido

		colorAleatorio = rand() % 3 + 1; //asignaci�n del n�mero aleatorio que dictaminar� el color de la ventana

		std::cout<< "El numero random es " << colorAleatorio<<"\n"; //un cout para saber el n�mero que nos da

		switch (colorAleatorio) {
		case 1: //color rojo
			rojo = 1.0f;
			verde = 0.0f;
			azul = 0.0f;
			break;
		case 2://color azul
			rojo = 0.0f;
			verde = 0.0f;
			azul = 1.0f;
			break;
		case 3://color verde
			rojo = 0.0f;
			verde = 1.0f;
			azul = 0.0f;
			break;
		default://por defecto, la pantalla se pondr� en negro
			rojo = 0.0f;
			verde = 0.0f;
			azul = 0.0f;
			break;
		}

		glClearColor(rojo,verde,azul,1.0f); //Se usa un solo glClearColor
				

		//glClearColor(rojo,verde,azul,1.0f); //la informaci�n de color siempre ser� en RGB, solo cuando se habla de texturizaci�n el alpha puede cambiar el valor de '1' a otro valor
		glClear(GL_COLOR_BUFFER_BIT); 

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}