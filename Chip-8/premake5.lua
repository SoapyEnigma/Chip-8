project "Chip8"
    language "C++"
    cppdialect "c++20"
    staticruntime "on"
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}")

    dependson { "Engine" }

    files { "**.h", "**.cpp" }

    includedirs
    {
        "%{wks.location}",
        "%{wks.location}/Chip-8",
        "%{wks.location}/Vendor/imgui",

        "%{wks.location}/Engine",
    }

    links
    {
        "Engine",
        "imgui",
    }

    vpaths
    {
        ["Chip8"] = { "Chip8/**.h", "Chip8/**.cpp" },
        ["Application"] = { "Application/**.h", "Application/**.cpp" },
        ["UI"] = { "UI/**.h", "UI/**.cpp" },
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
        optimize "Full"
        symbols "Off"
        flags { "LinkTimeOptimization" }
        postbuildcommands { "{COPYDIR} Roms %{cfg.targetdir}/Roms" }

    filter { "configurations:Release", "system:not windows" }
        buildoptions { "-ffunction-sections", "-fdata-sections" }
        linkoptions { "-Wl,--gc-sections" }