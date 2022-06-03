#include<gl/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<iostream>
#include<fstream>


using namespace std;
#define NumOfVAO 1
#define NumOfVBO 1

GLuint createProgram(const char* vert, const char* frag)
{
	ifstream in(vert, ios::in);
	string line, context;
	while (in.eof() == false)
	{
		getline(in, line);
		context.append(line + "\n");
	}
	in.close();
	const char* txt = context.c_str();
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &txt, 0);
	glCompileShader(vertShader);
	int error = 1;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &error);
	char msg[512];
	if (error == 0)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, msg);
		cout << "ERROR: compilation of vertex shader failed:" << msg << endl;
		exit(EXIT_FAILURE);
	}
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	in.open(frag, ios::in);
	context.clear();
	while (in.eof() == false)
	{
		getline(in, line);
		context.append(line + "\n");
	}
	in.close();
	txt = context.c_str();
	glShaderSource(fragShader, 1, &txt, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &error);
	if (error == 0)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, msg);
		cout << "ERROR:compile fragment shader failed:" << msg<<endl;
		exit(EXIT_FAILURE);
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glGetProgramiv(program, GL_LINK_STATUS, &error);
	if (error == 0)
	{
		glGetProgramInfoLog(program,512,NULL,msg);
		cout << "ERROR: link program failed:" << msg;
		exit(EXIT_FAILURE);
	}
	return program;
}



int main(void)
{
	if (!glfwInit())
	{
		cout << "glfw init fail" << endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800,600,"triangle",NULL,NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		cout << "glew init fail" << endl;
		exit(EXIT_FAILURE);
	}

	float points[] = {
		-0.5f,0,0,
		0.5f,0,0,
		0,0.5f,0
	};


	GLuint vao;
	GLuint vbo;

	
	glGenVertexArrays(NumOfVAO, &vao);
	glGenBuffers(NumOfVBO, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	GLuint program = createProgram("vert.glsl", "frag.glsl");
	while (glfwWindowShouldClose(window) == false)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
}


