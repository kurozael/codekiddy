/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef EVENTS_H
#define EVENTS_H

#include "CEventHandler.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CNonCopyable.h"
#include "CSingleton.h"
#include <list>
#include <map>
#include <set>

typedef std::map<const char*, CL_Slot> EventSlotsMap;
typedef std::map<int, CL_InputEvent::Type> EventKeyMap;
typedef std::set<CEventHandler*> EventHandlersSet;
typedef std::list<EventData> EventQueueList;

class Events : public CNonCopyable
{
public:
	void Bind();
	void Update();
	void Queue(EventData& eventData);
	void AddHandler(CEventHandler* e);
	void RemoveHandler(CEventHandler* e);
	void QuitEvent();
	void KeyEvent(const CL_InputEvent& input, const CL_InputState &state);
	void MouseEvent(const CL_InputEvent& input, const CL_InputState &state);
	~Events();
	Events();
private:
	EventHandlersSet m_handlers;
	EventQueueList m_addQueue;
	EventSlotsMap m_slots;
	EventKeyMap m_keyMap;
	int m_captureStack;
};

#define g_Events CSingleton<Events>::Instance()

#endif