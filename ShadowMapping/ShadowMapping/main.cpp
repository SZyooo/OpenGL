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

float plane[] = {
    -1,-1,0,    0,0,-1,  0,0,
    1,-1,0,     0,0,-1,  1,0,
    1,1,0,      0,0,-1,  1,1,
    -1,-1,0,    0,0,-1,  0,0,
    1,1,0,      0,0,-1,  1,1,
    -1,1,0,     0,0,-1,  0,1
};
//unsigned int indices[] = {
//    0,1,3,
//    1,2,3
//};

float cube[] = {
    // back face
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
    // front face
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
    // left face
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
    // right face
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
    // bottom face
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
    // top face
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
     1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
};

struct Camera
{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;
    float fov;
    float move_speed;
} main_camera{ {0,0,0},{0,0,-1},{0,1,0}, glm::radians(60.f),0.1f };

const int window_height = 600, window_width = 800;

double(*angle2radian)(double) = glm::radians;
void move_view(GLFWwindow* window, double x, double y)
{
    static int last_x = window_width / x, last_y = window_height / 2;
    static double yaw = 180, pitch = 0;
    yaw -= (x - last_x);
    if (yaw < 0)
        yaw += 360;
    else if (yaw > 360)
        yaw -= 360;
    pitch += (last_y - y);
    if (pitch < -89)
        pitch = -89;
    else if (pitch > 89)
        pitch = 89;
    last_x = x;
    last_y = y;
    glm::vec3& new_dir = main_camera.dir;
    new_dir.x = cos(angle2radian(pitch)) * sin(angle2radian(yaw));
    new_dir.y = sin(angle2radian(pitch));
    new_dir.z = cos(angle2radian(pitch)) * cos(angle2radian(yaw));
}
void read_key(GLFWwindow* window, int key, int, int action, int)
{
    if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        main_camera.pos += glm::normalize(main_camera.dir) * main_camera.move_speed;
    }
    else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        main_camera.pos -= glm::normalize(main_camera.dir) * main_camera.move_speed;
    }
    else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        glm::vec3 right = glm::cross(main_camera.dir, main_camera.up);
        main_camera.pos += glm::normalize(right) * main_camera.move_speed;
    }
    else if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        glm::vec3 left = glm::cross(main_camera.up, main_camera.dir);
        main_camera.pos += glm::normalize(left) * main_camera.move_speed;
    }
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

}
void resize(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

void sendMatrix4(GLuint program, glm::mat4& mat, const char* name)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUseProgram(program);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
void sendInt(GLuint program, int val, const char* name)
{
    GLint loc = glGetUniformLocation(program, name);
    glUseProgram(program);
    glUniform1i(loc, val);
}
void sendFloat(GLuint program, float val, const char* name)
{
    GLint loc = glGetUniformLocation(program, name);
    glUseProgram(program);
    glUniform1f(loc, val);
}
void sendVec3(GLuint program, glm::vec3& v, const char* name)
{
    GLint loc = glGetUniformLocation(program, name);
    glUseProgram(program);
    glUniform3f(loc, v.x, v.y, v.z);
}



struct point_light
{
    glm::vec3 pos;
    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shinness;
}_point_light{ {0,1,0},1.0,0.7,1.8,{0.2f, 0.2f, 0.2f},{0.5f, 0.5f, 0.5f},{1.0f, 1.0f, 1.0f},1 };

void sendPointLight(point_light& pl, const char* name, GLuint program)
{
    string str_name(name);
    sendVec3(program, pl.pos, string(str_name + ".pos").c_str());
    sendFloat(program, pl.constant, string(str_name + ".constant").c_str());
    sendFloat(program, pl.linear, string(str_name + ".linear").c_str());
    sendFloat(program, pl.quadratic, string(str_name + ".quadratic").c_str());
    sendVec3(program, pl.ambient, string(str_name + ".ambient").c_str());
    sendVec3(program, pl.diffuse, string(str_name + ".diffuse").c_str());
    sendVec3(program, pl.specular, string(str_name + ".specular").c_str());
    sendFloat(program, pl.shinness, string(str_name + ".shinness").c_str());

}


struct direct_light
{
    glm::vec3 direction;
    float ambent;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shinness;
} dLight{ {1,-1,0},0.1f,{1,1,1},{1,1,1},32.f };


void sendDirectLight(direct_light& dl, const char* name, GLuint program)
{
    sendVec3(program, dl.direction, string(string(name) + ".direction").c_str());
    sendFloat(program, dl.ambent, string(string(name) + ".ambent").c_str());
    sendVec3(program, dl.diffuse, string(string(name) + ".diffuse").c_str());
    sendVec3(program, dl.specular, string(string(name) + ".specular").c_str());
    sendFloat(program, dl.shinness, string(string(name) + ".shinness").c_str());
}




void loadTexture(GLuint* textureID, const char* path) {
    int wid, hei, channel;
    unsigned char* img = stbi_load(path, &wid, &hei, &channel, 4);
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

GLuint createShader(string v, string f, string g = "")
{
    GLuint _id;
    _id = glCreateProgram();
    GLuint vShader, fShader, gShader = 0;
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

    if (g.length() > 0)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        in.open(g.c_str(), ios::in);
        ss.str("");
        ss << in.rdbuf();
        in.close();
        string g_code = ss.str();
        const char* gc_code = g_code.c_str();
        glShaderSource(gShader, 1, &gc_code, 0);
        glCompileShader(gShader);
        glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char msg[128];
            glGetShaderInfoLog(gShader, 128, 0, msg);
            std::string wrong = std::string("Compiling geometry shader failed :: ") + std::string(msg);
            throw wrong;
        }
        glAttachShader(_id, gShader);
    }


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
    if (g.length() > 0)
    {
        glDeleteShader(gShader);
    }
    return _id;
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "advanceLighting", NULL, NULL);
    if (!window)
    {
        cout << "failed creating window" << endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, move_view);
    glfwSetKeyCallback(window, read_key);
    glfwSetFramebufferSizeCallback(window, resize);
    if (glewInit() != GLEW_OK)
    {
        cout << "glewinit failed" << endl;
        exit(EXIT_FAILURE);
    }
    GLuint plane_vao, plane_vbo;
    glGenBuffers(1, &plane_vbo);
    glGenVertexArrays(1, &plane_vao);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
    glBindVertexArray(plane_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    GLuint cube_vao, cube_vbo;
    glGenBuffers(1, &cube_vbo);
    glGenVertexArrays(1, &cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glBindVertexArray(cube_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glm::mat4 project = glm::perspective(glm::radians(60.f), 1.f * window_width / window_height, 0.1f, 100.f);


    GLuint program = createShader("cv.glsl", "f.glsl");
    GLuint texture;
    glGenTextures(1, &texture);
    loadTexture(&texture, "floor.jpg");
    sendMatrix4(program, project, "project");
    glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1, 0, 0));
    model = glm::scale(model, glm::vec3(50));
    sendMatrix4(program, model, "model");
    sendInt(program, 0, "tex");
    sendInt(program, 1, "depthMap");

    GLuint cube_program = createShader("cv.glsl", "f.glsl");
    GLuint cube_texture;
    glGenTextures(1, &cube_texture);
    loadTexture(&cube_texture, "box.jpg");
    sendMatrix4(cube_program, project, "project");
    sendInt(cube_program, 0, "tex");
    sendInt(cube_program, 1, "depthMap");

    GLuint test_texture;
    glGenTextures(1, &test_texture);
    loadTexture(&test_texture, "test.jpg");


    sendDirectLight(dLight,"dl",cube_program);
    sendDirectLight(dLight, "dl", program);

    glm::mat4 model0 = glm::mat4(1.f);
    glm::mat4 model1 = glm::translate(model0,glm::vec3(10,5,0));
    model1 = glm::rotate(model1,glm::radians(60.f),glm::vec3(1,1,0));
    glm::mat4 model2 = glm::translate(glm::mat4(1.f), glm::vec3(-10, 7, 0));
    model2 = glm::rotate(model2, glm::radians(45.f), glm::vec3(1, 3, 1));
    glm::mat4 model3 = glm::translate(glm::mat4(1.f), glm::vec3(-10, 6, -16));
    model3 = glm::rotate(model3, glm::radians(90.f), glm::vec3(2, 1, 1));



    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    enum {SHADOW_WIDTH = 5000, SHADOW_HEIGHT = 5000};
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float boarder_color[] = { 1,1,1,1 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, boarder_color);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  depthMap, 0);
    glDrawBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "framebuffer is not complete" << endl;
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glm::mat4 shadow_ortho = glm::ortho<float>(-100,100, -100, 100, 0.1f, 1000.f);
    glm::mat4 shadow_view = glm::lookAt(glm::vec3(-10, 10, 0), glm::vec3(0), glm::vec3(0, 1, 0));
    glm::mat4 to_shadow_space = shadow_ortho*shadow_view;


    GLuint shadow_program = createShader("shadow_v.glsl", "shadow_f.glsl");
    sendMatrix4(shadow_program, to_shadow_space, "to_shadow_space");
    sendMatrix4(program, to_shadow_space, "to_shadow_space");
    sendMatrix4(cube_program, to_shadow_space, "to_shadow_space");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    while (glfwWindowShouldClose(window) == false)
    {

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glCullFace(GL_FRONT);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(plane_vao);
        sendMatrix4(shadow_program, model, "model");
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(cube_vao);
        glUseProgram(shadow_program);
        sendMatrix4(shadow_program, model0, "model");
        glDrawArrays(GL_TRIANGLES, 0, 36);
        sendMatrix4(shadow_program, model1, "model");
        glDrawArrays(GL_TRIANGLES, 0, 36);
        sendMatrix4(shadow_program, model2, "model");
        glDrawArrays(GL_TRIANGLES, 0, 36);
        sendMatrix4(shadow_program, model3, "model");
        glDrawArrays(GL_TRIANGLES, 0, 36);



        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window_width, window_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(main_camera.pos, main_camera.pos + main_camera.dir, main_camera.up);
        sendMatrix4(program, view, "view");
        sendVec3(program, main_camera.pos, "eye");
        sendMatrix4(program, model, "model");
        glBindVertexArray(plane_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glCullFace(GL_BACK);
        sendMatrix4(cube_program, view, "view");
        sendVec3(program, main_camera.pos, "eye");
        glBindVertexArray(cube_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        sendMatrix4(cube_program, model0, "model");
        glUseProgram(cube_program);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        sendMatrix4(cube_program, model1, "model");
        glDrawArrays(GL_TRIANGLES, 0, 36);
        sendMatrix4(cube_program, model2, "model");
        glDrawArrays(GL_TRIANGLES, 0, 36);
        sendMatrix4(cube_program, model3, "model");
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


}
