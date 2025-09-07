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
vector<MeshColor*> meshColorList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";  // 
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearTetraedroNegro()
{
	GLfloat vertices_tetraedro[] = {
		// (base)
		 0.000000f,  0.577350f, -0.204124f,   0.0f, 0.0f, 0.0f,
		-0.500000f, -0.288675f, -0.204124f,   0.0f, 0.0f, 0.0f,
		 0.500000f, -0.288675f, -0.204124f,   0.0f, 0.0f, 0.0f,

		
		  0.000000f,  0.577350f, -0.204124f,   0.0f, 0.0f, 0.0f,
		  0.000000f,  0.000000f,  0.612372f,   0.0f, 0.0f, 0.0f,
		 -0.500000f, -0.288675f, -0.204124f,   0.0f, 0.0f, 0.0f,

		 -0.500000f, -0.288675f, -0.204124f,   0.0f, 0.0f, 0.0f,
		  0.000000f,  0.000000f,  0.612372f,   0.0f, 0.0f, 0.0f,
		  0.500000f, -0.288675f, -0.204124f,   0.0f, 0.0f, 0.0f,

		   0.500000f, -0.288675f, -0.204124f,   0.0f, 0.0f, 0.0f,
		   0.000000f,  0.000000f,  0.612372f,   0.0f, 0.0f, 0.0f,
		   0.000000f,  0.577350f, -0.204124f,   0.0f, 0.0f, 0.0f,
	};

	unsigned int indices_tetraedro[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9,10,11
	};

	MeshColor* tetraedo_base = new MeshColor();
	tetraedo_base->CreateMeshColor(vertices_tetraedro, 72); 
	meshColorList.push_back(tetraedo_base);
}

// Tetraedro regular con “aperturas” entre caras (triángulos más pequeños por cara)
void CrearTetraedro()
{
	GLfloat vertices_tetraedro[] = {
		// Cara 1 (rojo)  A,B,C  [base con huecos]
		 0.000000f,  0.490748f, -0.204124f,   1.0f, 0.0f, 0.0f,
		-0.425000f, -0.245374f, -0.204124f,   1.0f, 0.0f, 0.0f,
		 0.425000f, -0.245374f, -0.204124f,   1.0f, 0.0f, 0.0f,

		 // Cara 2 (verde) A,D,B  [separada de las otras]
		 -0.025000f,  0.505181f, -0.163299f,   0.0f, 1.0f, 0.0f,
		 -0.025000f,  0.014434f,  0.530722f,   0.0f, 1.0f, 0.0f,
		 -0.450000f, -0.230940f, -0.163299f,   0.0f, 1.0f, 0.0f,

		 // Cara 3 (azul)  B,D,C  [separada de las otras]
		 -0.425000f, -0.274241f, -0.163299f,   0.0f, 0.0f, 1.0f,
		  0.000000f, -0.028868f,  0.530722f,   0.0f, 0.0f, 1.0f,
		  0.425000f, -0.274241f, -0.163299f,   0.0f, 0.0f, 1.0f,

		  // Cara 4 (amarillo) C,D,A [separada de las otras]
		   0.450000f, -0.230940f, -0.163299f,   1.0f, 1.0f, 0.0f,
		   0.025000f,  0.014434f,  0.530722f,   1.0f, 1.0f, 0.0f,
		   0.025000f,  0.505181f, -0.163299f,   1.0f, 1.0f, 0.0f,
	};

	unsigned int indices_tetraedro[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9,10,11
	};

	MeshColor* tetraedo = new MeshColor();
	tetraedo->CreateMeshColor(vertices_tetraedro, 72);
	meshColorList.push_back(tetraedo);
}



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
	meshList.push_back(obj1);

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
	meshList.push_back(cilindro);
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
	meshList.push_back(cono);
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
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa
	CrearTetraedroNegro();
	CrearTetraedro();
	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(5, 1.0f);//�ndice 2 en MeshList
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

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

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

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// base de la escena frente a la camara
		glm::mat4 base = glm::mat4(1.0f);
		base = glm::translate(base, glm::vec3(0.0f, 0.0f, -8.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		/*¿Qué es lo que hace model? basicamente es el modelo del cual parti,
		es como una transformacion global y de esta manera se mueven los objetos juntos, evito estar epitiendo todo ese codigo
		para cada forma.*/
		glm::mat4 model;
		glm::vec3 color;

		model = base;
		shaderList[1].useShader();  // shader que usa vShaderColor + fShaderColor

		GLuint model2 = shaderList[1].getModelLocation();
		GLuint proj2 = shaderList[1].getProjectLocation();
		GLuint view2 = shaderList[1].getViewLocation();

		glUniformMatrix4fv(proj2, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(view2, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// Tetraedro base (negro, grande)
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor();

		model = base;
		model = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		model = glm::translate(model, glm::vec3(-1.995f, -0.8f, 1.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, 1.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		model = glm::translate(model, glm::vec3(2.0f, -0.8f, 1.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		/*boca abajo*/
		model = base;
		model = glm::translate(model, glm::vec3(1.0f, 0.8f, 0.6f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		model = glm::translate(model, glm::vec3(-1.0f, 0.8f, 0.6f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		/*PUNTA ABAJO*/
		model = base;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-39.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.0f, -0.6f, 0.9f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-39.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.0f, -0.6f, 0.9f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		// Punta abajo
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-38.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.15f, -0.73f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));

		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		/***************************CARA VERDE*****************************************/
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, -1.2f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -2.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		model = base;
		model = glm::translate(model, glm::vec3(-1.0f, -0.8f, -0.55f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//abajo-izquiedo
		model = base;
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.8f));
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();


		/* derecho-abajo*/

		model = base;
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, 0.8f));
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		/* arriba*/

		model = base;
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(-1.0f, 0.6f, -0.8f));
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		/*CARA AMARILLA*/

				/********Tringulos que faltan en la parte de abajo*********/
		model = base;
		model = glm::translate(model, glm::vec3(1.0f, -0.8f, -0.55f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();
	
		// arriba
		model = base;
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0)); 
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0)); 
		model = glm::rotate(model, glm::radians(-38.0f), glm::vec3(1, 0, 0)); 
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 0, 1)); 
		model = glm::translate(model, glm::vec3(0.95f, 0.58f, -0.75f));      
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//  (abajo-izq) 
		model = base;
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(-0.02f, 1.18f, 0.85f)); 
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//  (abajo-der)
		model = base;
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-38.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(1.0f, -0.55f, 0.85f)); 
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();


		/*************ROJAS ****************/
		
		// (arriba)
		model = base;
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(1, 0, 0)); 
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));   
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));    
		model = glm::translate(model, glm::vec3(0.00f,-1.1f, -0.85f)); 
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		// (abajo–izq) 
		model = base;
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(1, 0, 0));  
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));  
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));  
		model = glm::translate(model, glm::vec3(1.00f, 0.6f, -0.8f));
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//  (abajo–der)
		model = base;
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(1, 0, 0));   
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));   
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));   
		model = glm::translate(model, glm::vec3(-1.00f, 0.6f, -0.8f));
		model = glm::scale(model, glm::vec3(2.1f));
		glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();
	

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}