project "imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}")
    targetname "imgui"
    
    dependson { "glfw" }
    
    files
    {
        "*.h",
        "*.cpp",
        "backends/imgui_impl_glfw.h",
        "backends/imgui_impl_glfw.cpp",
        "backends/imgui_impl_opengl3.h",
        "backends/imgui_impl_opengl3.cpp"
    }

    removefiles
    {
        "imgui_demo.cpp",
        "misc/cpp/*.cpp"
    }
    
    includedirs
    {
        "%{wks.location}/Vendor/imgui",
        "%{wks.location}/Vendor/imgui/backends",
        "%{wks.location}/Vendor/glfw/include",
    }
    
    libdirs { libout }
    
    links
    {
        "glfw"
    }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Full"
        symbols "Off"
        flags { "LinkTimeOptimization" }

    filter { "configurations:Release", "system:not windows" }
        buildoptions { "-ffunction-sections", "-fdata-sections" }
        linkoptions { "-Wl,--gc-sections" }
