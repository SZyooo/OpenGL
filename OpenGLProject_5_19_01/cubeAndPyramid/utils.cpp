#include "utils.h"


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
/// <summary>
/// 生成着色程序
/// </summary>
/// <param name="vertfile">点着色器</param>
/// <param name="fragfile">片段着色器</param>
/// <returns></returns>
GLuint createShaderProgram(const char* vertfile, const char* fragfile) {
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
GLuint loadTexture(const char* imagePath)
{
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(imagePath,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
	if (textureID == 0)
	{
		std::cout << "could not find texture file" << imagePath << std::endl;
	}
	return textureID;
}