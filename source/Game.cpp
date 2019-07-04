/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CVertexAttribute.h"
#include "CVertexAttributes.h"
#include "CVertexBufferObject.h"
#include <Awesomium/WebCore.h>
#include "CBoundingBox.h"
#include "Network.h"
#include "Render.h"
#include "CSprite.h"
#include "Camera.h"
#include "Physics.h"
#include "Events.h"
#include "Inputs.h"
#include "CMesh.h"
#include "Sounds.h"
#include "Fonts.h"
#include "Files.h"
#include "Game.h"
#include "Time.h"

Game::Game()
{
	m_vfs = CL_VirtualFileSystem("addons/system.c2d", true);
	m_vd = CL_VirtualDirectory(m_vfs, "");
	m_running = false;
	m_console = NULL;
}

Game::~Game()
{
	if (m_console != NULL)
		delete m_console;
	
	delete g_Lua;
	delete g_Time;
	delete g_Files;
	delete g_Fonts;
	delete g_Camera;
	delete g_Inputs;
	delete g_Sounds;
	delete g_Events;
	delete g_Render;
	delete g_Physics;
	delete g_Display;
	delete g_Network;
}

void Game::SetTitle(const char* title)
{
	g_Display->Window().SetTitle(title);
}

void Game::Run(const std::vector<CL_String> &args)
{
	if (m_running)
		return;

	m_running = true;
	g_Lua->Include("init");

	luabind::object table = luabind::newtable(g_Lua->State());

	for (unsigned int i = 0; i < args.size(); i++)
	{
		table[i + 1] = args[i].c_str();
	}

	//try
	//{
	
	luabind::call_function<void>(g_Lua->State(), "OnInitGame", table);
	
	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}

	const int iLoopRate = 10; // 10 ms (100hz)!

	while (m_running)
	{
		int lastTime = CL_System::get_time();
			Update(); Draw();
		CL_KeepAlive::process();

		int curTime = CL_System::get_time();
		int sleepTime = iLoopRate - (curTime - lastTime);

		if (sleepTime > 0)
			CL_System::sleep(sleepTime);
	}

	g_Lua->Call("OnQuitGame");
}

void Game::CreateConsole()
{
	if (m_console == NULL)
		m_console = new CL_ConsoleWindow("Console", 80, 160);
}

void Game::Init(Awesomium::WebSession* webSession)
{
	// this->webSession = webSession;

	if (!g_Files->Exists("console.log"))
		g_Files->Write("console.log", "## Cloud 2D - Log File ##");

	g_Lua->Open();
	g_Display->Create(720, 480, "Cloud2D");
	g_Events->Bind();
	g_Physics->Init();

	luabind::object globals = luabind::globals( g_Lua->State() );

	// Bind the regular classes to Lua.
	CSprite::LuaBind(globals);
	CMesh::LuaBind(globals);
	CShader::LuaBind(globals);
	CBoundingBox::LuaBind(globals);
	CFontDescription::LuaBind(globals);
	CFontObject::LuaBind(globals);
	CGraphicContext::LuaBind(globals);
	CInputContext::LuaBind(globals);
	CInputDevice::LuaBind(globals);
	CInputEvent::LuaBind(globals);
	CDisplayWindow::LuaBind(globals);
	CFrameBuffer::LuaBind(globals);
	CVertexAttribute::LuaBind(globals);
	CVertexAttributes::LuaBind(globals);

	// Bind the singleton classes to Lua.
	g_Lua->LuaBind(globals);
	g_Game->LuaBind(globals);
	g_Time->LuaBind(globals);
	g_Files->LuaBind(globals);
	g_Fonts->LuaBind(globals);
	g_Camera->LuaBind(globals);
	g_Inputs->LuaBind(globals);
	g_Sounds->LuaBind(globals);
	g_Render->LuaBind(globals);
	g_Physics->LuaBind(globals);
	g_Display->LuaBind(globals);
	g_Network->LuaBind(globals);
}

void Game::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Game>("Game")
		.def("SetRunning", &Game::SetRunning)
		.def("IsRunning", &Game::IsRunning)
		.def("CreateConsole", &Game::CreateConsole)
		.def("SetTitle", &Game::SetTitle)
	];

	luabind::module(  g_Lua->State()  )
	[
		luabind::class_<CEventHandler>("EventHandler")
	];

	globals["GameInstance"] = g_Game;
}

void Game::SetRunning(bool running)
{
	m_running = running;
}

bool Game::IsRunning() const
{
	return m_running;
}

void Game::Update()
{
	g_Time->Update();
	g_Display->Update();

	//try
	//{
	
	luabind::call_function<void>( g_Lua->State(), "OnUpdateGame", g_Time->GetDT() );
	
	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}

	g_Events->Update();

	/*
		We have to update the WebCore or else
		Awesomium will not function.
	*/
	// Awesomium::WebCore::instance()->Update();
}

void Game::Draw()
{
	g_Display->Clear(CL_Colorf::skyblue);
		g_Lua->Call("OnDrawGame");
	g_Display->Flip(-1);

	g_Render->SetShading(false);
}

CL_VirtualFileSystem& Game::GetVFS()
{
	return m_vfs;
}

CL_VirtualDirectory& Game::GetVD()
{
	return m_vd;
}