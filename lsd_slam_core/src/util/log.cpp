/*
 * Author: Frank Erens
 */

#include "log.h"

#include <cstdio>
#include <cstdlib>

namespace lsd_slam
{
namespace log
{

static int loglevel = 3;

static inline void log(const char* c, const char* p, int l, const char* m)
{
	if(loglevel >= l)
	{
		std::fprintf(stderr, "\033[%sm[%s] %s\033[0m\n", c, p, m);
	}
}

void fatal(const char* m)
{
	log("1;35","XX",1,m);
}

void error(const char* m)
{
	log("1;31","EE",2,m);
}

void warning(const char* m)
{
	log("1;33","!!",3,m);
}

void info(const char* m)
{
	log("36","::",4,m);
}

void debug(const char* m)
{
	log("0","##",5,m);
}

void trace(const char* m)
{
	log("37",">>",6,m);
}

static bool _init_loglevel = [] {
	char* result = std::getenv("LOGLEVEL");
	if(result) {
		loglevel = std::atoi(result);
	}

	return true;
}();

}
}
