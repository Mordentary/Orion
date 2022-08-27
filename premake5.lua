workspace "Orion"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "Orion/vendor/GLFW/include"
IncludeDir["Glad"] = "Orion/vendor/Glad/include"
IncludeDir["ImGui"] = "Orion/vendor/imgui"
IncludeDir["assimp"] = "Orion/vendor/assimp/include"
IncludeDir["glm"] = "Orion/vendor/glm"
IncludeDir["stb_image"] = "Orion/vendor/stb_image"



group "Dependencies"
	include "Orion/vendor/GLFW"
	include "Orion/vendor/Glad"
	include "Orion/vendor/imgui"

group ""

project "Orion"
    location "Orion"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	uuid "57943020-8A99-EAB6-271F-61E0F7F6B73B"


	targetdir("bin/" .. outputdir ..  "/%{prj.name}")
	objdir("bin-int/" .. outputdir ..  "/%{prj.name}")

	pchheader "oripch.h"
	pchsource "Orion/src/oripch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/Platform/**.glsl",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	
	defines
	{	
		"_CRT_SECURE_NO_WARNINGS" 
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.glm}"
	}
	
	libdirs { "%{prj.name}/vendor/assimp/lib" }
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		--Assimp
		"zlibstaticd.lib",
		"assimp-vc143-mtd.lib"
	}
	filter "system:windows"
		systemversion "latest"


		defines 
		{
			"ORI_PLATFORM_WINDOWS",
			"ORI_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		
		}

	filter "configurations:Debug"
		defines "ORI_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "ORI_REALESE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ORI_DIST"
		runtime "Release"
		optimize "on"




project "Sandbox"
	location"Sandbox"
	kind"ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir ..  "/%{prj.name}")
	objdir("bin-int/" .. outputdir ..  "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/**.glsl"
	}

	includedirs
	{
		"Orion/vendor/spdlog/include",
		"Orion/src",
		"Orion/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Orion"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "ORI_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ORI_REALESE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ORI_DIST"
		runtime "Release"
		optimize "on"
