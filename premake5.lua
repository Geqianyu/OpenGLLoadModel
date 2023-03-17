workspace "OpenGLloadModel"
    architecture "x64"
    startproject "OpenGLloadModel"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include_dirs = {}
include_dirs["glfw"] = "%{wks.location}/OpenGLLoadModel/vender/glfw/include"
include_dirs["glad"] = "%{wks.location}/OpenGLLoadModel/vender/glad/include"
include_dirs["spdlog"] = "%{wks.location}/OpenGLLoadModel/vender/spdlog/include"
include_dirs["glm"] = "%{wks.location}/OpenGLLoadModel/vender/glm"
include_dirs["assimp"] = "%{wks.location}/OpenGLLoadModel/vender/assimp/include"
include_dirs["assimp_config_headers"] = "%{wks.location}/OpenGLLoadModel/vender/AssimpPremake/config_headers"
include_dirs["stb"] = "%{wks.location}/OpenGLLoadModel/vender/stb"

libdirs
{
    "%{wks.location}/OpenGLLoadModel/vender/AssimpPremake/lib"
}

group "Dependencies"
    include "OpenGLloadModel/vender"
group ""

group "Core"
    include "OpenGLloadModel"
group ""