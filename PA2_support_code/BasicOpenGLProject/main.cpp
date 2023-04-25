#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "stb/stb_image.h"
#include <iostream>
#include "shader.h"
#include "shaderprogram.h"


/*=================================================================================================
	DOMAIN
=================================================================================================*/

// Window dimensions
const int InitWindowWidth = 800;
const int InitWindowHeight = 800;
int WindowWidth = InitWindowWidth;
int WindowHeight = InitWindowHeight;

// Last mouse cursor position
int LastMousePosX = 0;
int LastMousePosY = 0;

// Arrays that track which keys are currently pressed
bool key_states[256];
bool key_special_states[256];
bool mouse_states[8];

// Other parameters
bool draw_wireframe = false;

/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;

//ShaderProgram persplight;
ShaderProgram texpersplight;

glm::mat4 PerspProjectionMatrix(1.0f);
glm::mat4 PerspViewMatrix(1.0f);
glm::mat4 PerspModelMatrix(1.0f);

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;


/*=================================================================================================
	OBJECTS
=================================================================================================*/

GLuint axis_VAO;
GLuint axis_VBO[2];

float axis_vertices[] = {
	//x axis
	-1.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  0.0f,  0.0f, 1.0f,
	//y axis
	0.0f, -1.0f,  0.0f, 1.0f,
	0.0f,  1.0f,  0.0f, 1.0f,
	//z axis
	0.0f,  0.0f, -1.0f, 1.0f,
	0.0f,  0.0f,  1.0f, 1.0f
};

float axis_colors[] = {
	//x axis
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	//y axis
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	//z axis
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f
};

/*=================================================================================================
	HELPER FUNCTIONS
=================================================================================================*/

void window_to_scene(int wx, int wy, float& sx, float& sy)
{
	sx = (2.0f * (float)wx / WindowWidth) - 1.0f;
	sy = 1.0f - (2.0f * (float)wy / WindowHeight);
}

/*=================================================================================================
	SHADERS
=================================================================================================*/

void CreateTransformationMatrices(void)
{
	// PROJECTION MATRIX
	PerspProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), (float)WindowWidth / (float)WindowHeight, 0.01f, 1000.0f);

	// VIEW MATRIX
	glm::vec3 eye(0.0, 0.0, 2.0);
	glm::vec3 center(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);

	PerspViewMatrix = glm::lookAt(eye, center, up);

	// MODEL MATR../../glew-2.1.0/lib/Release/x64IX
	PerspModelMatrix = glm::mat4(1.0);
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationX), glm::vec3(1.0, 0.0, 0.0));
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationY), glm::vec3(0.0, 1.0, 0.0));
	PerspModelMatrix = glm::scale(PerspModelMatrix, glm::vec3(perspZoom));
}

void CreateShaders(void)
{
	// Renders without any transformations
	PassthroughShader.Create("./shaders/simple.vert", "./shaders/simple.frag");

	// Renders using perspective projection
	PerspectiveShader.Create("./shaders/persp.vert", "./shaders/persp.frag");

	// I added this
	//persplight.Create("./shaders/persplight.vert", "./shaders/persplight.frag");

	// Add this 
	texpersplight.Create("./shaders/texpersplight.vert", "./shaders/texpersplight.frag");
}

/*=================================================================================================
	BUFFERS
=================================================================================================*/

