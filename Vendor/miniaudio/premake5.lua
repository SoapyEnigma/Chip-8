project "miniaudio"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	targetdir (libout)
	objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}")
	targetname "miniaudio"
	
	files
	{
		"*.h",
		"*.c",
	}
	
	includedirs
	{
		"%{wks.location}/Vendor/miniaudio",
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
