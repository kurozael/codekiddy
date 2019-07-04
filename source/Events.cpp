/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CSprite.h"
#include "Events.h"
#include "Inputs.h"
#include "Display.h"
#include "Lua.h"

Events::Events()
{
	m_captureStack = 0;
}

Events::~Events() {}

void Events::Bind()
{
	CL_DisplayWindow window = g_Display->Window().Object();
	CL_InputDevice keyboard = g_Inputs->Keyboard().Object();
	CL_InputDevice mouse = g_Inputs->Mouse().Object();

	m_slots["MouseDblClkEvent"] = mouse.sig_key_dblclk().connect(this, &Events::MouseEvent);
	m_slots["MouseDownEvent"] = mouse.sig_key_down().connect(this, &Events::MouseEvent);
	m_slots["KeyDownEvent"] = keyboard.sig_key_down().connect(this, &Events::KeyEvent);
	m_slots["MouseUpEvent"] = mouse.sig_key_up().connect(this, &Events::MouseEvent);
	m_slots["KeyUpEvent"] = keyboard.sig_key_up().connect(this, &Events::KeyEvent);
	m_slots["QuitEvent"] = window.sig_window_close().connect(this, &Events::QuitEvent);
}

void Events::Queue(EventData& eventData)
{
	m_addQueue.push_back(eventData);
}

void Events::QuitEvent()
{
	bool bQueueEvent = true;

	lua_State* lua = g_Lua->State();

	//try
	//{

	luabind::call_function<bool>(lua, "OnQuitEvent");

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}

	if (bQueueEvent)
	{
		EventData eventData;
			eventData.type = "QuitEvent";
		m_addQueue.push_back(eventData);
	}
}

void Events::KeyEvent(const CL_InputEvent& input, const CL_InputState &state)
{
	CInputEvent inputEvent( const_cast<CL_InputEvent&>(input) );

	EventData eventData;
		eventData.type = "KeyEvent";
		eventData.inputEvent = inputEvent;
	m_addQueue.push_back(eventData);

	lua_State* lua = g_Lua->State();

	//try
	//{

	luabind::call_function<void>(lua, "OnKeyEvent", eventData.inputEvent);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}

	if (input.type == CL_InputEvent::pressed && m_keyMap[input.id] != CL_InputEvent::pressed)
	{
		m_keyMap[input.id] = CL_InputEvent::pressed;
		
		//try
		//{
		
		luabind::call_function<void>(lua, "OnKeyPress", input.id, std::string(input.str));
		
		//}
		//catch (const luabind::error &e)
		//{
		//	g_Lua->Print( lua_tostring(e.state(), -1) );
		//}
	}
	else if (input.type == CL_InputEvent::released && m_keyMap[input.id] != CL_InputEvent::released)
	{
		m_keyMap[input.id] = CL_InputEvent::released;

		//try
		//{
		
		luabind::call_function<void>(lua, "OnKeyRelease", input.id, std::string(input.str));
		
		//}
		//catch (const luabind::error &e)
		//{
		//	g_Lua->Print( lua_tostring(e.state(), -1) );
		//}
	}
}

void Events::MouseEvent(const CL_InputEvent& input, const CL_InputState &state)
{
	CInputEvent inputEvent( const_cast<CL_InputEvent&>(input) );

	EventData eventData;
		eventData.type = "MouseEvent";
		eventData.inputEvent = inputEvent;
	m_addQueue.push_back(eventData);

	CL_DisplayWindow window = g_Display->Window().Object();

	if (input.type == CL_InputEvent::pressed)
	{
		window.capture_mouse(true);
	}
	else if (input.type == CL_InputEvent::released)
	{
		window.capture_mouse(false);
	}

	lua_State* lua = g_Lua->State();

	//try
	//{

	luabind::call_function<void>(lua, "OnMouseEvent", eventData.inputEvent);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}

	if (input.type == CL_InputEvent::pressed && m_keyMap[input.id] != CL_InputEvent::pressed)
	{
		m_keyMap[input.id] = CL_InputEvent::pressed;

		//try
		//{

		luabind::call_function<void>(lua, "OnMouseButtonPress", input.id);
		
		//}
		//catch (const luabind::error &e)
		//{
		//	g_Lua->Print( lua_tostring(e.state(), -1) );
		//}
	}
	else if (input.type == CL_InputEvent::released && m_keyMap[input.id] != CL_InputEvent::released)
	{
		m_keyMap[input.id] = CL_InputEvent::released;

		//try
		//{

		luabind::call_function<void>(lua, "OnMouseButtonRelease", input.id);

		//}
		//catch (const luabind::error &e)
		//{
		//	g_Lua->Print( lua_tostring(e.state(), -1) );
		//}
	}else if (input.type == CL_InputEvent::doubleclick)
	{
		//try
		//{
		
		luabind::call_function<void>(lua, "OnMouseDoubleClick", input.id);
		
		//}
		//catch (const luabind::error &e)
		//{
		//	g_Lua->Print( lua_tostring(e.state(), -1) );
		//}
	}
}

void Events::Update()
{
	for (EventQueueList::iterator it = m_addQueue.begin(); it != m_addQueue.end(); it++)
	{
		EventHandlersSet handlers = m_handlers;

		for (EventHandlersSet::iterator handler = handlers.begin(); handler != handlers.end(); handler++)
		{
			(*handler)->HandleEvent( (*it) );
		}
	}

	if ( !m_addQueue.empty() )
	{
		m_addQueue.clear();
	}
}

void Events::AddHandler(CEventHandler* eventHandler)
{
	m_handlers.insert(eventHandler);
}

void Events::RemoveHandler(CEventHandler* eventHandler)
{
	m_handlers.erase(eventHandler);
}