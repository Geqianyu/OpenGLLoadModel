
#ifndef GQY_LOG_H
#define GQY_LOG_H

#include <memory>
#include <spdlog/spdlog.h>

class Log
{
public:
    Log() = default;
    ~Log() = default;

    static void init();

    inline static std::shared_ptr<spdlog::logger>& get_logger()
    {
        return m_logger;
    }

private:
    static std::shared_ptr<spdlog::logger> m_logger;
};

#define GQY_OPENGL_LOAD_MODEL_ERROR(...)     ::Log::get_logger()->error(__VA_ARGS__)
#define GQY_OPENGL_LOAD_MODEL_WARN(...)      ::Log::get_logger()->warn(__VA_ARGS__)
#define GQY_OPENGL_LOAD_MODEL_INFO(...)      ::Log::get_logger()->info(__VA_ARGS__)
#define GQY_OPENGL_LOAD_MODEL_TRACE(...)     ::Log::get_logger()->trace(__VA_ARGS__)
#define GQY_OPENGL_LOAD_MODEL_FATAL(...)     ::Log::get_logger()->fatal(__VA_ARGS__)

#endif // !GQY_LOG_H
