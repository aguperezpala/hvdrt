/*
*  Useful functions used in debug
*
*  Created on: 18/11/2010
*      Author: agustin
*/

#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

/**
 * Define the different colors prints
 */
#define DEBUG_BLACK 	"\33[22;30m"
#define DEBUG_GRAY 		"\33[01;30m"
#define DEBUG_RED 		"\33[22;31m"
#define DEBUG_LRED	 	"\33[01;21m"
#define DEBUG_GREEN 	"\33[22;32m"
#define DEBUG_LGREEN 	"\33[01;32m"
#define DEBUG_BLUE 		"\33[22;34m"
#define DEBUG_LBLUE 	"\33[01;34m"
#define DEBUG_BROWN 	"\33[22;33m"
#define DEBUG_YELLOW 	"\33[01;33m"
#define DEBUG_CYAN 		"\33[22;36m"
#define DEBUG_LCYAN	 	"\33[22;36m"
#define DEBUG_MAGENTA 	"\33[22;35m"
#define DEBUG_LMAGENTA 	"\33[01;35m"
#define DEBUG_NC 		"\33[0m"
#define DEBUG_BOLD	 	"\33[1m"
#define DEBUG_ULINE	 	"\33[4m" //underline
#define DEBUG_BLINK	 	"\33[5m"
#define DEBUG_INVERT	"\33[7m"


#ifdef DEBUG
	#include <assert.h>
#include <stdio.h>

	#define ASSERT(x)	assert(x);
	#define debug(format, ...) {fprintf(stderr, "\33[0m DEBUG[%s, %s, %d]: ", \
					 __FILE__, __FUNCTION__, __LINE__); \
					fprintf(stderr, format , ## __VA_ARGS__);}

	#define debugRED(format, ...) {fprintf(stderr, DEBUG_RED " DEBUG[%s, %s, %d]: ", \
					 __FILE__, __FUNCTION__, __LINE__); \
					fprintf(stderr, format "\33[0m", ## __VA_ARGS__);}
	#define debugGREEN(format, ...) {fprintf(stderr, DEBUG_GREEN " DEBUG[%s, %s, %d]: ", \
					 __FILE__, __FUNCTION__, __LINE__); \
					fprintf(stderr, format "\33[0m", ## __VA_ARGS__);}
	#define debugBLUE(format, ...) {fprintf(stderr, DEBUG_BLUE " DEBUG[%s, %s, %d]: ", \
					 __FILE__, __FUNCTION__, __LINE__); \
					fprintf(stderr, format "\33[0m", ## __VA_ARGS__);}
	#define debugColor(color, format, ...) {fprintf(stderr, color " DEBUG[%s, %s, %d]: ", \
					 __FILE__, __FUNCTION__, __LINE__); \
					fprintf(stderr, format "\33[0m", ## __VA_ARGS__);}

#else
	#define ASSERT(x)
	#define debug(format,...)

#endif


#endif /* DEBUGUTIL_H_ */
