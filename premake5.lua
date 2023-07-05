workspace "CompGeoAlgoVisualizer"
    architecture "x64"
    configurations { "Debug", "Release" }
	startproject "CompGeoAlgoVisualizer"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CompGeoAlgoVisualizer"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "PCH.h"
    pchsource "CompGeoAlgoVisualizer/Source/PCH.cpp"

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
        kind "WindowedApp"
        targetdir("")