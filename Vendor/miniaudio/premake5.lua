project "miniaudio"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}")
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
