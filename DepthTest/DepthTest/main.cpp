#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<SOIL2/stb_image.h>
#include<memory>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;
float cubeVertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

GLuint createShader(string v,string f)
{
    GLuint _id;
    _id = glCreateProgram();
    GLuint vShader, fShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream in(v.c_str(), std::ios::in);
    std::stringstream ss;
    ss << in.rdbuf();
    std::string v_code = ss.str();
    ss.str("");
    in.close();
    in.open(f.c_str(), std::ios::in);
    ss << in.rdbuf();
    in.close();
    std::string f_code = ss.str();
    const char* vc_code = v_code.c_str();
    const char* fc_code = f_code.c_str();
    glShaderSource(vShader, 1, &vc_code, 0);
    glShaderSource(fShader, 1, &fc_code, 0);
    glCompileShader(vShader);
    int success;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char msg[128];
        glGetShaderInfoLog(vShader, 128, 0, msg);
        std::string wrong = std::string("Compiling vertex shader failed :: ") + std::string(msg);
        throw wrong;
    }
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char msg[128];
        glGetShaderInfoLog(fShader, 128, 0, msg);
        std::string wrong = std::string("Compiling fragment shader failed :: ") + std::string(msg);
        throw wrong;
    }
    glAttachShader(_id, vShader);
    glAttachShader(_id, fShader);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char msg[128];
        glGetProgramInfoLog(_id, 128, 0, msg);
        std::string wrong = std::string("Linking program failed :: ") + std::string(msg);
        throw wrong;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return _id;
}

const int window_height = 600, window_width = 800;



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(window_width,window_height,"blending",NULL,NULL));
    if (!window)
    {
        cout << "create window failed" << endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window.get());
    if (glewInit() != GLEW_OK)
    {
        cout << "init glew failed" << endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint cube_vbo, cube_vao;
    glGenBuffers(1, &cube_vbo);
    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    GLuint plane_vbo, plane_vao;
    glGenVertexArrays(1, &plane_vao);
    glGenBuffers(1, &plane_vbo);
    glBindVertexArray(plane_vao);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLuint grass, glass;
    glGenTextures(1, &grass);
    glGenTextures(1, &glass);
    void loadTexture(GLuint*,const char*);
    loadTexture(&glass, "blending_transparent_window.png");
    loadTexture(&grass, "grass.png");

    string fshader("fshader.glsl");
    string vshader("vshader.glsl");
    GLuint program = createShader(vshader, fshader);
    glUseProgram(program);
    GLint loc = glGetUniformLocation(program, "_texture");
    glUniform1i(loc, 0);
    glUseProgram(0);
    glClearColor(0, 0, 0, 1);
    glm::vec3 camePos = glm::vec3(0, 0, 20);
    glm::mat4 cube1_model = glm::rotate(glm::mat4(1), glm::radians(35.f), glm::vec3(0, 1, 0));
    cube1_model = glm::translate(cube1_model, glm::vec3(2, 0, -4));
    glm::mat4 cube2_model = glm::rotate(glm::mat4(1), glm::radians(-45.f), glm::vec3(0, 1, 0));
    cube2_model = glm::translate(cube2_model, glm::vec3(-3, 0, -10));
    glm::mat4 plane_model = glm::scale(glm::mat4(1.0f), glm::vec3(4, 4, 4));
    //plane_model = glm::rotate(plane_model,glm::radians(0.f),glm::vec3(1,0,0));

    glm::mat4 glass1 = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1, 0, 0));
    glass1 = glm::rotate(glass1, glm::radians(30.f), glm::vec3(0, 0, 1));

    glm::mat4 glass2 = glm::rotate(glass1,glm::radians(-45.f),glm::vec3(0,1,0));
    glass2 = glm::scale(glass2,glm::vec3(2,2,2));
    glass2 = glm::translate(glass1,glm::vec3(0,0,-5));

    glm::mat4 glass3 = glm::rotate(glass1, glm::radians(-90.f), glm::vec3(0, 1, 0));
    glass3 = glm::scale(glass3,glm::vec3(2,2,2));
    glass3 = glm::translate(glass3,glm::vec3(-5,0,-5));

    glm::mat4 project = glm::perspective(45.f, 1.0f * window_width / window_height, 0.1f, 100.f);
    glUseProgram(program);
    loc = glGetUniformLocation(program, "project");
    glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(project));
    glUseProgram(0);

    while (glfwWindowShouldClose(window.get()) == false)
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        camePos = glm::rotate(glm::mat4(1.f), glm::radians(0.005f), glm::vec3(0, 1, 0)) * glm::vec4(camePos, 1.0);
        //cout << camePos.x << "," << camePos.y << "," << camePos.z << endl;
        glm::mat4 look = glm::lookAt(camePos + glm::vec3(0,5,0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glUseProgram(program);
        loc = glGetUniformLocation(program, "view");
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(look));
        glUseProgram(0);

        //cube
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(cube_vao);
        glBindTexture(GL_TEXTURE_2D, glass);
        loc = glGetUniformLocation(program, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(cube1_model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        loc = glGetUniformLocation(program, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(cube2_model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        //plane
        loc = glGetUniformLocation(program, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(plane_model));
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(plane_vao);
        glBindTexture(GL_TEXTURE_2D, glass);
        glDrawArrays(GL_TRIANGLES, 0, 6);



        //glass
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(glass1));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(glass2));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(glass3));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&cube_vao);
    glDeleteVertexArrays(1,&plane_vao);
    glDeleteBuffers(1, &cube_vbo);
    glDeleteBuffers(1, &plane_vbo);
}

void loadTexture(GLuint* textureID, const char* path) {
    int wid, hei, channel;
    unsigned char* img = stbi_load(path, &wid, &hei, &channel, 0);
    if (!img)
    {
        cout << "load image failed" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, *textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wid, hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(img);
    }
}
