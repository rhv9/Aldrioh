workspace "Aldrioh"
    architecture "x86_64"
    startproject "Aldrioh"

    configurations
    {
        "Debug",
        "Release",
    }

    flags
    {
        "MultiProcessorCompile"
    }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
projectName = "Aldrioh"

IncludeDir = {}
IncludeDir["GLFW"]          = projectName .. "/vendor/GLFW/include"
IncludeDir["Spdlog"]        = projectName .. "/vendor/spdlog/include"
IncludeDir["Glad"]          = projectName .. "/vendor/Glad/include"
IncludeDir["ImGui"]         = projectName .. "/vendor/imgui/"
IncludeDir["Glm"]           = projectName .. "/vendor/glm"
IncludeDir["Emscripten"]    = projectName .. "/vendor/emscripten/system/include/emscripten"
IncludeDir["Stb_image"]     = projectName .. "/vendor/stb_image"
IncludeDir["Entt"]          = projectName .. "/vendor/entt/include"
IncludeDir["Miniaudio"]     = projectName .. "/vendor/miniaudio"
IncludeDir["Simdjson"]     = projectName .. "/vendor/simdjson/singleheader"


include (projectName .. "/vendor/glfw_premake5.lua")
include (projectName .. "/vendor/Glad")
include (projectName .. "/vendor/imgui_premake5.lua")
include (projectName .. "/vendor/simdjson_premake5.lua")

project "Aldrioh"
    filter "system:windows"
	    location "%{prj.name}"

    filter {}

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/glm/**.hpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",

        "%{prj.name}/todo.txt",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Spdlog}",
        "%{IncludeDir.Emscripten}",
        "%{IncludeDir.Glm}",
        "%{IncludeDir.Stb_image}",
        "%{IncludeDir.Entt}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Miniaudio}",
        "%{IncludeDir.Simdjson}",

        "%{prj.name}/include",
    }

    links 
    {
        "GLFW",
        "Glad",
        "ImGui",
        "Simdjson",
    }

    defines { "FMT_UNICODE=0" }

    filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_WINDOWS", "GLM_ENABLE_EXPERIMENTAL", "GLFW_INCLUDE_NONE" }

    filter "configurations:Debug"
        runtime "Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        symbols "on"


project "DesktopIcons"
    filter "system:windows"
	    location "%{prj.name}"

    filter {}

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/glm/**.hpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Spdlog}",
        "%{IncludeDir.Emscripten}",
        "%{IncludeDir.Glm}",
        "%{IncludeDir.Stb_image}",
        "%{IncludeDir.Entt}",
        "%{IncludeDir.ImGui}",

        "%{prj.name}/include",
    }

    links 
    {
        "GLFW",
        "Glad",
        "ImGui",
    }

    defines { "FMT_UNICODE=0" }

    filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_WINDOWS", "GLM_ENABLE_EXPERIMENTAL" }

    filter "configurations:Debug"
        runtime "Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        symbols "on"

