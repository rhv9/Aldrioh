project "GLFW"
    location ("GLFW")
	kind "StaticLib"
	language "C"
	staticruntime "on"
	warnings "off"

	targetdir ("GLFW/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("GLFW/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"GLFW/include/GLFW/glfw3.h",
		"GLFW/include/GLFW/glfw3native.h",
		"GLFW/src/glfw_config.h",
		"GLFW/src/context.c",
		"GLFW/src/init.c",
		"GLFW/src/input.c",
		"GLFW/src/monitor.c",

		"GLFW/src/null_init.c",
		"GLFW/src/null_joystick.c",
		"GLFW/src/null_monitor.c",
		"GLFW/src/null_window.c",

		"GLFW/src/platform.c",
		"GLFW/src/vulkan.c",
		"GLFW/src/window.c",
	}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"GLFW/src/win32_init.c",
			"GLFW/src/win32_joystick.c",
			"GLFW/src/win32_module.c",
			"GLFW/src/win32_monitor.c",
			"GLFW/src/win32_time.c",
			"GLFW/src/win32_thread.c",
			"GLFW/src/win32_window.c",
			"GLFW/src/wgl_context.c",
			"GLFW/src/egl_context.c",
			"GLFW/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "system:linux"
		
		defines
		{
			"_GLFW_WAYLAND",
			"_GLFW_X11",
			"_CRT_SECURE_NO_WARNINGSs",

		}

		-- Generate wayland protocol header files. I should find a better way to do this but it works for now.
		prebuildcommands
		{
			"pwd",
			"mkdir -p wl-includes",
			"test ! -e wl-includes/wayland-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/wayland.xml wl-includes/wayland-client-protocol.h || true",
			"test ! -e wl-includes/wayland-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/wayland.xml wl-includes/wayland-client-protocol-code.h || true",
			"test ! -e wl-includes/viewporter-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/viewporter.xml wl-includes/viewporter-client-protocol.h || true",
			"test ! -e wl-includes/viewporter-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/viewporter.xml wl-includes/viewporter-client-protocol-code.h || true",
			"test ! -e wl-includes/xdg-shell-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/xdg-shell.xml wl-includes/xdg-shell-client-protocol.h || true",
			"test ! -e wl-includes/xdg-shell-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/xdg-shell.xml wl-includes/xdg-shell-client-protocol-code.h || true",
			"test ! -e wl-includes/idle-inhibit-unstable-v1-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/idle-inhibit-unstable-v1.xml wl-includes/idle-inhibit-unstable-v1-client-protocol.h || true",
			"test ! -e wl-includes/idle-inhibit-unstable-v1-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/idle-inhibit-unstable-v1.xml wl-includes/idle-inhibit-unstable-v1-client-protocol-code.h || true",
			"test ! -e wl-includes/pointer-constraints-unstable-v1-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/pointer-constraints-unstable-v1.xml wl-includes/pointer-constraints-unstable-v1-client-protocol.h || true",
			"test ! -e wl-includes/pointer-constraints-unstable-v1-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/pointer-constraints-unstable-v1.xml wl-includes/pointer-constraints-unstable-v1-client-protocol-code.h || true",
			"test ! -e wl-includes/relative-pointer-unstable-v1-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/relative-pointer-unstable-v1.xml wl-includes/relative-pointer-unstable-v1-client-protocol.h || true",
			"test ! -e wl-includes/relative-pointer-unstable-v1-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/relative-pointer-unstable-v1.xml wl-includes/relative-pointer-unstable-v1-client-protocol-code.h || true",
			"test ! -e wl-includes/fractional-scale-v1-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/fractional-scale-v1.xml wl-includes/fractional-scale-v1-client-protocol.h || true",
			"test ! -e wl-includes/fractional-scale-v1-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/fractional-scale-v1.xml wl-includes/fractional-scale-v1-client-protocol-code.h || true",
			"test ! -e wl-includes/xdg-activation-v1-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/xdg-activation-v1.xml wl-includes/xdg-activation-v1-client-protocol.h || true",
			"test ! -e wl-includes/xdg-activation-v1-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/xdg-activation-v1.xml wl-includes/xdg-activation-v1-client-protocol-code.h || true",
			"test ! -e wl-includes/xdg-decoration-unstable-v1-client-protocol.h && /usr/bin/wayland-scanner client-header ./deps/wayland/xdg-decoration-unstable-v1.xml wl-includes/xdg-decoration-unstable-v1-client-protocol.h || true",
			"test ! -e wl-includes/xdg-decoration-unstable-v1-client-protocol-code.h && /usr/bin/wayland-scanner private-code  ./deps/wayland/xdg-decoration-unstable-v1.xml wl-includes/xdg-decoration-unstable-v1-client-protocol-code.h || true",
		}

		includedirs 
		{
			"GLFW/wl-includes",
		}

		files 
		{
			"GLFW/src/x11_init.c",
			"GLFW/src/x11_monitor.c",
			"GLFW/src/x11_window.c",

			"GLFW/src/wl_init.c",
			"GLFW/src/wl_monitor.c",
			"GLFW/src/wl_window.c",

			"GLFW/src/egl_context.c",
			"GLFW/src/osmesa_context.c",

			"GLFW/src/posix_module.c",
			"GLFW/src/posix_time.c",
			"GLFW/src/posix_thread.c",
			"GLFW/src/posix_poll.c",
			"GLFW/src/xkb_unicode.c",
			"GLFW/src/glx_context.c",
			"GLFW/src/linux_joystick.c",
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"