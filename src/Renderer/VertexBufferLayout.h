#pragma once

#include <vector>
#include <glad/glad.h>

namespace RenderEngine
{
	struct VertexBufferLayoutElement
	{
		GLint count;
		GLenum type;
		GLboolean normilized;
		unsigned int size;
	};
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();

		void addElementLayoutFloat(const int count, const bool normalized);
		void reserveElements(const size_t count);
		unsigned int getStride() const { return m_stride; }
		const std::vector<VertexBufferLayoutElement>& getLayoutElements() const { return m_layoutElements; };
	private:
		std::vector<VertexBufferLayoutElement> m_layoutElements;
		unsigned int m_stride;

	};
}