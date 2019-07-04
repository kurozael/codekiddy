/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CShader.h"
#include "Render.h"
#include "Display.h"
#include "Util.h"
#include "Game.h"
#include "Files.h"

CShader::CShader(const char* vertex, const char* fragment)
{
	std::string newVertexPath = Util::FixFilePath(vertex, "shaders/", ".fx");
	std::string newFragmentPath = Util::FixFilePath(fragment, "shaders/", ".fx");

	bool bIsVertLocal = g_Files->IsLocal(newVertexPath.c_str());
	bool bIsFragLocal = g_Files->IsLocal(newFragmentPath.c_str());

	if (bIsVertLocal != bIsFragLocal)
	{
		g_Lua->Print("Vertex and fragment shaders are not within the same container!");
		return;
	}

	if (bIsVertLocal)
		m_object = CL_ProgramObject::load(
			g_Display->Graphics().Object(), newVertexPath, newFragmentPath);
	else
		m_object = CL_ProgramObject::load(
			g_Display->Graphics().Object(), newVertexPath, newFragmentPath, g_Game->GetVD());

	m_object.bind_attribute_location(CL_StandardAttributeIndex::cl_attrib_position, "Position");
	m_object.bind_attribute_location(CL_StandardAttributeIndex::cl_attrib_texture_position, "TexCoord");
	m_object.bind_attribute_location(CL_StandardAttributeIndex::cl_attrib_color, "Color");
	m_object.bind_attribute_location(5, "Generic");
	
	if (!m_object.link())
		g_Lua->Print(("Unable to link shader program: " + std::string(m_object.get_info_log().c_str())).c_str());
}

void CShader::SetTexture(const char* key, CL_Texture& texture, int id)
{
	CL_GraphicContext& gc = g_Display->Graphics().Object();
		m_idMap[id] = gc.get_texture(id);
		gc.set_texture(id, texture);
	m_object.set_uniform1i(key, id);
}

void CShader::SetColor(const char* key, CL_Colorf& color)
{
	m_object.set_uniform4f(key, CL_Vec4f(color.r, color.g, color.b, color.a));
}

void CShader::SetVec2(const char* key, CL_Vec2f& vec2)
{
	m_object.set_uniform2f(key, vec2);
}

void CShader::SetFloat(const char* key, float value)
{
	m_object.set_uniform1f(key, value);
}

void CShader::SetMat4(const char* key, LuaMat4f& mat4)
{
	m_object.set_uniform_matrix(key, mat4.Object());
}

void CShader::SetInt(const char* key, int value)
{
	m_object.set_uniform1i(key, value);
}

CShader::CShader(CL_ProgramObject& shader)
{
	m_object = shader;
}

void CShader::Bind()
{
	CL_GraphicContext& gc = g_Display->Graphics().Object();
		gc.set_program_object(m_object, cl_program_matrix_modelview_projection);
	m_idMap.clear();

	g_Render->SetShading(true);
}

void CShader::Unbind()
{
	CL_GraphicContext& gc = g_Display->Graphics().Object();
	gc.reset_program_object();

	for (std::map<int, CL_Texture>::iterator it = m_idMap.begin(); it != m_idMap.end(); it++)
	{
		gc.set_texture(it->first, it->second);
	}

	g_Render->SetShading(false);
}

CL_ProgramObject& CShader::Object()
{
	return m_object;
}

bool CShader::IsNull()
{
	return m_object.is_null();
}

void CShader::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CShader>("Shader")
		.def(luabind::constructor<const char*, const char*>())
		.def("SetMatrix", &CShader::SetMat4)
		.def("SetTexture", &CShader::SetTexture)
		.def("SetColor", &CShader::SetColor)
		.def("SetFloat", &CShader::SetFloat)
		.def("SetVec2", &CShader::SetVec2)
		.def("SetInt", &CShader::SetInt)
		.def("Unbind", &CShader::Unbind)
		.def("Bind", &CShader::Bind)
		.def("IsNull", &CShader::IsNull)
	];
}