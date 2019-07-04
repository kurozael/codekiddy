/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include <Awesomium/ChildProcess.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/WebConfig.h>
#include <Awesomium/WebString.h>
#include <Awesomium/WebView.h>
#include <Awesomium/WebCore.h>
#include <ClanLib/application.h>
#include <ClanLib/network.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include "Files.h"
#include "Game.h"

class AwesomiumDataSource : public Awesomium::DataSource
{
public:
    void OnRequest(int request_id, const Awesomium::WebString& path)
    {
		std::string filePath = Awesomium::ToString(path);
		std::string extension = CL_PathHelp::get_extension(filePath);
		CL_DataBuffer buffer = g_Files->Fetch(Awesomium::ToString(path).c_str());
		std::string mimeType = "text/html";
		
		if (extension == "js")
			mimeType = "application/javascript";
		else if (extension == "png")
			mimeType = "image/png";

		SendResponse(request_id, buffer.get_size(), (unsigned char *)buffer.get_data(), Awesomium::ToWebString(mimeType));
    };
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		/*
		Awesomium::WebConfig config;
			config.child_process_path = Awesomium::ToWebString("html/bin/");
			config.package_path = Awesomium::ToWebString("html/bin/");
			config.plugin_path = Awesomium::ToWebString("html/bin/");
			config.log_path = Awesomium::ToWebString("html/bin/");
		Awesomium::WebCore* webCore = Awesomium::WebCore::Initialize(config);
		Awesomium::WebPreferences webPref;
			webPref.enable_javascript = true;
			webPref.enable_smooth_scrolling = true;
			webPref.load_images_automatically = true;
			webPref.enable_gpu_acceleration = true;
			webPref.enable_web_gl = true;
			webPref.enable_web_audio = true;
		Awesomium::WebSession* webSession = webCore->CreateWebSession(Awesomium::ToWebString(""), webPref);
		
		AwesomiumDataSource* dataSource = new AwesomiumDataSource; 
		webSession->AddDataSource(Awesomium::ToWebString("core"), dataSource);
		*/
		
		CL_SetupDisplay setup_display;
		CL_SetupNetwork setup_network;
		CL_SetupCore setup_core;
		CL_SetupGL setup_gl;

		try
		{
			g_Game->Init();
			g_Game->Run(args);
		}
		catch (CL_Exception &exception)
		{
			CL_ConsoleWindow console("Console", 80, 160);
			CL_Console::write_line(exception.get_message_and_stack_trace());
			g_Lua->Print(exception.get_message_and_stack_trace().c_str());

			console.display_close_message();

			return -1;
		}
		
		// We don't need this web session anymore!
		// webSession->Release();

		/*
			Update one last time and then shutdown
			for good measure.
		*/
		// Sleep(300); 

		// Awesomium::WebCore::Shutdown();

		// And finally we can release the game from its duties!
		delete g_Game;

		return 0;
	}
};

CL_ClanApplication app(&Program::main);