/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;
float avanceoffset;

float puertas_rejas;

// Movimiento lateral cuando rueda
float dadoLadoX;
float dadoLadoZ;
// Rotación del dado
float dadoRotX;
float dadoRotY;
float dadoRotZ;
float angulo;

// Número que salió (1-8)
float numeroSalio = 1.0;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Texture CartelReja;

Texture dadoTexture;


Model Tiamat_M;

Model Arco_M;
Model Reja_Deslizante;
Model Reja_Abre;
Model Cartel_Reja;

Skybox skybox;

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
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
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


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int letreroIndices[] = {
   0, 1, 2,
   0, 2, 3,
	};

	GLfloat letreroVertices[] = {
		-0.5f, 0.0f, -0.5f,		0.0f,  0.1f,		0.0f, 1.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,		0.2f,  0.1f,	    0.0f, 1.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,		0.2f,  1.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f,  0.5f,		0.0f,  1.0f,		0.0f, 1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(letreroVertices, letreroIndices, 32, 6);
	meshList.push_back(obj8);

}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// cara 0
		0, 1, 2,
		// cara 1
		3, 4, 5,
		// cara 2
		6, 7, 8,
		// cara 3
		9, 10, 11,
		// cara 4
		12, 13, 14,
		// cara 5
		15, 16, 17,
		// cara 6
		18, 19, 20,
		// cara 7
		21, 22, 23

	};
	// average normals
	GLfloat cubo_vertices[] = {
		// face 0 (top, front, right) 7 treboles azules
		// x      y      z      S     T        NX        NY        NZ
		 0.0f,  0.5f,  0.0f,   0.26f, 0.50f,   0.577350f,  0.577350f,  0.577350f, //0
		 0.0f,  0.0f,  0.5f,   0.51f, 0.75f,   0.577350f,  0.577350f,  0.577350f, //1
		 0.5f,  0.0f,  0.0f,   0.01f, 0.75f,   0.577350f,  0.577350f,  0.577350f, //2

		 // face 1 (top, right, back) (2-rombos rojos)
		  0.0f,  0.5f,  0.0f,   0.74f, 0.50f,   0.577350f,  0.577350f, -0.577350f, //3
		  0.5f,  0.0f,  0.0f,   0.49f, 0.258f,   0.577350f,  0.577350f, -0.577350f, //4
		  0.0f,  0.0f, -0.5f,   0.99f, 0.258f,   0.577350f,  0.577350f, -0.577350f, //5

		  // face 2 (top, back, left) 3-corazones
		   0.0f,  0.5f,  0.0f,   0.74f, 1.00f,  -0.577350f,  0.577350f, -0.577350f, //6
		   0.0f,  0.0f, -0.5f,   0.49f, 0.748f,  -0.577350f,  0.577350f, -0.577350f, //7
		  -0.5f,  0.0f,  0.0f,   0.99f, 0.748f,  -0.577350f,  0.577350f, -0.577350f, //8

		  // face 3 (top, left, front) 6 naipes verdes
		   0.0f,  0.5f,  0.0f,   0.50f, 0.25f,  -0.577350f,  0.577350f,  0.577350f, //9
		  -0.5f,  0.0f,  0.0f,   0.75f, 0.505f,  -0.577350f,  0.577350f,  0.577350f, //10
		   0.0f,  0.0f,  0.5f,   0.25f, 0.505f,  -0.577350f,  0.577350f,  0.577350f, //11

		   // face 4 (bottom, right, front) 4-rombos-azules
			0.0f, -0.5f,  0.0f,   0.74f, 0.50f,   0.577350f, -0.577350f,  0.577350f, //12
			0.5f,  0.0f,  0.0f,   0.99f, 0.75f,   0.577350f, -0.577350f,  0.577350f, //13
			0.0f,  0.0f,  0.5f,   0.49f, 0.75f,   0.577350f, -0.577350f,  0.577350f, //14

			// face 5 (bottom, back, right) 5-naipes-verdes-reves
			 0.0f, -0.5f,  0.0f,   0.26f, 0.51f,   0.577350f, -0.577350f, -0.577350f, //15
			 0.0f,  0.0f, -0.5f,   0.01f, 0.255f,   0.577350f, -0.577350f, -0.577350f, //16
			 0.5f,  0.0f,  0.0f,   0.51f, 0.255f,   0.577350f, -0.577350f, -0.577350f, //17

			 // face 6 (bottom, left, back) 9- corazones rojos
			  0.0f, -0.5f,  0.0f,   0.255f, 0.01f,  -0.577350f, -0.577350f, -0.577350f, //18
			 -0.5f,  0.0f,  0.0f,   0.50f, 0.26f,  -0.577350f, -0.577350f, -0.577350f, //19
			  0.0f,  0.0f, -0.5f,   0.01f, 0.26f,  -0.577350f, -0.577350f, -0.577350f, //20

			  // face 7 (bottom, front, left)
			  0.0f, -0.5f,  0.0f,   0.50f, 0.75f,  -0.577350f, -0.577350f,  0.577350f, //21
			  0.0f,  0.0f,  0.5f,   0.25f, 0.50f,  -0.577350f, -0.577350f,  0.577350f, //22
			 -0.5f,  0.0f,  0.0f,   0.75f, 0.50f,  -0.577350f, -0.577350f,  0.577350f, //23
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 24);
	meshList.push_back(dado);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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

	dadoTexture = Texture("Textures/dado_ocho_caras_optimizada.png");
	dadoTexture.LoadTextureA();



	CartelReja = Texture("Textures/cartel_tipografia.png");
	CartelReja.LoadTextureA();


	Arco_M.LoadModel("Models/arco.obj");
	Reja_Deslizante = Model();
	Reja_Deslizante.LoadModel("Models/reja_deslizante.obj");
	Reja_Abre = Model();
	Reja_Abre.LoadModel("Models/reja_abre.obj");
	Cartel_Reja = Model();
	Cartel_Reja.LoadModel("Models/cartel_reja.obj");

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
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	avanceoffset = 0.1f;


	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	puertas_rejas = 0.0f;


	dadoLadoX = 0.0f;
	dadoLadoZ = 0.0f;
	dadoRotX = 0.0f;
	dadoRotY = 0.0f;
	dadoRotZ = 0.0f;
	angulo = 0.0f;
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		if (mainWindow.abrirreja()) {
			if (puertas_rejas <= 90) {
				puertas_rejas += 1.0 * deltaTime;
			}
		}
		else {
			if (puertas_rejas > 0) {
				puertas_rejas -= 1.0 * deltaTime;
			}
		}


		//dado 
		if (mainWindow.dadocae()) {
			if (dragonavance == 0.0f) {
				numeroSalio = (rand() % 8) + 1;
				//printf("Numero salio randonm:\n %f", numeroSalio);
				angulo = (rand() % 360) * toRadians;
				dadoLadoX = cos(angulo) * 3.0f;
				dadoLadoZ = sin(angulo) * 3.0f;
			}
			if (dragonavance > -12.0f) {
				dragonavance -= avanceoffset * deltaTime;
				// Girar mientras cae
				dadoRotX += 300.0f * deltaTime;
				dadoRotY += 250.0f * deltaTime;
				dadoRotZ += 200.0f * deltaTime;
			}
			// YA EN EL PISO, RODANDO
			else {
				// Acercar poco a poco a donde va a quedar
				dadoLadoX *= 0.97f; 
				dadoLadoZ *= 0.97f;

				dadoRotX += 50.0f * deltaTime;
				dadoRotY += 40.0f * deltaTime;
				dadoRotZ += 30.0f * deltaTime;

				 //ajustar a la cara
				if (fabs(dadoLadoX) < 0.1f && fabs(dadoLadoZ) < 0.1f) {
					//  1 corazon 
					if (numeroSalio == 1) { dadoRotX = -135.0f; dadoRotY = 45.0f; }
					//  2 rombos rojos 
					if (numeroSalio == 2) { dadoRotX = 45.0f; dadoRotY = 45.0f; }
					// 3 corazones 
					if (numeroSalio == 3) { dadoRotX = 45.0f; dadoRotY = -45.0f; }
					//4 rombos azules
					if (numeroSalio == 4) { dadoRotX = -135.0f; dadoRotY = -45.0f; } 
					// 5 naipes 
					if (numeroSalio == 5) { dadoRotX = 135.0f; dadoRotY = 45.0f; }
					//6 naipes verdes
					if (numeroSalio == 6) { dadoRotX = -45.0f; dadoRotY = 45.0f; } 
					//7 treboles 
					if (numeroSalio == 7) { dadoRotX = -45.0f; dadoRotY = -45.0f; } 
					//9 coraonez rojes
					if (numeroSalio == 8) { dadoRotX = 135.0f; dadoRotY = -45.0f; }
					dadoRotZ = 0.0f;
				}
			}
		}
		else {
			dragonavance = 0.0f;
			dadoLadoX = 0.0f;
			dadoLadoZ = 0.0f;
			dadoRotX = 0.0f;
			dadoRotY = 0.0f;
			dadoRotZ = 0.0f;
		}
		
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
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f + dadoLadoX, 10.5f + dragonavance, 4.0f + dadoLadoZ));
		model = glm::rotate(model, dadoRotX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, dadoRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, dadoRotZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		dadoTexture.UseTexture();
		meshList[8]->RenderMesh();


		//Instancia del arco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 7.7f, -2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco_M.RenderModel();

		//Reja que se abre
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.1199f, -6.48f, -5.5469f));
		model = glm::rotate(model, -puertas_rejas * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja_Abre.RenderModel();

		//Reja que se desliza
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.7708f, -9.341, 5.8329f + (puertas_rejas / 18)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja_Deslizante.RenderModel();

		//Cartel
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.08328f, 2.642f, -0.17226f));
		modelaux= model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cartel_Reja.RenderModel();

		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//Letras en movimiento
		toffsetnumerocambiau += 0.002 * deltaTime;
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.25f)); //
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(6.8f, 1.0f, 2.2f)); //horizontal x, vertival z
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CartelReja.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}