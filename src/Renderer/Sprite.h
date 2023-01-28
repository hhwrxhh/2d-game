#pragma once
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine 
{
	class Texture;
	class ShaderProgram;
	class Sprite
	{
	public:
		Sprite(std::shared_ptr<Texture> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram,
			std::string initialSubTexture);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		virtual void render(const glm::vec2& position, const glm::vec2& size, const float rotation) const;

	protected:
		std::shared_ptr<Texture> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
} 