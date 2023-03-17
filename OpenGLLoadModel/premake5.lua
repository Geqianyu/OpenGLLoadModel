project "OpenGLLoadModel"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin_intermediate/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vender/glm/glm/**.hpp",
        "vender/glm/glm/**.inl"
    }

    includedirs
    {
        "%{wks.location}/OpenGLLoadModel/src",
        "%{include_dirs.spdlog}",
        "%{include_dirs.glfw}",
        "%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.assimp}",
        "%{include_dirs.assimp_config_headers}",
        "%{include_dirs.stb}"
    }

    links
    {
        "glfw",
        "Glad",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "GQY_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "OPENGL_LOAD_MODEL_DEBUG"
        runtime "Debug"
        symbols "on"
        links
        {
            "assimp-vc143-mtd.lib",
        }
        postbuildcommands
        {
            "{COPY} %{wks.location}/OpenGLLoadModel/vender/AssimpPremake/dll/assimp-vc143-mtd.dll %{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
        }

    filter "configurations:Release"
        defines "OPENGL_LOAD_MODEL_RELEASE"
        runtime "Release"
        optimize "on"
        links
        {
            "assimp-vc143-mt.lib"
        }
        postbuildcommands
        {
            "{COPY} %{wks.location}/OpenGLLoadModel/vender/AssimpPremake/dll/assimp-vc143-mt.dll %{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
        }
