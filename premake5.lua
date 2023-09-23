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
IncludeDir["libigl"] = "Application/vendor/libigl/include"
IncludeDir["glad"] = "Application/vendor/glad/include"
IncludeDir["GLFW"] = "Application/vendor/glfw/include"

include "Application/vendor/glad"
include "Application/vendor/glfw"

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
        "%{IncludeDir.libigl}"
    }

    links
    {
        "glad",
        "GLFW"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        staticruntime "On"

    filter { "system:windows", "action:vs*" }        
        buildoptions {"/bigobj"}

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"