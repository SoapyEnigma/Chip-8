project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}")
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