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

float points[] = {
	0.5f,0.5,-0.1,
	-0.4,0.6,-0.2,
	-0.7,-0.5,-0.4
};


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


GLuint createShader(string v, string f)
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

void loadTexture(GLuint* textureID, const char* path) {
    int wid, hei, channel;
    unsigned char* img = stbi_load(path, &wid, &hei, &channel, 0);
    GLenum mode;
    if (channel == 1)
    {
        mode = GL_RED;
    }
    else if (channel == 3)
    {
        mode = GL_RGB;
    }
    else {
        mode = GL_RGBA;
    }
    if (!img)
    {
        cout << "load image failed" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, *textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, mode, wid, hei, 0, mode, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(img);
    }
}
void loadTexture2(GLuint* textureID, const char* path) {
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

struct Camera
{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;
    float fov;
    float move_speed;
} main_camera{ {0,0,0},{0,0,-1},{0,1,0}, glm::radians(60.f),0.1f };

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


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "framebuffer", NULL, NULL);
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

    GLuint points_vao, points_vbo;
    glGenBuffers(1, &points_vbo);
    glGenVertexArrays(1, &points_vao);
    glBindBuffer(GL_ARRAY_BUFFER,points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBindVertexArray(points_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    GLuint cube_vbo, cube_vao;
    glGenBuffers(1, &cube_vbo);
    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    glfwSetCursorPos(window, window_width / 2, window_height / 2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    GLuint points_program = createShader("p_v.glsl", "p_f.glsl");
    GLuint cube1_program = createShader("cube1_v.glsl", "cube1_f.glsl");
    GLuint cube2_program = createShader("cube2_v.glsl", "cube2_f.glsl");
    sendInt(cube2_program, 0, "inner");
    sendInt(cube2_program, 1, "outer");
    GLuint cube2_texs[2];
    glGenTextures(2, cube2_texs);
    loadTexture2(&cube2_texs[0], "box.jpg");
    loadTexture2(&cube2_texs[1], "pic.jpg");


    GLuint color1 = createShader("c_v.glsl", "c1_f.glsl");
    GLuint color2 = createShader("c_v.glsl", "c2_f.glsl");
    GLuint color3 = createShader("c_v.glsl", "c3_f.glsl");
    GLuint color4 = createShader("c_v.glsl", "c4_f.glsl");

    GLuint idx1 = glGetUniformBlockIndex(color1, "Matricies");
    GLuint idx2 = glGetUniformBlockIndex(color2, "Matricies");
    GLuint idx3 = glGetUniformBlockIndex(color3, "Matricies");
    GLuint idx4 = glGetUniformBlockIndex(color4, "Matricies");

    glUniformBlockBinding(color1, idx1, 0);
    glUniformBlockBinding(color2, idx2, 0);
    glUniformBlockBinding(color3, idx3, 0);
    glUniformBlockBinding(color4, idx4, 0);


    glm::mat4 view = glm::lookAt(main_camera.pos, main_camera.pos + main_camera.dir, main_camera.up);
    glm::mat4 proj = glm::perspective(main_camera.fov, 1.0f * window_width / window_height, 0.1f, 100.f);
    glm::mat4 m1 = glm::translate(glm::mat4(1.f), glm::vec3(-0.75f, 0.75f, 0.0f));
    glm::mat4 m2 = glm::translate(glm::mat4(1.f), glm::vec3(0.75f, 0.75f, 0.0f));
    glm::mat4 m3 = glm::translate(glm::mat4(1.f), glm::vec3(-0.75f, -0.75f, 0.0f));
    glm::mat4 m4 = glm::translate(glm::mat4(1.f), glm::vec3(0.75f, -0.75f, 0.0f));
    sendMatrix4(color1, m1, "model");
    sendMatrix4(color2, m2, "model");
    sendMatrix4(color3, m3, "model");
    sendMatrix4(color4, m4, "model");


    glm::mat4 VP[2] = {
        proj,view
    };


    GLuint ubf;
    glGenBuffers(1, &ubf);
    glBindBuffer(GL_UNIFORM_BUFFER, ubf);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(VP), VP, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);//!!!!!!!!!!!!!!
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubf);



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    while (glfwWindowShouldClose(window) == false)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(main_camera.pos, main_camera.pos + main_camera.dir, main_camera.up);
        proj = glm::perspective(main_camera.fov, 1.0f * window_width / window_height, 0.1f, 100.f);

        glBindBuffer(GL_UNIFORM_BUFFER, ubf);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        //sendMatrix4(points_program, view, "view");
        //sendMatrix4(points_program, proj, "project");

        //glBindVertexArray(points_vao);
        //glUseProgram(points_program);
        //glDrawArrays(GL_POINTS, 0, 3);
        //sendMatrix4(cube2_program, view, "view");
        //sendMatrix4(cube2_program, proj, "project");

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, cube2_texs[0]);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, cube2_texs[1]);
        glBindVertexArray(cube_vao);
        glUseProgram(color1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUseProgram(color2);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUseProgram(color3);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUseProgram(color4);
        glDrawArrays(GL_TRIANGLES, 0, 36);






        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}