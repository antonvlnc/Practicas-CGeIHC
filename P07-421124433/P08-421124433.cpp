/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model cuerpo;
Model capo;
Model puerta;
Model trasera_izq;
Model trasera_der;
Model delantera_der;
Model delantera_izq;
//FRENOS DELANTEROS
Model freno_del_izq;
Model freno_del_der;
//FRENOS TRASEROS
Model freno_tras_izq;
Model freno_tras_der;
Model cofre_text;


//MODELO DE LÁMPARA
Model desk_lamp;


Skybox skybox;



Texture dado10Caras; //Nueva textura para el dado de 10 caras

Model Dado_M;





//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1); //meshlist0

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2); //mesh list 1

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3); //mesh list 2

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4); //mesh list 3

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}






void CrearDado10C()
{
	// clang-format off
	unsigned int cubo_indices_10[] = {
		// CARA SUPERIOR 01
		0, 1, 2,

		// CARA SUPERIOR 02
		3 ,4, 5,

		// CARA SUPERIOR 03
		6, 7, 8,

		// CARA SUPERIOR 04
		9, 10, 11,

		// CARA SUPERIOR 05
		12, 13, 14,

		// "ABAJO"

		//CARA INFERIOR 01
		15, 16, 17,

		//CARA INFERIOR 02
		18, 19 ,20,

		//CARA INFERIOR 03
		21, 22, 23,

		//CARA INFERIOR 04
		24, 25, 26,

		//CARA INFERIOR 05
		27, 28, 29,

	};

	GLfloat cubo_vertices_10[] = {
		//ARRIBA, DE CENTRO A LA DERECHA
		//// front
		////x		y		z		S		T			NX		NY		NZ
		//0.3f, 0.0f,  0.5f,		0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//A
		//-0.3f, 0.0f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//B
		//0.0f,  0.45f,  1.0f,	0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//C
		//0.45f,  0.6f,  0.5f,	 0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//D
		//// right
		////x		y		z		S		T
		//-0.45f, 0.6f,  0.5f,	0.0f,  0.33f,		-1.0f,	0.0f,	0.0f, //E
		//0.0f, 0.95f,  0.5f,		0.25f,	0.33f,		-1.0f,	0.0f,	0.0f, //F, hasta ac 
		//0.0f,  0.45f,  0.0f,    0.25f,	0.66f,		-1.0f,	0.0f,	0.0f, //H


		//--------------CARAS SUPERIORES------------

		//CARA SUPERIOR 1
		//x		y		z		S		T			NX		NY		NZ
		-0.3f, 0.0f,  0.5f,		0.495f,	0.935f,		0.0f,	1.0f,	-0.33f,	//1 (B)
		0.3f, 0.0f,  0.5f,	   0.28f,  0.87f,		0.0f,	1.0f,	-0.33f,	//0 (A)
		0.0f,  0.45f,  1.0f,	0.48f,	0.72f,		0.0f,	1.0f,	-0.33f,	//2 (C)

		//CARA SUPERIOR 2
		//x		y		z		S		T			NX		NY		NZ
		0.3f, 0.0f,  0.5f,		0.638f,  0.347f,	0.0f,	1.0f,	-0.33f,	//3 (A).383 .284
		0.45f, 0.6f,  0.5f,		0.49f,	0.478f,		0.0f,	1.0f,	-0.33f,	//4 (D) .63
		0.0f,  0.45f,  1.0f,	0.383f,	0.284f,		0.0f,	1.0f,	-0.33f,	//5 (C) .49 .478

		//CARA SUPERIOR 3
		//x		y		z		S		T			NX		NY		NZ
		0.45f, 0.6f,  0.5f,		0.371f,	0.533f,		0.0f,	1.0f,	-0.5f,	//7 (D)
		0.0f, 0.95f,  0.5f,		0.602f,  0.528f,	0.0f,	1.0f,	-0.5f,	//6 (F)
		0.0f,  0.45f,  1.0f,	0.478f,	0.729f,		0.0f,	1.0f,	-0.5f,	//8 (C)

		//CARA SUPERIOR 4
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.95f,  0.5f,		0.417f,  0.074f,	.0f,	1.0f,	-1.0f,	//9 (F)
		-0.45f, 0.6f,  0.5f,	0.607f,	0.168f,		0.0f,	1.0f,	-1.0f,	//10 (E)
		0.0f,  0.45f,  1.0f,	0.383f,	0.284f,		0.0f,	1.0f,	-1.0f,	//11 (C)

		//CARA SUPERIOR 5
		//x		y		z		S		T			NX		NY		NZ
		-0.45f, 0.6f,  0.5f,	0.741f,	0.675f,		0.0f,	1.0f,	-0.5f,	//13 (E)
		-0.3f, 0.0f,  0.5f,		0.695f,  0.852f,	0.0f,	1.0f,	-0.5f,	//12 (B)		
		0.0f,  0.45f,  1.0f,	0.478f,	0.725f,		0.0f,	1.0f,	-0.5f,	//14 (C)

		//------------------CARAS INFERIORES----------------------

		//CARA INFERIOR 1
		//x		y		z		S		T			NX		NY		NZ
		-0.3f, 0.0f,  0.5f,		0.609f,	0.170f,		0.0f,	-1.0f,	-0.33f,	//16 (B)
		0.3f, 0.0f,  0.5f,		0.639f,  0.345f,	0.0f,	-1.0f,	-0.33f,	//15 (A)
		0.0f,  0.45f,  0.0f,	0.388f,	0.284f,		0.0f,	-1.0f,	-0.33f,	//17 (H)

		//CARA INFERIOR 2
		//x		y		z		S		T			NX		NY		NZ
		0.3f, 0.0f,  0.5f,		0.484f,  0.479f,	0.0f,	-1.0f,	-0.33f,	//18 (A)
		0.45f, 0.6f,  0.5f,		0.263f,	0.473f,		0.0f,	-1.0f,	-0.33f,	//19 (D)
		0.0f,  0.45f,  0.0f,	0.383f,	0.291f,		0.0f,	-1.0f,	-0.33f,	//20 (H)

		//CARA INFERIOR 3
		//x		y		z		S		T			NX		NY		NZ
		0.45f, 0.6f,  0.5f,		0.695f,	0.853f,		0.0f,	-1.0f,	-0.5f,	//22 (D)
		0.0f, 0.95f,  0.5f,		0.497f,  0.938f,	0.0f,	-1.0f,	-0.5f,	//21 (F)
		0.0f,  0.45f,  0.0f,	0.479f,	0.726f,		0.0f,	-1.0f,	-0.5f,	//23 (H)

		//CARA INFERIOR 4
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.95f,  0.5f,		0.201f,  0.127f,	0.0f,	-1.0f,	-0.33f,	//24 (F)
		-0.45f, 0.6f,  0.5f,	0.417f,	0.074f,		0.0f,	-1.0f,	-0.33f,	//25 (E)
		0.0f,  0.45f,  0.0f,	0.382f,	0.284f,		0.0f,	-1.0f,	-0.33f,	//26 (H)

		//CARA INFERIOR 5
		//x		y		z		S		T			NX		NY		NZ
		-0.45f, 0.6f,  0.5f,	0.6f,	0.535f,		0.0f,	-1.0f,	-0.5f,	//28 (E)
		-0.3f, 0.0f,  0.5f,		0.74f,  0.676f,		0.0f,	-1.0f,	-0.5f,	//27 (B)
		0.0f,  0.45f,  0.0f,	0.48f,	0.725f,		0.0f,	-1.0f,	-0.5f,	//29 (H)
	};

	Mesh* dado10 = new Mesh();
	dado10->CreateMesh(cubo_vertices_10, cubo_indices_10, 250, 50); //192,36
	meshList.push_back(dado10);
}



