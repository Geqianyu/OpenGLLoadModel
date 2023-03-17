
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Log.h"

std::shared_ptr<spdlog::logger> Log::m_logger;

void Log::init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_mt("OpenGL load model");
    m_logger->set_level(spdlog::level::trace);
}