project "Chip8"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++20"
	staticruntime "on"
	targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}")
	
	dependson { "glad", "glfw" }
	
	files { "**.h", "**.cpp" }
	
    includedirs
	{
		"%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glad/include",
    }
	
	libdirs { libout } 
	
	links
	{
		"opengl32",
		"glad",
		"glfw"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"