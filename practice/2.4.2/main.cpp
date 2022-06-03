#include<iostream>
#include<fstream>
#include<sstream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

using namespace std;
#define numOfVao 2
#define numOfVbo 1

void attachVertShader(const char* vert,GLuint program)
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	ifstream in(vert,ios::in);
	stringstream stream;
	stream << in.rdbuf();
	in.close();
	string context = stream.str();
	const char* txt = context.c_str();
	glShaderSource(vShader, 1, &txt, NULL);
	glCompileShader(vShader);

	int error;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &error);
	if (!error) {
		char msg[128];
		glGetShaderInfoLog(vShader, 128, NULL, msg);
		cout << "COMPILE VERTEXT SHADER FAILED:" << msg << endl;
		exit(EXIT_FAILURE);
	}
	glAttachShader(program, vShader);
}
void attachFragShader(const char* frag, GLuint program)
{
	ifstream in(frag, ios::in);
	stringstream stream;
	stream << in.rdbuf();
	in.close();
	string code = stream.str();
	const char* txt = code.c_str();
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader,1,&txt,NULL);
	glCompileShader(fShader);
	int error;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &error);
	if (!error)
	{
		char msg[128];
		glGetShaderInfoLog(fShader, 128, NULL, msg);
		cout << "COMPILE FRAGMENT SHADER FAILED:" << msg << endl;
		exit(EXIT_FAILURE);
	}
	glAttachShader(program, fShader);
}

float triangles[] = {
	-0.5f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.5f,0.0f,
	0.5f,0.0f,0.0f
};

unsigned int index[] = {
	0,1,2,
	1,2,3
};

int main(void)
{
	if (!glfwInit())
	{
		cout << "GLFW INIT FAIL" << endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "trangles", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW INIT FAIL" << endl;
		exit(EXIT_FAILURE);
	}


	GLuint VAO[numOfVao], VBO[numOfVbo];
	glGenVertexArrays(numOfVao, VAO);
	glGenBuffers(numOfVbo, VBO);
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint program1 = glCreateProgram();
	attachVertShader("vert.glsl", program1);
	attachFragShader("color1Frag.glsl", program1);
	glLinkProgram(program1);
	int error;
	glGetProgramiv(program1, GL_LINK_STATUS, &error);
	if (!error)
	{
		char msg[128];
		glGetProgramInfoLog(program1, 128, NULL, msg);
		cout << "LINK ERROR:" << msg << endl;
		exit(EXIT_FAILURE);
	}
	GLuint program2 = glCreateProgram();
	attachVertShader("vert.glsl", program2);
	attachFragShader("color2Frag.glsl", program2);
	glLinkProgram(program2);
	glGetProgramiv(program2, GL_LINK_STATUS, &error);
	if (!error)
	{
		char msg[128];
		glGetProgramInfoLog(program2, 128, NULL, msg);
		cout << "LINK ERROR:" << msg << endl;
		exit(EXIT_FAILURE);
	}
	while (glfwWindowShouldClose(window) == false) {
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program1);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, index);
		glUseProgram(program2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, index+3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}