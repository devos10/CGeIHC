//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vSolid = "shaders/shader.vert";
// Nuevos
static const char* fRed = "shaders/rojo.frag";
static const char* vRed = "shaders/rojo.vert";
static const char* fGreen = "shaders/verde.frag";
static const char* vGreen = "shaders/verde.vert";
static const char* fBlue = "shaders/azul.frag";
static const char* vBlue = "shaders/azul.vert";
static const char* fBrown = "shaders/cafe.frag";
static const char* vBrown = "shaders/cafe.vert";
static const char* fDGreen = "shaders/verde_fuerte.frag";
static const char* vDGreen = "shaders/verde_fuerte.vert";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";


float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
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

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X     Y    Z        R   G    B
		//  LETRA O (Azul) 
		-0.9f,-0.5f,0.0f,   0.0f,0.0f,1.0f,
		-0.4f,-0.5f,0.0f,   0.0f,0.0f,1.0f,
		-0.4f,-0.3f,0.0f,   0.0f,0.0f,1.0f,

		-0.9f,-0.5f,0.0f,   0.0f,0.0f,1.0f,
		-0.4f,-0.3f,0.0f,   0.0f,0.0f,1.0f,
		-0.9f,-0.3f,0.0f,   0.0f,0.0f,1.0f,

		-0.9f,0.3f,0.0f,    0.0f,0.0f,1.0f,
		-0.4f,0.3f,0.0f,    0.0f,0.0f,1.0f,
		-0.4f,0.5f,0.0f,    0.0f,0.0f,1.0f,

		-0.9f,0.3f,0.0f,    0.0f,0.0f,1.0f,
		-0.4f,0.5f,0.0f,    0.0f,0.0f,1.0f,
		-0.9f,0.5f,0.0f,    0.0f,0.0f,1.0f,

		-0.9f,-0.3f,0.0f,   0.0f,0.0f,1.0f,
		-0.8f,-0.3f,0.0f,   0.0f,0.0f,1.0f,
		-0.8f,0.3f,0.0f,    0.0f,0.0f,1.0f,

		-0.9f,-0.3f,0.0f,   0.0f,0.0f,1.0f,
		-0.8f,0.3f,0.0f,    0.0f,0.0f,1.0f,
		-0.9f,0.3f,0.0f,    0.0f,0.0f,1.0f,

		-0.5f,-0.3f,0.0f,   0.0f,0.0f,1.0f,
		-0.4f,-0.3f,0.0f,   0.0f,0.0f,1.0f,
		-0.4f,0.3f,0.0f,    0.0f,0.0f,1.0f,

		-0.5f,-0.3f,0.0f,   0.0f,0.0f,1.0f,
		-0.4f,0.3f,0.0f,    0.0f,0.0f,1.0f,
		-0.5f,0.3f,0.0f,    0.0f,0.0f,1.0f,

		//  LETRA V (Roja) 
		-0.1f,-0.5f,0.0f,   1.0f,0.0f,0.0f,
		 0.1f,-0.5f,0.0f,   1.0f,0.0f,0.0f,
		-0.1f,-0.3f,0.0f,   1.0f,0.0f,0.0f,

		 0.1f,-0.5f,0.0f,   1.0f,0.0f,0.0f,
		 0.1f,-0.3f,0.0f,   1.0f,0.0f,0.0f,
		-0.1f,-0.3f,0.0f,   1.0f,0.0f,0.0f,

		 0.0f,-0.3f,0.0f,   1.0f,0.0f,0.0f,
		-0.1f,0.5f,0.0f,    1.0f,0.0f,0.0f,
		-0.1f,-0.3f,0.0f,   1.0f,0.0f,0.0f,

		 0.0f,-0.3f,0.0f,   1.0f,0.0f,0.0f,
		 0.1f,-0.3f,0.0f,   1.0f,0.0f,0.0f,
		 0.1f,0.5f,0.0f,    1.0f,0.0f,0.0f,

		-0.1f,-0.5f,0.0f,   1.0f,0.0f,0.0f,
		-0.1f,0.5f,0.0f,    1.0f,0.0f,0.0f,
		-0.3f,0.5f,0.0f,    1.0f,0.0f,0.0f,

		 0.1f,-0.5f,0.0f,   1.0f,0.0f,0.0f,
		 0.3f,0.5f,0.0f,    1.0f,0.0f,0.0f,
		 0.1f,0.5f,0.0f,    1.0f,0.0f,0.0f,

		 // LETRA C (Gris) 
		  0.4f,-0.5f,0.0f,   0.5f,0.5f,0.5f,
		  0.9f,-0.5f,0.0f,   0.5f,0.5f,0.5f,
		  0.4f,-0.3f,0.0f,   0.5f,0.5f,0.5f,

		  0.9f,-0.5f,0.0f,   0.5f,0.5f,0.5f,
		  0.9f,-0.3f,0.0f,   0.5f,0.5f,0.5f,
		  0.4f,-0.3f,0.0f,   0.5f,0.5f,0.5f,

		  0.4f,0.3f,0.0f,    0.5f,0.5f,0.5f,
		  0.9f,0.3f,0.0f,    0.5f,0.5f,0.5f,
		  0.9f,0.5f,0.0f,    0.5f,0.5f,0.5f,

		  0.4f,0.3f,0.0f,    0.5f,0.5f,0.5f,
		  0.9f,0.5f,0.0f,    0.5f,0.5f,0.5f,
		  0.4f,0.5f,0.0f,    0.5f,0.5f,0.5f,

		  0.4f,-0.3f,0.0f,   0.5f,0.5f,0.5f,
		  0.6f,-0.3f,0.0f,   0.5f,0.5f,0.5f,
		  0.6f,0.3f,0.0f,    0.5f,0.5f,0.5f,

		  0.4f,-0.3f,0.0f,   0.5f,0.5f,0.5f,
		  0.6f,0.3f,0.0f,    0.5f,0.5f,0.5f,
		  0.4f,0.3f,0.0f,    0.5f,0.5f,0.5f,
	};

	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, sizeof(vertices_letras) / sizeof(GLfloat));
	meshColorList.push_back(letras);


	//	MeshColor* cuadradoverde = new MeshColor();
	//	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	//	meshColorList.push_back(cuadradoverde);
}

