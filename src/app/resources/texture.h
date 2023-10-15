#pragma once
#include "app/common.h"

#include <GLFW/glfw3.h>

namespace MiniG::Resources
{
class Texture
{
	public:
		Texture() {}
		Texture(const std::string& path); /* load texture from file */
		Texture(const ImVec4& color, MGVec2<int> size, bool is_byte = false); /* create a one-color texture */

		void Delete();

		GLuint GetID() const;
		MGVec2<int> GetTextureSize() const;
		ImVec2 GetTextureSizeImGui() const;
		bool IsReady() const;

	private:
		bool m_Succeeded = false;
		GLuint m_TextureID = 0;
		MGVec2<int> m_TextureSize = {0, 0};
};
} /* namespace MiniG::Resources */