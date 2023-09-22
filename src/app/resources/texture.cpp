#include "texture.h"

#include "image_loader.h"

namespace MiniG::Resources
{
Texture::Texture(const ImVec4& color, MGVec2<int> size)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const uint64_t texture_data_size = size.x * size.y * 4; /* RGBA */
	uint8_t* texture_data = (unsigned char*)malloc(texture_data_size);

	/* Fill texture */
	for(uint64_t i = 0; i < texture_data_size; i += 4)
	{
		texture_data[i + 0] = static_cast<uint8_t>(color.x * 255);
		texture_data[i + 1] = static_cast<uint8_t>(color.y * 255);
		texture_data[i + 2] = static_cast<uint8_t>(color.z * 255);
		texture_data[i + 3] = static_cast<uint8_t>(color.w * 255);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	free(texture_data);

	m_TextureSize = size;
	m_Succeeded = true;
}

Texture::Texture(const std::string& path)
{
	int texture_width = 0, texture_height = 0;
	
	uint8_t* texture_data = stbi_load(path.c_str(), &texture_width, &texture_height, nullptr, 4 /* RGBA */);
	if(!texture_data)
	{
		return;
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	stbi_image_free(texture_data);

	m_TextureSize = {texture_width, texture_height};
	m_Succeeded = true;
}

void Texture::Delete()
{
	glDeleteTextures(1, &m_TextureID);
	m_TextureID = 0;
	m_TextureSize = {0, 0};
	m_Succeeded = false;
}

GLuint Texture::GetID() const
{
	return m_TextureID;
}

MGVec2<int> Texture::GetTextureSize() const
{
	return m_TextureSize;
}

bool Texture::IsReady() const
{
	return m_Succeeded;
}
} /* namespace MiniG::Resources */