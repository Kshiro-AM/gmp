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

project "Application"
    location "Application"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.h",
    }

    includedirs 
    {
        IncludeDir["Eigen"],
        IncludeDir["libigl"]
    }

    links
    {
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
        }

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"