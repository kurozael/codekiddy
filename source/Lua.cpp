/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
#include <sstream>
#include "Render.h"
#include <exception>
#include "Util.h"
#include "Game.h"
#include "Files.h"
#include "Lua.h"

static int WrapLuaExceptions(lua_State *state, lua_CFunction function)
{
	try
	{
		return function(state);
	}
	catch (const char *error)
	{
		g_Lua->Print("LuaError Char:");
		g_Lua->Print(error);
	}
	catch (std::exception& error) {
		g_Lua->Print("LuaError Exception:");
		g_Lua->Print(error.what());
	}
	catch (...)
	{
		g_Lua->Print("Unable to catch Lua exception!");
	}

	return lua_error(state);
}

Lua::Lua()
{
	m_state = lua_open();
	m_fileLoc = "";
}

Lua::~Lua()
{
	lua_close(m_state);
}

lua_State* Lua::State()
{
	return m_state;
}

void Lua::Open()
{
	luabind::open(m_state);
	luabind::bind_class_info(m_state);
	luaL_openlibs(m_state);
	luaJIT_setmode(m_state, 0, LUAJIT_MODE_ON);
}

void Lua::Call(const char* name)
{
	luabind::call_function<void>(m_state, name);
}

bool Lua::FuncExists(const char* funcName)
{
    luabind::object globals = luabind::globals(m_state);
    luabind::object funcObj = globals[funcName];

    if (funcObj && luabind::type(funcObj) == LUA_TFUNCTION)
		return true;
	else
		return false;
}

void Lua::Print(const char* text)
{
	std::string console = g_Files->Read("console.log");

	if (console == "")
		g_Files->Write("console.log", text);
	else
		g_Files->Write("console.log", (console + "\n" + std::string(text)).c_str());

	if (FuncExists("OnConsolePrint"))
	{
		luabind::call_function<void>(m_state, "OnConsolePrint", text);
	}
	else
	{
		luabind::object globals = luabind::globals(m_state);
		globals["InitPrint"] = text;
	}

	CL_Console::write_line(text);
}

void Lua::Include(const char* scriptPath)
{
	std::string extension = g_Files->GetExt(scriptPath);
	std::string filePath = scriptPath;
	std::string fileName = g_Files->GetName(filePath.c_str());
	
	filePath = filePath.substr(0, filePath.length() - fileName.length());
	if (filePath == "/") filePath = "";

	for (unsigned int i = 0; i < m_fileStack.size(); i++)
	{
		filePath = m_fileStack[i] + filePath;
	}
	
	if (filePath != "")
		m_fileStack.push_back(filePath);

	std::string oldScriptPath = Util::FixFilePath(scriptPath, "lua/", ".lua");
	std::string newScriptPath = Util::FixFilePath((filePath + fileName).c_str(), "lua/", ".lua");
	std::string fileData = "";

	if (g_Files->Exists(newScriptPath.c_str()))
		fileData = g_Files->Read(newScriptPath.c_str());
	else
		fileData = g_Files->Read(oldScriptPath.c_str());
	
	int errorID = luaL_loadbuffer(m_state, fileData.c_str(), fileData.length(), newScriptPath.c_str());

	if (!errorID) {
		errorID = luabind::detail::pcall(m_state, 0, LUA_MULTRET);
	}

	if (errorID) {
		std::string errCode;

		switch(errorID)
		{
			case LUA_YIELD:
				errCode = "LUA_YIELD"; break;
			case LUA_ERRRUN:
				errCode = "LUA_ERRRUN"; break;
			case LUA_ERRSYNTAX:
				errCode = "LUA_ERRSYNTAX"; break;
			case LUA_ERRMEM:
				errCode = "LUA_ERRMEM"; break;
			case LUA_ERRERR:
				errCode = "LUA_ERRERR"; break;
			default:
				errCode = "??"; break;
		}

		std::string errString;
		const char* errStringC = lua_tostring(m_state, -1);

		lua_pop(m_state, 1);
		
		if (errStringC != NULL) {
			errString = errStringC;
			Print(errString.c_str());
		}else{
			Print(errCode.c_str());
		}
	}

	if (filePath != "")
		m_fileStack.pop_back();
}

void Lua::DoString(const char* string)
{
	try
	{
		if ( luaL_dostring(m_state, string) )
		{
			Print( lua_tostring(m_state, -1) );
		}
	}
	catch(...)
	{
		Print("Unable to run the Lua string.");
	}
}

