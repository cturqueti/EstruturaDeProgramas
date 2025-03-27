#pragma once

#define LOG_DEBUG(format, ...) Utils::log(LogLevel::DEBUG_ALL, "DEBUG", format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Utils::log(LogLevel::INFO_AND_ABOVE, "INFO", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) Utils::log(LogLevel::WARNING_AND_ABOVE, "WARN", format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Utils::log(LogLevel::ERROR_ONLY, "ERROR", format, ##__VA_ARGS__)