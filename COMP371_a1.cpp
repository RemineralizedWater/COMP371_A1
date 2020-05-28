//
// COMP 371 Assignment 1
// Created by Michael Rowe (Student ID: 26101267)
// (Uses: COMP 371 Labs Framework, Created by Nicolas Bergeron on 20/06/2019)
//

// Libraries needed for OpenGL
#include <iostream>
#include <list>
#include <algorithm>

#include <string>		// Required library for some functions in the LoadShaders function (ie getline)
#include <fstream>		// Required library for some functions in the LoadShaders function (ie VertexShaderStream & FragmentShaderStream)
#include <vector>		// Required library for some functions in the LoadShaders function (ie vector)

// Libraries specific to OpenGL (GLEW, GLFW, GLM, & STB_Image)
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"		// STB_Image is an OpenGL texture library

// Allows shortened use of glm & std libraries
using namespace glm;
using namespace std;

// Functions declared at start of file to avoid needing header file
GLuint  LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path);
GLuint loadTexture(const char *filename);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);		// Resets framebuffer size when reset window size
int createCubeVertexArrayObject();
int createLineVertexArrayObject();

struct ColoredVertex
{
	ColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
		: position(_position), color(_color), uv(_uv) {}

	vec3 position;
	vec3 color;
	vec2 uv;
};

// Textured Cube model (used for all models except grid)
const ColoredVertex cubeVertexArray[] = {  // position,					color
	ColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), //left - red
	ColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),

	ColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

	ColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)), // far - red
	ColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),

	ColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	ColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),
	ColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),

	ColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)), // bottom - red
	ColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	ColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

	ColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	ColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
	ColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),

	ColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)), // near - red
	ColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	ColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

	ColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
	ColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

	ColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)), // right - red
	ColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	ColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

	ColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	ColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	ColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),

	ColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)), // top - red
	ColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),

	ColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	ColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f))
};

