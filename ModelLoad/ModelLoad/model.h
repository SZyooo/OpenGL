#pragma once
#include "mesh.h"
#include <map>
#include "program.h"
#include "mesh.h"
namespace model {
	class Model {
	public:
		void Draw(const glm::mat4& view, const glm::mat4& pro);
		void Move(const glm::vec3& pos);
		void Rotate(const glm::vec3& rot);
		void Scale(const glm::vec3& s);
	private:
		friend void load_model(Model& model, std::string path, std::string vShader, std::string fShader);
		std::map<std::string , Texture> texturesHasLoaded;
		program::Program _program;
		std::vector<model::Mesh> _meshs;
		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;
	};
};