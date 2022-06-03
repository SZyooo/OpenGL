#include "mesh.h"
#include <GL/glew.h>

void model::Mesh::Draw(GLuint program)
{
	std::string diffuse = "diffuse";
	std::string specular = "sepcular";
	int diffNum = 0, specNum = 0;
	glUseProgram(program);
	for (auto& tex : _textures)
	{
		glActiveTexture(GL_TEXTURE0 + diffNum + specNum);
		std::string name;
		if (tex.TextureType() == texture_type::DIFFUSE)
		{
			name = diffuse + std::to_string(diffNum++);
		}
		else {
			name = specular + std::to_string(specNum++);
		}
		GLint loc = glGetUniformLocation(program, name.c_str());
		glUniform1i(loc, diffNum + specNum - 1);
		glBindTexture(GL_TEXTURE_2D, tex.ID());
	}
	glActiveTexture(0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void model::Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,this->_vetices.size() * sizeof(Vetex), &_vetices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vetex), (void*)Vetex::GetPositionOffset());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vetex), (void*)Vetex::GetNormalOffset());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vetex), (void*)/*Vetex::GetTCoordOffset()*/24);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}