// Line model (used for grid)
const ColoredVertex lineVertexArray[] = {  // position,				color
	ColoredVertex(vec3(0.0f,0.0f,0.0f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),		//vertices are yellow, to match Assignment 1 photo
	ColoredVertex(vec3(0.0f,0.0f,-100.0f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
};

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setTransWorldMatrix(int shaderProgram, mat4 transWorldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint transWorldMatrixLocation = glGetUniformLocation(shaderProgram, "transWorldMatrix");
	glUniformMatrix4fv(transWorldMatrixLocation, 1, GL_FALSE, &transWorldMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}


int main(int argc, char*argv[])
{
	// Initialize GLFW and OpenGL version
	glfwInit();

	// OS specific OpenGL settings
	#if defined(PLATFORM_OSX)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	#endif

	// Create Window and rendering context using GLFW, resolution is 1024x768
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Assignment 1", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);


	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set viewport size to size of window, the callback function should be called on every window resize to reset viewport to window size
	glViewport(0, 0, 1024, 768);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load Textures (Downloaded from third-parties for Fair Use Educational Purposes only)
	#if defined(PLATFORM_OSX)
		GLuint carrotTextureID = loadTexture("Textures/carrot.jpg");
		GLuint coalTextureID = loadTexture("Textures/coal.jpg");
		GLuint cornTextureID = loadTexture("Textures/corn.jpg");
		GLuint hairTextureID = loadTexture("Textures/hair.jpg");
		GLuint snowTextureID = loadTexture("Textures/snow.jpg");
		GLuint woodTextureID = loadTexture("Textures/wood.jpg");
	#else
		GLuint carrotTextureID = loadTexture("../Assets/Textures/carrot.jpg");
		GLuint coalTextureID = loadTexture("../Assets/Textures/coal.jpg");
		GLuint cornTextureID = loadTexture("../Assets/Textures/corn.jpg");
		GLuint hairTextureID = loadTexture("../Assets/Textures/hair.jpg");
		GLuint snowTextureID = loadTexture("../Assets/Textures/snow.jpg");
		GLuint woodTextureID = loadTexture("../Assets/Textures/wood.jpg");
	#endif

	// Changed background to match assignment 1 background color (RGB: 51,76,76)
	glClearColor(0.2f, 0.298f, 0.298f, 1.0f);
	glClearDepth(1.0f);		// specifies the clear value for the depth buffer

	// Load, compile and link external shader files here ...
	#if defined(PLATFORM_OSX)
		int colorShaderProgram = LoadShaders("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
		int texturedShaderProgram = LoadShaders("Shaders/TexturedVertexShader.glsl", "Shaders/TexturedFragmentShader.glsl");
	#else
		int colorShaderProgram = LoadShaders("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentShader.glsl");
		int texturedShaderProgram = LoadShaders("../Assets/Shaders/TexturedVertexShader.glsl", "../Assets/Shaders/TexturedFragmentShader.glsl");
	#endif

	// Camera parameters for view transform
	vec3 cameraPosition(0.0f, 0.0f, 25.0f);
	vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Other camera parameters
	float cameraSpeed = 8.0f;
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;

	// Used to transform the world
	mat4 transWorldMatrix = mat4(1.0f);

	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
		1024.0f / 768.0f,  // aspect ratio
		0.01f, 20000.0f);   // near and far (near > 0)		// Far plane set to 20000.0f

	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	// Set View and Projection matrices on both shaders
	setViewMatrix(colorShaderProgram, viewMatrix);
	setViewMatrix(texturedShaderProgram, viewMatrix);

	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);

	// Define and upload geometry to the GPU here ...
	int cubeVAO = createCubeVertexArrayObject();
	int lineVAO = createLineVertexArrayObject();

	// For frame time
	float lastFrameTime = glfwGetTime();
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once
	// Enable Backface culling
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);	// Enables Depth Test for Hidden Surface Removal. By default the glDepthFunc is set to GL_LESS (ie closer object passes test and further removed)

	glBindVertexArray(cubeVAO);

	glActiveTexture(GL_TEXTURE0); // #0 texture unit 
								  // GL_TEXTURE1, .. GL_TEXTURE31
								  // GL_TEXTURE0 + 1
	glBindTexture(GL_TEXTURE_2D, carrotTextureID);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, coalTextureID);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, cornTextureID);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, hairTextureID);

	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, snowTextureID);

	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, woodTextureID);

	// Set Render Mode
	GLenum renderMode = GL_TRIANGLES;

	// Olaf variables
	float olafTransX = 0.0f;		// Olaf X position
	float olafTransZ = 0.0f;		// Olaf Z position
	float olafScale = 1.0f;			// Olaf scale (1 = normal)
	float olafRotateY = 0.0f;			// Olaf rotation (in radians?)

	/* COMMENT: Implement other rotations later
	// Additional Olaf rotation and translation parameters
	float olafRotateX = 0.0f;
	float olafRotateZ = 0.0f;
	float olafTransY = 0.0f;		// Olaf Z position */

	float transWorldRotateX = 0.0f;		// Tracks Transformed world X rotation
	float transWorldRotateY = 0.0f;		// Tracks Transformed world Y rotation
	float transWorldRotateZ = 0.0f;		// Tracks Transformed world Y rotation

	int lastKeySpaceState = GLFW_RELEASE;		// Ensures random position isn't spammed
	int lastMouseLeftState = GLFW_RELEASE;
	int lastMouseMiddleState = GLFW_RELEASE;
	int lastMouseRightState = GLFW_RELEASE;
	bool heldMouseLeftState = false;					// Used to check whether left mouse button is being held
	bool heldMouseMiddleState = false;					// Used to check whether middle mouse button is being held
	bool heldMouseRightState = false;					// Used to check whether right mouse button is being held
	//bool capsLockToggle = false;						// Used to check whether caps lock is toggled
	//int lastKeyCapsState = GLFW_RELEASE;				// Ensures caps lock isn't spammed

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");	// Used to set texture shader program to load own textures

		GLuint vertexColorLocation = glGetUniformLocation(colorShaderProgram, "ourColor");		// Used to set the fragment shader program, to use our own color for vertices

		glUseProgram(colorShaderProgram);
		glUniform4f(vertexColorLocation, 1.0f, 1.0f, 0.0f, 1.0f);			// Sets fragment color for our vertex array to yellow

		// World Matrix initializations
		glBindVertexArray(lineVAO);		// Binds line vertex array object
		mat4 worldMatrix = mat4(1.0f);

		// Draws Grid, starting at -50.0f, 0.0f, 50.0f, so that it's centered at origin
		for (int i = 0; i <= 100; i++) {
			worldMatrix = translate(mat4(1.0f), vec3(-50.0f, 0.0f, 50.0f)) * translate(mat4(1.0f), vec3(i * 1.0f, 0.0f, 0.0f));
			setWorldMatrix(colorShaderProgram, worldMatrix);
			glDrawArrays(GL_LINES, 0, 2);
		}
		for (int k = 0; k <= 100; k++) {
			worldMatrix = translate(mat4(1.0f), vec3(-50.0f, 0.0f, 50.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, -k * 1.0f)) * rotate(mat4(1.0f), radians(270.0f), vec3(0.0f, 1.0f, 0.0f));
			setWorldMatrix(colorShaderProgram, worldMatrix);
			glDrawArrays(GL_LINES, 0, 2);
		}
		
		// Switch to cube vertex array object
		glBindVertexArray(cubeVAO);
		glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);			// Sets fragment color for our vertex array to red

		// Draw Coordinate Axes
		worldMatrix = translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));		// X-Coordinate Axis
		setWorldMatrix(colorShaderProgram, worldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);			// Sets fragment color for our vertex array to green

		worldMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));		// Y-Coordinate Axis
		setWorldMatrix(colorShaderProgram, worldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform4f(vertexColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);			// Sets fragment color for our vertex array to green

		worldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));		// Z-Coordinate Axis
		setWorldMatrix(colorShaderProgram, worldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Olaf feet
		for (int i = -1; i < 2; i+=2) {
			// Translate feet to olafRotate - 90.0f due to feet positions being perpendicular to facing direction
			worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * translate(mat4(1.0f), vec3(sinf(radians(olafRotateY - 90.0f)) * (i * 0.75f) * olafScale, 0.375f * olafScale, cosf(radians(olafRotateY - 90.0f)) * (i * 0.75f) * olafScale))
				* rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f * olafScale, 0.75f * olafScale, 0.75f * olafScale));
			setWorldMatrix(texturedShaderProgram, worldMatrix);
			glUniform1i(textureLocation, 4);                // Set our Texture sampler to use Texture Unit 4 (snow)
			glDrawArrays(renderMode, 0, 36);
		}

		// Olaf lower body
		worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * translate(mat4(1.0f), vec3(0.0f, 2.725f * olafScale, 0.0f)) *
			rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(4.0f * olafScale, 4.0f * olafScale, 1.0f * olafScale));
		setWorldMatrix(texturedShaderProgram, worldMatrix);
		glDrawArrays(renderMode, 0, 36);

		// Olaf upper body
		worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * translate(mat4(1.0f), vec3(0.0f, 5.4f * olafScale, 0.0f)) *
			rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(3.0f * olafScale, 1.5f * olafScale, 1.0f * olafScale));
		setWorldMatrix(texturedShaderProgram, worldMatrix);
		glDrawArrays(renderMode, 0, 36);

		// Olaf head
		worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * translate(mat4(1.0f), vec3(0.0f, 6.9f * olafScale, 0.0f)) *
			rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.25f * olafScale, 1.5f * olafScale, 1.0f * olafScale));
		setWorldMatrix(texturedShaderProgram, worldMatrix);
		glDrawArrays(renderMode, 0, 36);

		// Olaf arms
		for (int i = -1; i < 2; i += 2) {
			// Translate arms to olafRotate - 90.0f due to arm positions being perpendicular to facing direction
			worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * translate(mat4(1.0f), vec3(sinf(radians(olafRotateY - 90.0f)) * (i * 3.5f) * olafScale, 5.4125f * olafScale, cosf(radians(olafRotateY - 90.0f)) * (i * 3.5f) * olafScale)) *
				rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(4.0f * olafScale, 0.5f * olafScale, 0.5f * olafScale));
			setWorldMatrix(texturedShaderProgram, worldMatrix);
			glUniform1i(textureLocation, 5);                // Set our Texture sampler to use Texture Unit 5 (wood)
			glDrawArrays(renderMode, 0, 36);
		}

		// Olaf fingers
		for (int i = -1; i < 2; i += 2) {
			for (int j = 0; j < 4; j++) {
				// Translate fingers to olafRotate - 90.0f due to finger positions being perpendicular to facing direction
				worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * translate(mat4(1.0f), vec3(sin(radians(olafRotateY - 90.0f)) * (i * 6.0f) * olafScale, (5.2125f + j * 0.1325f) * olafScale, cos(radians(olafRotateY - 90.0f)) * (i * 6.0f) * olafScale)) *
					rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f * olafScale, 0.1f * olafScale, 0.125f * olafScale));
				setWorldMatrix(texturedShaderProgram, worldMatrix);
				glUniform1i(textureLocation, 5);
				glDrawArrays(renderMode, 0, 36);
			}
			// Translate thumbs to olafRotate - 90.0f due to thumb positions being perpendicular to facing direction
			worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * translate(mat4(1.0f), vec3(sin(radians(olafRotateY - 90.0f)) * (i * 5.25f) * olafScale, 5.8625f * olafScale, cos(radians(olafRotateY - 90.0f)) * (i * 5.25f) * olafScale)) *
				rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f * olafScale, 0.5f * olafScale, 0.125f * olafScale));
			setWorldMatrix(texturedShaderProgram, worldMatrix);
			glUniform1i(textureLocation, 5);
			glDrawArrays(renderMode, 0, 36);
		}

		// Olaf eyes
		for (int i = -1; i < 2; i += 2) {
			worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) *
				translate(mat4(1.0f), vec3((i * 0.5625f) * olafScale, 7.15f * olafScale, 0.5f * olafScale))	* scale(mat4(1.0f), vec3(0.375f * olafScale, 0.375f * olafScale, 0.125f * olafScale));
			setWorldMatrix(texturedShaderProgram, worldMatrix);
			glUniform1i(textureLocation, 1);                // Set our Texture sampler to use Texture Unit 1 (coal)
			glDrawArrays(renderMode, 0, 36);
		}

		// Olaf nose
		worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) *
			translate(mat4(1.0f), vec3(0.0f, 6.9f * olafScale, 1.25f * olafScale)) * scale(mat4(1.0f), vec3(0.375f * olafScale, 0.375f * olafScale, 1.5f * olafScale));
		setWorldMatrix(texturedShaderProgram, worldMatrix);
		glUniform1i(textureLocation, 0);                // Set our Texture sampler to use Texture Unit 0 (carrot)
		glDrawArrays(renderMode, 0, 36);

		// Olaf mouth
			worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) *
				translate(mat4(1.0f), vec3(0.0f, 6.55f * olafScale, 0.5f * olafScale))	* scale(mat4(1.0f), vec3(1.00f * olafScale, 0.225f * olafScale, 0.125f * olafScale));
			setWorldMatrix(texturedShaderProgram, worldMatrix);
			glUniform1i(textureLocation, 2);                // Set our Texture sampler to use Texture Unit 2 (corn)
			glDrawArrays(renderMode, 0, 36);

		// Olaf hair
		for (int i = 0; i < 18; i++) {
			for (int k = 0; k < 8; k++) {
				worldMatrix = translate(mat4(1.0f), vec3(olafTransX, 0.0f, olafTransZ)) * rotate(mat4(1.0f), radians(olafRotateY), vec3(0.0f, 1.0f, 0.0f)) *
					translate(mat4(1.0f), vec3((-1.0625f + i * 0.125f) * olafScale, 7.775f * olafScale, (0.425f - 0.125 * k) * olafScale)) * scale(mat4(1.0f), vec3(0.1f * olafScale, 0.25f * olafScale, 0.1f * olafScale));
				setWorldMatrix(texturedShaderProgram, worldMatrix);
				glUniform1i(textureLocation, 3);                // Set our Texture sampler to use Texture Unit 3 (hair)
				glDrawArrays(renderMode, 0, 36);
			}
		}

		// Transform World
		transWorldMatrix = rotate(mat4(1.0f), radians(transWorldRotateX), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(transWorldRotateY), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(transWorldRotateZ), vec3(0.0f, 0.0f, 1.0f));
		setTransWorldMatrix(colorShaderProgram, transWorldMatrix);
		setTransWorldMatrix(texturedShaderProgram, transWorldMatrix);
		
		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// - Calculate mouse motion dx and dy
		// - Update camera horizontal and vertical angle
		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX;
		double dy = mousePosY;
		dx = mousePosX - lastMousePosX;
		dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		const float cameraAngularSpeed = 60.0f;

		// Clamp vertical angle to [-89, 89] degrees (avoid risk of gimbal issues)
		cameraVerticalAngle = std::max(-89.0f, std::min(89.0f, cameraVerticalAngle));

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		//vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
		//glm::normalize(cameraSideVector);

		// Modifies scene render mode between triangles, lines and points
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {	// Set render mode to points
			renderMode = GL_POINTS;
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {	// Set render mode to lines
			renderMode = GL_LINES;
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {	// Set render mode to triangles
			renderMode = GL_TRIANGLES;
		}

		// Increases/Decreases Olaf's size
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) { // Multiplies Olaf scale by 0.01f
			olafScale *= 1.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) { // Divides Olaf scale by 0.01f
			if (olafScale > 0.01f) {
				olafScale /= 1.01f;
			}
		}

		/* Was used to track caps lock toggle
		if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS) {
			if (capsLockToggle) {
				capsLockToggle = false;
			}
			else {
				capsLockToggle = true;
			}
		}
		lastKeyCapsState = (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS); */

		// Reposition Olaf (by using shift modifier), or rotate Olaf
		if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafTransZ += 0.1 * cosf(radians(olafRotateY));
			olafTransX += 0.1 * sinf(radians(olafRotateY));
		}
		if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafTransZ -= 0.1 * cosf(radians(olafRotateY));
			olafTransX -= 0.1 * sinf(radians(olafRotateY));
		}
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafTransZ -= 0.1 * sinf(radians(olafRotateY));
			olafTransX += 0.1 * cosf(radians(olafRotateY));
		}
		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafTransZ += 0.1 * sinf(radians(olafRotateY));
			olafTransX -= 0.1 * cosf(radians(olafRotateY));
		}

		// Rotate Olaf
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafRotateY += 5.0f;
			if (olafRotateY >= 360.0f) {
				olafRotateY -= 360.0f;
			}
		}
		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafRotateY -= 5.0f;
			if (olafRotateY <= -360.0f) {
				olafRotateY += 360.0f;
			}
		}
		/* COMMENT: Implement other rotations later
		if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafRotateX += 5.0f;
			if (olafRotateX >= 360.0f) {
				olafRotateX -= 360.0f;
			}
		}
		if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafRotateX -= 5.0f;
			if (olafRotateX <= -360.0f) {
				olafRotateX += 360.0f;
			}
		}
		if ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafRotateZ += 5.0f;
			if (olafRotateZ >= 360.0f) {
				olafRotateZ -= 360.0f;
			}
		}
		if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) == GLFW_PRESS) {
			olafRotateZ -= 5.0f;
			if (olafRotateZ <= -360.0f) {
				olafRotateZ += 360.0f;
			}
		} */

		if (lastKeySpaceState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // Re-position Olaf at random location on the grid
			olafTransX = rand() % 101 - 50;
			olafTransZ = rand() % 101 - 50;
		}
		lastKeySpaceState = glfwGetKey(window, GLFW_KEY_SPACE);

		// Transforms the world space orientation around all 3 axes, home resets the world orientation transformations
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			transWorldRotateX += 5.0f;
			if (transWorldRotateX >= 360.0f) {
				transWorldRotateX -= 360.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			transWorldRotateX -= 5.0f;
			if (transWorldRotateX <= 360.0f) {
				transWorldRotateX += 360.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			transWorldRotateY += 5.0f;
			if (transWorldRotateY >= 360.0f) {
				transWorldRotateY -= 360.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			transWorldRotateY -= 5.0f;
			if (transWorldRotateY <= 360.0f) {
				transWorldRotateY += 360.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
			transWorldRotateZ += 5.0f;
			if (transWorldRotateZ >= 360.0f) {
				transWorldRotateZ -= 360.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
			transWorldRotateZ -= 5.0f;
			if (transWorldRotateZ <= 360.0f) {
				transWorldRotateZ += 360.0f;
			}
		}
		// Resets World Orientation transformations
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
			transWorldRotateX = transWorldRotateY = transWorldRotateZ = 0.0f;
		}

		// Mouse button toggles to enable camera zoom, tilt & pan upon mouse movements
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cameraPosition -= cameraLookAt * dt * cameraSpeed * (float)dy;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
			cameraVerticalAngle -= -1 * (dy * cameraAngularSpeed * dt);
			//cout << cameraVerticalAngle << "\n";
			if ((cameraVerticalAngle >= 90.0f && cameraVerticalAngle < 180.0f) || (cameraVerticalAngle <= -90.0f  && cameraVerticalAngle > -180.0f) 
			|| (cameraVerticalAngle >= 270.0f && cameraVerticalAngle < 360.0f) || (cameraVerticalAngle <= -270.0f && cameraVerticalAngle < -360.0f)) {
				// COMMENT: FIX FOR INVERTED VERTICAL CAMERA
			}
			if (cameraVerticalAngle <= -360.0f) {
				cameraVerticalAngle += 360.0f;
			}
			if (cameraVerticalAngle >= 360.0f) {
				cameraVerticalAngle -= 360.0f;
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
			//cout << cameraHorizontalAngle << "\n";
			if (cameraHorizontalAngle <= -360.0f) {
				cameraHorizontalAngle += 360.0f;
			}
			if (cameraHorizontalAngle >= 360.0f) {
				cameraHorizontalAngle -= 360.0f;
			}
		}

		// Set the view matrix for first person camera
		// - In first person, camera lookat is set like below
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

		setViewMatrix(colorShaderProgram, viewMatrix);
		setViewMatrix(texturedShaderProgram, viewMatrix);
	}

	glfwTerminate();

	return 0;
}

// The following code is taken from
// www.opengl-tutorial.org
GLuint  LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// return shader program id
	return ProgramID;
}

GLuint loadTexture(const char *filename)
{
	// Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);


	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // shrink the texture, one pixel is mapped many texels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

// Vertex Buffer Object for the Cube Vertex Array
int createCubeVertexArrayObject()
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexArray), cubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(ColoredVertex), // stride - each vertex contain 2 vec3 (position, color), 1 vec1 (uv)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ColoredVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ColoredVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

	return vertexArrayObject;
}

// Vertex Buffer Object for the Line Vertex Array
int createLineVertexArrayObject()
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertexArray), lineVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(ColoredVertex), // stride - each vertex contain 2 vec3 (position, color), 1 vec1 (uv)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ColoredVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ColoredVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

	return vertexArrayObject;
}

// Called to modify framebuffer size when window size is modified
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
