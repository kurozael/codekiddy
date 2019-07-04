/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CVertexBufferObject.h"
#include <sstream>

int CVertexBufferObject::CreateBufferObject()
{
    glGenBuffers(1, &m_tmpHandle);
    return m_tmpHandle;
}

CVertexAttributes& CVertexBufferObject::GetAttributes()
{
    return m_attributes;
}

int CVertexBufferObject::GetNumVertices()
{
    return m_buffer.limit() * 4 / m_attributes.vertexSize;
}

int CVertexBufferObject::GetNumMaxVertices()
{
    return m_byteBuffer.capacity() / m_attributes.vertexSize;
}

float_buffer& CVertexBufferObject::GetBuffer()
{
    m_isDirty = true;
    return m_buffer;
}

void CVertexBufferObject::SetVertices(std::vector<float>& vertices, int offset, int count) 
{
    m_isDirty = true;

    if (m_isDirect) {
	  std::stringstream ss;
	   ss << m_buffer.capacity();
		g_Lua->Print(ss.str().c_str());

		std::vector<char> test;
		for (unsigned int i = 0; i < vertices.size(); i++)
			test.push_back((char)vertices[i]);

		//m_byteBuffer.put(test, count, count);
		m_byteBuffer.copy(test, count, offset);
		g_Lua->Print("BARE LOLS");
		m_buffer.position(0);
		g_Lua->Print("BARE LOLS2");
		m_buffer.limit(count);
		g_Lua->Print("BARE LOLS3");
    }else{
		m_buffer.clear();
		m_buffer.put(vertices, count, count);
		m_buffer.flip();
		m_byteBuffer.position(0);
		m_byteBuffer.limit(m_buffer.limit() << 2);
    }
	
    if (m_isBound) {
        glBufferData(GL_ARRAY_BUFFER, m_byteBuffer.limit(), m_byteBuffer, m_usage);
        m_isDirty = false;
    }
}

void CVertexBufferObject::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle);

    if (m_isDirty) {
        m_byteBuffer.limit(m_buffer.limit() * 4);
        glBufferData(GL_ARRAY_BUFFER, m_byteBuffer.limit(), m_byteBuffer, m_usage);
        m_isDirty = false;
    }

    int textureUnit = 0;
    int numAttributes = m_attributes.Size();

    for (int i = 0; i < numAttributes; i++)
	{
        CVertexAttribute& attribute = m_attributes.Get(i);

        switch (attribute.usage) {
        case CVertexAttributes::Usage::Position:
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(attribute.numIdx, GL_FLOAT, m_attributes.vertexSize, (const void*)(intptr_t) attribute.offset);
            break;

        case CVertexAttributes::Usage::Color:
        case CVertexAttributes::Usage::ColorPacked:
        {
            int colorType = GL_FLOAT;
            if (attribute.usage == CVertexAttributes::Usage::ColorPacked) colorType = GL_UNSIGNED_BYTE;

            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(attribute.numIdx, colorType, m_attributes.vertexSize, (const void*)(intptr_t) attribute.offset);
        }
            break;

        case CVertexAttributes::Usage::Normal:
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, m_attributes.vertexSize, (const void *) (intptr_t) attribute.offset);
            break;

        case CVertexAttributes::Usage::TextureCoordinates:
            //glClientActiveTexture(GL_TEXTURE0 + textureUnit);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(attribute.numIdx, GL_FLOAT, m_attributes.vertexSize, (void*)(intptr_t) attribute.offset);
            textureUnit++;
            break;

        default:
            break;
        }
    }

    m_isBound = true;
}

