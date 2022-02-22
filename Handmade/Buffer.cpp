#include <assert.h>
#include "Buffer.h"

std::map<std::string, Buffer> Buffer::s_buffers;

//======================================================================================================
void Buffer::SetPointSize(GLfloat size)
{
	glPointSize(size);
}
//======================================================================================================
void Buffer::SetLineWidth(GLfloat lineWidth)
{
	glLineWidth(lineWidth);
}
//======================================================================================================
void Buffer::SetCullingMode(Culling culling)
{
	if (culling == Culling::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(static_cast<GLenum>(culling));
		glFrontFace(GL_CW);
	}
}
//======================================================================================================
void Buffer::Destroy(const std::string& tag)
{
	if (!tag.empty())
	{
		auto it = s_buffers.find(tag);
		assert(it != s_buffers.end());

		if (it->second.m_hasEBO)
		{
			glDeleteBuffers(1, &it->second.m_EBO);
		}
		glDeleteBuffers(4, it->second.m_VBOs.data());
		glDeleteVertexArrays(1, &it->second.m_VAO);

		s_buffers.erase(it);
	}
	else
	{
		for (auto& buffer : s_buffers)
		{
			if (buffer.second.m_hasEBO)
			{
				glDeleteBuffers(1, &buffer.second.m_EBO);
			}
			glDeleteBuffers(4, buffer.second.m_VBOs.data());
			glDeleteVertexArrays(1, &buffer.second.m_VAO);
		}
		s_buffers.clear();
	}
}
//======================================================================================================
void Buffer::SetRenderStyle(RenderStyle renderStyle)
{
	glPolygonMode(GL_FRONT_AND_BACK, renderStyle == RenderStyle::Polygonal ? GL_LINE : GL_FILL);
}
//======================================================================================================
Buffer::Buffer(const std::string& tag, GLsizei totalVertices, bool hasEBO) 
	  :m_tag(tag),m_totalVertices(totalVertices),m_hasEBO(hasEBO)
{
	for (auto& ID : m_VBOs)
	{
		ID = 0;
	}
	if (totalVertices > 0)
	{
		assert(s_buffers.find(tag) == s_buffers.end());
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(4, m_VBOs.data());
		if (hasEBO)
		{
			glGenBuffers(1, &m_EBO);
		}
		s_buffers[tag] = *this;
	}
	else if (!tag.empty())
	{
		SetBuffer(tag);
	}
}
//======================================================================================================
const std::string& Buffer::GetTag() const
{
	return m_tag;
}
//======================================================================================================
void Buffer::SetBuffer(const std::string& tag)
{
	auto it = s_buffers.find(tag);
	assert(it != s_buffers.end());
	*this = it->second;
}
//======================================================================================================
void Buffer::FillEBO(const GLuint* data, GLsizeiptr bufferSize, Fill fill)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fill));
}
//======================================================================================================
void Buffer::FillVBO(VBO vbo, const void* data, GLsizeiptr bufferSize, Fill fill)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[static_cast<int>(vbo)]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fill));
}
//======================================================================================================
void Buffer::AppendEBO(const GLuint* data, GLsizeiptr size, GLuint offset)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
//======================================================================================================
void Buffer::AppendVBO(VBO vbo, const void* data, GLsizeiptr size, GLuint offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[static_cast<int>(vbo)]);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
//======================================================================================================
void Buffer::LinkEBO()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBindVertexArray(0);
}
//======================================================================================================
void Buffer::LinkVBO(GLint attributeID, VBO vbo, ComponentSize componentSize, DataType dataType)
{
	assert(attributeID > -1);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[static_cast<int>(vbo)]);
	glVertexAttribPointer(attributeID, static_cast<GLint>(componentSize),
		static_cast<GLenum>(dataType), GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(attributeID);
	glBindVertexArray(0);
}
//======================================================================================================
void Buffer::Render(RenderMode renderMode, GLuint index, GLuint totalRenderVertices)
{
	assert(!m_tag.empty());

	glBindVertexArray(m_VAO);
	if (m_hasEBO)
	{
		if (index > 0)
		{
			glDrawElements(static_cast<GLenum>(renderMode),
				totalRenderVertices, GL_UNSIGNED_INT, (const void*)(index));
		}
		else
		{
			glDrawElements(static_cast<GLenum>(renderMode),
				m_totalVertices, GL_UNSIGNED_INT, (const void*)(nullptr));
		}
	}
	else
	{
		glDrawArrays(static_cast<GLenum>(renderMode), 0, m_totalVertices);
	}
	glBindVertexArray(0);
}