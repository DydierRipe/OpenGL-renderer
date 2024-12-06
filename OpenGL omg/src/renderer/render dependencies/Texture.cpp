#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const string& path)
	: rendererID(0), path(path), localBuffer(nullptr), width(0), height(0), bPP(0)
{

	string def = "res/images/default.png";

	stbi_set_flip_vertically_on_load(1);
	if (path != "-1") localBuffer = stbi_load(path.c_str(), &width, &height, &bPP, 4);
	else localBuffer = stbi_load(def.c_str(), &width, &height, &bPP, 4);

	glGenTextures(1, &rendererID);
	glBindTexture(GL_TEXTURE_2D, rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer) stbi_image_free(localBuffer);
}

Texture::~Texture()
{
	if (path != "- 1") glDeleteTextures(1, &rendererID);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, rendererID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
