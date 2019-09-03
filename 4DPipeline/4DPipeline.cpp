#include "pch.h"
#include <iostream>

// GL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Stl
#include <vector>

// Local
#include "GLHelpers.h"
#include "Vertex.h"
#include "Shaders.h"
#include "FibonacciPathGenerator.h"
#include "CirclePathGenerator.h"
#include "SinPathGenerator.h"

using namespace std;
using namespace glm;

vec3 light_position(0, 0, -100);
vec3 camera_position(0, 0, -200);
vec3 lookat_position(0, 0, 0);
vec3 camera_up(0, 1, 0);
vec3 up(0, 0, -1);

void error_callback(int error, const char* description)
{
	cerr << "Error: " << description << endl;
}


void update_light_position(GLFWwindow* window, float windowWidth, float windowHeight, const mat4 &projection, const mat4 &view)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	float x = (2.0f * xpos) / windowWidth - 1.0f;
	float y = 1.0f - (2.0f * ypos) / windowHeight;
	float z = 1.0f;
	vec3 ray_nds = vec3(x, y, z);
	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	vec4 ray_eye = inverse(projection) * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	vec3 r = normalize((inverse(view) * ray_eye));
	float d = -dot(vec3(0, 0, light_position.z), up);
	float v = dot(r, up);
	float t = -(dot(camera_position, up) + d) / v;
	vec3 intersection = camera_position + (r * t);

	//
	light_position.x = intersection.x;
	light_position.y = intersection.y;
}


bool nextGenerator = true;
int currentGenerator = -1;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		nextGenerator = true;
	}

	if (key == GLFW_KEY_W && action == GLFW_REPEAT)
		light_position += vec3(0, 0.0f, 1);

	if (key == GLFW_KEY_S && action == GLFW_REPEAT)
		light_position += vec3(0, 0.0f, -1);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main()
{
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Fib Spiral", NULL, NULL);
	if (!window)
		return -1;

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK)
		return -1;

	GLuint vertex_buffer, vertex_shader, fragment_shader, geometry_shader, program;
	GLint mvp_location, model_location, view_location, vertex_position_location, vertex_color_location, light_position_location;

	// 
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	// Setup shaders
	if (GLHelpers::CompileShader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_text) != 0)
		return -1;
	if (GLHelpers::CompileShader(geometry_shader, GL_GEOMETRY_SHADER, geometry_shader_text) != 0)
		return -1;
	if (GLHelpers::CompileShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_text) != 0)
		return -1;
	if (GLHelpers::CreateProgram(program, { vertex_shader, geometry_shader, fragment_shader }) != 0)
		return -1;

	mvp_location = glGetUniformLocation(program, "mvp");
	model_location = glGetUniformLocation(program, "model");
	view_location = glGetUniformLocation(program, "view");
	vertex_position_location = glGetAttribLocation(program, "position");
	vertex_color_location = glGetAttribLocation(program, "color");
	light_position_location = glGetUniformLocation(program, "light_position");

	glEnableVertexAttribArray(vertex_position_location);
	glVertexAttribPointer(vertex_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(vertex_color_location);
	glVertexAttribPointer(vertex_color_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));


	//
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	// Path generators	
	CirclePathGenerator circleGen(30, 100);
	FibonacciPathGenerator fibGen;
	SinPathGenerator sinGen;
	vector<PathGenerator*> generators = { &fibGen, &circleGen, &sinGen };

	//
	mat4 m, v, p, mvp;
	int windowWidth, windowHeight;
	int numVertices = 0;

	float float_move_direction = -0.2f;

	while (!glfwWindowShouldClose(window))
	{
		// Check if we need to generate a new model
		if (nextGenerator)
		{
			currentGenerator++;
			if (currentGenerator > generators.size() - 1)
				currentGenerator = 0;

			vector<Vertex> vertices = generators[currentGenerator]->Generate();

			//
			numVertices = (int)vertices.size();

			// Upload the new data
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, (float*)vertices.data(), GL_STATIC_DRAW);

			nextGenerator = false;
		}

		// Setup
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Models at 0
		m = mat4(1.0f);
		
		// Camera rotating
		v = lookAt<float>(camera_position, lookat_position, camera_up) *
			rotate(mat4(1.0f), (float)-glfwGetTime() * 0.5f, vec3(0, 0, 1));

		//
		p = perspective<float>(radians(60.0f), windowWidth / (float)windowHeight, 0.1f, 2000.0f);

		//
		mvp = p * v * m;		

		// Draw
		glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, value_ptr(mvp));

		if (model_location != -1)
			glUniformMatrix4fv(model_location, 1, GL_FALSE, value_ptr(m));

		if (view_location != -1)
			glUniformMatrix4fv(view_location, 1, GL_FALSE, value_ptr(v));

		// Light
		update_light_position(window, windowWidth, windowHeight, p, v);
		glUniform3fv(light_position_location, 1, value_ptr(light_position));

		glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, numVertices);

		glfwSwapBuffers(window);
		glfwPollEvents();

		int err = glGetError();
		if (err != 0)
		{
			cout << "glError: " << err << endl;
			return -1;
		}
	}

	// Cleanup
	glDeleteProgram(program);
	glDeleteShader(fragment_shader);
	glDeleteShader(geometry_shader);
	glDeleteShader(vertex_shader);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
