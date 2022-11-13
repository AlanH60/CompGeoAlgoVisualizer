workspace "CSE355"
    architecture "x64"
    configurations { "Debug", "Release" }
	startproject "CSE355"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CSE355"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "PCH.h"
    pchsource "CSE355/Source/PCH.cpp"

    files 
    { 
		"%{prj.location}/**.cpp",
        "%{prj.location}/**.h"
    }

    includedirs
    {
        "%{prj.location}/Source"
    }



    links
    {
        "d3d11.lib",
        "d2d1.lib",
		"D3DCompiler.lib",
		"dwrite.lib"
    }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"