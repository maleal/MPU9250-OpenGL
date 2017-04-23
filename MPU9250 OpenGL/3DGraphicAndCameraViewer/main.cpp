#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include <cstdio>
#include "Serial.h"
#include "Overlapped.h"
#include <tchar.h>



void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* initWindow(const int resX, const int resY)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

									 // Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, controls);

	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	return window;
}


void drawCube()
{
	/*cartesian axes*/
	GLfloat aver[] = { 0,0,0, 1,0,0, 
		               0,0,0, 0,1,0,
		               0,0,0, 0,0,1 };
	GLfloat acol[] = { 1,0,0, 1,0,0,
					   0,1,0, 0,1,0,
					   0,0,1, 0,0,1 };

	/* A Cube*/
	GLfloat vertices[] =
	{
		-1, -0.5, -1,   -1, -0.5,  1,   -1,  0.5,  1,   -1,  0.5, -1,
		 1, -0.5, -1,    1, -0.5,  1,    1,  0.5,  1,    1,  0.5, -1,
		-1, -0.5, -1,   -1, -0.5,  1,    1, -0.5,  1,    1, -0.5, -1,	//Cara inferior
		-1,  0.5, -1,   -1,  0.5,  1,    1,  0.5,  1,    1,  0.5, -1, //Cara sperior
		-1, -0.5, -1,   -1,  0.5, -1,    1,  0.5, -1,    1, -0.5, -1,
		-1, -0.5,  1,   -1,  0.5,  1,    1,  0.5,  1,    1, -0.5,  1
	};
	GLfloat colors[] =
	{
		0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1, // FONDO
		0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0, // ABAJO
		1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0, // DERECHA
		1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0, // IZQUIERDA
		1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1, // ARRIBA
		0, 1, 1,   0, 1, 1,   0, 1, 1,   0, 1, 1  // FRENTE
	};


	gluLookAt(0.5, 0.5, 1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//glRotatef(45, 1, 0, 0);
	//glRotatef(45, 0, 1, 0);

	/* We have a color array and a vertex array */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Set Axes data
	glVertexPointer(3, GL_FLOAT, 0, aver);
	glColorPointer(3, GL_FLOAT, 0, acol);

	// Set Cube data
	//glVertexPointer(3, GL_FLOAT, 0, vertices);
	//glColorPointer(3, GL_FLOAT, 0, colors);

	/* Send data Axes: 6 vertices */
	glDrawArrays(GL_LINES, 0, 6);
	/* Send data Cube: 24 vertices */
	//glDrawArrays(GL_QUADS, 0, 24);


	/* Cleanup states */
	static float alpha = 0;
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	alpha += 1;


	
}

void display(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		// Scale to window size
		GLint windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		// Draw stuff
		//glClearColor(0.0, 0.8, 0.3, 1.0);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();

		//gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
		
		glMatrixMode(GL_MODELVIEW_MATRIX);
		glTranslatef(0, 0, -5);

		

		drawCube();

		// Update Screen
		glfwSwapBuffers(window);

		// Check for any input, or window movement
		glfwPollEvents();
	}
}

int main(int argc, char** argv)
{
	CAsynSerial Ardu;
	Ardu.Open(_T("COM3"),0,0);
	Ardu.Setup(CSerial::EBaud38400,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);

	GLFWwindow* window = initWindow(1024, 620);
	if (NULL != window)
	{
		display(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}