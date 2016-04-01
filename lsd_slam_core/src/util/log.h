/*
 * Author: Frank Erens
 */

#pragma once


namespace lsd_slam
{
namespace log
{

void fatal(const char* m, ...);
void error(const char* m, ...);
void warning(const char* m, ...);
void info(const char* m, ...);
void debug(const char* m, ...);
void trace(const char* m, ...);

}
}
