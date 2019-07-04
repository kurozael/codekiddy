/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CPhysData.h"

CPhysData::CPhysData() { m_value = ""; }

CPhysData::~CPhysData() {}

void CPhysData::SetValue(const std::string& value)
{
	m_value = value;
}

std::string& CPhysData::GetValue()
{
	return m_value;
}