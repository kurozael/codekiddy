/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef GAME_H
#define GAME_H

#include <Awesomium/WebSession.h>
#include <ClanLib/core.h>
#include <ClanLib/gl1.h>
#include "CNonCopyable.h"
#include "CSingleton.h"
#include "Display.h"
#include "Lua.h"

class Game : public CNonCopyable
{
public:
	CL_VirtualFileSystem& GetVFS();
	CL_VirtualDirectory& GetVD();
public:
	bool IsRunning() const;
	void SetRunning(bool running);
	void LuaBind(luabind::object& globals);
	void SetTitle(const char* title);
	void CreateConsole();
	void Update();
	void Draw();
	void Run(const std::vector<CL_String> &args);
	void Init(Awesomium::WebSession* webSession = NULL);
	~Game();
	Game();
private:
	CL_VirtualFileSystem m_vfs;
	CL_VirtualDirectory m_vd;
	CL_ConsoleWindow* m_console;
	bool m_running;
public:
	Awesomium::WebSession* webSession;
};

#define g_Game CSingleton<Game>::Instance()

#endif