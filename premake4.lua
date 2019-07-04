solution "codekiddy"
	configurations { "Debug", "Release" }
	location ("solution/".._ACTION)
	
project "codekiddy"
	kind "WindowedApp"
	language "C++"
	targetdir "build"
	targetname "play"
	files { "source/**.h", "source/**.cpp" }

	includedirs {
		"depends/box2d/include",
		"depends/luabind/include",
		"depends/clanlib/include",
		"depends/luajit/include",
		"depends/awesomium/include"
	}
	
	libdirs {
		"depends/box2d/build",
		"depends/luabind/build",
		"depends/clanlib/lib",
		"depends/luajit/lib",
		"depends/awesomium/lib"
	}
	
	links {
		"box2d",
		"luabind",
		"Lua51",
		"clanApp-static-mt",
		"clanCore-static-mt",
		"clanCSSLayout-static-mt",
		"clanDatabase-static-mt",
		"clanDisplay-static-mt",
		"clanGL1-static-mt",
		"clanGL-static-mt",
		"clanGUI-static-mt",
		"clanMikMod-static-mt",
		"clanNetwork-static-mt",
		"clanRegExp-static-mt",
		"clanSound-static-mt",
		"clanSqlite-static-mt",
		"clanSWRender-static-mt",
		"clanVorbis-static-mt"
	}
	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		
project "box2d"
	kind "StaticLib"
	language "C++"
	targetdir "depends/box2D/build"
	targetname("box2d")
	files {
		"depends/box2d/source/**.*", 
		"depends/box2d/include/**.*"
	}
	includedirs {
		"depends/box2d/include/"
	}
	
project "luabind"
	kind "StaticLib"
	language "C++"
	targetdir "depends/luabind/build"
	targetname("luabind")
	files {
		"depends/luabind/source/**.*", 
		"depends/luabind/include/**.*"
	}
	includedirs {
		"depends/luabind/include/"
	}
	