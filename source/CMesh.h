/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CMESH_H
#define CMESH_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CVertexBufferObject.h"
#include <vector>
#include "Lua.h"
#include <set>
#include <map>

class CVertex
{
public:
	CVertex(const CL_Vec2f& position)
	{
		this->position = position;
		this->color = CL_Colorf::white;
		this->data = 0.0f;
	}
	void SetColor(const CL_Colorf& color)
	{
		this->color = color;
	}
	void SetCoord(const CL_Vec2f& coord)
	{
		this->coord = coord;
	}
	void SetData(float data)
	{
		this->data = data;
	}
	CL_Vec2f coord;
	CL_Colorf color;
	CL_Vec2f position;
	float data;
};

class CMesh {
public:
    CMesh(bool isStatic, int maxVertices, CVertexAttributes& attributes);
	CVertexAttribute* const GetVertexAttribute(int usage);
	CVertexAttributes& GetVertexAttributes();
	float_buffer& GetVerticesBuffer();
    void SetVertices(const luabind::object& vertices, int offset, int count);
    void GetVertices(std::vector<float>& vertices);
	void AddVertex(CVertex& vertex);
    unsigned int GetNumVertices();
    int GetMaxVertices();
    int GetVertexSize();
    void SetAutoBind(bool m_autoBind);
    void Bind(CShader& shader);
    void Unbind(CShader& shader);
	void Render(int primitiveType);
	void Render(int primitiveType, int offset, int count);
    void Render(CShader& shader, int primitiveType);
    void Render(CShader& shader, int primitiveType, int offset, int count);
	void Draw(CShader& shader, int primitiveType);
	void Reset();
    void Dispose();
	static void LuaBind(luabind::object& globals);
    ~CMesh();
protected:
    CVertexBufferObject m_vertices;
	CVertexAttributes m_attributes;
	std::vector<CVertex> m_verts;
    bool m_autoBind;
};

#endif