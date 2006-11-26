#define _BSD_SOURCE /* Needed for Linux to give us M_PI etc. */

#include <math.h>

#ifdef __WIN32
#define LACK_ISINFF
#endif

#ifdef LACK_ISINFF
int isinff(float x);
#endif
