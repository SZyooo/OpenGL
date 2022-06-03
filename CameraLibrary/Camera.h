#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class DIRECTION {
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	FRONT_AND_LEFT,
	FRONT_AND_RIGHT,
	BACK_AND_LEFT,
	BACK_AND_RIGHT
};

class Camera {
private:
	std::string name;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	glm::mat4 view;
	glm::mat4 projection;
	float width, height;
	float nPlaneDis, fPlaneDis;
	float viewDegree;
	float moveSpeed;
	float rotateSpeed;
	float pitch, yaw;
public:
	Camera(std::string p_name,
		float p_moveSpeed,
		float p_rotateSpeed,
		float p_width, float p_height,
		float p_nPlaneDis = 0.1,
		float p_fPlaneDis = 100.0,
		float p_viewDegree = 60.f,
		glm::vec3 p_pos = glm::vec3(0, 0, 0),
		glm::vec3 p_dir = glm::vec3(0, 0, -1),
		glm::vec3 p_up = glm::vec3(0, 1, 0));
	void move(DIRECTION p_dir);
	void rotate(float deltaPitch,float deltaYaw);
};



#endif 