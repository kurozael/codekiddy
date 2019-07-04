/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CShader.h"
#include "Display.h"
#include "Camera.h"
#include "CMesh.h"

CMesh::CMesh(bool isStatic, int maxVertices, CVertexAttributes& attributes) : m_autoBind(true)
{
	m_vertices = CVertexBufferObject(isStatic, maxVertices, attributes);
	m_attributes = attributes;
}

void CMesh::SetVertices(const luabind::object& vertices, int offset, int count)
{
	std::vector<float> vertVec;
	
	for (int i = 1; i <= count; i++)
	{
		vertVec.push_back(luabind::object_cast<float>(vertices[i]));
	}

	m_vertices.SetVertices(vertVec, offset, count);
}

void CMesh::GetVertices(std::vector<float>& vertices)
{
	if (vertices.size() < GetNumVertices() * GetVertexSize() / 4)
	{
		// "not enough room in vertices array, has " << vertices.size() << " floats, needs " << getNumVertices() * getVertexSize() / 4;
	}
	
	int position = GetVerticesBuffer().position();
	GetVerticesBuffer().position(0);
		GetVerticesBuffer().get(vertices, 0, GetNumVertices() * GetVertexSize() / 4);
	GetVerticesBuffer().position(position);
}

unsigned int CMesh::GetNumVertices()
{
	return m_vertices.GetNumVertices();
}

int CMesh::GetMaxVertices()
{
	return m_vertices.GetNumMaxVertices();
}

int CMesh::GetVertexSize()
{
	return m_vertices.GetAttributes().vertexSize;
}

void CMesh::SetAutoBind(bool autoBind)
{
	m_autoBind = autoBind;
}

void CMesh::Bind(CShader& shader)
{
	m_vertices.Bind(shader);
}

void CMesh::Unbind(CShader& shader)
{
	m_vertices.Unbind(shader);
}

void CMesh::Render(int primitiveType)
{
	if(m_autoBind) m_vertices.Bind();
		glDrawArrays(primitiveType, 0, GetNumVertices());
	if(m_autoBind) m_vertices.Unbind();
}

void CMesh::Render(int primitiveType, int offset, int count)
{
	if(m_autoBind) m_vertices.Bind();
		glDrawArrays(primitiveType, offset, count);
	if(m_autoBind) m_vertices.Unbind();
}

void CMesh::Render(CShader& shader, int primitiveType)
{
	Render(shader, primitiveType, 0, GetNumVertices());
}

void CMesh::Render(CShader& shader, int primitiveType, int offset, int count)
{
	g_Lua->Print("1");
	if(m_autoBind) Bind(shader);
	g_Lua->Print("2");
		glDrawArrays(primitiveType, offset, count);
		g_Lua->Print("3");
	if(m_autoBind) Unbind(shader);
	g_Lua->Print("4");
}

void CMesh::AddVertex(CVertex& vertex)
{
	m_verts.push_back(vertex);
}

void CMesh::Reset()
{
	m_verts.clear();
}

void CMesh::Draw(CShader& shader, int primitiveType)
{
	CL_PrimitivesArray vertices(g_Display->Graphics().Object());
	int genericIdx = 1;

	std::vector<CL_Vec2i> positions;
	std::vector<CL_Vec4f> colors;
	std::vector<CL_Vec1f> generics;
	std::vector<CL_Vec2f> texCoords;

	for (unsigned int i = 0; i < m_verts.size(); i++)
	{
		CL_Vec2f worldPos(m_verts[i].position.x, m_verts[i].position.y);
		positions.push_back(worldPos);
		generics.push_back(m_verts[i].data);
		texCoords.push_back(m_verts[i].coord);
		colors.push_back(
			CL_Vec4f(m_verts[i].color.r, m_verts[i].color.g, m_verts[i].color.b, m_verts[i].color.a)
		);
	}

	for (int i = 0; i < m_attributes.Size(); i++)
	{
		CVertexAttribute& attribute = m_attributes.Get(i);
		shader.Object().bind_attribute_location(attribute.numIdx, attribute.alias);

		if (attribute.usage == CVertexAttributes::Usage::Generic)
			vertices.set_attributes(attribute.numIdx, &generics[0]);
		else if (attribute.usage == CVertexAttributes::Usage::Color)
			vertices.set_attributes(attribute.numIdx, &colors[0]);
		else if (attribute.usage == CVertexAttributes::Usage::Position)
			vertices.set_attributes(attribute.numIdx, &positions[0]);
		else if (attribute.usage == CVertexAttributes::Usage::TextureCoordinates)
			vertices.set_attributes(attribute.numIdx, &texCoords[0]);
	}
	
	g_Display->Graphics().Object().draw_primitives((CL_PrimitivesType)primitiveType, positions.size(), vertices);
}

CVertexAttribute* const CMesh::GetVertexAttribute(int usage)
{
	CVertexAttributes& attributes = m_vertices.GetAttributes();
	int length = attributes.Size();

	for(int i = 0; i < length; i++)
	{
		if(attributes.Get(i).usage == usage)
			return &attributes.Get(i);
	}

	return NULL;
}

CVertexAttributes& CMesh::GetVertexAttributes()
{
	return m_vertices.GetAttributes();
}

float_buffer& CMesh::GetVerticesBuffer()
{
	return m_vertices.GetBuffer();
}

CMesh::~CMesh() {}

void CMesh::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CMesh>("Mesh")
		.def(luabind::constructor<bool, int, CVertexAttributes>())
		.def("SetVertices", (void(CMesh::*)(const luabind::object&, int, int)) &CMesh::SetVertices)
		.def("Render", (void(CMesh::*)(int)) &CMesh::Render)
		.def("Render", (void(CMesh::*)(int, int, int)) &CMesh::Render)
		.def("Render", (void(CMesh::*)(CShader&, int)) &CMesh::Render)
		.def("Render", (void(CMesh::*)(CShader&, int, int, int)) &CMesh::Render)
		.def("AddVertex", &CMesh::AddVertex)
		.def("Reset", &CMesh::Reset)
		.def("Draw", &CMesh::Draw)
		.def("SetAutoBind", &CMesh::SetAutoBind)
		.def("GetVertexSize", &CMesh::GetVertexSize)
		.def("GetMaxVertices", &CMesh::GetMaxVertices)
		.def("Unbind", &CMesh::Unbind)
		.def("Bind", &CMesh::Bind)
	];

	luabind::module( g_Lua->State() )
	[
		luabind::class_<CVertex>("Vertex")
		.def(luabind::constructor<const CL_Vec2f&>())
		.def("SetColor", &CVertex::SetColor)
		.def("SetData", &CVertex::SetData)
		.def("SetCoord", &CVertex::SetCoord)
	];
}