//void handleKeypress(unsigned char key, int x, int y) {
//
//
//	switch (key) {
//	case 'q':
//		glDisable(GL_LIGHT3);
//		break;
//	case 'e':
//		glEnable(GL_LIGHT3);
//		break;
//
//	case 'd':
//		glDisable(GL_LIGHT1);
//		break;
//	case 'f':
//		glEnable(GL_LIGHT1);
//		break;
//	case 27: //Escape key
//		exit(0);
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}






int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado10C();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");



	cuerpo = Model();
	cuerpo.LoadModel("Models/AUDI_separado/cuerpo.obj");

	/*capo = Model();
	capo.LoadModel("Models/AUDI_separado/capo.obj");*/

	puerta = Model();
	puerta.LoadModel("Models/AUDI_separado/puerta.obj");

	trasera_izq = Model();
	trasera_izq.LoadModel("Models/AUDI_separado/trasera_izq_text.obj");

	trasera_der = Model();
	trasera_der.LoadModel("Models/AUDI_separado/trasera_der_text.obj");


	delantera_izq = Model();
	delantera_izq.LoadModel("Models/AUDI_separado/delantera_izq_text.obj");

	delantera_der = Model();
	delantera_der.LoadModel("Models/AUDI_separado/delantera_der_text.obj");

	//	NO HE IMPORTADO LOS FRENOS


	//CAPÓ TEXTURIZADO

	cofre_text = Model();
	cofre_text.LoadModel("Models/cofre_text.obj");

	//LAMPARA PARA REPORTE 07

	desk_lamp = Model();
	desk_lamp.LoadModel("Models/lampara_escritorio.obj");



	dado10Caras = Texture("Textures/dado10caras.png");
	dado10Caras.LoadTextureA();


	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);


	//contador de luces puntuales

	unsigned int pointLightCount = 0;
	unsigned int pointLightCount2 = 0;

	//Declaración de primer luz puntual (ROJA)
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		1.0f, 0.14f, 0.07f);
	pointLightCount++;

	//LUZ PUNTUAL PARA LA DESK LAMP
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 3.0f,
		25.0f, 2.5f, 7.0f,
		1.0f, 0.14f, 0.07f);
	pointLightCount++;



	pointLights2[0] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 24.0f,
		4.0f, 9.0f, 12.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount2++;


	//25.0 20.0 15.0


	unsigned int spotLightCount = 2;
	////linterna
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	5.0f);
	//spotLightCount++;

	////luz fija verde
	//spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
	//	1.0f, 2.0f,
	//	5.0f, 10.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	15.0f);
	//spotLightCount++;
	


	// Luz vehiculo
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 50.0f, -10.0f,
		0.0f, 0.0f, -10.0f,
		1.0f, 0.01f, 0.001f,
		20.0f);
	spotLightCount++;


	// Luz helicoptero
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.01f, 0.001f,
		15.0f);


	spotLightCount++;






	//se crean mas luces puntuales y spotlight 



	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0); //PARA EL CHASIS
	glm::mat4 modelaux2(1.0);



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		if (mainWindow.getlight() == 0)
		{

			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else
		{

			shaderList[0].SetPointLights(pointLights2, pointLightCount2);
		}



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//DADO DE 10 CARAS POR C DIGO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -0.9f, 7.0f)); //4.5,9.5,-2.0
		model = glm::rotate(model, 90 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f)); //inclinaci n
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10Caras.UseTexture(); //usamos la textura
		meshList[4]->RenderMesh();





		//INICIO DEL DIBUJO DEL AUTO
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, -2.0, 0.0f)); //TRASLACION INICIAL -2.8


		modelaux = model;


		//CUERPO DEL AUTO
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de coche de color negro

		/*model = glm::mat4(1.0);*/
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, -1.5f)); //traslación inicial para posicionarlo

		//aplicarle escala al auto para que tenga más espacio para desplazarse


		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -mainWindow.getmovimiento_avanza_retrocede()));


		modelaux = model;


		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		cuerpo.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		model = modelaux;


		//LUZ
		// Luz vehiculo
		model = modelaux;
		glm::vec3 carLightPos = model[3]; //og 3
		//carLightPos.x += 0.0f;//2.5
		carLightPos.z += 3.0f; //3.0
		spotLights[2].SetPos(carLightPos);






		//CAPÓ


		//color = glm::vec3(1.0f, 0.0f, 0.0f); //capo color magenta

		/*model = glm::mat4(1.0);*/
		model = glm::translate(model, glm::vec3(0.0f, 1.8f, -7.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_capo()), glm::vec3(1.0f, 0.0f, 0.0f)); //CORREGIR ROTACIÓN

		//modelaux = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre_text.RenderModel();


		model = modelaux;



		//PUERTA

		//color = glm::vec3(0.0f, 0.0f, 0.2f); //PUERTA COLOR BLANCO

		/*model = glm::mat4(1.0);*/
		model = glm::translate(model, glm::vec3(-5.6f, 0.0f, -6.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_puerta()), glm::vec3(0.0f, 1.0f, 0.0f)); //CORREGIR ROTACIÓN

		//modelaux = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta.RenderModel();

		model = modelaux;



		//RUEDA DELANTERA IZQUIERDA  mainWindow.getmovimiento_avanza_retrocede()

		//color = glm::vec3(0.0f, 0.0f, 0.0f); //RUEDA COLOR AZUL

		/*model = glm::mat4(1.0);*/
		model = glm::translate(model, glm::vec3(-5.2f, -2.0f, -9.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_avanza()), glm::vec3(-1.0f, 0.0f, 0.0f)); //CORREGIR ROTACIÓN

		//modelaux = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		delantera_izq.RenderModel();

		model = modelaux;



		//RUEDA DELANTERA DERECHA
		model = glm::translate(model, glm::vec3(5.2f, -2.0f, -9.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_avanza()), glm::vec3(-1.0f, 0.0f, 0.0f));

		//modelaux = model;

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		delantera_der.RenderModel();

		model = modelaux;






		//RUEDA TRASERA DERECHA

		model = glm::translate(model, glm::vec3(5.2f, -2.0f, 8.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_avanza()), glm::vec3(-1.0f, 0.0f, 0.0f)); //CORREGIR 

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		trasera_der.RenderModel();

		model = modelaux;






		//RUEDA TRASERA IZQUIERDA

		
		model = glm::translate(model, glm::vec3(-5.2f, -2.0f, 8.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_avanza()), glm::vec3(-1.0f, 0.0f, 0.0f)); //CORREGIR ROTACIÓN
		/*modelaux = model;*/
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		trasera_izq.RenderModel();

		model = modelaux;//puede que lo cambie a modelaux






		//HELICOPTERO

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 50.0f, 6.0));
		model = glm::translate(model, glm::vec3(-mainWindow.getmovimiento_helicoptero(), 0.0f, 0.0f));
		spotLights[3].SetPos(model[3]);
		//model = glm::translate(model, glm::vec3(mainWindow.getmovimiento_helicoptero(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();




	

		

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);


		model = modelaux;


		//LÁMPARA PARA REPORTE 07

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -0.95f, 5.0f));
		model = glm::scale(model, glm::vec3(0.008f,0.008f,0.008f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desk_lamp.RenderModel();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