class LuaAwesomium : public Awesomium::JSMethodHandler
{
public:
	static void InjectMouseDown(int button);
	static void InjectMouseUp(int button);
	static void InjectMouseWheel(int scrollV, int scrollH);
	static void InjectMouseMove(int x, int y);
	static void InjectKeyboardEvent(const CL_InputEvent& input);
	Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller, unsigned int objID, const Awesomium::WebString& methodName, const Awesomium::JSArray& args);
	Awesomium::WebView* GetWV();
	void OnMethodCall(Awesomium::WebView* caller, unsigned int objID, const Awesomium::WebString& methodName,  const Awesomium::JSArray& args);
	void SetActive(bool bActive);
	void SendJS(const char* js);
	void LoadURL(const char* url);
	void Update();
	void Draw();
	LuaAwesomium(const char* id, int width, int height);
	~LuaAwesomium();
private:
	Awesomium::WebView* m_webView;
	unsigned int m_remoteID;
	std::string m_strID;
	CL_Texture m_texture;
	int m_width;
	int m_height;
public:
	int x;
	int y;
};

LuaAwesomium* gActiveWebView = NULL;

LuaAwesomium::LuaAwesomium(const char* id, int width, int height)
{
	Awesomium::WebCore* webCore = Awesomium::WebCore::instance();
	m_webView = webCore->CreateWebView(width, height, g_Game->webSession);
	m_webView->SetTransparent(true);
	m_texture = CL_Texture(g_Display->Graphics().Object(), width, height);
	m_width = width;
	m_height = height;
		
	Awesomium::JSValue lua = m_webView->CreateGlobalJavascriptObject(
		Awesomium::WSLit("lua")
	);
	Awesomium::JSObject& luaObj = lua.ToObject();
		
	luaObj.SetCustomMethod(Awesomium::WSLit("Send"), false);

	std::stringstream stringStream;
		stringStream << "html/bin/webviews/";
		stringStream << id;
		stringStream << ".png";

	m_remoteID = luaObj.remote_id();
	m_webView->set_js_method_handler(this);
	m_strID = stringStream.str();
	x = 0;
	y = 0;
}

Awesomium::JSValue LuaAwesomium::OnMethodCallWithReturnValue(Awesomium::WebView* caller, unsigned int objID, const Awesomium::WebString& methodName, const Awesomium::JSArray& args) {
	return Awesomium::JSValue::Undefined();
}

void LuaAwesomium::OnMethodCall(Awesomium::WebView* caller, unsigned int objID, const Awesomium::WebString& methodName,  const Awesomium::JSArray& args)
{
	if (objID == m_remoteID && methodName == Awesomium::WSLit("Send"))
		g_Lua->DoString(Awesomium::ToString(args[0].ToString()).c_str());
}

Awesomium::WebView* LuaAwesomium::GetWV()
{
	return m_webView;
}

void LuaAwesomium::InjectMouseDown(int button)
{
	if (gActiveWebView == NULL) return;

	if (button == CL_MOUSE_LEFT)
		button = Awesomium::MouseButton::kMouseButton_Left;
	else if (button == CL_MOUSE_RIGHT)
		button = Awesomium::MouseButton::kMouseButton_Right;
	else if (button == CL_MOUSE_MIDDLE)
		button = Awesomium::MouseButton::kMouseButton_Middle;

	gActiveWebView->GetWV()->InjectMouseDown((Awesomium::MouseButton)button);
}

void LuaAwesomium::InjectMouseUp(int button)
{
	if (gActiveWebView == NULL) return;

	if (button == CL_MOUSE_LEFT)
		button = Awesomium::MouseButton::kMouseButton_Left;
	else if (button == CL_MOUSE_RIGHT)
		button = Awesomium::MouseButton::kMouseButton_Right;
	else if (button == CL_MOUSE_MIDDLE)
		button = Awesomium::MouseButton::kMouseButton_Middle;

	gActiveWebView->GetWV()->InjectMouseUp((Awesomium::MouseButton)button);
}

void LuaAwesomium::InjectMouseWheel(int scrollV, int scrollH)
{
	if (gActiveWebView == NULL) return;

	gActiveWebView->GetWV()->InjectMouseWheel(scrollV, scrollH);
}

