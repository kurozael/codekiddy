/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CEVENTHANDLER_H
#define CEVENTHANDLER_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CInputEvent.h"
#include "CNonCopyable.h"

struct EventData
{
	const char* type;
	CInputEvent inputEvent;
};

class CEventHandler : public CNonCopyable
{
public:
	CEventHandler();
	~CEventHandler();
	void HandleEvent(EventData &eventData);
	virtual void OnKeyEvent(CInputEvent& inputEvent) {};
	virtual void OnMouseEvent(CInputEvent& inputEvent) {};
	virtual void OnQuitEvent() {};
};

#endif