void CVertexBufferObject::Bind(CShader& shader)
{
	g_Lua->Print("CVBO: 1");
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle);

    if (m_isDirty) {
		g_Lua->Print("CVBO: 2");
        m_byteBuffer.limit(m_buffer.limit() * 4);
		g_Lua->Print("CVBO: 3");
        glBufferData(GL_ARRAY_BUFFER, m_byteBuffer.limit(), m_byteBuffer, m_usage);
		g_Lua->Print("CVBO: 4");
        m_isDirty = false;
		g_Lua->Print("CVBO: 5");
    }

	g_Lua->Print("CVBO: 6");
    int numAttributes = m_attributes.Size();

	g_Lua->Print("CVBO: 7");
    for (int i = 0; i < numAttributes; i++)
	{
        CVertexAttribute& attribute = m_attributes.Get(i);
		
		int location = -1;
        int colorType = GL_FLOAT;
        bool normalize = false;
		
        switch (attribute.usage) {
        case CVertexAttributes::Usage::Position:
			location = 0;
            break;
        case CVertexAttributes::Usage::Color:
			location = 2;
        case CVertexAttributes::Usage::ColorPacked:
            colorType = GL_UNSIGNED_BYTE;
            normalize = true;
			location = 2;
            break;
        case CVertexAttributes::Usage::Normal:
            break;
        case CVertexAttributes::Usage::TextureCoordinates:
            location = 1;
            break;
        default:
            break;
        }

		if (location == -1)
			continue;

		g_Lua->Print("CVBO: 8");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, attribute.numIdx, colorType, normalize, m_attributes.vertexSize, (const GLvoid*)attribute.offset);
    }

	g_Lua->Print("CVBO: 9");

    m_isBound = true;
}

void CVertexBufferObject::Unbind()
{
    int textureUnit = 0;
    int numAttributes = m_attributes.Size();

    for (int i = 0; i < numAttributes; i++) {
        CVertexAttribute attribute = m_attributes.Get(i);

        switch (attribute.usage) {
        case CVertexAttributes::Usage::Position:
            break;
        case CVertexAttributes::Usage::Color:
        case CVertexAttributes::Usage::ColorPacked:
            glDisableClientState(GL_COLOR_ARRAY);
            break;
        case CVertexAttributes::Usage::Normal:
           glDisableClientState(GL_NORMAL_ARRAY);
            break;
        case CVertexAttributes::Usage::TextureCoordinates:
            //glClientActiveTexture(GL_TEXTURE0 + textureUnit);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            textureUnit++;
            break;
        default:
            break;
        }
    }

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_isBound = false;
}

void CVertexBufferObject::Unbind(CShader& shader)
{
    int numAttributes = m_attributes.Size();

    for (int i = 0; i < numAttributes; i++)
	{
        CVertexAttribute attribute = m_attributes.Get(i);

		int location = -1;
		
        switch (attribute.usage) {
        case CVertexAttributes::Usage::Position:
			location = 0;
            break;
        case CVertexAttributes::Usage::Color:
			location = 2;
        case CVertexAttributes::Usage::ColorPacked:
			location = 2;
            break;
        case CVertexAttributes::Usage::Normal:
            break;
        case CVertexAttributes::Usage::TextureCoordinates:
            location = 1;
            break;
        default:
            break;
        }

		if (location == -1)
			continue;

        glDisableVertexAttribArray(location);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_isBound = false;
}

void CVertexBufferObject::Invalidate()
{
    m_bufferHandle = CreateBufferObject();
    m_isDirty = true;
}

void CVertexBufferObject::Dispose()
{
    m_tmpHandle = m_bufferHandle;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_tmpHandle);
    m_bufferHandle = 0;
}

CVertexBufferObject::CVertexBufferObject(bool isStatic, int numVertices, const CVertexAttributes& attributes) :
	m_bufferHandle(0),
	m_tmpHandle(0),
	m_isDirect(true),
	m_usage(0),
	m_isDirty(false),
	m_attributes(attributes),
	m_isBound(false),
	m_isStatic(isStatic)
{
	m_byteBuffer = byte_buffer(attributes.vertexSize * numVertices);
    m_byteBuffer.flip();
    m_buffer = float_buffer(attributes.vertexSize * numVertices);
    //m_buffer = m_byteBuffer.convert<float>();
	m_buffer.flip();
    m_bufferHandle = CreateBufferObject();
    m_usage = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}


CVertexBufferObject::CVertexBufferObject(bool isStatic, int numVertices, const std::vector<CVertexAttribute>& attributes) :
	m_bufferHandle(0),
	m_tmpHandle(0),
	m_isDirect(true),
	m_usage(0),
	m_isDirty(false),
	m_attributes(attributes),
	m_isBound(false),
	m_isStatic(isStatic)
{
	m_byteBuffer = byte_buffer(m_attributes.vertexSize * numVertices);
    m_buffer = m_byteBuffer.convert<float>();
    m_byteBuffer.flip();
    m_buffer.flip();
    m_bufferHandle = CreateBufferObject();
    m_usage = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}

CVertexBufferObject::CVertexBufferObject() {}