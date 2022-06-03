#include "utils.h"
#include<cmath>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<stack>


using namespace std;
#define numVAOs 1
#define numVBOs 3
struct Camera {
	float x;
	float y;
	float z;
} camera;
struct {
	float x, y, z;
	float points[108] = {
		-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,-1.0f,
		1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,
		1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,
		-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,
		-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f
	};
	void copyDate(GLuint buffer) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	}
} cube;
struct {
	float x, y, z;
	float points[56] = {
		-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,0.0f,1.0f,0.0f,
		1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f,
		1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f,
		-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,0.0f,1.0f,0.0f,
		-1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,
		1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f
	};
	float textureCoord[36] = {
		0.0f,0.0f,1.0f,0.0f,0.5f,1.0f,0.0f,0.0f,1.0f,0.0f,0.5f,1.0f,
		0.0f,0.0f,1.0f,0.0f,0.5f,1.0f,0.0f,0.0f,1.0f,0.0f,0.5f,1.0f,
		0.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f
	};
	void copyData(GLuint buffer)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	}
	void copyCoord(GLuint buffer)
	{
		glBufferData(GL_ARRAY_BUFFER,sizeof(textureCoord),textureCoord,GL_STATIC_DRAW);
	}
} pyramid;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mvLoc;
GLuint projectLoc;
int width, height;
float aspect;
GLuint brickTexture;
glm::mat4 pMat, vMat, mMat, mvMat;
void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram("vertex.glsl", "fragement.glsl");
	camera.x = 0.f;
	camera.y = 0.f;
	camera.z = 8.f;
	cube.x = 0.f;
	cube.y = -2.f;
	cube.z = 0.f;
	pyramid.x = 2.0f;
	pyramid.y = 0.0f;
	pyramid.z = 0.f;
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	cube.copyDate(vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	pyramid.copyData(vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	pyramid.copyCoord(vbo[2]);
	glfwGetFramebufferSize(window,&width,&height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	brickTexture = loadTexture("iceBrick.png");
}
stack<glm::mat4> mvStack;
void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projectLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-camera.x, -camera.y, -camera.z));
	mvStack.push(vMat);
	//Ì«Ñô
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.f,0.f,0.f));//v1
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));//v2


	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	mvStack.pop();


	//µØÇò
	mvStack.push(mvStack.top());
	//v3
	mvStack.top() *=
		glm::translate(glm::mat4(1.f), glm::vec3(sin((float)currentTime) * 4.0f, 0.0f, cos((float)currentTime) * 4.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));//v4


	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack.pop();


	//ÔÂÁÁ
	mvStack.push(mvStack.top());
	mvStack.top() *=
		glm::translate(glm::mat4(1.f), glm::vec3(0.0f,sin((float)currentTime)*2.0, cos((float)currentTime) * 2.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f),(float)currentTime,glm::vec3(0,0,1));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));

	glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	for (int i = 0; i < 4; i++)mvStack.pop();
}
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	aspect = (float)newWidth / (float)newHeight;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}
int main(void)
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "cube", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_reshape_callback);
	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}