#pragma once
#include <glm/glm.hpp>
#include <string>
namespace camera {
	typedef glm::vec3 v3;
	typedef glm::vec2 v2;
	
	class Camera
	{
	public:
		/// <summary>
		/// constructor
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="rotate">(p,y)</param>
		/// <param name="up"></param>
		Camera(std::string name,v3 pos = v3(0), v2 rotate = v2(0,0), v3 up = v3(0., 1., 0.), float closeDis = 0.1, float farDis = 100, float fov = 45., float ms = 1, float rs = 1);
		v3& Position() { return _position; }
		v3& Direction() { return _direction; }
		v3& Up() { return _up; }
		void RotateVeticlally(float delta);
		void RotateHorizontally(float delta);
		glm::mat4 LookAtMat();
		void Move(glm::vec3, double dis);
		std::string Name() const { return _name; }
		void Zoom(double delta);
		float& FOV() { return _fov; }
		float NearPlane() { return _closeDis; }
		float FarPlane() { return _farDis; }
		void Reset();
	private:
		v3 _position;
		v3 _direction;
		v3 _up;
		float _closeDis, _farDis;
		double _yaw, _pitch;
		void ReDirect();
		std::string _name;
		float _fov;
		float _move_speed, _rotate_speed;
	};
}


