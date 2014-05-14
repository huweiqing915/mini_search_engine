/*************************************************************************
	> File Name: Log.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 15时56分05秒
 ************************************************************************/

#ifndef LOG_H_
#define LOG_H_

#include <fstream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#define FILE_BASENAME strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define LogDebug(fmt, ...) Log::LoggingF(LL_DEBUG, FILE_BASENAME, __LINE__, fmt, ## __VA_ARGS__)
#define LogInfo(fmt, ...) Log::LoggingF(LL_INFO, FILE_BASENAME, __LINE__, fmt, ## __VA_ARGS__)
#define LogWarn(fmt, ...) Log::LoggingF(LL_WARN, FILE_BASENAME, __LINE__, fmt, ## __VA_ARGS__)
#define LogError(fmt, ...) Log::LoggingF(LL_ERROR, FILE_BASENAME, __LINE__, fmt, ## __VA_ARGS__)
#define LogFatal(fmt, ...) Log::LoggingF(LL_FATAL, FILE_BASENAME, __LINE__, fmt, ## __VA_ARGS__)

enum {LL_DEBUG = 0, LL_INFO = 1, LL_WARN = 2, LL_ERROR = 3, LL_FATAL = 4, LEVEL_ARRAY_SIZE = 5, CSTR_BUFFER_SIZE = 32};
static const char * LOG_LEVEL_ARRAY[LEVEL_ARRAY_SIZE]= {"DEBUG","INFO","WARN","ERROR","FATAL"};
static const char * LOG_FORMAT = "%s %s:%d %s %s\n";
static const char * LOG_TIME_FORMAT = "%Y-%m-%d %H:%M:%S";

class Log {
public:
	static void Logging(size_t level, const std::string& msg, const char* fileName, int lineno);
	static void LoggingF(size_t level, const char* fileName, int lineno, const std::string &fmt, ...);
};
#endif

