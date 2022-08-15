#include <assert.h>
#include "Buffer.h"

std::map<std::string, Buffer> Buffer::buffers;

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
		auto it = buffers.find(tag);
		assert(it != buffers.end());

		if (it->second.hasEBO)
		{
			glDeleteBuffers(1, &it->second.EBO);
		}

		glDeleteBuffers(4, it->second.VBOs.data());
		glDeleteVertexArrays(1, &it->second.VAO);

		buffers.erase(it);
	}

	else
	{
		for (auto& buffer : buffers)
		{
			if (buffer.second.hasEBO)
			{
				glDeleteBuffers(1, &buffer.second.EBO);
			}

			glDeleteBuffers(4, buffer.second.VBOs.data());
			glDeleteVertexArrays(1, &buffer.second.VAO);
		}

		buffers.clear();
	}
}
//======================================================================================================
void Buffer::SetRenderStyle(RenderStyle renderStyle)
{
	glPolygonMode(GL_FRONT_AND_BACK, renderStyle == RenderStyle::Polygonal ? GL_LINE : GL_FILL);
}
//======================================================================================================
Buffer::Buffer(const std::string& tag, GLsizei totalVertices, bool hasEBO)
	: hasEBO(hasEBO), tag(tag), totalVertices(totalVertices)
{
	for (auto& ID : VBOs)
	{
		ID = 0;
	}

	if (totalVertices > 0)
	{
		assert(buffers.find(tag) == buffers.end());

		glGenVertexArrays(1, &VAO);
		glGenBuffers(4, VBOs.data());

		if (hasEBO)
		{
			glGenBuffers(1, &EBO);
		}

		buffers[tag] = *this;
	}

	else if (!tag.empty())
	{
		SetBuffer(tag);
	}
}
//======================================================================================================
const std::string& Buffer::GetTag() const
{
	return   tag;
}
//======================================================================================================
void Buffer::SetBuffer(const std::string& tag)
{
	auto it = buffers.find(tag);
	assert(it != buffers.end());
	*this = it->second;
}
//======================================================================================================
void Buffer::FillEBO(const GLuint* data, GLsizeiptr bufferSize, Fill fill) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fill));
}
//======================================================================================================
void Buffer::FillVBO(VBO vbo, const void* data, GLsizeiptr bufferSize, Fill fill) const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[static_cast<int>(vbo)]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fill));
}
//======================================================================================================
void Buffer::AppendEBO(const GLuint* data, GLsizeiptr size, GLuint offset) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
//======================================================================================================
void Buffer::AppendVBO(VBO vbo, const void* data, GLsizeiptr size, GLuint offset) const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[static_cast<int>(vbo)]);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
//======================================================================================================
void Buffer::LinkEBO() const
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
}
//======================================================================================================
void Buffer::LinkVBO(GLint attributeID, VBO vbo, ComponentSize componentSize, DataType dataType) const
{
	assert(attributeID > -1);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[static_cast<int>(vbo)]);

	glVertexAttribPointer(attributeID, static_cast<GLint>(componentSize),
		static_cast<GLenum>(dataType), GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(attributeID);
	glBindVertexArray(0);
}
//======================================================================================================
void Buffer::Render(RenderMode renderMode, GLuint index, GLuint totalRenderVertices) const
{
	assert(!tag.empty());

	glBindVertexArray(VAO);

	if (hasEBO)
	{
		if (index > 0)
		{
			glDrawElements(static_cast<GLenum>(renderMode),
				totalRenderVertices, GL_UNSIGNED_INT, (const void*)(index));
		}

		else
		{
			glDrawElements(static_cast<GLenum>(renderMode),
				totalVertices, GL_UNSIGNED_INT, (const void*)(nullptr));
		}
	}

	else
	{
		glDrawArrays(static_cast<GLenum>(renderMode), 0, totalVertices);
	}

	glBindVertexArray(0);
}