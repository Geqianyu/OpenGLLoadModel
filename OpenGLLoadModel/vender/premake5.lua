project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"glad/include/glad/glad.h",
		"glad/include/KHR/khrplatform.h",
		"glad/src/glad.c"
	}

	includedirs
	{
		"glad/include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


project "glfw"
    kind "StaticLib"
    language "C"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin_intermediate/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/glfw_config.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",

        "glfw/src/null_init.c",
        "glfw/src/null_joystick.c",
        "glfw/src/null_monitor.c",
        "glfw/src/null_window.c",

        "glfw/src/platform.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c"
    }
        
    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        files
        {
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_module.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
        }

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        