/*====================================================
 Variables for torus calculation
======================================================*/
#include <vector>
#include <iostream>
static float R = 1.0f;
static float r = 0.5f;
static int n = 6;
using namespace std;
float pi = 3.14159f;
float pi2 = 2 * pi;
vector<vector<float>> torusVec;
GLuint torus_VAO;
GLuint torus_VBO[4];
bool flat = false;
bool smooth = true;
bool normalSmooth = false;
GLuint norm_VAO;
GLuint norm_VBO[3];
int counter2 = 0;
/*=================================================================================================
New torus Generating
=================================================================================================*/
vector<float> torusVector2(int n, float R, float r) {
	//create all poosible points of connection through getting theta and phi
	//Then holding these values for points of connection for triangle.
	//making two triganles till all are connected. 
	vector<vector<float>> points;
	vector<float> holder;
	for (float i = 0; i < n; i++) {
		for (float j = 0; j < n; j++) {
			//================================================
			//  First Triangle
			//================================================

			//HOLD A
			holder = { i, j };
			points.push_back(holder);

			//HOLD B
			if (j + 1 == n) {
				holder = { i, 0 };
				points.push_back(holder);
			}
			else {
				holder = { i, j + 1 };
				points.push_back(holder);
			}

			//Hold D
			//If i+1 == n and j+1 == n  if any are == n then use zero
			// else do i+1, j+1
			if (i + 1 == n && j + 1 == n) {
				holder = { 0 , 0 };
				points.push_back(holder);
			}
			else if (j + 1 == n) {
				holder = { i + 1 , 0 };
				points.push_back(holder);
			}
			else if (i + 1 == n) {
				holder = { 0 , j + 1 };
				points.push_back(holder);
			}
			else {
				holder = { i + 1, j + 1 };
				points.push_back(holder);
			}

			//========================================
			//Second triangle 
			//========================================

			//HOLD A
			holder = { i, j };
			points.push_back(holder);

			//Hold D
			//If i+1 == n and j+1 == n  if any are == n then use zero
			// else do i+1, j+1
			if (i + 1 == n && j + 1 == n) {
				holder = { 0 , 0 };
				points.push_back(holder);
			}
			else if (j + 1 == n) {
				holder = { i + 1 , 0 };
				points.push_back(holder);
			}
			else if (i + 1 == n) {
				holder = { 0 , j + 1 };
				points.push_back(holder);
			}
			else {
				holder = { i + 1, j + 1 };
				points.push_back(holder);
			}

			//HOLD C
			if (i + 1 == n) {
				holder = { 0 , j };
				points.push_back(holder);
			}
			else {
				holder = { i + 1, j };
				points.push_back(holder);
			}

		}//end of j loop
	}//end of i loop

	float slice = pi2 / n;
	vector <float> torus2;
	//calculate all points
	for (int k = 0; k < points.size(); k++)
	{
		float theta = slice * points[k][0];
		float phi = slice * points[k][1];
		float x = (R + r * cos(theta)) * cos(phi);
		float y = (R + r * cos(theta)) * sin(phi);
		float z = r * sin(theta);
		torus2.push_back(x);
		torus2.push_back(y);
		torus2.push_back(z);
		torus2.push_back(1.0f);
	}
	return torus2;

}

vector <float> torusColor(int n)
{
	vector <float> torusC;
	for (int i = 0; i < n * n * 6; i++) {
		//blue
		torusC.push_back(1.0f);
		torusC.push_back(1.0f);
		torusC.push_back(1.0f);
		torusC.push_back(1.0f);


	}
	return torusC;
}
/*=================================================================================================
//Failed code
void torusVector(int n, float R, float r) {
	vector<vector<float>> holdVec;
	//calculating all the coords
	float phi = pi2 / n;
	float theta = pi2 / n;
	for (float i = 0; i <= pi2; i += theta) {
		for (float j = 0; j <= pi2; j += phi) {
			float x = (R + r * cos(i)) * cos(j);
			float y = (R + r * cos(i)) * sin(j);
			float z = r * sin(i);
			vector<float> v1 = { x, y, z, 1.0f };
			holdVec.push_back(v1);
		}
	}

	//creating index base on n
	vector<int> index;
	for (int i = 0; i < n-1; i++) {
		int v1 = i * (n + 1);
		int v2 = v1 + n + 1;
		for (int j = 0; j < n; j++) {
			if (v1 >= n*n) {
				v1 = v1 - n*n;
			}
			if (v2 >= n*n) {
				v2 = v2 - n * n;
			}

			index.push_back(v1);
			index.push_back(v2);

			if (v1 + 1 == n * n) {
				v1 = -1;
			}

			index.push_back(v1 + 1);

			index.push_back(v1 + 1);

			if (v2 + 1 == n * n) {
				index.push_back(v2);
				v2 = -1;
				index.push_back(v2 + 1);
			}
			else {
				index.push_back(v2);
				index.push_back(v2 + 1);
			}

			v1++;
			v2++;
		}
	}

	//storing all coords in a global vector of vector
	for (int z = 0; z < index.size(); z++) {
		int numIndex = index[z];
		vector<float> v2 = holdVec.at(numIndex);
		torusVec.push_back(v2);
	}
}
 //Failed test code for torus generating
=================================================================================================*/

