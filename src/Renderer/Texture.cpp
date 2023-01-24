#include "Texture.h"

namespace RenderEngine
{
	Texture::Texture(const GLuint width, GLuint height, const unsigned char* data,
		const unsigned int channels, const GLenum filter, const GLenum wrapMode)
		: m_width(width), m_height(height)
	{

		switch (channels)
		{
		case 3:
			m_mode = GL_RGB;
			break;
		case 4:
			m_mode = GL_RGBA;
			break;
		default:
			m_mode = GL_RGBA;
			break;
		}
		glGenTextures(1, &m_ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID); // make active
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, wrapMode);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0); // make inactive
		
	}
	Texture::Texture(Texture&& texture)
	{
		m_ID = texture.m_ID;
		texture.m_ID = 0;
		m_mode = texture.m_mode;
		m_width = texture.m_width;
		m_height = texture.m_height;
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	Texture& Texture::operator=(Texture&& texture)
	{
		glDeleteTextures(1, &m_ID);

		m_ID = texture.m_ID;
		texture.m_ID = 0;
		m_mode = texture.m_mode;
		m_width = texture.m_width;
		m_height = texture.m_height;
		
		return *this;
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture::addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightBottomUV)
	{
		m_subTextures.emplace(std::move(name), SubTexture(leftBottomUV, rightBottomUV));
	}

	const Texture::SubTexture& Texture::getSubTexture(const std::string& name) const
	{
		auto it = m_subTextures.find(name);
		if (it != m_subTextures.end())
		{
			return it->second;
		}

		const static SubTexture defaultSubTexture;
		return defaultSubTexture;
	}
	

}
