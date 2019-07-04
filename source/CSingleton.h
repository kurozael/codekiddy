/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CSINGLETON_H
#define CSINGLETON_H

template <class T>
class CSingleton
{
public:
	static T* Instance()
	{
		static T* instance = 0;

		if (instance == 0)
			instance = new T;
		
		return instance;
	}
};

#endif