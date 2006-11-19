#ifdef __WIN32
typedef unsigned short u_int16_t;
typedef unsigned long u_int32_t;
typedef short int16_t;
typedef long int32_t;
#define letoh16(x) (x) /* little endian only */
#define htole16(x) (x) /* little endian only */
#define letoh32(x) (x) /* little endian only */
#define htole32(x) (x) /* little endian only */
#else
#include <sys/types.h>
#endif
