project "ImGui"
    location ("imgui")
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("imgui/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("imgui/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_internal.h",
		"imgui/imgui_widgets.cpp",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",
		"imgui/imgui_demo.cpp",
        "imgui/imgui_tables.cpp",

		"imgui/backends/imgui_impl_opengl3.cpp",
		"imgui/backends/imgui_impl_glfw.cpp",
	}

	includedirs
    {
		"imgui",
		"GLFW/include",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
