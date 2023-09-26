workspace "gmp"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Eigen"] = "Application/vendor/eigen"
IncludeDir["stb"] = "Application/vendor/stb"
IncludeDir["imgui"] = "Application/vendor/imgui"
IncludeDir["ImGuizmo"] = "Application/vendor/ImGuizmo"
IncludeDir["libigl"] = "Application/vendor/libigl/include"
IncludeDir["glad"] = "Application/vendor/glad/include"
IncludeDir["GLFW"] = "Application/vendor/glfw/include"

include "Application/vendor/glad"
include "Application/vendor/glfw"
include "Application/vendor/libigl"
include "Application/vendor/imgui"
include "Application/vendor/ImGuizmo"

project "Application"
    location "Application"
    kind "ConsoleApp"
    language "C++"
    characterset "MBCS"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.h"
    }

    includedirs 
    {
        "%{IncludeDir.glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Eigen}",
        "%{IncludeDir.libigl}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.ImGuizmo}",
    }

    links
    {
        "glad",
        "GLFW",
        "libigl",
        "imgui",
        "ImGuizmo",
    }

    defines
    {
        "IGL_STATIC_LIBRARY",
        "WIN32",
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        staticruntime "On"

    filter { "system:windows", "action:vs*" }        
        --buildoptions {"/bigobj"}

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"