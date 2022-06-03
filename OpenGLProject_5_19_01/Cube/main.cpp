#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<iostream>
#include<fstream>
#include<cmath>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

#define numVAOs 1
#define numVBOs 2

using namespace std;

namespace cube{

	float cameraX, cameraY, cameraZ;
	float cubeLocX, cubeLocY, cubeLocZ;
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];
	GLuint mvLoc, projLoc,vLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mMat, mvMat,tMat,rMat;

	/// <summary>
	/// ������6���棬ÿ�����Ϊ���������Σ�ÿ���������������㣬���6*2*3 = 36������
	/// ÿ��������x��y��z����ֵ�����һ����36*3 = 108��ֵ
	/// </summary>
	/// <param name=""></param>
	void setupVerticles(void) {
		float vertexPositions[108] = {
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
		float pyramidPositions[54] =
		{
			-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,0.0f,1.0f,0.0f,
			1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f,
			1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f,
			-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,0.0f,1.0f,0.0f,
			-1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,
			1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f
		};

		//���������������VAO
		glGenVertexArrays(1, vao);
		//�󶨸ö����������
		glBindVertexArray(vao[0]);

		//�������㻺�����glDrawArray�����������VBO��ȡ����������ָ���������ͣ��������Ƕ�������vec3��
		//����openGL�ǲ�֪��VBO�������ݵľ��庬���
		glGenBuffers(numVBOs, vbo);
		//��ĳһ��VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//�����ݸ��ƽ��뵽VBO��
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
	}

	std::string readShaderSource(const char* filePath)
	{
		std::string content;
		std::ifstream fileStream(filePath, std::ios::in);
		std::string line = "";
		while (!fileStream.eof())
		{
			getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();
		return content;
	}

	void printShaderLog(GLuint shader)
	{
		int len = 0;
		int chWritten = 0;
		char* log;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetShaderInfoLog(shader, len, &chWritten, log);
			std::cout << "Shader info Log:" << log << std::endl;
			free(log);
		}
	}
	void printProgramLog(int prog)
	{
		int len = 0;
		int chWritten = 0;
		char* log;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetProgramInfoLog(prog, len, &chWritten, log);
			std::cout << "Program Info Log:" << log << std::endl;
			free(log);
		}
	}
	bool checkOpenGLError()
	{
		bool foundError = false;
		int glErr = glGetError();
		while (glErr != GL_NO_ERROR)
		{
			std::cout << "glError:" << glErr << std::endl;
			foundError = true;
			glErr = glGetError();
		}
		return foundError;
	}
	GLuint createShaderProgram(const char* vertfile,const char* fragfile) {
		GLint vertCompiled;
		GLint fragCompiled;
		GLint linked;
		bool checkOpenGLError();
		void printShaderLog(GLuint);
		void printProgramLog(int);
		std::string readShaderSource(const char* filePath);
		std::string vertShaderStr = readShaderSource(vertfile);
		std::string fragShaderStr = readShaderSource(fragfile);
		const char* vshaderSource = vertShaderStr.c_str();

		const char* fshaderSource = fragShaderStr.c_str();
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vShader, 1, &vshaderSource, NULL);
		glShaderSource(fShader, 1, &fshaderSource, NULL);
		glCompileShader(vShader);
		checkOpenGLError();
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1)
		{
			std::cout << "fragment compilation failed" << std::endl;
			printShaderLog(vShader);
		}
		glCompileShader(fShader);
		checkOpenGLError();
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != 1)
		{
			std::cout << "fragment compilation failed" << std::endl;
			printShaderLog(fShader);
		}
		GLuint vfProgram = glCreateProgram();
		glAttachShader(vfProgram, vShader);
		glAttachShader(vfProgram, fShader);
		glLinkProgram(vfProgram);
		checkOpenGLError();
		glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
		if (linked != 1)
		{
			std::cout << "linked failed" << std::endl;
			printProgramLog(vfProgram);
		}
		return vfProgram;
	}
	void init(GLFWwindow* window)
	{
		renderingProgram = createShaderProgram("VertexShader.glsl","fragShader.glsl");
		cameraX = 0.0f;
		cameraY = 0.0f;
		cameraZ = 420.0f;
		cubeLocX = 0.0f;
		cubeLocY = -2.0f;
		cubeLocZ = 0.0f;
		setupVerticles();
	}
	void display(GLFWwindow* window, double currentTime)
	{
		//�����Ȼ�����
		//���������ʹ��Ĭ��ֵ1�������Ȼ����������������Ļ��п���ʲô����ʾ������
		glClear(GL_DEPTH_BUFFER_BIT);
		//�����ɫ������,���������Ļ��ͻὫ��ɫ�����ڻ�������
		glClear(GL_COLOR_BUFFER_BIT);
		//������ɫ���������GPU�ϰ�װglsl���룬���Ⲣ����������ɫ������
		glUseProgram(renderingProgram);
		//��ȡͳһ����
		vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
		projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
		glfwGetFramebufferSize(window, &width, &height);
		aspect = (float)width / (float)height;
		pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);//1.0472 = 60��

		//translate�����ǹ����ƶ��任����
		//����Ϊ����һ��������mat��ÿһ�г��Եڶ���vec3������ÿһ��ֵ��Ȼ��ӵ���������һ��
		//�����е�λ������vec3translate֮��Ľ�����ǣ�
		//1 0 0 -camera.x
		//0 1 0 -camera.y
		//0 0 1 -camera.z
		//0 0 0     1
		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
		//float timeFactor = ((float)currentTime);
		//float tfLoc = glGetUniformLocation(renderingProgram, "tf");
		//glUniform1f(tfLoc,timeFactor);
		//float tf = currentTime + i;
		//ģ�;���
		//�����ǽ�ģ�͵ľֲ�����ת��Ϊ��������
		//tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * tf) * 8.0f, cos(0.52f * tf) * 8.0f, sin(0.7f * tf) * .0f));
		//��ת����
		//��һ������ָ������ת�ľ���
		//�ڶ�������ָ����ת�Ƕ� 0-2PI
		//����������ָ����ת��
		//rMat = glm::rotate(glm::mat4(1.0f), 3.f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
		//rMat = glm::rotate(rMat, 3.f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
		//rMat = glm::rotate(rMat, 3.f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
		//mMat = glm::translate(glm::mat4(1.0f),glm::vec3(cubeLocX,cubeLocY,cubeLocZ));
		//mMat = tMat * rMat;
		mMat = glm::translate(glm::mat4(1.0f),glm::vec3(cubeLocX,cubeLocY,cubeLocZ));
		GLuint tmMat = glGetUniformLocation(renderingProgram,"m_matrix");
		glUniformMatrix4fv(tmMat, 1, GL_FALSE, glm::value_ptr(mMat));
		//mvMat = vMat * mMat;

		//��������
		//��һ��������ָ����ͬһ������
		//��Ҫ���ص�����Ԫ�ص�����
		//�����������������ȣ�GL_FALSE�����������ȣ�GL_TRUE��
		//ָ�������ָ��
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		//�󶨻�����
		//��һ������ָ���󶨵���ǰʲô���͵Ļ�������
		//�ڶ�������ָ�������󶨵Ļ�������������������򴴽���
		//��֮����ǰ�İ󶨽��ᱻȡ��
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//��һ��������ָ�����õĶ���λ�ã�glsl����layout ָ����location��
		//�ڶ���������ָ������Ĵ�С����Ϊ�����иö���Ϊһ��vec3���꣬��˴�СΪ3
		//������������ָ�����ݵ����͡�glsl��vec����float���͵�
		//���ĸ��������Ƿ�ϣ�����ݱ���׼����GL_FALSE˵������Ҫ
		//�����������ָ��������������˵Ӧ����3*sizeof��float���������ڽ������е������У�������0��gl�����㲽��
		//���һ��������������void*������ָ�룬 ���ֵ�ܵ�VBO��Ӱ�죻
		//1���ڲ�ʹ��VBO������£�����һ��ָ�룬ָ�������Ҫ�ϴ�����������ָ�룬
		//��Ŀ��ͨ���ڲ�ʹ��VBO������£�����֮ǰ��ִ��glBindBuffer(GL_ARRAY_BUFFER, 0)��
		//����ᵼ�����鶥����Ч�������޷���ʾ��
		//2��ʹ��VBO������£���Ҫִ��glBindBuffer(GL_ARRAY_BUFFER, 1)��
		//���һ�����Ʒ���Ļ�����󱻰���GL_ARRAY_BUFFERĿ�꣨��glBindBuffer���Ҵ�ʱһ�������������鱻ָ���ˣ�
		//��ôpointer�������û���������ݴ洢�����ֽ�ƫ���������ң�
		//�������󶨣�GL_ARRAY_BUFFER_BINDING���ᱻ��Ϊ����Ϊindex�Ķ�����������ͻ���״̬��
		//��ʱָ��ָ��ľͲ��Ǿ���������ˡ���Ϊ�����Ѿ������ڻ������ˡ�
		//�����ָ���ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)��
		//����λ������������Ŀ�ͷ������������0��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//��������Ȳ��ԡ�
		glEnable(GL_DEPTH_TEST);
		//ָ�����Ա����Ƶ����ص㣨Ҫ��������Ȳ��ԡ������ã�
		//GL_NEVER:��Զ��ͨ������������ֵ��ȡ���ο�ֵ��
		//GL_LESS:�����������ֵС�ڲο�ֵ����ȡ��
		//GL_EQUAL:�����������ֵ���ڲο�ֵ��ȡ��
		//GL_LEQUAL:�����������ֵС�ڵ��ڲο�ֵ��ȡ��
		//GL_GREATER:�����������ֵ���ڲο�ֵ��ȡ��
		//GL_NOTE_QUAL:�����������ֵ�����ڲο�ֵ��ȡ��
		//GL_GEQUAL�������������ֵ���ڵ��ڲο�ֵ��ȡ��
		//GL_ALWAYS������ȡ��
		glDepthFunc(GL_LEQUAL);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
	}


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
	cube::init(window);
	while (!glfwWindowShouldClose(window))
	{
		cube::display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}