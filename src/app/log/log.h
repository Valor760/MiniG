#pragma once

namespace Log
{
namespace Consts
{
	const int LEVEL_ERROR  = 1;
	const int LEVEL_INFO   = 2;
	const int LEVEL_DEBUG  = 3;
} /* namespace Consts */

void write_log(int level, const char* fmt, ...);
} /* namespace Log */

#define LOG_DEBUG(fmt, ...) do { Log::write_log(Log::Consts::LEVEL_DEBUG, "%s - " fmt, __FUNCTION__, ##__VA_ARGS__); } while(false)
#define LOG_ERROR(fmt, ...) do { Log::write_log(Log::Consts::LEVEL_ERROR, "%s - " fmt, __FUNCTION__, ##__VA_ARGS__); } while(false)
#define LOG_INFO(fmt, ...) do { Log::write_log(Log::Consts::LEVEL_INFO, "%s - " fmt, __FUNCTION__, ##__VA_ARGS__); } while(false)