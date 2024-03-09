/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
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
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
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
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
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
	meshList.push_back(cilindro);
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
	meshList.push_back(cono);
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
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(8, 1.0f);//índice 2 en MeshList
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
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía



	glm::mat4 modelaux2(1.0); //para la otra rama (llanta 1 o todas las llantas??? creo que cada llanta debe tener su propia)

	glm::mat4 modelaux_llanta_der_adelante(1.0);
	glm::mat4 modelaux_llanta_izq_adelante(1.0);

	glm::mat4 modelaux_llanta_der_atras(1.0);
	glm::mat4 modelaux_llanta_izq_atras(1.0);


	glm::mat4 modelaux_cuello(1.0);
	glm::mat4 modelaux_colaCerca(1.0);
	glm::mat4 modelaux_colaLejos(1.0);

	glm::mat4 modelaux_muslo_IzqAd(1.0);
	glm::mat4 modelaux_muslo_DerAd(1.0);

	glm::mat4 modelaux_muslo_IzqAt(1.0);
	glm::mat4 modelaux_muslo_DerAt(1.0);

	glm::mat4 modelaux_pata_IzqAd(1.0);
	glm::mat4 modelaux_pata_DerAd(1.0);

	glm::mat4 modelaux_pata_IzqAt(1.0);
	glm::mat4 modelaux_pata_DerAt(1.0);

	glm::mat4 modelaux_cabeza(1.0);

	glm::mat4 modelaux_orejaI(1.0);
	glm::mat4 modelaux_orejaD(1.0);

	glm::mat4 modelaux_hocico(1.0);











	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();



		model = glm::mat4(1.0);



		//CÓDIGO PARA EL ANIMAL ROBÓTICO

		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -4.0f)); //POSICIONAMIENTO INICIAL


		//PARA DECLARAR DESPUÉS (COMO LAS LLANTEAS EN LA GRÚA)

		/*modelaux_cuello = model;
		modelaux_muslo_IzqAd = model;
		modelaux_muslo_DerAd = model;
		modelaux_muslo_IzqAt = model;
		modelaux_muslo_DerAt = model;
		modelaux_colaCerca = model;
		modelaux_colaLejos = model;
		//uno modelaux_cuello para mover la cabeza??
		*/




		modelaux = model;








		//CUERPO

		/*modelaux_cuello = modelaux;*/


		model = glm::scale(model, glm::vec3(8.0f, 5.0f, 5.0f)); //cabina = cubo

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.5f, 0.5f, 0.5f); //cuerpo-> gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();


		model = modelaux; //para no heredar la escala

		//DIBUJAMOS UNA PEQUEÑA ESFERA BLANCA PARA LA ARTICULACIÓN DE LA PARTE CERCANA DE LA COLA











		model = modelaux;

		//COLA CERCA

		modelaux_colaCerca = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, 1.25f, 0.0f)); //PRIMERA ARTICULACIÓN (PARA BRAZO 1)
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_colaCerca()), glm::vec3(0.0f, 1.0f, 0.0f));


		modelaux_colaCerca = model; //COMENTAR

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //ESFERA BLANCA PARA COLA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();


		model = modelaux_colaCerca;





		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f)); //posición de la esfera en la cola

		modelaux_colaCerca = model;





		//modelaux_colaCerca = model; //hasta acá se guarda la información del modelo, para que no afecte la escala de nuestro brazo 1


		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f)); //hacer laega la cola

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //Primer brazo -> rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_colaCerca;












		//COLA LEJANA

		//PATA IZQ ADELANTE (DEBE ESTAR ROTADA)


		modelaux_colaLejos = modelaux_colaCerca;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_colaLejos()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		//model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_colaLejos = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 0.0f, 1.0f); //ESFERA ROJA PARA PATA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_colaLejos;


		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); //posición de la articulación en la cola

		//modelaux_colaLejos = model;


		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //cuerpo-> verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_colaLejos;


















		model = modelaux;
		//CUELLO (CON ARTICULACIONES)

		




		modelaux_cuello = modelaux;


		model = glm::translate(model, glm::vec3(-4.0f, 1.5f, 0.0f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_cuello()), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux_cuello = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //ESFERA bklnca PARA COLA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_cuello;


		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));

		modelaux_cuello = model;


		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); //PATA CERCANA = 3

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f); //cuerpo-> gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_cuello;






		//model = modelaux;
		//CABEZA


		modelaux_cabeza = modelaux_cuello;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));//POSICIÓN
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_cabeza = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_cabeza;


		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f)); //con respecto al cuello

		modelaux_cabeza = model;


		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); //CABEZA=DE RADIO 2

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //cuerpo-> verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_cabeza;














		///----------------visor---------- (esfera)






		modelaux_hocico = modelaux_cabeza;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));//POSICIÓN
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_hocico = model;

		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//color = glm::vec3(1.0f, 1.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render();

		//model = modelaux_hocico;


		model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f));

		modelaux_hocico = model;


		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f)); //visor

		//AQUI VA EL CONO (INDIC 3)


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 0.0f); //cuerpo-> verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		model = modelaux_hocico;



		//model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f)); //hacerlo largo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //llanta -> gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro y cono
























		 
		//OREJA IZQUIERDA (CONO


		modelaux_orejaI = modelaux_cabeza;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.5f, 1.0f, -1.0f));//POSICIÓN
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_orejaI = model;

		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//color = glm::vec3(1.0f, 1.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render();

		//model = modelaux_orejaI;


		model = glm::translate(model, glm::vec3(-0.25f, 1.0f, 0.5f));

		modelaux_orejaI = model;


		model = glm::scale(model, glm::vec3(0.4f,0.8f, 0.4f)); //CABEZA=DE RADIO 2

		//AQUI VA EL CONO (INDIC 3)


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //cuerpo-> verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[3]->RenderMeshGeometry();

		model = modelaux_orejaI;

























		//OREJA derecha (CONO


		modelaux_orejaD = modelaux_cabeza;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.2f, -1.0f, 1.5f));//POSICIÓN
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_orejaD = model;

		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//color = glm::vec3(1.0f, 1.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render();

		//model = modelaux_orejaD;


		model = glm::translate(model, glm::vec3(-0.25f, 1.0f, -0.5f));

		modelaux_orejaD = model;


		model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f)); //CABEZA

		//AQUI VA EL CONO (INDIC 3)


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(.0f, 1.0f, 0.0f); //cuerpo-> verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[3]->RenderMeshGeometry();

		model = modelaux_orejaD;




































		












		



















		model = modelaux;
		//MUSLO IZQUIERDO DE ADELANTE


		modelaux_muslo_IzqAd = modelaux;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.5f, -2.5f, 1.5f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_muslo_IzqAd()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_muslo_IzqAd = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //ESFERA VERDE PARA COLA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_muslo_IzqAd;


		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));

		modelaux_muslo_IzqAd = model;


		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f)); //PATA CERCANA = 3

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //cuerpo-> gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_muslo_IzqAd;




















		//PATA IZQ ADELANTE (DEBE ESTAR ROTADA)


		modelaux_pata_IzqAd = modelaux_muslo_IzqAd;

		
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));//POSICIÓN relativa al muslo
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_pata_IzqAd()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_pata_IzqAd = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_pata_IzqAd;


		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f)); //posición de la bola en la pata

		modelaux_pata_IzqAd = model;


		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); //PATA CERCANA = 2

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 1.0f); //cuerpo-> verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_pata_IzqAd;



		














		model = modelaux;
		//MUSLO DERECHO DE ADELANTE


		modelaux_muslo_DerAd = modelaux;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.5f, -2.5f, -1.5f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_muslo_DerAd()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_muslo_DerAd = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //ESFERA VERDE PARA COLA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_muslo_DerAd;


		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));

		modelaux_muslo_DerAd = model;


		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f)); //PATA CERCANA = 3

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //pata -> azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_muslo_DerAd;










		//PATA DER ADELANTE (DEBE ESTAR ROTADA)


		modelaux_pata_DerAd = modelaux_muslo_DerAd;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_pata_DerAd()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_pata_DerAd = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_pata_DerAd;


		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));

		modelaux_pata_DerAd = model;


		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); //PATA CERCANA = 2

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 1.0f); //cuerpo-> verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_pata_DerAd;



























		model = modelaux;
		//MUSLO DERECHO DE atras


		modelaux_muslo_DerAt = modelaux;

		//ARTICULACIÓN MUSLO derecho atras
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.5f, -2.5f, -1.5f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_muslo_DerAt()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_muslo_DerAt = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //ESFERA VERDE PARA COLA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_muslo_DerAt;


		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));

		modelaux_muslo_DerAt = model;


		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f)); //PATA CERCANA = 3

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //pata -> azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_muslo_DerAt;










		//PATA DER ATRAS (DEBE ESTAR ROTADA)


		modelaux_pata_DerAt = modelaux_muslo_DerAt;

		//ARTICULACIÓN MUSLO IZQUIERDA ENFRENTE
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_pata_DerAt()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_pata_DerAt = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_pata_DerAt;


		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));

		modelaux_pata_DerAt = model;


		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); //PATA CERCANA = 2

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 1.0f); //cuerpo-> cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_pata_DerAt;





































		model = modelaux;
		//MUSLO izquierdo DE atras


		modelaux_muslo_IzqAt = modelaux;

		//ARTICULACIÓN MUSLO derecho atras
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.5f, -2.5f, 1.5f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_muslo_IzqAt()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_muslo_IzqAt = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //ESFERA VERDE PARA COLA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_muslo_IzqAt;


		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));

		modelaux_muslo_IzqAt = model;


		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f)); //PATA CERCANA = 3

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //pata -> azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_muslo_IzqAt;













		//PATA Izq ATRAS (DEBE ESTAR ROTADA)


		modelaux_pata_IzqAt = modelaux_muslo_IzqAt;

		
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));//POSICIÓN
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_pata_IzqAt()), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE GIRE AHÍ
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //PARA QUE EL PRISMA ESTÉ VERTICAL


		modelaux_pata_IzqAt = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //ESFERA ROJA PARA PATA LEJANA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = modelaux_pata_IzqAt;


		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));

		modelaux_pata_IzqAt = model;


		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); //PATA CERCANA = 2

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 1.0f); //cuerpo-> cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		model = modelaux_pata_IzqAt;


































































































































		//CÓDIGO PARA LA GRÚA








		//model = glm::translate(model, glm::vec3(1.0f, 1.0f, -4.0f)); //POSICIONAMOS TODO EN 1,1,-4

		//modelaux2 = model;

		//


		//modelaux = model;

		//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f)); //cabina = cubo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 1.0f, 1.0f); //cuerpo-> blanco
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();


		//model = modelaux; //para no heredar la escala




		////modelaux = model;
		//model = glm::translate(model, glm::vec3(-2.5f, 2.5f, 0.0f)); //PRIMERA ARTICULACIÓN (PARA BRAZO 1)

		//modelaux = model; //COMENTAR

		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//color = glm::vec3(0.0f, 0.0f, 1.0f); //ESFERA AZUL
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render();


		//model = modelaux;


		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f)); //para controlar la rotación con el teclado

		//model = glm::translate(model, glm::vec3(-1.8f, 1.8f, 0.0f)); //Se mueve el primer brazo un poco a la izquierda y arriba, para que entre la cabina proximamente, 2,2
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Rotación vista en clase, 135 grados en el plano Z
		//modelaux = model; //hasta acá se guarda la información del modelo, para que no afecte la escala de nuestro brazo 1
		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f)); //hacerlo largo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f); //Primer brazo -> rojo
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();




		//model = modelaux;






		//model = modelaux2;





		////ARTICULACIÓN PARA LA BASE (PIRÁMIDE CUADRANGULAR)

		//model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f)); //ESTAMOS TOMANDO DEL MISMO NODO PADRE

		//modelaux2 = model; //AHORA guardamos esta info en una nueva matriz auxiliar 2

		//modelaux_llanta_der_adelante = modelaux2; //para los ejes de las llantas de adelante (izquierda en el plano xy)
		//modelaux_llanta_izq_adelante = modelaux2; //para ligarlo al nodo padre (base)

		//modelaux_llanta_der_atras = modelaux2; //para los ejes (articulaciones de atrás) (a la derecha en el plano xy)
		//modelaux_llanta_izq_atras = modelaux2;
		//



		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//color = glm::vec3(0.0f, 1.0f, 1.0f); //ESFERA CYAN ESTÁ MUY ATRÁS
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render();

		//model = modelaux2;


		////model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f)); //para controlar la rotación con el teclado

		////model = glm::translate(model, glm::vec3(-f, 1.8f, 0.0f)); //Se mueve el primer brazo un poco a la izquierda y arriba, para que entre la cabina proximamente, 2,2
		////model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Rotación vista en clase, 135 grados en el plano Z
		//modelaux2 = model; //hasta acá se guarda la información del modelo, para que no afecte la escala de nuestro brazo 1
		//model = glm::scale(model, glm::vec3(6.0f, 2.0f, 5.0f)); //hacerlo largo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f); //Primer brazo -> rojo
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[4]->RenderMesh();










		//model = modelaux_llanta_izq_adelante;


		////-------------------LLANTAS------------------------------

		//

		////ARTICULACION PARA LLANTA IZQ DE ADELANTE

		//model = glm::translate(model, glm::vec3(-2.5f, -1.5f, 2.0f)); //ESTAMOS TOMANDO DEL MISMO NODO PADRE
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_IzqAd()), glm::vec3(0.0f, 0.0f, 1.0f)); //1 en z para que giren (llantas delanteras) , 1 en y para que avance (llantas traseras, tracción trasera)

		//modelaux_llanta_izq_adelante=model;

		//model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f)); //hacerlo largo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //llanta -> gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro y cono

		//model = modelaux2;













		////LLANTA DERECHA DE ENFRENTE


		//model= modelaux_llanta_der_adelante;

		//model = glm::translate(model, glm::vec3(-2.5f, -1.5f, -2.0f)); //ESTAMOS TOMANDO DEL MISMO NODO PADRE
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_DerAd()), glm::vec3(0.0f, 0.0f, 1.0f)); //1 en z para que giren (llantas delanteras) , 1 en y para que avance (llantas traseras, tracción trasera)

		//modelaux_llanta_der_adelante = model;

		//model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f)); //hacerlo largo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //llanta -> gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro y cono

		//model = modelaux2;






		////LLANTA DERECHA DE ATRAS


		//model = modelaux_llanta_der_atras;

		//model = glm::translate(model, glm::vec3(2.5f, -1.5f, -2.0f)); //ESTAMOS TOMANDO DEL MISMO NODO PADRE
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_DerAt()), glm::vec3(0.0f, 1.0f, 0.0f)); //1 en z para que giren (llantas delanteras) , 1 en y para que avance (llantas traseras, tracción trasera)

		//modelaux_llanta_der_atras = model;

		//model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f)); //hacerlo largo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //llanta -> gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro y cono

		//model = modelaux2;





		////LLANTA IZQUIERDA DE ATRAS


		//model = modelaux_llanta_izq_atras;

		//model = glm::translate(model, glm::vec3(2.5f, -1.5f, 2.0f)); //ESTAMOS TOMANDO DEL MISMO NODO PADRE
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_IzqAt()), glm::vec3(0.0f, 1.0f, 0.0f)); //1 en z para que giren (llantas delanteras) , 1 en y para que avance (llantas traseras, tracción trasera)

		//modelaux_llanta_izq_atras = model;

		//model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f)); //hacerlo largo

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		////se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //llanta -> gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro y cono

		//model = modelaux2;



		//model = modelaux;



		////articulación 2 (NUEVO PUNTO DE REFERENCIA)
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f)); //se pone la mitad de la longitud del cuerpo al que está conectado
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f)); //para controlar la rotación con el teclado


		//modelaux = model;


		////dibujar una pequeña esfera en el extremo más lejano del origen
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render();

		//model = modelaux;

		////segundo brazo
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		//modelaux = model;
		//model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		//model = modelaux; //reinicio [  SI

		////articulación 3 extremo derecho del segundo brazo
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f)); //hacemos más arriba el eje para acomodar el tercer brazo (límite del brazo anterior)
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f)); //para controlar la rotación del tercer brazo
		//
		//modelaux = model; //reinicio ]
		////SI 2

		////dibujar una pequeña esfera
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f); //ESFERA BLANCA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//sp.render();

		//model = modelaux;

		////tercer brazo
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f)); //2.5,0,0 la mitad de la magnitud, pero ahora en X, por lo mismo de la rotación en el eje Z, que hace que nuestro eje X quede a 135 grados

		//modelaux = model;
		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f)); //5,1,1, por la rotación sobre el eje Z, ahora el eje x queda a 135 grados, por lo que prolongamos nuestro prisma en esta dirección
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 0.0f, 1.0f); //brazo 3 = azul
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		//model = modelaux;


		////articulación 4 extremo izquierdo del tercer brazo
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f)); //hacemos más arriba el eje para acomodar el tercer brazo (límite del brazo anterior)
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f)); //para controlar la rotación del tercer brazo

		//modelaux = model; //guardamos la info de la articulación

		////dibujar una pequeña esfera
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f); //ESFERA AZUL
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//sp.render();

		//model = modelaux;

		////CANASTA
		//model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f)); //2.5,0,0 la mitad de la magnitud, pero ahora en X, por lo mismo de la rotación en el eje Z, que hace que nuestro eje X quede a 135 grados

		//modelaux = model;

		//model = glm::scale(model, glm::vec3(2.5f, 4.0f, 3.0f)); //5,1,1, por la rotación sobre el eje Z, ahora el eje x queda a 135 grados, por lo que prolongamos nuestro prisma en esta dirección
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //canasta = gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		//model = modelaux;

		//// Crear instancias para completar el brazo y  la cabina. Imporante considerar que la cabina es el nodo padre. 
		////La cabina y el brazo deben de estar unidos a la cabina 




		

		







		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


