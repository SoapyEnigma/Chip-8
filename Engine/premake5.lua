project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "c++20"
    staticruntime "on"
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}")

    dependson { "glad", "glfw", "miniaudio" }

    files { "**.h", "**.cpp" }

    includedirs
    {
        "%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glad/include",
        "%{wks.location}/Vendor/imgui",
        "%{wks.location}/Vendor/miniaudio",

        "%{wks.location}/Engine",
    }

    links
    {
        "opengl32",
        "glad",
        "glfw",
        "miniaudio"
    }

    vpaths
    {
        ["UI"] = { "UI/**.h", "UI/**.cpp" },
        ["Window"] = { "Window/**.h", "Window/**.cpp" },
        ["Util"] = { "Util/**.h", "Util/**.cpp" },
        ["Audio"] = { "Audio/**.h", "Audio/**.cpp" },
        ["Input"] = { "Input/**.h", "Input/**.cpp" },
        ["Renderer"] = { "Renderer/**.h", "Renderer/**.cpp" },
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"