void LuaAwesomium::InjectMouseMove(int x, int y)
{
	if (gActiveWebView == NULL) return;
		
	gActiveWebView->GetWV()->InjectMouseMove(x, y);
}

void LuaAwesomium::InjectKeyboardEvent(const CL_InputEvent& input)
{
	if (!(input.type == CL_InputEvent::Type::pressed
		|| input.type == CL_InputEvent::Type::released))
		return;

	if (gActiveWebView == NULL) return;
		
	Awesomium::WebKeyboardEvent keyboardEvent;
		keyboardEvent.type = (input.type == CL_InputEvent::Type::pressed ? 
			Awesomium::WebKeyboardEvent::Type::kTypeKeyDown :
			Awesomium::WebKeyboardEvent::Type::kTypeKeyUp
		);
		keyboardEvent.virtual_key_code = input.id;
	gActiveWebView->GetWV()->InjectKeyboardEvent(keyboardEvent);
}

void LuaAwesomium::SetActive(bool bActive)
{
	gActiveWebView = this;
}

void LuaAwesomium::SendJS(const char* js)
{
	m_webView->ExecuteJavascript(Awesomium::ToWebString(js), Awesomium::ToWebString(""));
}

void LuaAwesomium::LoadURL(const char* url)
{
	m_webView->LoadURL(Awesomium::WebURL(Awesomium::WSLit(url)));
}

void LuaAwesomium::Update()
{
	Awesomium::WebCore* webCore = Awesomium::WebCore::instance();

	while (m_webView->IsLoading())
	{
		webCore->Update();
	}

	Awesomium::BitmapSurface* surface = (Awesomium::BitmapSurface*)m_webView->surface();

	if (surface != NULL && surface->is_dirty())
	{
		surface->set_is_dirty(false);
		surface->SaveToPNG(Awesomium::ToWebString(m_strID), true);
	}
}

void LuaAwesomium::Draw()
{
	if (g_Files->IsLocal(m_strID.c_str()))
		g_Render->DrawTexture(g_Render->GetTexture(m_strID.c_str()), float(x), float(y), float(m_width), float(m_height), CL_Colorf::white);
}

LuaAwesomium::~LuaAwesomium()
{
	if (gActiveWebView == this)
		gActiveWebView = NULL;

	m_webView->Destroy();
	g_Files->Delete(m_strID.c_str());
}

