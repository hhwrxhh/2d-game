#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>


namespace Renderer
{
	class Texture
	{
	public:
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

	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width, m_height;
	};
}