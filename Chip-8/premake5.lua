project "Chip8"
	language "C++"
	cppdialect "c++20"
	staticruntime "on"
	targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}")
	
	dependson { "glad", "glfw", "imgui", "miniaudio" }
	
	files { "**.h", "**.cpp" }
	
    includedirs
	{
		"%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glad/include",
		"%{wks.location}/Vendor/imgui",
		"%{wks.location}/Vendor/miniaudio",
		
		"Chip8",
		"Entry",
		"UI",
		"Util",
		"Window",
		"Audio"
    }
	
	libdirs { libout } 
	
	links
	{
		"opengl32",
		"glad",
		"glfw",
		"imgui",
		"miniaudio"
	}
	
	vpaths
	{
		["Chip8"] = { "Chip8/**.h", "Chip8/**.cpp" },
		["UI"] = { "UI/**.h", "UI/**.cpp" },
		["Window"] = { "Window/**.h", "Window/**.cpp" },
		["Util"] = { "Util/**.h", "Util/**.cpp" },
		["Entry"] = { "Entry/**.h", "Entry/**.cpp" },
		["Audio"] = { "Audio/**.h", "Audio/**.cpp" }
	}
	
	filter "configurations:Debug"
		kind "ConsoleApp"
		defines { "DEBUG" }
		symbols "On"
		postbuildcommands { "{COPYDIR} Roms %{cfg.targetdir}/Roms" }

	filter "configurations:Release"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"
		defines { "NDEBUG" }
		optimize "On"
		postbuildcommands { "{COPYDIR} Roms %{cfg.targetdir}/Roms" }