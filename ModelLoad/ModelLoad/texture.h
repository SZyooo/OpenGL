#pragma once
#include <string>
#include "textureTypes.h"
namespace model {
	using tID = unsigned int;
	class Texture {
	public:
		const tID ID() const { return id; }
		const texture_type TextureType() const { return _type; }
		Texture(const std::string path, texture_type type);
	private:
		tID id;
		texture_type _type;
		std::string _path;
	};
};
