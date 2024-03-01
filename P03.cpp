//práctica 3: Modelado Geométrico y Cámara Sintética.
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
//clases para dar orden y limpieza al còdigo
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

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			0,1,3,
			1,2,3,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-1.0f, -0.5f,0.25f,	//0
		1.0f,-0.5f,0.25f,	//1
		0.0f,1.0f, 0.25f,	//2
		0.0f,0.0f,2.0f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1); //meshList[1] pirámide

}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/

void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
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

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro); //meshList[2] cilindro
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
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

//función para crear pirámide cuadrangular unitaria
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
	meshList.push_back(piramide); //meshList[4] pirámide cuadrangular
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
	mainWindow = Window(1000, 1000);
	mainWindow.Initialise();

	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(50, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();



	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);


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
		//Cámara
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


	


		//CUERPO (TRIÁNGULO NEGRO)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.7f, 1.6f, 1.7f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[1]->RenderMeshGeometry();

		//TRIANGULO ROJO 1 (PUNTA)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.07f, -2.56f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO ROJO 2 (MEDIO IZQUIERDA)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.3f, -0.36f, -2.72f));
		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();


		//TRIANGULO ROJO 3 (MEDIO DERECHA)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.3f, -0.36f, -2.72f));
		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		meshList[4]->RenderMeshGeometry();


		//TRIANGULO ROJO 3 (MEDIO CENTRO)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.36f, -2.725f));
		model = glm::rotate(model, 200 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //voltearlo
		//model = glm::rotate(model,  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO ROJO 4 (ABAJO IZQ )



		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.56f, -0.8f, -2.87f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();


		//TRIANGULO ROJO 5 (ABAJO CENTRO)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -2.87f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO ROJO 5 (ABAJO IZQUIERDA)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.56f, -0.8f, -2.87f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//TRIANGULO ROJO  (ABAJO VOLTEADO IZQ)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.28f, -0.8f, -2.87f));
		model = glm::rotate(model, 200 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //voltearlo
		//model = glm::rotate(model,  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();


		//TRIANGULO ROJO  (ABAJO VOLTEADO DER)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.28f, -0.8f, -2.87f));
		model = glm::rotate(model, 200 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //voltearlo
		//model = glm::rotate(model,  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//  ----------------------  CARA VERDE ---------------------------------------------




		//TRIANGULO verde  (ABAJO IZQ)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.73F, -0.82f, -2.62f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model,  25* toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z

		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();


		//TRIANGULO verde  (ABAJO DER)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.17f, -0.87f, -1.6f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z

		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO verde  (ABAJO CENTRO)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.46f, -0.85f, -2.135f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//TRIANGULO verde  (abajo izq) INVERTIDA

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.62f, -0.82f, -2.36f));

		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 210 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//TRIANGULO verde  (abajo DER) INVERTIDA

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.33f, -0.84f, -1.85f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 210 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();





		//TRIANGULO verde  (MEDIO DER)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.14f, -0.38f, -1.96f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO verde  (MEDIO centro) INVERTIDA

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.31f, -0.34f, -2.2f)); original: X = (-)->menos profundidad, Y= altura
		model = glm::translate(model, glm::vec3(-0.31f, -0.35f, -2.25f));

		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		//model = glm::rotate(model, 206 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); inclinación original
		model = glm::rotate(model, 208 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::rotate(model, -2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//TRIANGULO verde  (MEDIO IZQ)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.45f, -0.36f, -2.48f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();


		//TRIANGULO verde  (punta)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::translate(model, glm::vec3(-0.12f, 0.1f, -2.33f));
		model = glm::translate(model, glm::vec3(-0.11f, 0.1f, -2.33f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model,1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		/*-----------------------------------CARA AZUL---------------------------------------*/
		/*
		
		*************************************************************************************


		*/





		//TRIANGULO AZUL  (punta)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.1f, 0.1f, -2.35f));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO AZUL  (MEDIO IZQ)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.1f, -0.36f, -1.98f));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 22 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO AZUL  (MEDIO der)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.4f, -0.36f, -2.5F));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 22 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();





		//TRIANGULO AZUL  (MEDIO CENTRO) INVERTIDO

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		//model = glm::translate(model, glm::vec3(0.3f, -0.36f, -2.225F)); ORIGINAL
		model = glm::translate(model, glm::vec3(0.25f, -0.36f, -2.225F));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 22 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();





		//TRIANGULO AZUL  (ABAJO DER)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.67f, -0.82f, -2.68F));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 22 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();






		//TRIANGULO AZUL  (ABAJO DERECHO) INVERTIDO

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.54f, -0.82f, -2.435F));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 22 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -182 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//TRIANGULO AZUL  (ABAJO izq) INVERTIDO

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.24f, -0.83f, -1.89F));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 22 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();







		//TRIANGULO AZUL  (ABAJO centro)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.37f, -0.87f, -2.17F));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 22 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, -3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//TRIANGULO AZUL  (ABAJO izq)

		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		model = glm::translate(model, glm::vec3(0.14f, -0.84f, -1.62F));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 24 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		model = glm::rotate(model, 1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();





		/*
		
		
		--------------------------------------- CARA AMARILLA ----------------------------------------------------
		---------------------------------------------------------------------------------------------------------
		-------------------------------------------------------------------------------------------------------
		
		
		*/



		//X empuja hacia la punta de rojo y verde, junto con Z; + ACERCA A LA PUNTA (SUBE)

		//Y es la profundidad, no mover
		
		//Z es para mover a los lados, junto con X; + HACERCA A LA IZQUIERDA


		//TRIANGULO amarillo (PUNTA)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.75f, -1.1f, -2.78F)); //y es la profundidad
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 83 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación 90 ORIGINAL

		//model = glm::rotate(model, 1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();





		//TRIANGULO amarillo (MEDIO IZQ)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.1f, -1.12f, -2.75F)); //y es la profundidad
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		//model = glm::rotate(model, 1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();






		//TRIANGULO amarillo (medio centro) INVERTIDA

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.27f, -1.128f, -2.5F)); //y es la profundidad
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 93 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación 90 RIGINAL

		model = glm::rotate(model, -2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();









		//TRIANGULO amarillo (MEDIO DEDRECHA)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.399f, -1.16f, -2.22F)); //y es la profundidad
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo  240,90

		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		//model = glm::rotate(model, -5 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();




		//TRIANGULO amarillo (abajo DERECHA)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(-0.02f, -1.18f, -1.68F)); //y es la profundidad
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo, 240

		model = glm::rotate(model, 86 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		//model = glm::rotate(model, 1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();



		//TRIANGULO amarillo (abajo CENTRO)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.25f, -1.15f, -2.2F)); //y es la profundidad
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo 240 original,260

		model = glm::rotate(model, 85 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación 90 original

		//model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z -2 ORIGINAL (PARA ALINEAR CON EL EDGE DE LA PIRAMIDE),45


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();






		//TRIANGULO amarillo (abajo izq) INVERTIDA

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.365f, -1.15f, -2.45F)); //y es la profundidad
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 93 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		//model = glm::rotate(model, 2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();





		//TRIANGULO amarillo (abajo DER) INVERTIDA

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.11f, -1.157f, -1.955F)); //y es la profundidad
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo  60 original, 93

		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		//model = glm::rotate(model, -2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();








		//TRIANGULO amarillo (abajo IZQ)

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 0.0f);

		model = glm::translate(model, glm::vec3(0.53f, -1.095f, -2.72F)); //y es la profundidad
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //voltearlo

		model = glm::rotate(model, 83 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); //inclinación

		//model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //Ligera rotación en el eje Z  //40,280,90


		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		meshList[4]->RenderMeshGeometry();












		/*
		//ejercicio: Instanciar primitivas geométricas para recrear el dibujo de la práctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.
		model = glm::mat4(1.0f);
		color=glm::vec3(0.0f,1.0f,0.0f);
		//Opcional duplicar esta traslación inicial para posicionar en -Z a los objetos en el mismo punto
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


