/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CEventHandler.h"
#include "Events.h"

CEventHandler::CEventHandler() {}

CEventHandler::~CEventHandler()
{
	g_Events->RemoveHandler(this);
}

void CEventHandler::HandleEvent(EventData &eventData)
{
	if (eventData.type == "QuitEvent")
	{
		OnQuitEvent();
	}
	else if (eventData.type == "KeyEvent")
	{
		OnKeyEvent(eventData.inputEvent);
	}
	else if (eventData.type == "MouseEvent")
	{
		OnMouseEvent(eventData.inputEvent);
	}
}