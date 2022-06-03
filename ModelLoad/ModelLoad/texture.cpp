#include "texture.h"
#include <GL/glew.h>
#include <SOIL2/stb_image.h>

model::Texture::Texture(const std::string path, model::texture_type type)
	:_type(type),_path(path)
{
	glGenTextures(1, &this->id);
	int width, height, channelNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* img = stbi_load(path.c_str(), &width, &height, &channelNum, 0);
	if (img)
	{
		GLenum format;
		if (channelNum == 1)
		{
			format = GL_RED;
		}
		else if (channelNum == 3)
		{
			format = GL_RGB;
		}
		else {
			format = GL_RGBA;
		}
		glBindTexture(GL_TEXTURE_2D, this->ID());
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(img);
	}
	else {
		throw "Load Image Failed";
	}
	stbi_set_flip_vertically_on_load(false);
}
