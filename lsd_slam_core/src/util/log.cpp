/*
 * Author: Frank Erens
 */

#include "log.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace lsd_slam
{
namespace log
{

static int loglevel = 3;
static int logfilter = true;


static int log_enabled(int l)
{
	return loglevel >= l && logfilter;
}

static inline void log(const char* c, const char* p, int l, const char* m,
		va_list ap)
{
#define BUFFER_SIZE 1024
	static char buf[BUFFER_SIZE];

	if(log_enabled(l))
	{
		std::vsnprintf(buf, BUFFER_SIZE, m, ap);
		std::fprintf(stderr, "\033[%sm[%s] <lsd_slam> %s\033[0m", c, p, buf);
	}
}

void fatal(const char* m, ...)
{
	va_list args;
	va_start(args, m);
	log("1;35","XX",1,m,args);
	va_end(args);

}

void error(const char* m, ...)
{
	va_list args;
	va_start(args, m);
	log("1;31","EE",2,m,args);
	va_end(args);
}

void warning(const char* m, ...)
{
	va_list args;
	va_start(args, m);
	log("1;33","!!",3,m,args);
	va_end(args);
}

void info(const char* m, ...)
{
	va_list args;
	va_start(args, m);
	log("36","::",4,m,args);
	va_end(args);
}

void debug(const char* m, ...)
{
	va_list args;
	va_start(args, m);
	log("0","##",5,m,args);
	va_end(args);
}

void trace(const char* m, ...)
{
	va_list args;
	va_start(args, m);
	log("37",">>",6,m,args);
	va_end(args);
}

static bool _init_loglevel = [] {
	char* result = std::getenv("LOGLEVEL");
	if(result) {
		loglevel = std::atoi(result);
	}

	result = std::getenv("LOGFILTER");
	if(result && std::strstr(result, "-lsd_slam")) {
		logfilter = false;
	}

	return true;
}();

}
}
