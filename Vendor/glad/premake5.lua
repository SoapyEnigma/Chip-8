project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	targetdir (libout)
	objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}")
	targetname "glad"
	
	files
	{
		"include/**.h",
		"src/**.c"
	}
	
	includedirs
	{
		"include"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"