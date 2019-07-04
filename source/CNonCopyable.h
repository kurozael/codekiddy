/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CNONCOPYABLE_H
#define CNONCOPYABLE_H

class CNonCopyable
{
public:
	CNonCopyable() {};
	~CNonCopyable() {};
private:
	CNonCopyable(const CNonCopyable&);
	CNonCopyable& operator=(const CNonCopyable&);
};

#endif