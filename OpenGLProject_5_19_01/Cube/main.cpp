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
	/// 方块有6个面，每个面分为两个三角形，每个三角形三个顶点，因此6*2*3 = 36个顶点
	/// 每个顶点有x，y，z三个值，因此一共有36*3 = 108个值
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

		//建立顶点数组对象VAO
		glGenVertexArrays(1, vao);
		//绑定该顶点数组对象
		glBindVertexArray(vao[0]);

		//建立顶点缓冲对象。glDrawArray函数会逐个从VBO中取出点来构成指定数据类型（本例中是顶点类型vec3）
		//不过openGL是不知道VBO里面数据的具体含义的
		glGenBuffers(numVBOs, vbo);
		//绑定某一个VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//将数据复制进入到VBO中
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
		//清除深度缓冲区
		//这个函数会使用默认值1来填充深度缓冲区，如果不清理的话有可能什么都显示不出来
		glClear(GL_DEPTH_BUFFER_BIT);
		//清除颜色缓冲区,如果不清理的花就会将颜色遗留在缓冲区中
		glClear(GL_COLOR_BUFFER_BIT);
		//启用着色器，这会在GPU上安装glsl代码，但这并不会运行着色器程序
		glUseProgram(renderingProgram);
		//获取统一变量
		vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
		projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
		glfwGetFramebufferSize(window, &width, &height);
		aspect = (float)width / (float)height;
		pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);//1.0472 = 60°

		//translate函数是构建移动变换矩阵
		//过程为将第一个参数的mat的每一列乘以第二个vec3参数的每一个值，然后加到结果的最后一列
		//本例中单位矩阵与vec3translate之后的结果就是：
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
		//模型矩阵
		//作用是将模型的局部坐标转化为世界坐标
		//tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * tf) * 8.0f, cos(0.52f * tf) * 8.0f, sin(0.7f * tf) * .0f));
		//旋转矩阵
		//第一个参数指定被旋转的矩阵
		//第二个参数指定旋转角度 0-2PI
		//第三个参数指定旋转轴
		//rMat = glm::rotate(glm::mat4(1.0f), 3.f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
		//rMat = glm::rotate(rMat, 3.f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
		//rMat = glm::rotate(rMat, 3.f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
		//mMat = glm::translate(glm::mat4(1.0f),glm::vec3(cubeLocX,cubeLocY,cubeLocZ));
		//mMat = tMat * rMat;
		mMat = glm::translate(glm::mat4(1.0f),glm::vec3(cubeLocX,cubeLocY,cubeLocZ));
		GLuint tmMat = glGetUniformLocation(renderingProgram,"m_matrix");
		glUniformMatrix4fv(tmMat, 1, GL_FALSE, glm::value_ptr(mMat));
		//mvMat = vMat * mMat;

		//参数解析
		//第一个参数：指定的同一变量的
		//需要加载的数组元素的数量
		//第三个参数：列优先（GL_FALSE）还是行优先（GL_TRUE）
		//指向数组的指针
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		//绑定缓存区
		//第一个参数指定绑定到当前什么类型的缓冲区上
		//第二个参数指定用来绑定的缓冲区。（如果不存在则创建）
		//绑定之后以前的绑定将会被取消
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//第一个参数：指定设置的顶点位置（glsl中用layout 指定的location）
		//第二个参数：指定顶点的大小。因为本例中该顶点为一个vec3坐标，因此大小为3
		//第三个参数：指定数据的类型。glsl中vec都是float类型的
		//第四个参数：是否希望数据被标准化。GL_FALSE说明不需要
		//第五个参数：指定步长。正常来说应该是3*sizeof（float），但是在紧密排列的数据中，可以填0让gl来计算步长
		//最后一个参数的类型是void*，数据指针， 这个值受到VBO的影响；
		//1：在不使用VBO的情况下，就是一个指针，指向的是需要上传到顶点数据指针，
		//项目中通常在不使用VBO的情况下，绘制之前，执行glBindBuffer(GL_ARRAY_BUFFER, 0)，
		//否则会导致数组顶点无效，界面无法显示；
		//2：使用VBO的情况下，先要执行glBindBuffer(GL_ARRAY_BUFFER, 1)，
		//如果一个名称非零的缓冲对象被绑定至GL_ARRAY_BUFFER目标（见glBindBuffer）且此时一个定点属性数组被指定了，
		//那么pointer被当做该缓冲对象数据存储区的字节偏移量。并且，
		//缓冲对象绑定（GL_ARRAY_BUFFER_BINDING）会被存为索引为index的顶点属性数组客户端状态；
		//此时指针指向的就不是具体的数据了。因为数据已经缓存在缓冲区了。
		//这里的指针表示位置数据在缓冲中起始位置的偏移量(Offset)。
		//由于位置数据在数组的开头，所以这里是0。
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//启动“深度测试”
		glEnable(GL_DEPTH_TEST);
		//指定可以被绘制的像素点（要开启“深度测试”才有用）
		//GL_NEVER:永远不通过（输入的深度值不取代参考值）
		//GL_LESS:如果输入的深度值小于参考值，则取代
		//GL_EQUAL:如果输入的深度值等于参考值，取代
		//GL_LEQUAL:如果输入的深度值小于等于参考值，取代
		//GL_GREATER:如果输入的深度值大于参考值，取代
		//GL_NOTE_QUAL:如果输入的深度值不等于参考值，取代
		//GL_GEQUAL：如果输入的深度值大于等于参考值，取代
		//GL_ALWAYS：总是取代
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