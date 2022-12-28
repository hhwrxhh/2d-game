#pragma once
#include <string>
#include <glad/glad.h>

namespace Renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
		~ShaderProgram();
		const bool isCompiled() { return m_isCompiled; }
		const void use();

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
		

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}