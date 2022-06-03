#include "model.h"
#include <glm/gtc/matrix_transform.hpp>


void model::Model::Draw(const glm::mat4& view,const glm::mat4& pro)
{
	
	_program.SetMat4("model",glm::mat4(1.));
	_program.SetMat4("view", view);
	_program.SetMat4("projection", pro);
	for (auto& m : this->_meshs)
	{
		m.Draw(_program.ID());
	}
}

void model::Model::Move(const glm::vec3& pos)
{
	this->_position += pos;
}

void model::Model::Rotate(const glm::vec3& rot)
{
	this->_rotation += rot;
}

void model::Model::Scale(const glm::vec3& s)
{
	this->_scale += s;
}
