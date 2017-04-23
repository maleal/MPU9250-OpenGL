#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h> //for gluLookAt function
#include <cstdio>

#include "Serial.h"
#include "Overlapped.h"
#include "Util.h"

#include <tchar.h>
#include <exception>
#include <vector>
#include <string>
#include <algorithm>	// remove and remove_if

std::string myStr;
std::string delim("|");
float fYaw=0.0f, fPitch=0.0f, fRoll=0.0f;
std::vector<std::string> tokens;

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
	fprintf(stdout, "Renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, "OpenGL version supported %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	return window;
}

void drawCube(CAsynSerial &Ardu, char* buffer)
{
	/*
	GLfloat vertices[] =
	{
		-1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
		1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
		-1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
		-1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
		-1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
		-1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
	};
	GLfloat vertices[] =
	{
		-1, -0.5, -1,   -1, -0.5,  1,   -1,  0.5,  1,   -1,  0.5, -1,	//cara izquierda
		 1, -0.5, -1,    1, -0.5,  1,    1,  0.5,  1,    1,  0.5, -1,	//cara derecha
		-1, -0.5, -1,   -1, -0.5,  1,    1, -0.5,  1,    1, -0.5, -1,	//Cara inferior
		-1,  0.5, -1,   -1,  0.5,  1,    1,  0.5,  1,    1,  0.5, -1,	//Cara sperior
		-1, -0.5, -1,   -1,  0.5, -1,    1,  0.5, -1,    1, -0.5, -1,	//Cara posterior
		-1, -0.5,  1,   -1,  0.5,  1,    1,  0.5,  1,    1, -0.5,  1
	};
	*/
	GLfloat vertices[] =
	{
		-2, -0.5, -1,   -2, -0.5,  1,   -2,  0.5,  1,   -2,  0.5, -1,	//cara izquierda
		 2, -0.5, -1,    2, -0.5,  1,    2,  0.5,  1,    2,  0.5, -1,	//cara derecha
		-2, -0.5, -1,   -2, -0.5,  1,    2, -0.5,  1,    2, -0.5, -1,	//Cara inferior
		-2,  0.5, -1,   -2,  0.5,  1,    2,  0.5,  1,    2,  0.5, -1,	//Cara sperior
		-2, -0.5, -1,   -2,  0.5, -1,    2,  0.5, -1,    2, -0.5, -1,	//Cara posterior
		-2, -0.5,  1,   -2,  0.5,  1,    2,  0.5,  1,    2, -0.5,  1
	};
	/*GLfloat colors[] =
	{
		0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
		1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
		0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
		0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
	};
	*/
	GLfloat colors[] =
	{
		0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1, // FONDO
		0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0, // ABAJO
		1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0, // DERECHA
		1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0, // IZQUIERDA
		1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1, // ARRIBA
		0, 1, 1,   0, 1, 1,   0, 1, 1,   0, 1, 1  // FRENTE
	};
			//Camera view
			//gluLookAt(0.5, 0.5, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	

	try{
		
		Ardu.Readii(buffer, 1024+1);
		myStr.assign(buffer);

		remove_if(myStr.begin(), myStr.end(), ::isspace);
		myStr.erase(remove_if(myStr.begin(), myStr.end(), isspace), myStr.end());

		my_split(myStr, tokens, delim);
		GetYPRValuesFromVector(tokens, fYaw, fPitch, fRoll);

		memset(buffer, '\0', 1024+1);
		myStr.clear();
		tokens.clear();
		

	}catch(std::exception &ex){
		fprintf(stderr, "Comm Error:%s", ex.what()); 
	}

	static float alpha = 0;
	//attempt to rotate cube
			glRotatef(fYaw, 0, 1, 0);	//yaw
			glRotatef(fPitch, 0, 0, 1);	//pitch
			glRotatef(fRoll, 1, 0, 0);	//roll

	/* We have a color array and a vertex array */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	/* Send data : 24 vertices */
	glDrawArrays(GL_QUADS, 0, 24);

	/* Cleanup states */
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	//fYaw += 1;
	//Sleep(250);
}

void display(GLFWwindow* window, CAsynSerial &Ardu)
{
	char buffer[1024+1];

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

		//Perpectiva
		//gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

		glMatrixMode(GL_MODELVIEW_MATRIX);
		glTranslatef(0, 0, -8);

		drawCube(Ardu, buffer);

		// Update Screen
		glfwSwapBuffers(window);

		// Check for any input, or window movement
		glfwPollEvents();
	}
}

int main(int argc, char** argv)
{
	//Comm with Arduino settingup
	
	CAsynSerial Ardu;
	try{
		Ardu.Open(_T("COM3"),0,0);
		Ardu.Setup(CSerial::EBaud115200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	}catch(std::exception &ex) {
		fprintf(stderr, "Exception when SettingUp Comm:%s", ex.what()); 
	}


	GLFWwindow* window = initWindow(1024, 620);
	if (NULL != window)
	{
		//Opened Comm

		display(window, Ardu);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}