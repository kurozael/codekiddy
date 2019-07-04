/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CVertexAttributes.h"

#include <sstream>
#include <cassert>
#include <stdexcept>

CVertexAttributes::CVertexAttributes(const std::vector<CVertexAttribute>& m_attributes) : m_attributes(m_attributes)
{
    if (m_attributes.size() == 0)
        throw std::runtime_error("You did not define any attributes for CVertexAttributes!");

    vertexSize = CalculateOffsets();
}

CVertexAttributes::CVertexAttributes() {}

void CVertexAttributes::Add(CVertexAttribute& attribute)
{
	m_attributes.push_back(attribute);
	vertexSize = CalculateOffsets();
}

int CVertexAttributes::CalculateOffsets()
{
    int count = 0;
	
    for (unsigned int i = 0; i < m_attributes.size(); i++)
	{
        CVertexAttribute& attribute = m_attributes[i];
        attribute.offset = count;
		
        if (attribute.usage == CVertexAttributes::Usage::ColorPacked)
            count += 4;
        else
            count += 4 * attribute.numIdx;
    }

    return count;
}

int CVertexAttributes::Size()
{
    return m_attributes.size();
}

CVertexAttribute& CVertexAttributes::Get(int index)
{
    return m_attributes[index];
}

std::string CVertexAttributes::toString()
{
    std::stringstream builder;
	
    for (unsigned int i = 0; i < m_attributes.size(); i++)
	{
        builder << m_attributes[i].alias << ", " <<
        m_attributes[i].usage << ", " <<
        m_attributes[i].numIdx <<  ", " <<
        m_attributes[i].offset <<  std::endl;
    }
	
    return builder.str();
}

void CVertexAttributes::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CVertexAttributes>("VertexAttributes")
		.def(luabind::constructor<>())
		.def("Add", &CVertexAttributes::Add)
	];

	globals["VERTEX_COLOR"] = Usage::Color;
	globals["VERTEX_POSITION"] = Usage::Position;
	globals["VERTEX_COLOR_PACKED"] = Usage::ColorPacked;
	globals["VERTEX_GENERIC"] = Usage::Generic;
	globals["VERTEX_NORMAL"] = Usage::Normal;
	globals["VERTEX_TEX_COORDS"] = Usage::TextureCoordinates;
}