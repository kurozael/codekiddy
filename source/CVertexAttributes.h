/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CVERTEXATTRIBUTES_H
#define CVERTEXATTRIBUTES_H

#include "CVertexAttribute.h"
#include <string>
#include <vector>
#include "Lua.h"

class CVertexAttributes
{
public:
    class Usage
	{
    public:
        const static int Position = CL_StandardAttributeIndex::cl_attrib_position;
        const static int Color = CL_StandardAttributeIndex::cl_attrib_color;
        const static int ColorPacked = 6;
        const static int Normal = CL_StandardAttributeIndex::cl_attrib_normal;
        const static int TextureCoordinates = CL_StandardAttributeIndex::cl_attrib_texture_position;
        const static int Generic = 5;
    };

	void Add(CVertexAttribute& attribute);
    CVertexAttributes(const std::vector<CVertexAttribute>& attributes);
	CVertexAttributes();
    CVertexAttribute& Get(int index);
	static void LuaBind(luabind::object& globals);
    
    std::string toString();
    int vertexSize;
    int Size();
private:
    int CalculateOffsets();
    std::vector<CVertexAttribute> m_attributes;
};

#endif