vector <float> torusNormal(vector<float> torusArray, int n, float R, float r) {
	vector<vector<float>> holderVec;
	for (int i = 0; i < torusArray.size(); i += 4) {
		vector<float> vec1;
		vec1 = { torusArray[i], torusArray[i + 1], torusArray[i + 2], torusArray[i + 3] };
		holderVec.push_back(vec1);
	}

	vector<float> normal;
	vector<vector<float>> points;
	vector<float> holder;
	if (flat == true) {
		/*
		for (int j = 0; j < holderVec.size(); j += 3) {
			glm::vec3 v0(holderVec[j][0], holderVec[j][1], holderVec[j][2]);
			glm::vec3 v1(holderVec[j + 1][0], holderVec[j + 1][1], holderVec[j + 1][2]);
			glm::vec3 v2(holderVec[j + 2][0], holderVec[j + 2][1], holderVec[j + 2][2]);

			glm::vec3 holder1(1, 1, 1);
			glm::vec3 holder2(1, 1, 1);
			glm::vec3 holder3(1, 1, 1);

			//Getting the normal for v0 of the triangle
			holder1 = v1 - v0;
			holder2 = v2 - v0;
			holder3 = cross(holder1, holder2);
			normal.push_back(holder3.x);
			normal.push_back(holder3.y);
			normal.push_back(holder3.z);
			normal.push_back(1.0f);

			normal.push_back(holder3.x);
			normal.push_back(holder3.y);
			normal.push_back(holder3.z);
			normal.push_back(1.0f);

			normal.push_back(holder3.x);
			normal.push_back(holder3.y);
			normal.push_back(holder3.z);
			normal.push_back(1.0f);
		}
		*/
		for (float i = 0; i < n; i++) {
			for (float j = 0; j < n; j++) {
				holder = { i + 0.5f, j + 0.5f };
				points.push_back(holder);
				points.push_back(holder);
				points.push_back(holder);

				points.push_back(holder);
				points.push_back(holder);
				points.push_back(holder);

			}//end of i loop
		}//end of true for flat
	}
	else if (smooth == true) {
		for (float i = 0; i < n; i++) {
			for (float j = 0; j < n; j++) {
				//================================================
				//  First Triangle
				//================================================

				//HOLD A
				holder = { i, j };
				points.push_back(holder);

				//HOLD B
				if (j + 1 == n) {
					holder = { i, 0 };
					points.push_back(holder);
				}
				else {
					holder = { i, j + 1 };
					points.push_back(holder);
				}

				//Hold D
				//If i+1 == n and j+1 == n  if any are == n then use zero
				// else do i+1, j+1
				if (i + 1 == n && j + 1 == n) {
					holder = { 0 , 0 };
					points.push_back(holder);
				}
				else if (j + 1 == n) {
					holder = { i + 1 , 0 };
					points.push_back(holder);
				}
				else if (i + 1 == n) {
					holder = { 0 , j + 1 };
					points.push_back(holder);
				}
				else {
					holder = { i + 1, j + 1 };
					points.push_back(holder);
				}

				//========================================
				//Second triangle 
				//========================================

				//HOLD A
				holder = { i, j };
				points.push_back(holder);

				//Hold D
				//If i+1 == n and j+1 == n  if any are == n then use zero
				// else do i+1, j+1
				if (i + 1 == n && j + 1 == n) {
					holder = { 0 , 0 };
					points.push_back(holder);
				}
				else if (j + 1 == n) {
					holder = { i + 1 , 0 };
					points.push_back(holder);
				}
				else if (i + 1 == n) {
					holder = { 0 , j + 1 };
					points.push_back(holder);
				}
				else {
					holder = { i + 1, j + 1 };
					points.push_back(holder);
				}

				//HOLD C
				if (i + 1 == n) {
					holder = { 0 , j };
					points.push_back(holder);
				}
				else {
					holder = { i + 1, j };
					points.push_back(holder);
				}
			}//end of j loop
		}//end of i loop
	}
	float slice = pi2 / n;
	//calculate all points
	for (int k = 0; k < points.size(); k++)
	{
		float theta = slice * points[k][0];
		float phi = slice * points[k][1];
		float x = (cos(theta)) * cos(phi);
		float y = (cos(theta)) * sin(phi);
		float z = sin(theta);
		normal.push_back(x);
		normal.push_back(y);
		normal.push_back(z);
		normal.push_back(1.0f);
	}
	return normal;
}

