#pragma once

#include <iostream>
#include <string>
#include <map>

#include <glad/glad.h>
#include <glm/vec2.hpp>


namespace Renderer
{
	class Texture
	{
	public:
		struct SubTexture
		{
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			SubTexture(const glm::vec2& _leftBottomUV, const glm::vec2& _rightBottomUV)
				: leftBottomUV(_leftBottomUV), rightTopUV(_rightBottomUV)
			{}

			SubTexture()
				: leftBottomUV(0.f), rightTopUV(1.f)
			{}
		};

		Texture(const GLuint width, GLuint height, const unsigned char* data,
			const unsigned int channels = 4, const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		Texture(Texture&& texture);
		~Texture();

		Texture() = delete;
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&& texture);

		void bind() const;
		unsigned int width() const { return m_width; };
		unsigned int height() const { return m_height; };

		void addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightBottomUV);
		const SubTexture& getSubTexture(const std::string& name) const;

	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width, m_height;
		
		std::map<std::string, SubTexture> m_subTextures;
	};
}