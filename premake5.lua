newoption
{
	trigger = "graphics",
	value = "OPENGL_VERSION",
	description = "version of OpenGL to build raylib against",
	allowed = {
		{ "opengl11", "OpenGL 1.1"},
		{ "opengl21", "OpenGL 2.1"},
		{ "opengl33", "OpenGL 3.3"},
		{ "opengl43", "OpenGL 4.3"}
	},
	default = "opengl33"
}

function define_C()
	language "C"
end

function define_Cpp()
	language "C++"
end

function string.starts(String,Start)
	return string.sub(String,1,string.len(Start))==Start
end

function link_to(lib)
	links (lib)
	includedirs ("../"..lib.."/include", "../"..lib.."/" )
end

function download_progress(total, current)
	local ratio = current / total;
	ratio = math.min(math.max(ratio, 0), 1);
	local percent = math.floor(ratio * 100);
	print("Download progress (" .. percent .. "%/100%)")
end

function check_raylib()
	if(os.isdir("raylib") == false and os.isdir("raylib-6.0") == false) then
		if(not os.isfile("raylib-6.0.zip")) then
			print("Raylib not found, downloading from github")
			local result_str, response_code = http.download("https://github.com/raysan5/raylib/archive/refs/tags/6.0.zip", "raylib-6.0.zip", {
				progress = download_progress,
				headers = { "From: Premake", "Referer: Premake" }
			})
		end
		print("Unzipping to " ..  os.getcwd())
		zip.extract("raylib-6.0.zip", os.getcwd())
		os.remove("raylib-6.0.zip")
	end
end

function check_imgui()
	if(os.isdir("imgui") == false and os.isdir("imgui-1.92.7") == false) then
		if(not os.isfile("imgui-1.92.7.zip")) then
			print("imgui not found, downloading from github")
			local result_str, response_code = http.download("https://github.com/ocornut/imgui/archive/refs/tags/v1.92.7.zip", "imgui-1.92.7.zip", {
				progress = download_progress,
				headers = { "From: Premake", "Referer: Premake" }
			})
		end
		print("Unzipping to " ..  os.getcwd())
		zip.extract("imgui-1.92.7.zip", os.getcwd())
		os.remove("imgui-1.92.7.zip")
	end
end

workspace "rlImGui"
	configurations { "Debug", "Release" }
	platforms { "x64"}
	defaultplatform "x64"
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter { "platforms:x64" }
		architecture "x86_64"

	filter { "system:linux" }
		defines { "_GLFW_X11" }
		defines { "_GNU_SOURCE" }
		
	targetdir "bin/%{cfg.buildcfg}/"
	

	cdialect "C99"
	cppdialect "C++17"
	check_raylib()
	check_imgui()

	include ("raylib_premake5.lua")
		
project "rlImGui"
	kind "StaticLib"
	location "build"
	targetdir "bin/%{cfg.buildcfg}"
	language "C++"
	cdialect "C99"
	cppdialect "C++17"
	include_raylib()
	includedirs { "rlImGui", "imgui-1.92.7"}
	vpaths 
	{
		["Header Files"] = { "*.h"},
		["Source Files"] = {"*.cpp"},
		["ImGui Files"] = { "imgui-1.92.7/*.h","imgui-1.92.7/*.cpp" },
	}
	files {"imgui-1.92.7/*.h", "imgui-1.92.7/*.cpp", "*.cpp", "*.h", "extras/**.h"}

	defines {"IMGUI_DISABLE_OBSOLETE_FUNCTIONS","IMGUI_DISABLE_OBSOLETE_KEYIO"}

group "Examples"
project "simple"
	kind "ConsoleApp"
	language "C++"
	cdialect "C99"
	cppdialect "C++17"
	location "build"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "examples/**.h"},
		["Source Files"] = {"examples/**.cpp", "examples/**.c"},
	}
	files {"examples/simple.cpp"}
	link_raylib()
	links {"rlImGui"}
	includedirs {"./", "imgui-1.92.7" }
		
    filter "action:vs*"
		debugdir "$(SolutionDir)"	
	
		
project "editor"
	kind "ConsoleApp"
	language "C++"
	cdialect "C99"
	cppdialect "C++17"
	location "build"
	targetdir "bin/%{cfg.buildcfg}"


	vpaths 
	{
		["Header Files"] = { "examples/**.h"},
		["Source Files"] = {"examples/**.cpp", "examples/**.c"},
	}
	files {"examples/editor.cpp"}
	link_raylib()
	links {"rlImGui"}
	includedirs {"./", "imgui-1.92.7" }

    filter "action:vs*"
		debugdir "$(SolutionDir)"	
        
        
project "imgui_style_example"
	kind "ConsoleApp"
	language "C++"
	cdialect "C99"
	cppdialect "C++17"
	location "build"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "examples/**.h"},
		["Source Files"] = {"examples/**.cpp", "examples/**.c"},
	}
	files {"examples/imgui_style_example.cpp"}
	link_raylib()
	links {"rlImGui"}
	includedirs {"./", "imgui-1.92.7" }
		
    filter "action:vs*"
		debugdir "$(SolutionDir)"
	
project "docking_example"
	kind "ConsoleApp"
	language "C++"
	cdialect "C99"
	cppdialect "C++17"
	location "build"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "examples/**.h"},
		["Source Files"] = {"examples/**.cpp", "examples/**.c"},
	}
	files {"examples/docking_example.cpp"}
	link_raylib()
	links {"rlImGui"}
	includedirs {"./", "imgui-1.92.7" }
		
    filter "action:vs*"
		debugdir "$(SolutionDir)"

project "asset_browser"
	kind "ConsoleApp"
	language "C++"
	cdialect "C99"
	cppdialect "C++17"
	location "build"
	targetdir "bin/%{cfg.buildcfg}"
	
	vpaths 
	{
		["Header Files"] = { "examples/asset_browser/**.h"},
		["Source Files"] = {"examples/asset_browser/**.cpp", "examples/asset_browser/**.c"},
	}
	files {"examples/asset_browser/**.cpp", "examples/asset_browser/**.h"}
	link_raylib()
	links {"rlImGui"}
	includedirs {"./", "examples/asset_browser/", "imgui-1.92.7" }
		
    filter "action:vs*"
		debugdir "$(SolutionDir)"
		