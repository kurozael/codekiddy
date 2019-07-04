/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CPHYSDATA_H
#define CPHYSDATA_H

#include <string>

class CPhysData
{
public:
	void SetValue(const std::string& value);
	std::string& GetValue();
	CPhysData();
	~CPhysData();
private:
	std::string m_value;
};

#endif