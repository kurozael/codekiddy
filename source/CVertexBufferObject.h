/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CVERTEX_BUFFER_OBJECT_H
#define CVERTEX_BUFFER_OBJECT_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include "CVertexAttributes.h"
#include "CBuffer.h"
#include "CShader.h"
#include "Lua.h"

class CVertexBufferObject
{
public:
    CVertexBufferObject(bool isStatic, int numVertices, const CVertexAttributes& attributes);
    CVertexBufferObject(bool isStatic, int numVertices, const std::vector<CVertexAttribute>& attributes);
	CVertexBufferObject();
    CVertexAttributes& GetAttributes();
    int GetNumVertices();
    int GetNumMaxVertices();
    void SetVertices(std::vector<float>& vertices, int offset, int count);
	float_buffer& GetBuffer();
    void Bind();
    void Bind (CShader& shader);
    void Unbind();
    void Unbind(CShader& shader);
    void Invalidate();
	void Dispose();
protected:
    CVertexAttributes m_attributes;
    int m_bufferHandle;
    GLuint m_tmpHandle;
    bool m_isDirect;
    int m_usage;
    bool m_isDirty;
    bool m_isBound;
    bool m_isStatic;
    byte_buffer m_byteBuffer;
    float_buffer m_buffer;
private:
    int CreateBufferObject();
};

#endif
