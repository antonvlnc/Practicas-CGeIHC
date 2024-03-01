//pr�ctica 3: Modelado Geom�trico y C�mara Sint�tica.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";


Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36); //vertices, indices
	meshList.push_back(cubo); //meshList[0]=cubo
}

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1); //meshList[1] pir�mide

}
/*
Crear cilindro, cono y esferas con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/

void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro); //meshList[2] cilindro
}

//funci�n para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los v�rtices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono); //meshList[3] cono
}

//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide); //meshList[4] pir�mide cuadrangular
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(50, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CreateShaders();



	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform; , RGB??

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


		////PISO (CUBO BLANCO)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 1.0f, 1.0f);

		//model = glm::translate(model, glm::vec3(0.0f, -1.25f, -3.0f));

		//model = glm::scale(model, glm::vec3(5.0f, 0.0005f, 5.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();


	


		//CUERPO (TRI�NGULO NEGRO)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.4f, 2.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[1]->RenderMesh();

		////TRIANGULO ROJO 1 (PUNTA)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.0f, -0.12f, -2.8f));
		//model = glm::rotate(model, 32 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();



		////TRIANGULO ROJO 2 (MEDIO IZQUIERDA)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.26f, -0.54f, -2.58f));
		//model = glm::rotate(model, 32 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();


		////TRIANGULO ROJO 3 (MEDIO DERECHA)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.26f, -0.54f, -2.58f));
		//model = glm::rotate(model, 32 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();



		////TRIANGULO ROJO 3 (MEDIO CENTRO)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.0f, -0.54f, -2.58f));
		//model = glm::rotate(model, 155 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //voltearlo
		////model = glm::rotate(model,  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinaci�n
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();



		////TRIANGULO ROJO 4 (ABAJO DERECHA )



		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.48f, -0.95f, -2.335f));
		//model = glm::rotate(model, 30 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();


		////TRIANGULO ROJO 5 (ABAJO CENTRO)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.0f, -0.95f, -2.335f));
		//model = glm::rotate(model, 30 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();



		////TRIANGULO ROJO 5 (ABAJO IZQUIERDA)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.48f, -0.95f, -2.335f));
		//model = glm::rotate(model, 30 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();




		////TRIANGULO ROJO  (ABAJO VOLTEADO DERECHA)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.235f, -0.94f, -2.3f));
		//model = glm::rotate(model, 155 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //voltearlo
		////model = glm::rotate(model,  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinaci�n
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();


		////TRIANGULO ROJO  (ABAJO VOLTEADO IZQUIERDA)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.235f, -0.94f, -2.3f));
		//model = glm::rotate(model, 155 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //voltearlo
		////model = glm::rotate(model,  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinaci�n
		//model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();





		////TRIANGULO ROJO 3 (CENTRI VOLTEADO)

		//model = glm::mat4(1.0);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.55f));
		//model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, -35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		//meshList[4]->RenderMeshGeometry();




		////VENTANA IZQUIERDA FRONTAL (CUBO VERDE)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.25f, -0.12f, -2.4f));

		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.08f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();



		////VENTANA DERECHA  FRONTAL (CUBO VERDE)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.25f, -0.12f, -2.4f));

		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.08f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();


		////VENTANA IZQUIERDA CARA DERECHA (CUBO VERDE)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.6f, -0.12f, -2.7f));

		//model = glm::scale(model, glm::vec3(0.08f, 0.4f, 0.4f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();


		////VENTANA DERECHA  CARA DERECHA (CUBO VERDE)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.6f, -0.12f, -3.3f));

		//model = glm::scale(model, glm::vec3(0.08f, 0.4f, 0.4f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();


		////VENTANA IZQUIEDA  CARA IZQUIERDA (CUBO VERDE)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.6f, -0.12f, -3.3f));

		//model = glm::scale(model, glm::vec3(0.08f, 0.4f, 0.4f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();



		////VENTANA DERECHA CARA IZQUIERDA (CUBO VERDE)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.6f, -0.12f, -2.7f));

		//model = glm::scale(model, glm::vec3(0.08f, 0.4f, 0.4f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();


		////PUERTA (CUBO VERDE)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(0.0f, -0.92f, -2.4f));

		//model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.08f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[0]->RenderMesh();


		////TRONCO DERECHA (CUBO CAF�)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.478f, 0.255f, 0.067f);

		//model = glm::translate(model, glm::vec3(1.2f, -1.0f, -3.0f));

		//model = glm::scale(model, glm::vec3(0.12f, 0.45f, 0.12f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[2]->RenderMeshGeometry();



		////TRONCO IZQUIERDA (CILINDRO CAF�)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.478f, 0.255f, 0.067f);

		//model = glm::translate(model, glm::vec3(-1.2f, -1.0f, -3.0f));

		//model = glm::scale(model, glm::vec3(0.12f, 0.45f, 0.12f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[2]->RenderMeshGeometry();



		////HOJAS IZQUIERDA (CONO VERDE OSCURO)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 0.5f, 0.0f);

		//model = glm::translate(model, glm::vec3(-1.2f, -0.6f, -3.0f));

		//model = glm::scale(model, glm::vec3(0.15f, 0.7f, 0.15f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[3]->RenderMeshGeometry();



		////HOJAS DERECHA (CONO VERDE OSCURO)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 0.5f, 0.0f);

		//model = glm::translate(model, glm::vec3(1.2f, -0.6f, -3.0f));

		//model = glm::scale(model, glm::vec3(0.15f, 0.7f, 0.15f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//meshList[3]->RenderMeshGeometry();


		////VENTANA TRASERA (CILINDRO AZUL)

		//model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 0.0f, 1.0f);

		//model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.6f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.3f, 0.05f, 0.3f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//sp.render();








		/*
		//ejercicio: Instanciar primitivas geom�tricas para recrear el dibujo de la pr�ctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.
		model = glm::mat4(1.0f);
		color=glm::vec3(0.0f,1.0f,0.0f);
		//Opcional duplicar esta traslaci�n inicial para posicionar en -Z a los objetos en el mismo punto
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();
		*/

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


