libout = "%{wks.location}/lib/%{cfg.buildcfg}"
	
workspace "Chip8"
	architecture "x86_64"
	configurations { "Debug", "Release" }
	
	group "Libs"
		include "Vendor/glfw/premake5.lua"
		include "Vendor/glad/premake5.lua"
		include "Vendor/imgui/premake5.lua"
	
	group "Chip-8"
		include "Chip-8/premake5.lua"