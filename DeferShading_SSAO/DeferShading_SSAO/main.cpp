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
#include<vector>
using namespace std;



struct Camera
{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;
    float fov;
    float move_speed;
} main_camera{ {0,0,0},{0,0,-1},{0,1,0}, glm::radians(60.f),0.01f };

int window_height = 600, window_width = 800;
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
    window_height = h;
    window_width = w;
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

float outter_cube[] = {
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

float pure_cube[] = {
    // back face
    -1.0f, -1.0f, -1.0f,  // bottom-left
     1.0f,  1.0f, -1.0f,   // top-right
     1.0f, -1.0f, -1.0f,   // bottom-right         
     1.0f,  1.0f, -1.0f,   // top-right
    -1.0f, -1.0f, -1.0f,   // bottom-left
    -1.0f,  1.0f, -1.0f,   // top-left
    // front face
    -1.0f, -1.0f,  1.0f,   // bottom-left
     1.0f, -1.0f,  1.0f,   // bottom-right
     1.0f,  1.0f,  1.0f,   // top-right
     1.0f,  1.0f,  1.0f,   // top-right
    -1.0f,  1.0f,  1.0f,   // top-left
    -1.0f, -1.0f,  1.0f,   // bottom-left
    // left face
    -1.0f,  1.0f,  1.0f,  // top-right
    -1.0f,  1.0f, -1.0f,  // top-left
    -1.0f, -1.0f, -1.0f,  // bottom-left
    -1.0f, -1.0f, -1.0f,  // bottom-left
    -1.0f, -1.0f,  1.0f,  // bottom-right
    -1.0f,  1.0f,  1.0f,  // top-right
    // right face
     1.0f,  1.0f,  1.0f,   // top-left
     1.0f, -1.0f, -1.0f,   // bottom-right
     1.0f,  1.0f, -1.0f,   // top-right         
     1.0f, -1.0f, -1.0f,   // bottom-right
     1.0f,  1.0f,  1.0f,   // top-left
     1.0f, -1.0f,  1.0f,   // bottom-left     
    // bottom face
    -1.0f, -1.0f, -1.0f,   // top-right
     1.0f, -1.0f, -1.0f,   // top-left
     1.0f, -1.0f,  1.0f,   // bottom-left
     1.0f, -1.0f,  1.0f,   // bottom-left
    -1.0f, -1.0f,  1.0f,   // bottom-right
    -1.0f, -1.0f, -1.0f,   // top-right
    // top face
    -1.0f,  1.0f, -1.0f,   // top-left
     1.0f,  1.0f , 1.0f,  // bottom-right
     1.0f,  1.0f, -1.0f,   // top-right     
     1.0f,  1.0f,  1.0f,   // bottom-right
    -1.0f,  1.0f, -1.0f,   // top-left
    -1.0f,  1.0f,  1.0f    // bottom-left        
};

const float plane[] = {
    -1.f,-1.f,  0,0,
    1.f,-1.f,   1,0,
    1.f,1.f,    1,1,
    -1.f,1.f,   0,1
};
unsigned int indices[] = {
    0,1,2,
    0,2,3
};


struct point_light
{
    glm::vec3 pos;
    float constant;
    float linear;
    float quadratic;

    float ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shinness;
};


void sendPointLight(point_light& pl, const char* name, GLuint program)
{
    string str_name(name);
    sendVec3(program, pl.pos, string(str_name + ".pos").c_str());
    sendFloat(program, pl.constant, string(str_name + ".constant").c_str());
    sendFloat(program, pl.linear, string(str_name + ".linear").c_str());
    sendFloat(program, pl.quadratic, string(str_name + ".quadratic").c_str());
    sendFloat(program, pl.ambient, string(str_name + ".ambient").c_str());
    sendVec3(program, pl.diffuse, string(str_name + ".diffuse").c_str());
    sendVec3(program, pl.specular, string(str_name + ".specular").c_str());
    sendFloat(program, pl.shinness, string(str_name + ".shinness").c_str());

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

GLuint init_cube(float cube[], size_t arr_size, bool has_norm = false, int norm_idx = 0, bool has_texCoord = false, int tCoord_idx = 0)
{
    GLuint cube_vao, cube_vbo;
    glGenBuffers(1, &cube_vbo);
    glGenVertexArrays(1, &cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, arr_size, cube, GL_STATIC_DRAW);
    glBindVertexArray(cube_vao);
    int size = 3 + ((has_norm) ? 3 : 0) + ((has_texCoord) ? 2 : 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    int id = 1;
    if (has_norm)
    {
        glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(norm_idx * 3 * sizeof(float)));
        glEnableVertexAttribArray(id);
        id++;
    }
    if (has_texCoord)
    {
        glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(tCoord_idx * 3 * sizeof(float)));
        glEnableVertexAttribArray(id);
    }
    glBindVertexArray(0);
    return cube_vao;
}




int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "HDR-Bloom", NULL, NULL);
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

    const int light_amount = 100;
    point_light pl[light_amount];
    for (int i = 0; i < light_amount; i++)
    {

    }


    glm::mat4 proj = glm::perspective(main_camera.fov, 1.f * window_width / window_height, 0.1f, 100.f);
    GLuint outter_cube_vao = init_cube(outter_cube, sizeof(outter_cube), true, 1, true, 2);
    
    GLuint cube_program = createShader("cv.glsl", "cf.glsl");


    GLuint light_program = createShader("lv.glsl", "lf.glsl");
    
    
    GLuint quad_program = createShader("qv.glsl", "qf.glsl");

}