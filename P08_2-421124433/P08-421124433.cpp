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

//Para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
bool abre;
bool desplaza;
float rotPuerta;
float rotPuertaOffset;
float movVeh;
float giraVeh;


// Dado de 10 caras
float girosDadoX;
float girosDadoY;
float girosDadoZ;
float rotacionProgresivaX;
float rotacionProgresivaY;
float rotacionProgresivaZ;
float rotacionProgresivaOffSet;
bool obtenerValoresAleatorios;
bool realizarTodosLosMovimientos;

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



//modelo Vespa
Model vespa;


//MODELO DE LÁMPARA
Model desk_lamp;


//MODELO DE PUERTA METÁLICA CON CARTEL
Model puerta_marco;
Model reja_der;
Model reja_izq;


//MODELO DE NUEVA LUMINARIA PARA REPORTE 08
Model luminaria;

//Model puertaLetrero;



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
SpotLight spotLights2[MAX_SPOT_LIGHTS];
//SpotLight spotLights3[MAX_SPOT_LIGHTS];

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



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado10C();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.8f, 0.5f);

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

	//puertaLetrero = Model();
	//puertaLetrero.LoadModel("Models/puerta2.obj");



	//	NO HE IMPORTADO LOS FRENOS


	//CAPÓ TEXTURIZADO

	cofre_text = Model();
	cofre_text.LoadModel("Models/cofre_text.obj");

	//LAMPARA PARA REPORTE 07

	desk_lamp = Model();
	desk_lamp.LoadModel("Models/lampara_escritorio.obj");


	//PUERTA METÁLICA CON PANEL PARA REPORTE 08
	puerta_marco = Model();
	puerta_marco.LoadModel("Models/puerta_marco.obj");

	reja_der = Model();
	reja_der.LoadModel("Models/reja_der.obj");

	reja_izq = Model();
	reja_izq.LoadModel("Models/reja_izq.obj");





	//LUMINARIA PARA REPORTE 08
	luminaria = Model();
	luminaria.LoadModel("Models/luminaria_text.obj");




	dado10Caras = Texture("Textures/dado10caras.png");
	dado10Caras.LoadTextureA();




	//VESPA


	vespa = Model();
	vespa.LoadModel("Models/vespa.obj");

	

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
	/*unsigned int pointLightCount3 = 0;*/


	//Declaración de primer luz puntual (magenta) | PARA LUMINARIA
	pointLights[0] = PointLight(1.0f, 0.0f, 1.0f,
		0.0f, 3.0f,
		-60.0f, 25.5f, 65.0f,
		1.0f, 0.045f, 0.0075f);
	pointLightCount++;




	//LUZ PUNTUAL PARA LA DESK LAMP
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 3.0f,
		25.0f, 2.5f, 7.0f,
		1.0f, 0.14f, 0.07f);
	pointLightCount++;





	//LUZ NEGRA PARA APAGAR (ARREGLO 2 DE POINTLIGHTS)
	pointLights2[0] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 24.0f,
		4.0f, 9.0f, 12.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount2++;





	



	////LUZ PUNTUAL DE LA DESK LAMP EN EL SEGUNDO ARREGLO
	//pointLights2[1] = PointLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 3.0f,
	//	25.0f, 2.5f, 7.0f,
	//	1.0f, 0.14f, 0.07f);
	//pointLightCount2++;




	//25.0 20.0 15.0


	unsigned int spotLightCount = 0; //ARREGLO 0 -> TODAS LAS LUCES ENCENDIDAS
	unsigned int spotLightCount2 = 0; //ARREGLO 1 -> CUANDO EL AUTO AVANZA
	//unsigned int spotLightCount3 = 0; //ARREGLO 2 -> CUANDO EL AUTO RETROCEDE

	////linterna
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	5.0f);
	//spotLightCount++;


	//PRIMER ARREGLO DE SPOTLIGHTS (TODAS LAS SPOTLIGHTS ENCENDIDAS)



	// Luz vehiculo delantera (azul)
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 50.0f, -10.0f,
		0.0f, 0.0f, -10.0f,
		1.0f, 0.01f, 0.001f,
		20.0f);
	spotLightCount++;
	

	// Luz helicoptero
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.01f, 0.001f,
		15.0f);


	spotLightCount++;


	//LUZ SPOTLIGHT QUE ILUMINA PUERTA DE REJA

	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		3.0f, 5.0f,
		-2.0f, 12.0f, -5.0f,
		0.0f, 0.0f, 5.0f,
		1.0f, 0.01f, 0.001f,
		15.0f);


	spotLightCount++;



	// Luz vehiculo trasera (rojo) APAGADA
	spotLights[3] = SpotLight(0.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		0.0f, 50.0f, 10.0f,
		0.0f, 0.0f, 10.0f,
		1.0f, 0.01f, 0.001f,
		20.0f);
	spotLightCount++;





	//SEGUNDO ARREGLO (CUANDO EL AUTO RETROCEDE)



	// Luz vehiculo delantera (azul) APAGADA
	spotLights2[0] = SpotLight(0.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		0.0f, 50.0f, -10.0f,
		0.0f, 0.0f, -10.0f,
		1.0f, 0.01f, 0.001f,
		20.0f);
	spotLightCount2++;



	// Luz helicoptero
	spotLights2[1] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.01f, 0.001f,
		15.0f);
	spotLightCount2++;


	//LUZ SPOTLIGHT QUE ILUMINA PUERTA DE REJA

	spotLights2[2] = SpotLight(0.0f, 1.0f, 1.0f,
		3.0f, 5.0f,
		-2.0f, 12.0f, -5.0f,
		0.0f, 0.0f, 5.0f,
		1.0f, 0.01f, 0.001f,
		15.0f);
	spotLightCount2++;


	//luz trasera del auto ENCENDIDA

	spotLights2[3] = SpotLight(1.0f, 0.0f, 0.0f, 
		1.0f, 2.0f,
		0.0f, 50.0f, 10.0f,
		0.0f, 0.0f, 10.0f,
		1.0f, 0.01f, 0.001f,
		20.0f);
	spotLightCount2++;






	////TERCER ARREGLO DE SPOTLIGHTS (CUANDO EL AUTO RETROCEDE)



	//// Luz vehiculo delantera APAGADA (NEGRA)
	//spotLights3[0] = SpotLight(0.0f, 0.0f, 0.0f,
	//	1.0f, 2.0f,
	//	0.0f, 50.0f, -10.0f,
	//	0.0f, 0.0f, -10.0f,
	//	1.0f, 0.01f, 0.001f,
	//	20.0f);
	//spotLightCount3++;






	//// Luz helicoptero
	//spotLights3[1] = SpotLight(1.0f, 1.0f, 0.0f,
	//	1.0f, 2.0f,
	//	5.0f, 10.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.01f, 0.001f,
	//	15.0f);
	//spotLightCount3++;



	////LUZ SPOTLIGHT QUE ILUMINA PUERTA DE REJA

	//spotLights3[2] = SpotLight(0.0f, 1.0f, 1.0f,
	//	3.0f, 5.0f,
	//	-16.0f, 12.0f, 8.0f,
	//	0.0f, 0.0f, 5.0f,
	//	1.0f, 0.01f, 0.001f,
	//	15.0f);


	//spotLightCount3++;





	//// Luz vehiculo trasera (rojo) APAGADA
	//spotLights3[3] = SpotLight(0.0f, 0.0f, 0.0f,
	//	1.0f, 2.0f,
	//	0.0f, 50.0f, 10.0f,
	//	0.0f, 0.0f, 10.0f,
	//	1.0f, 0.01f, 0.001f,
	//	20.0f);
	//spotLightCount3++;


	




	//se crean mas luces puntuales y spotlight 



	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0); //PARA EL CHASIS
	glm::mat4 modelaux2(1.0);



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);



	girosDadoX = 0.0f;
	girosDadoY = 0.0f;
	girosDadoZ = 0.0f;
	rotacionProgresivaOffSet = 20.0f;
	obtenerValoresAleatorios = true;
	realizarTodosLosMovimientos = false;
	mainWindow.setEnciende(false);

	movCoche = 0.0f;
	movOffset = 0.8f;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	rotPuertaOffset = 5.0f;
	avanza = true;
	abre = true;
	rotPuerta = 0.5f;

	movVeh = 0.0f;
	giraVeh = 0.0f;


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//ANIMACION: PUERTA QUE ROTA

		if (abre) {


			//rotPuerta > 0.0f && rotPuerta < 145.0f
			if (rotPuerta >= 90.0f) //-0.2
			{
				rotPuerta -= movOffset * deltaTime; //movOffset en lugar de rotPuertOffset
				//printf("avanza%f \n ",movCoche);
				/*rotllanta += rotllantaOffset * deltaTime;*/
				//rotPuerta -= rotPuertaOffset * deltaTime;
			}

			else if (rotPuerta<=0)  /*if (rotPuerta==25.0f)*//* if (rotPuerta==144.0)*/ {
				/*rotPuerta -= movOffset * deltaTime;*/
				rotPuerta+movOffset*deltaTime;
			}
		}

		//else {
		//	if (rotPuerta > 25.0f)
		//	{

		//		//esperar 2 segundos
		//		rotPuerta -= movOffset * deltaTime;
		//		//printf("avanza%f \n ",movCoche);
		//		/*rotllanta -= rotllantaOffset * deltaTime;*/
		//		//rotPuerta += rotPuertaOffset * deltaTime;
		//	}

		//	else {
		//		abre = false;
		//	}
		

		//rotPuerta += movOffset * deltaTime; //esta es la que permite que haya movimiento por que? movOffset hace que vaya más lento en lugar de rotPuertaOffset


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





		//PARA CONTROLAR EL ENCENDIDO DE LAS LUCES PUNTUALES

		if (mainWindow.getlight() == 0) //si k en 0, 
		{

			shaderList[0].SetPointLights(pointLights, pointLightCount); //luz 1 = roja
		}
		else
		{

			shaderList[0].SetPointLights(pointLights2, pointLightCount2); //luz negra (apagada)

		}


		//PARA CONTROLAR EL ENCENDIDO Y APAGADO DE LA LUZ DELANTERA

		if (mainWindow.getLuzDel() == 0)
		{

			shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
		}
		else
		{

			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}



		//PARA CONTROLAR EL ENCENDIDO Y APAGADO DE LA LUZ TRASERA

		if (mainWindow.getLuzTras() == 0)
		{

			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		else
		{

			shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
		}


		//Ruta vehiculo
		if (avanza)
		{
			if (movVeh > -75.0f)
			{
				movVeh -= movOffset * deltaTime;

			}
			else
			{
				avanza = false;
				giraVeh = -90.0f;
			}
		}
		else
		{
			if (movVeh < 75.0f)
			{
				movVeh += movOffset * deltaTime;
			}
			else
			{
				avanza = true;
				giraVeh = 90.0f;
			}
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

		model = modelaux;


		////DADO DE 10 CARAS POR C DIGO
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(25.0f, -0.9f, 7.0f)); //4.5,9.5,-2.0
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f)); //inclinaci n
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dado10Caras.UseTexture(); //usamos la textura
		//meshList[4]->RenderMesh();










		//Dado de 10 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -0.9f, 7.0f)); //4.5,9.5,-2.0
		model = glm::rotate(model, 90 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f)); //inclinaci n
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

		/*Rotacion base del dado, donde permite estar acostado el dado*/
		model = glm::rotate(model, glm::radians(46.33f + rotacionProgresivaY), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotacionProgresivaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotacionProgresivaY), glm::vec3(0.0f, 1.0f, 0.0f));
		/*Aqui se definirá la rutina del dado para girar aleatoriamente*/
		printf("%d", mainWindow.getEnciende());

		if (mainWindow.getEnciende()) {
			if (obtenerValoresAleatorios) {
				girosDadoX = (float)(180.0f * (rand() % 11));
				girosDadoY = (float)(072.0f * (rand() % 11));
				girosDadoZ = (float)(180.0f * (rand() % 11));
				rotacionProgresivaX = rotacionProgresivaY = rotacionProgresivaZ = 0.0f;
				realizarTodosLosMovimientos = true;
				printf("Los valores random son (%f,%f,%f)-----------------\n", girosDadoX, girosDadoY, girosDadoZ);
				obtenerValoresAleatorios = false;
			}

			if (rotacionProgresivaX < girosDadoX && realizarTodosLosMovimientos) {
				rotacionProgresivaX += rotacionProgresivaOffSet * deltaTime;
			}
			if (rotacionProgresivaY < girosDadoY && realizarTodosLosMovimientos) {
				rotacionProgresivaY += rotacionProgresivaOffSet * deltaTime;
			}
			if (rotacionProgresivaZ < girosDadoZ && realizarTodosLosMovimientos) {
				rotacionProgresivaZ += rotacionProgresivaOffSet * deltaTime;
			}

			printf("--> %d, %d, %d, %f, %f, %f \n", rotacionProgresivaX < girosDadoX, rotacionProgresivaY < girosDadoY, rotacionProgresivaZ < girosDadoZ, rotacionProgresivaX, rotacionProgresivaY, rotacionProgresivaZ);
			if (!(rotacionProgresivaX < girosDadoX) && !(rotacionProgresivaY < girosDadoY) && !(rotacionProgresivaZ < girosDadoZ)) {
				printf("REINICIO LOS VALORES\n");
				realizarTodosLosMovimientos = false;
				//rotacionProgresivaX = rotacionProgresivaY = rotacionProgresivaZ = 0.0f;
				obtenerValoresAleatorios = true;
				mainWindow.setEnciende(false);
			}
		}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10Caras.UseTexture();
		meshList[4]->RenderMesh();



		model = modelaux;


		////DADO DE 10 CARAS POR C DIGO
		//model = glm::mat4(1.0);
		//
		//
		////glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//////dado10Caras.UseTexture(); //usamos la textura
		//meshList[4]->RenderMesh();
















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
		
		// Luz vehiculo delantera (azul)
		model = modelaux;
		glm::vec3 carLightPos = model[3]; //og 3
		//carLightPos.x += 0.0f;//2.5
		carLightPos.z += 3.0f; //3.0
		spotLights[0].SetPos(carLightPos);


		// Luz vehiculo trasera (rojo)
		model = modelaux;
		//glm::vec3 carLightPos = model[3]; //og 3
		//carLightPos.x += 0.0f;//2.5
		carLightPos.z -= 3.0f; //3.0
		spotLights2[3].SetPos(carLightPos);








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
		model = glm::translate(model, glm::vec3(0.0f, 100.0f, 6.0));
		model = glm::translate(model, glm::vec3(-mainWindow.getmovimiento_helicoptero(), 0.0f, 0.0f));
		spotLights[1].SetPos(model[3]);
		spotLights2[1].SetPos(model[3]);
		
		//model = glm::translate(model, glm::vec3(mainWindow.getmovimiento_helicoptero(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.5f, 8.5f, 8.5f));
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


		model = modelaux;

		//PUERTA METÁLICA CON MARCO PARA LETRERO, PARA PRACTICA 08

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.95f, 70.0f));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta_marco.RenderModel();

		model = modelaux;




		//REJA DERECHA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.5f, 10.25f, 70.0f));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::rotate(model,rotPuerta*toRadians, glm::vec3(0.0f, 0.1f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reja_der.RenderModel();

		model = modelaux;



		//REJA IZQUIERDA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.5f, -0.95f, 70.0f));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));


		//model = glm::rotate(model, rotPuerta, glm::vec3(0.0f, 1.0f, 0.0f)); //CORREGIR ROTACIÓN

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reja_izq.RenderModel();

		model = modelaux;






		//MODELO DE LUMINARIA PARA REPORTE 08

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, -0.95f, 65.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		luminaria.RenderModel();

		model = modelaux;





		//VESPA PARA REPORTE 09

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));

		model = glm::translate(model, glm::vec3(-movVeh, 10.0f, 10.0f));
		model = glm::rotate(model, glm::radians(giraVeh), glm::vec3(0.0f, 1.0f, 0.0f));
		
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vespa.RenderModel();

		model = modelaux;



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
