#include "log.h"
#include "app/common.h"

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#ifdef MINIG_PLATFORM_WINDOWS
#include <Windows.h>
#endif /* MINIG_PLATFORM_WINDOWS */

namespace Log
{
#ifdef MINIG_PLATFORM_WINDOWS
	const auto COLOR_ERR   = FOREGROUND_RED;
	const auto COLOR_INFO  = FOREGROUND_GREEN;
	#define WIN32_SET_COLOR(hConsole, color) SetConsoleTextAttribute(hConsole, color);
#else
	#ifndef DISABLE_COLOR_LOG
		#define COLOR_ERR    "\033[31m"
		#define COLOR_INFO   "\033[32m"
		#define COLOR_DEBUG  "\033[34m"
		#define COLOR_STOP   "\033[0m"
	#else
		#define COLOR_ERR    ""
		#define COLOR_INFO   ""
		#define COLOR_DEBUG  ""
		#define COLOR_STOP   ""
	#endif /* DISABLE_COLOR_LOG */
#endif /* MINIG_PLATFORM_WINDOWS */

void write_log(int level, const char* fmt, ...)
{
	if(!fmt)
	{
		return;
	}

	/* Add time as prefix */
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
	std::string log_str = "[" + ss.str() + "]";

	/* Set coloring */
#ifdef MINIG_PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch(level)
	{
		case Consts::LEVEL_ERROR:
		{
			WIN32_SET_COLOR(hConsole, COLOR_ERR);
			log_str.append("[ERROR]: ");
			break;
		}
		case Consts::LEVEL_INFO:
		{
			WIN32_SET_COLOR(hConsole, COLOR_INFO);
			log_str.append("[INFO]: ");
			break;
		}
		case Consts::LEVEL_DEBUG:
		{
			log_str.append("[DEBUG]: ");
			break;
		}
	}
#else
	switch(level)
	{
		case Consts::LEVEL_ERROR:
		{
			log_str.append("[" COLOR_ERR "ERROR" COLOR_STOP "]: ");
			break;
		}
		case Consts::LEVEL_INFO:
		{
			log_str.append("[" COLOR_INFO "INFO" COLOR_STOP "]: ");
			break;
		}
		case Consts::LEVEL_DEBUG:
		{
			log_str.append("[" COLOR_DEBUG "DEBUG" COLOR_STOP "]: ");
			break;
		}
	}
#endif /* MINIG_PLATFORM_WINDOWS */

	va_list args;
	va_start(args, fmt);

	/* TODO: Write to a file, but on linux all colors should be ommitted */
	log_str += std::string(fmt) + "\n";
	vprintf(log_str.c_str(), args);

	va_end(args);
}

#ifdef MINIG_PLATFORM_WINDOWS
	#undef WIN32_SET_COLOR
#else
	#undef COLOR_ERR
	#undef COLOR_INFO
	#undef COLOR_DEBUG
	#undef COLOR_STOP
#endif /* MINIG_PLATFORM_WINDOWS */
} /* namespace Log */