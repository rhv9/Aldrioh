#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Core log macros
#define LOG_CORE_TRACE(...)       ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)        ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)        ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)       ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...)    ::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...)            ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)             ::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)             ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)            ::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)         ::Log::GetClientLogger()->critical(__VA_ARGS__)



// If core log macros
#define LOG_CORE_TRACE_IF(x,...)       if(x)::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO_IF(x,...)        if(x)::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN_IF(x,...)        if(x)::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR_IF(x,...)       if(x)::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL_IF(x,...)    if(x)::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE_IF(...)            if(x)::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO_IF(...)             if(x)::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN_IF(...)             if(x)::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR_IF(...)            if(x)::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL_IF(...)         if(x)::Log::GetClientLogger()->critical(__VA_ARGS__)
