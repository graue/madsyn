#include <sys/types.h>

#ifdef __WIN32
#include <stdint.h>
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
/* stdint.h already defines int16_t and int32_t. */
#endif

/*
 * The following definitions supply the swap16, swap32, htobe{16,32},
 * htole{16,32}, letoh{16,32} and betoh{16,32} macros if they are
 * missing.
 *
 * If it has no information, it assumes the host is little endian. To
 * correct this, you can define WORDS_BIGENDIAN to 1 before including
 * this file.
 *
 * These macros may evaluate their arguments multiple times; avoid
 * using arguments with side effects.
 */

#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#endif

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif

#ifndef BYTE_ORDER
#if defined (WORDS_BIGENDIAN)
#define BYTE_ORDER BIG_ENDIAN
#else
#define BYTE_ORDER LITTLE_ENDIAN
#endif
#endif

#ifndef swap16
#define swap16(x) \
	((((u_int16_t)x) >> 8) & 0xff) | ((((u_int16_t)x) & 0xff) << 8))
#endif

#ifndef swap32
#define swap32(x) \
	(((((u_int32_t)x) & 0xff000000) >> 24) | \
	 ((((u_int32_t)x) & 0x00ff0000) >>  8) | \
	 ((((u_int32_t)x) & 0x0000ff00) <<  8) | \
	 ((((u_int32_t)x) & 0x000000ff) << 24))
#endif

#ifndef htobe16 /* If one isn't defined, assume that none are. */

#if BYTE_ORDER == BIG_ENDIAN
#define htobe16(x) (x)
#define htobe32(x) (x)
#define betoh16(x) (x)
#define betoh32(x) (x)
#define htole16 swap16
#define letoh16 swap16
#define htole32 swap32
#define letoh32 swap32
#else
#define htobe16 htons
#define htobe32 htonl
#define betoh16 ntohs
#define betoh32 ntohl
#define htole16(x) (x)
#define letoh16(x) (x)
#define htole32(x) (x)
#define letoh32(x) (x)
#endif

#endif
