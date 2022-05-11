workspace "rlImGui"
	configurations { "Debug","Debug.DLL", "Release", "Release.DLL" }
	platforms { "x64"}
	defaultplatform "x64"
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Debug.DLL"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter "configurations:Release.DLL"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter { "platforms:x64" }
		architecture "x86_64"
		
	targetdir "bin/%{cfg.buildcfg}/"
		
project "raylib"
	
	filter "configurations:Debug.DLL OR Release.DLL"
		kind "SharedLib"
		defines {"BUILD_LIBTYPE_SHARED"}
		
	filter "configurations:Debug OR Release"
		kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
		
	filter{}
		
	
	location "build"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src", "raylib/src/external/glfw/include"}
	vpaths 
	{
		["Header Files"] = { "raylib/src/**.h"},
		["Source Files/*"] = {"raylib/src/**.c"},
	}
	files {"raylib/src/*.h", "raylib/src/*.c"}
	
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
		
project "rlImGui"
	kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "build"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src","rlImGui", "imgui"}
	vpaths 
	{
		["Header Files"] = { "*.h"},
		["Source Files"] = {"*.cpp"},
		["ImGui Files"] = { "imgui/*.h","imgui/*.cpp" },
	}
	files {"imgui/*.h", "imgui/*.cpp", "*.cpp", "*.h", "extras/**.h"}


group "Examples"
project "simple"
	kind "ConsoleApp"
	location "examples"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "examples/**.h"},
		["Source Files"] = {"examples/**.cpp", "examples/**.c"},
	}
	files {"examples/simple.cpp"}

	links {"raylib","rlImGui"}
	
	includedirs {"raylib/src", "./", "imgui" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
		
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
		
	filter{}
		
project "editor"
	kind "ConsoleApp"
	location "examples"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "examples/**.h"},
		["Source Files"] = {"examples/**.cpp", "examples/**.c"},
	}
	files {"examples/editor.cpp"}

	links {"raylib","rlImGui"}
	
	includedirs {"raylib/src", "./", "imgui" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
		
	filter{}
	