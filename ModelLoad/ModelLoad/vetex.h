#pragma once
#include <glm/glm.hpp>
#include <iostream>

namespace model {

	class Vetex {
	public:
		Vetex(glm::vec3 position, glm::vec3 normal, glm::vec2 tcoord);
		glm::vec3 Position() const { return _position; }
		glm::vec3 Normal() const { return _normal; }
		glm::vec2 TextureCoord() const { return _texture_coord; }
		static size_t GetPositionOffset() {
			size_t offset = (size_t)(&((Vetex*)0)->_position);
			std::cout << "position offset=" << offset << std::endl;
			return offset;
		}
		static size_t GetNormalOffset() {
			size_t offset = (size_t)(&((Vetex*)0)->_normal);
			std::cout << "norm offset=" << offset << std::endl;
			return offset;
		}
		static size_t GetTCoordOffset(){
			size_t offset = (size_t)(&((Vetex*)0)->_tcoord_offset);
			std::cout << "coord offset=" << offset << std::endl;
			return offset;
		}
	private:
		glm::vec3 _position;
		glm::vec3 _normal;
		glm::vec2 _texture_coord;
		size_t _pos_offset, _nor_offset, _tcoord_offset;
	};
}