vector <float> torusTexture(int n, float R, float r) {
	vector<vector<float>> points;
	vector<float> holder;
	for (float i = 0; i < n; i++) {
		for (float j = 0; j < n; j++) {
			//================================================
			//  First Triangle
			//================================================

			//HOLD A
			holder = { i, j };
			points.push_back(holder);

			//HOLD B
			if (j + 1 == n) {
				holder = { i, 0 };
				points.push_back(holder);
			}
			else {
				holder = { i, j + 1 };
				points.push_back(holder);
			}

			//Hold D
			//If i+1 == n and j+1 == n  if any are == n then use zero
			// else do i+1, j+1
			if (i + 1 == n && j + 1 == n) {
				holder = { 0 , 0 };
				points.push_back(holder);
			}
			else if (j + 1 == n) {
				holder = { i + 1 , 0 };
				points.push_back(holder);
			}
			else if (i + 1 == n) {
				holder = { 0 , j + 1 };
				points.push_back(holder);
			}
			else {
				holder = { i + 1, j + 1 };
				points.push_back(holder);
			}

			//========================================
			//Second triangle 
			//========================================

			//HOLD A
			holder = { i, j };
			points.push_back(holder);

			//Hold D
			//If i+1 == n and j+1 == n  if any are == n then use zero
			// else do i+1, j+1
			if (i + 1 == n && j + 1 == n) {
				holder = { 0 , 0 };
				points.push_back(holder);
			}
			else if (j + 1 == n) {
				holder = { i + 1 , 0 };
				points.push_back(holder);
			}
			else if (i + 1 == n) {
				holder = { 0 , j + 1 };
				points.push_back(holder);
			}
			else {
				holder = { i + 1, j + 1 };
				points.push_back(holder);
			}

			//HOLD C
			if (i + 1 == n) {
				holder = { 0 , j };
				points.push_back(holder);
			}
			else {
				holder = { i + 1, j };
				points.push_back(holder);
			}

		}//end of j loop
	}//end of i loop

	float slice = pi2 / n;
	vector <float> torus2;
	//calculate all points
	for (int k = 0; k < points.size(); k++)
	{
		float theta = slice * points[k][0];
		float phi = slice * points[k][1];
		float x = (R + r * cos(theta)) * cos(phi);
		float y = (R + r * cos(theta)) * sin(phi);
		float z = r * sin(theta);
		
		float v = theta/pi2;
		//push v into torus2
		torus2.push_back(v);
		
		float u = phi/pi2;
		//push u into torus2
		torus2.push_back(u);
	}
	return torus2;

}

