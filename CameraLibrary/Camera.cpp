#include "Camera.h"

Camera::Camera(std::string p_name,
	float p_moveSpeed,
	float p_rotateSpeed,
	float p_width, float p_height,
	float p_nPlaneDis = 0.1,
	float p_fPlaneDis = 100.0,
	float p_viewDegree = 60.f,
	glm::vec3 p_pos = glm::vec3(0, 0, 0),
	glm::vec3 p_dir = glm::vec3(0, 0, -1),
	glm::vec3 p_up = glm::vec3(0, 1, 0))
	:name(p_name), position(p_pos), direction(p_dir), up(p_up),
	width(p_width), height(p_height), nPlaneDis(p_nPlaneDis), fPlaneDis(p_fPlaneDis),
	viewDegree(p_viewDegree), moveSpeed(p_moveSpeed), rotateSpeed(p_rotateSpeed)
{
	view = glm::lookAt(position, position + direction, up);
	projection = glm::perspective(glm::radians(viewDegree), width / height, nPlaneDis, fPlaneDis);
	pitch = 0;
	yaw = 180;
}

void Camera::move(DIRECTION p_dir)
{
	direction = glm::normalize(direction);
	switch (p_dir)
	{
	case DIRECTION::FRONT:
		
	default:
		break;
	}
}

void Camera::rotate(float deltaPitch, float deltaYaw)
{
	pitch += deltaPitch;
	if (pitch > 89) pitch = 89;
	else if (pitch < -89) pitch = -89;
	yaw += deltaYaw;
	if (yaw > 360) yaw -= 360;
	else if (yaw < -360) yaw += 360;
	glm::vec3 newDir;
	newDir.x = cos(pitch) * sin(yaw);
	newDir.y = sin(pitch);
	newDir.z = cos(pitch) * cos(yaw);
	direction = newDir;
	view = glm::lookAt(position, position + direction, up);
}