void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	/*shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);*/
	Shader* sRed = new Shader();  sRed->CreateFromFiles(vRed, fRed);     shaderList.push_back(*sRed);
	Shader* sGreen = new Shader();  sGreen->CreateFromFiles(vGreen, fGreen); shaderList.push_back(*sGreen);
	Shader* sBlue = new Shader();  sBlue->CreateFromFiles(vBlue, fBlue);   shaderList.push_back(*sBlue);
	Shader* sBrown = new Shader();  sBrown->CreateFromFiles(vBrown, fBrown); shaderList.push_back(*sBrown);
	Shader* sDGrn = new Shader();  sDGrn->CreateFromFiles(vDGreen, fDGreen); shaderList.push_back(*sDGrn);
	// [0]=rojo, [1]=verde, [2]=azul, [3]=café, [4]=verde oscuro
	
	Shader* shader2 = new Shader();//shader para usar color como letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
	
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		
		/*
		//Actividad Letras
		//Para las letras hay que usar el segundo set de shaders en mi caso el 5 en ShaderList 
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
		*/
		
		/*Creamos estas variables para el control de z y son constantes por que las vamos a estar repitiendo dependiendo de la figura*/
		
		const float Z_BODY = -4.0f;  // capa base
		const float Z_FRONT = -3.9f;  // un poquito más cerca (ventanas/puerta)

		glm::mat4 model;

		// CUBO ROJO
		shaderList[0].useShader(); // rojo
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, Z_BODY));
		model = glm::scale(model, glm::vec3(0.85f, 1.10f, 1.0f)); // ancho, alto, espesor Z
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // CUBO

		// PIRÁMIDE AZUL
		shaderList[2].useShader(); // azul
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.55f, Z_BODY));
		model = glm::scale(model, glm::vec3(1.0f, 0.80f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // PIRAMIDE

		shaderList[1].useShader(); // verde 
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.20f, -0.10f, Z_FRONT));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // CUBO

		// derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.20f, -0.10f, Z_FRONT));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // CUBO

		// CUBO VERDE 
		shaderList[1].useShader(); // verde
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, Z_FRONT));
		model = glm::scale(model, glm::vec3(0.30f, 0.40f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // CUBO

		
		//pirámide verde
		shaderList[4].useShader(); // verde fuerte
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, -0.45f, Z_BODY));
		model = glm::scale(model, glm::vec3(0.40f, 0.60f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//  cubo café
		shaderList[3].useShader(); // café
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, -0.85f, Z_BODY));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		shaderList[4].useShader(); // verde fuerte
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.45f, Z_BODY));
		model = glm::scale(model, glm::vec3(0.45f, 0.60f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Tronco
		shaderList[3].useShader(); // café
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.85f, Z_BODY));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/