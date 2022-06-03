#pragma once

#include "vetex.h"
#include <vector>
#include "texture.h"
#include <GL/glew.h>
namespace model {
	using Vetices = std::vector<model::Vetex>;
	using Indices = std::vector<unsigned int>;
	using Textures = std::vector<Texture>;
	class Mesh {
	public:
		void Draw(GLuint program);
		Mesh(Vetices v, Indices i, Textures t)
			:_vetices(v), _indices(i), _textures(t),VAO(0),VBO(0),EBO(0)
		{
			SetupMesh();
		}
		void SetupMesh();
	private:
		Vetices _vetices;
		Indices _indices;
		Textures _textures;
		unsigned int VAO, VBO, EBO;
	};
};