void CreateTorusBuffers(void) {
	//torusVector(n, R, r);
	vector<float> torusVect = torusVector2(n, R, r);
	vector<float> torusC = torusColor(n);
	vector<float> torusNorm = torusNormal(torusVect, n, R, r);
	vector<float> torusTex = torusTexture( n, R, r);

	glGenVertexArrays(1, &torus_VAO);
	glBindVertexArray(torus_VAO);

	glGenBuffers(4, &torus_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, torusVect.size() * sizeof(float), &torusVect[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, torusC.size() * sizeof(float), &torusC[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, torusNorm.size() * sizeof(float), &torusNorm[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, torusTex.size() * sizeof(float), &torusTex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);


	glBindVertexArray(0);
}

vector <float>normColor(int n) {
	vector <float> torusC;
	for (int i = 0; i < n * n * 6 * 2; i++) {
		//red
		torusC.push_back(1.0f);
		torusC.push_back(0.0f);
		torusC.push_back(0.0f);
		torusC.push_back(1.0f);

	}
	return torusC;
}
vector <float> normalCal(int n, float R, float r) {
	vector<float> torusArray = torusVector2(n, R, r);
	vector<float> normalArray = torusNormal(torusArray, n, R, r);
	vector<float> norm;
	if (smooth == true) {
		for (int i = 0; i < torusArray.size(); i += 4) {

			norm.push_back(torusArray[i]);
			norm.push_back(torusArray[i + 1]);
			norm.push_back(torusArray[i + 2]);
			norm.push_back(torusArray[i + 3]);

			norm.push_back(torusArray[i] + normalArray[i]);
			norm.push_back(torusArray[i + 1] + normalArray[i + 1]);
			norm.push_back(torusArray[i + 2] + normalArray[i + 2]);
			norm.push_back(1.0f);

		}
	}
	else if (flat == true) {
		vector<vector<float>> points;
		vector<float> holder;
		cout << "flat color " << endl;
		for (float i = 0; i < n; i++) {
			for (float j = 0; j < n; j++) {
				holder = { i+ 0.5f, j + 0.5f  };
				//holder = { i , j  };
				points.push_back(holder);
				//holder = { i , j + 0.5f };
				points.push_back(holder);
				//holder = { i + 0.5f, j + 0.5f };
				points.push_back(holder);
				//holder = { i, j  };
				points.push_back(holder);
				//holder = { i + 0.5f, j + 0.5f };
				points.push_back(holder);
				//holder = { i + 0.5f, j };
				points.push_back(holder);

			}//end of i loop
		}//end of true for j
		float slice = pi2 / n;
		vector <float> torus2;
		for (int k = 0; k < points.size(); k++)
		{
			float theta = slice * points[k][0];
			float phi = slice * points[k][1];
			float x = (R + r  * cos(theta)) * cos(phi);
			float y = (R + r * cos(theta)) * sin(phi);
			float z = r * sin(theta);
			torus2.push_back(x);
			torus2.push_back(y);
			torus2.push_back(z);
			torus2.push_back(1.0f);
		}
		cout << "HELL)1" << endl;
		for (int l = 0; l < torus2.size(); l += 4) {

			norm.push_back(torus2[l]);
			norm.push_back(torus2[l + 1]);
			norm.push_back(torus2[l + 2]);
			norm.push_back(torus2[l + 3]);

			norm.push_back(torus2[l] + normalArray[l]);
			norm.push_back(torus2[l + 1] + normalArray[l + 1]);
			norm.push_back(torus2[l + 2] + normalArray[l + 2]);
			norm.push_back(1.0f);

		}
		cout << "HELL)2" << endl;
	}

	return norm;
}

void CreateNormalLine(void)
{
	vector<float> normC = normColor(n);
	vector<float> normalLine = normalCal(n, R, r);
	vector<float> torusTex = torusTexture( n, R, r);
	
	glGenVertexArrays(1, &torus_VAO);
	glBindVertexArray(norm_VAO);

	glGenBuffers(3, &norm_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, norm_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, normalLine.size() * sizeof(float), &normalLine[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, norm_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normC.size() * sizeof(float), &normC[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, norm_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, torusTex.size() * sizeof(float), &torusTex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
}

void CreateAxisBuffers(void)
{
	glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	glGenBuffers(2, &axis_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_colors), axis_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


string* name = new string[3];

void CreateTextureBuffers(void)
{
	int widthImg, heightImg, numColCh;
	string arrName[3] = { "myheart.png", "pumpkin.png", "4d73pr.jpg" };
	string* name = &arrName[counter2];
	
	unsigned char* bytes = stbi_load("myheart.png", &widthImg, &heightImg, &numColCh, 0);
	
	GLuint textures;
	glGenTextures(1, &textures);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//float flatColor[] = {1.0f, 0.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(bytes);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void CreateTextureBuffers2(void)
{
	int widthImg, heightImg, numColCh;

	unsigned char* bytes = stbi_load("4d73pr.jpg", &widthImg, &heightImg, &numColCh, 0);

	GLuint textures;
	glGenTextures(1, &textures);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//float flatColor[] = {1.0f, 0.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	//glBindTexture(GL_TEXTURE_2D, 0);
}
void CreateTextureBuffers3(void) {
	int widthImg, heightImg, numColCh;

	unsigned char* bytes = stbi_load("pumpkin.png", &widthImg, &heightImg, &numColCh, 0);

	GLuint textures;
	glGenTextures(1, &textures);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//float flatColor[] = {1.0f, 0.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	//glBindTexture(GL_TEXTURE_2D, 0);
	
}

/*=================================================================================================
	CALLBACKS
=================================================================================================*/

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;

	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

void keyboard_func(unsigned char key, int x, int y)
{
	key_states[key] = true;

	switch (key)
	{

	case 'z': {
		if (flat == true && smooth == false) {
			//if you trying turn off both noraml will cause an error in calculation
			//this prevent system breaking with no normal
		}
		else {
			flat = !flat;
		}

		break;
	}
	case 'x': {
		if (flat == false && smooth == true) {
			//if you trying turn off both noraml will cause an error in calculation
			//this prevent system breaking with no normal
		}
		else {
			smooth = !smooth;
		}
		break;
	}
	case 'c': {
		normalSmooth = !normalSmooth;
		break;
	}
	case 'w':
	{
		r += 0.05f;
		draw_wireframe = !draw_wireframe;
		if (draw_wireframe == true)
			std::cout << "Wireframes on.\n";
		else
			std::cout << "Wireframes off.\n";
		break;
	}
	case 'q': {
		n += 1;
		break;
	}
	case 'a': {
		n -= 1;
		if (n < 3) {
			n = 3;
		}
		break;
	}
	case 's': {
		r -= 0.05f;
		break;
	}
	case 'e': {
		R += 0.05f;
		break;
	}
	case 'd': {
		R -= 0.05f;
		break;
	}

	case ' ': {
		
		if (counter2 == 3) {
			counter2 = 0;
		}
		if (counter2 == 0) {
			CreateTextureBuffers();
		}
		if (counter2 == 1) {
			CreateTextureBuffers2();
		}
		if (counter2 == 2) {
			CreateTextureBuffers3();
		}
		counter2++;
		break;
	}

			// Exit on escape key press
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}//end switch key
}

void key_released(unsigned char key, int x, int y)
{
	key_states[key] = false;
	cout << "flat: " << flat << endl;
	cout << "Smooth: " << smooth << endl;
	cout << "Normal: " << normalSmooth << endl;
	if (normalSmooth == true) {
		CreateNormalLine();
	}
	CreateTorusBuffers();

}

void key_special_pressed(int key, int x, int y)
{
	key_special_states[key] = true;
}

void key_special_released(int key, int x, int y)
{
	key_special_states[key] = false;
}

void mouse_func(int button, int state, int x, int y)
{
	// Key 0: left button
	// Key 1: middle button
	// Key 2: right button
	// Key 3: scroll up
	// Key 4: scroll down

	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (button == 3)
	{
		perspZoom += 0.03f;
	}
	else if (button == 4)
	{
		if (perspZoom - 0.03f > 0.0f)
			perspZoom -= 0.03f;
	}

	mouse_states[button] = (state == GLUT_DOWN);

	LastMousePosX = x;
	LastMousePosY = y;
}

void passive_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	LastMousePosX = x;
	LastMousePosY = y;
}

void active_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (mouse_states[0] == true)
	{
		perspRotationY += (x - LastMousePosX) * perspSensitivity;
		perspRotationX += (y - LastMousePosY) * perspSensitivity;
	}

	LastMousePosX = x;
	LastMousePosY = y;
}

/*=================================================================================================
	RENDERING
=================================================================================================*/

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CreateTransformationMatrices();

	texpersplight.Use();
	texpersplight.SetUniform("projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	texpersplight.SetUniform("viewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	texpersplight.SetUniform("modelMatrix", glm::value_ptr(PerspModelMatrix), 4, GL_FALSE, 1);
	texpersplight.SetUniform("texId", 0);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(axis_VAO);
	glDrawArrays(GL_LINES, 0, 6);

	glBindVertexArray(0);

	glBindVertexArray(torus_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * n * n);
	if (normalSmooth == true) {
		glBindVertexArray(norm_VAO);
		glDrawArrays(GL_LINES, 0, 6 * n * n * 2);
	}
	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glutSwapBuffers();
}

/*=================================================================================================
	INIT
=================================================================================================*/
void init(void)
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	// Set OpenGL settings
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_CULL_FACE); // enable back-face culling

	// Create shaders
	CreateShaders();
	CreateTextureBuffers();
	// Create buffers
	CreateAxisBuffers();
	CreateTorusBuffers();
	std::cout << "Finished initializing...\n\n";
}

/*=================================================================================================
	MAIN
=================================================================================================*/

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitWindowWidth, InitWindowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("CSE-170 Computer Graphics");

	// Initialize GLEW
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString(ret);
		return -1;
	}

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(key_released);
	glutSpecialFunc(key_special_pressed);
	glutSpecialUpFunc(key_special_released);
	glutMouseFunc(mouse_func);
	glutMotionFunc(active_motion_func);
	glutPassiveMotionFunc(passive_motion_func);

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