void Lua::LuaBind(luabind::object& globals)
{
	luabind::module(m_state)
	[
		luabind::class_<Lua>("Lua")
		.def("Print", &Lua::Print)
		.def("DoString", &Lua::DoString)
		.def("Include", &Lua::Include)
	];

	//luabind::module(m_state)
	//[
	//	luabind::class_<LuaAwesomium>("Awesomium")
	//	.def(luabind::constructor<const char*, int, int>())
	//	.def("LoadURL", &LuaAwesomium::LoadURL)
	//	.def("SendJS", &LuaAwesomium::SendJS)
	//	.def("Update", &LuaAwesomium::Update)
	//	.def("SetActive", &LuaAwesomium::SetActive)
	//	.def("Draw", &LuaAwesomium::Draw)
	//	.def_readwrite("x", &LuaAwesomium::x)
	//	.def_readwrite("y", &LuaAwesomium::y)
	//];

	luabind::module(m_state)
	[
		luabind::class_<CL_Gradient>("Gradient")
		.def(luabind::constructor<const CL_Colorf, const CL_Colorf, const CL_Colorf, const CL_Colorf>())
		.def_readwrite("top_left", &CL_Gradient::top_left)
		.def_readwrite("top_right", &CL_Gradient::top_right)
		.def_readwrite("bottom_left", &CL_Gradient::bottom_left)
		.def_readwrite("bottom_right", &CL_Gradient::bottom_right)
	];
	
	luabind::module(m_state)
	[
		luabind::class_<CL_Texture>("Texture")
		.def("GetW", &CL_Texture::get_width)
		.def("GetH", &CL_Texture::get_height)
		.def("GetSize", &CL_Texture::get_size)
	];

	luabind::module(m_state)
	[
		luabind::class_<CL_PixelBuffer>("PixelData")
		.def("GetPixel", &CL_PixelBuffer::get_pixel)
		.def("GetPalette", &CL_PixelBuffer::get_palette)
	];
	
	luabind::module(m_state)
	[
		luabind::class_<LuaMat4f>("Matrix4")
		.def(luabind::constructor<LuaMat4f>())
		.def(luabind::constructor<>())
		.def("Set", &LuaMat4f::Set)
		.def("Get", &LuaMat4f::Get)

	];
	
	luabind::module(m_state)
	[
		luabind::class_<CL_Image>("Image")
		.def("GetW", &CL_Image::get_width)
		.def("GetH", &CL_Image::get_height)
		.def("GetSize", &CL_Image::get_size)
	];

	luabind::module(m_state)
	[
		luabind::class_<CL_Colorf>("Color")
		.def(luabind::constructor<float, float, float, float>())
		.def_readwrite("r", &CL_Colorf::r)
		.def_readwrite("g", &CL_Colorf::g)
		.def_readwrite("b", &CL_Colorf::b)
		.def_readwrite("a", &CL_Colorf::a)
	];

	luabind::module(m_state)
	[
		luabind::class_<CL_Angle>("Angle")
		.def(luabind::constructor<float, CL_AngleUnit>())
		.def(luabind::const_self + luabind::other<CL_Angle>())
		.def(luabind::const_self - luabind::other<CL_Angle>())
		.def(luabind::const_self * luabind::other<CL_Angle>())
		.def(luabind::const_self / luabind::other<CL_Angle>())
		.def(luabind::const_self * float())
		.def(luabind::const_self / float())
		.def(luabind::const_self < luabind::other<CL_Angle>())
		.def(luabind::const_self <= luabind::other<CL_Angle>())
		.def(luabind::const_self == luabind::other<CL_Angle>())
		.def("Degrees", &CL_Angle::to_degrees)
		.def("Radians", &CL_Angle::to_radians)
		.def("Normalize", &CL_Angle::normalize)
	];
	
	globals["BLEND_ZERO"] = cl_blend_zero;
	globals["BLEND_ONE"] =  cl_blend_one;
	globals["BLEND_DST_COLOR"] = cl_blend_dest_color;
	globals["BLEND_SRC_COLOR"] = cl_blend_src_color;
	globals["BLEND_ONE_MINUS_DST_COLOR"] = cl_blend_one_minus_dest_color;
	globals["BLEND_ONE_MINUS_SRC_COLOR"] = cl_blend_one_minus_src_color;
	globals["BLEND_SRC_ALPHA"] = cl_blend_src_alpha;
	globals["BLEND_ONE_MINUS_SRC_ALPHA"] = cl_blend_one_minus_src_alpha;
	globals["BLEND_DST_ALPHA"] = cl_blend_dest_alpha;
	globals["BLEND_ONE_MINUS_DEST_ALPHA"] = cl_blend_one_minus_dest_alpha;
	globals["BLEND_SRC_ALPHA_SATURATE"] = cl_blend_src_alpha_saturate;
	globals["BLEND_CONSTANT_COLOR"] = cl_blend_constant_color;
	globals["BLEND_ONE_MINUS_CONSTANT_COLOR"] = cl_blend_one_minus_constant_color;
	globals["BLEND_CONSTANT_ALPHA"] = cl_blend_constant_alpha;
	globals["BLEND_ONE_MINUS_CONSTANT_ALPHA"] = cl_blend_one_minus_constant_alpha;

	globals["CL_TRIANGLE_FAN"] = cl_triangle_fan;
	globals["CL_TRIANGLE_STRIP"] = cl_triangle_strip;
	globals["CL_TRIANGLES"] = cl_triangles;

	globals["ANGLE_DEGREES"] = cl_degrees;
	globals["ANGLE_RADIANS"] = cl_radians;

	globals["ORIGIN_TOP_LEFT"] = origin_top_left;
	globals["ORIGIN_TOP_CENTER"] = origin_top_center;
	globals["ORIGIN_TOP_RIGHT"] = origin_top_right;
	globals["ORIGIN_CENTER_LEFT"] = origin_center_left;
	globals["ORIGIN_CENTER"] = origin_center;
	globals["ORIGIN_CENTER_RIGHT"] = origin_center_right;
	globals["ORIGIN_BOTTOM_LEFT"] = origin_bottom_left;
	globals["ORIGIN_BOTTOM_CENTER"] = origin_bottom_center;
	globals["ORIGIN_BOTTOM_RIGHT"] = origin_bottom_right;

	globals["CURSOR_ARROW"] = cl_cursor_arrow;
	globals["CURSOR_APP_STARTING"] = cl_cursor_appstarting;
	globals["CURSOR_CROSS"] = cl_cursor_cross;
	globals["CURSOR_HAND"] = cl_cursor_hand;
	globals["CURSOR_IBEAM"] = cl_cursor_ibeam;
	globals["CURSOR_NO"] = cl_cursor_no;
	globals["CURSOR_SIZE_ALL"] = cl_cursor_size_all;
	globals["CURSOR_SIZE_NESW"] = cl_cursor_size_nesw;
	globals["CURSOR_SIZE_NS"] = cl_cursor_size_ns;
	globals["CURSOR_SIZE_NWSE"] = cl_cursor_size_nwse;
	globals["CURSOR_SIZE_WE"] = cl_cursor_size_we;
	globals["CURSOR_UP_ARROW"] = cl_cursor_uparrow;
	globals["CURSOR_WAIT"] = cl_cursor_wait;

	globals["BLEND_REVERSE_SUBTRACT"] = cl_blend_equation_reverse_subtract;
	globals["BLEND_MAX"] = cl_blend_equation_max;
	globals["BLEND_SUBTRACT"] = cl_blend_equation_subtract;
	globals["BLEND_ADD"] = cl_blend_equation_add;
	globals["BLEND_MIN"] = cl_blend_equation_min;

	luabind::module(m_state)
	[
		luabind::class_<CL_Rectf>("Rect")
		.def(luabind::constructor<float, float, float, float>())
		.def("GetW", &CL_Rectf::get_width)
		.def("GetH", &CL_Rectf::get_height)
		.def("GetSize", &CL_Rectf::get_size)
		.def("Contains", &CL_Rectf::contains)
		.def("GetCenter", &CL_Rectf::get_center)
		.def("IsInside", &CL_Rectf::is_inside)
		.def("IsOverlapped", &CL_Rectf::is_overlapped)
		.def_readwrite("x", &CL_Rectf::left)
		.def_readwrite("y", &CL_Rectf::top)
		.def_readwrite("l", &CL_Rectf::left)
		.def_readwrite("t", &CL_Rectf::top)
		.def_readwrite("r", &CL_Rectf::right)
		.def_readwrite("b", &CL_Rectf::bottom)
	];

	luabind::module(m_state)
	[
		luabind::class_<CL_Point>("Point")
		.def(luabind::constructor<int, int>())
		.def_readwrite("x", &CL_Point::x)
		.def_readwrite("y", &CL_Point::y)
	];

	luabind::module(m_state)
	[
		luabind::class_<CL_Sizef>("Size")
		.def(luabind::constructor<float, float>())
		.def_readwrite("w", &CL_Sizef::width)
		.def_readwrite("h", &CL_Sizef::height)
	];

	luabind::module(m_state)
	[
		luabind::class_<CL_Size>("Size")
		.def(luabind::constructor<int, int>())
		.def_readwrite("w", &CL_Size::width)
		.def_readwrite("h", &CL_Size::height)
	];

	luabind::module(m_state)
	[
		luabind::class_<CL_Vec2f>("Vec2")
		.def(luabind::constructor<>())
		.def(luabind::constructor<float, float>())
		.def(luabind::constructor<const CL_Vec2<float>>())
		.def(luabind::const_self == luabind::other<CL_Vec2f>())
		.def(luabind::const_self + luabind::other<CL_Vec2f>())
		.def(luabind::const_self - luabind::other<CL_Vec2f>())
		.def(luabind::const_self * luabind::other<CL_Vec2f>())
		.def(luabind::const_self / luabind::other<CL_Vec2f>())
		.def(luabind::const_self * float())
		.def(luabind::const_self / float())
		.def("Normalize", (CL_Vec2f&(CL_Vec2f::*)(void)) &CL_Vec2f::normalize)
		.def("Distance", &CL_Vec2f::distance)
		.def("Length", &CL_Vec2f::length)
		.def("Rotate", (CL_Vec2f&(CL_Vec2f::*)(const CL_Vec2f&, const CL_Angle&)) &CL_Vec2f::rotate)
		.def("Angle", &CL_Vec2f::angle)
		.def("Dot", (float(CL_Vec2f::*)(const CL_Vec2f&)const) &CL_Vec2f::dot)
		.def_readwrite("x", &CL_Vec2f::x)
		.def_readwrite("y", &CL_Vec2f::y)
	];

	globals["LuaInstance"] = g_Lua;
}