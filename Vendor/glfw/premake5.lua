project "glfw"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	targetdir (libout)
	objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}")
	targetname "glfw"
	
	files
	{
		"include/**.h",
		"src/glfw_config.h",
		"src/context.c",
		"src/init.c",
		"src/input.c",
		"src/monitor.c",
		"src/vulkan.c",
		"src/window.c",
		"src/platform.c",
		"src/null_init.c",
		"src/null_monitor.c",
		"src/null_window.c",
		"src/null_joystick.c",
		
		"src/win32_init.c",
		"src/win32_joystick.c",
		"src/win32_monitor.c",
		"src/win32_time.c",
		"src/win32_thread.c",
		"src/win32_window.c",
		"src/win32_module.c",
		"src/wgl_context.c",
		"src/egl_context.c",
		"src/osmesa_context.c"
	}
	
	includedirs
	{
		"include"
	}
	
	defines 
	{ 
		"_GLFW_WIN32",
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
