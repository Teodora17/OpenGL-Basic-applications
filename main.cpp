// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//We need that randomness
#include <ctime> 

//variables
GLFWwindow* window;
const int width = 1024, height = 768;

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides); //function for drawing circle not working atm when combined with the other 2. On empty window it works
//TODO: fix circle drawing
// init camera params

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 200.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//^ Camera vectors.

void kill_self(GLuint& vao, GLuint& vbo, GLuint programID)
{
	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

int drawStage;
float rand_x;
float rand_y;
float rand_z;
GLuint programID;
glm::mat4 MVP;
unsigned int transformLoc;
//glm::mat4 model;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	int pixel[4];
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//green_change = 1.0f;
		//transf = glm::rotate(transf, 0.0f, glm::vec3(0.0, 0.0, 1.0));
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glReadPixels(xpos, ypos, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &pixel);
		std::cout << xpos << "-" << ypos << "\n";
		std::cout << pixel[0] << " " << pixel[1] << " " << pixel[2] << " " << pixel[3] << "\n";

		if (pixel[1] == 0 && pixel[2] == 0)
		{
			if (drawStage == 2)
			{
				glBegin(GL_POLYGON);
				glColor3f(0, 1, 1);
				glVertex2i(-0.5 + rand_x, 0.5 + rand_y);
				glColor3f(0, 1, 1);
				glVertex2i(-0.5 + rand_x, -0.5 + rand_y);
				glColor3f(0, 1, 1);
				glVertex2i(0.5 + rand_x, -0.5 + rand_y);
				glColor3f(0, 1, 1);
				glVertex2i(0.5 + rand_x, 0.5 + rand_y);
				glEnd();
				glFlush();
			}
			if (drawStage == 3)
			{
				glBegin(GL_TRIANGLES);
				glColor3f(0, 1, 1);
				glVertex2i(0.0 + rand_x, 0.5 + rand_y);
				glColor3f(0, 1, 1);
				glVertex2i(-0.5 + rand_x, -0.5 + rand_y);
				glColor3f(0, 1, 1);
				glVertex2i(0.5 + rand_x, -0.5 + rand_y);
				glEnd();
				glFlush();
			}
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		float timer = glfwGetTime();
		glUseProgram(programID);
		MVP = glm::rotate(MVP, 45.0f, glm::vec3(0, 0, 1));
		//unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));
	}
}

float rand_gen() { //generate num between -1 and 1
	float dec = ((float)rand() / RAND_MAX);
	if ((rand() % (10) % 2 == 0)) {
		dec = -dec;
	}
	//std::cout << "TESTRAND-" << dec << "\n";
	return dec;
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	// More about this in the following labs
	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	float g_vertex_buffer_data2[] = {	//decagon
	0.0f,0.0f,0.0f,
	0.0f,0.5f,0.0f,
	0.294f,0.405f,0.0f,
	0.476f,0.1545f,0.0f,
	0.476f,-0.1545f,0.0f,
	0.294f,-0.405f,0.0f,
	0.0f,-0.5f,0.0f,
	-0.294f,-0.405f,0.0f,
	-0.476f,-0.1545f,0.0f,
	-0.476f,0.1545f,0.0f,
	-0.294f,0.405f,0.0f
	};

	unsigned int indices2[] = {  // note that we start from 0!
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 9,
		0, 9, 10,
		0, 10, 1
	};

	float g_vertex_buffer_data[] = {	//square
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2, 3
	};
	float g_vertex_buffer_data1[] = {	//triangle
		0.0f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,-0.5f,0.0f
	};

	unsigned int indices1[] = {  // note that we start from 0!
		0,1,2
	};

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//MOUSE CONTROLS DISABLED FOR EX1

	drawStage = 1; //drawing has 4 stages. 1. Nothing is being drawn, 2. Draw Square, 3. Draw triangle, 4. Draw circle
	rand_x = 0;
	rand_y = 0;
	rand_z = 0;
	int lastStage = drawStage;

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			kill_self(vao, vbo, programID);
			return 0;
		}
		float cam_speed = 0.5f;
		if (glfwGetKey(window, GLFW_KEY_UP)) {
			cameraPos += cam_speed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			cameraPos -= cam_speed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			cameraPos.x -= (cam_speed / 100);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			cameraPos.x += (cam_speed / 100);
		}
		if (glfwGetKey(window, GLFW_KEY_W)) {
			cameraPos.y += (cam_speed / 100);
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			cameraPos.y -= (cam_speed / 100);
		}
		if (glfwGetKey(window, GLFW_KEY_1)) {
			drawStage = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_2)) {
			drawStage = 2;
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_3)) {
			drawStage = 3;
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_STATIC_DRAW);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_4)) {
			drawStage = 4;

			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

		}
		//^ Switch buffers
		if (glfwGetKey(window, GLFW_KEY_4) || glfwGetKey(window, GLFW_KEY_3) || glfwGetKey(window, GLFW_KEY_2)) {
			if (lastStage != drawStage) {
				rand_x = rand_gen();
				rand_y = rand_gen();
				rand_z = cameraPos.z - 200;
				lastStage = drawStage;
			}

		}
		//^ Pick random position if we change drawing mode

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		glm::mat4 model;

		model = glm::translate(model, glm::vec3(rand_x, rand_y, rand_z));
		//^ Translate to random point

		glm::mat4 view;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 trans;
		trans = glm::rotate(trans, glm::radians((float)glfwGetTime() * 100), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), (float)width / height, 0.1f, 1000.0f); //QUAKE-like FOV

		MVP = projection * view * model;

		transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

		//bind VAO
		glBindVertexArray(vao);
		if (drawStage == 2) {
			glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, 0);
		}
		else if (drawStage == 3) {
			glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, 0);
		}
		else if (drawStage == 4) {
			glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			glfwSetMouseButtonCallback(window, mouse_button_callback);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			glfwSetMouseButtonCallback(window, mouse_button_callback);
		//
		//
		//
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();
	}
	return 0;
}


