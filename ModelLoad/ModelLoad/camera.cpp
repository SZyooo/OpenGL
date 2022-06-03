#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

camera::Camera::Camera(std::string name,v3 pos, v2 rot, v3 up,float cd,float fd,float fov,float ms, float rs)
	:_name(name),_position(pos), _up(glm::normalize(up)), _closeDis(cd), _farDis(fd),_fov(fov),_move_speed(ms),_rotate_speed(rs)
{
	this->_direction = v3(0, 0, -1);
	this->_yaw = 180;
	this->_pitch = 0;
	RotateVeticlally(rot.x);
	RotateHorizontally(rot.y);
}

void camera::Camera::RotateVeticlally(float delta)
{
	this->_pitch -= delta;
	if (_pitch > 89)
		_pitch = 89;
	else if (_pitch < -89)
		_pitch = -89;
	ReDirect();
}

void camera::Camera::RotateHorizontally(float delta)
{
	this->_yaw += delta;
	if (_yaw < -360)
		_yaw += 360;
	else if (_yaw > 360)
		_yaw -= 360;
	ReDirect();
}

glm::mat4 camera::Camera::LookAtMat()
{
	return glm::lookAt(_position, _position + _direction, _up);
}


glm::vec3 operator*(glm::vec3& v, double val)
{
	glm::vec3 res;
	res.x = v.x * val;
	res.y = v.y * val;
	res.z = v.z * val;
	return res;
}

void camera::Camera::Move(glm::vec3 dir, double dis)
{
	glm::vec3 norm = glm::normalize(dir);
	this->_position += norm * dis;
}

void camera::Camera::Zoom(double delta)
{
	std::cout << "scroll delta" << delta << std::endl;
	_fov -= delta;
	std::cout << "fov now = " << _fov << std::endl;
	if (_fov < 1)
		_fov = 1;
	else if (_fov > 60)
	{
		_fov = 60;
	}
}

void camera::Camera::Reset()
{
	this->_position = v3(0);
	this->_direction = v3(0, 0, -1);
	this->_up = v3(0, 1, 0);
	this->_yaw = 180;
	this->_pitch = 0;
	ReDirect();
	this->_fov = 45;
}

void camera::Camera::ReDirect()
{
	_direction.x = cos(glm::radians(_pitch)) * sin(glm::radians(_yaw));
	_direction.y = sin(glm::radians(_pitch));
	_direction.z = cos(glm::radians(_pitch)) * cos(glm::radians(_yaw));
}
