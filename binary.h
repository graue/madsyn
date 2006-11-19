#ifdef _WIN32
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(n) setmode(n, O_BINARY)
#else
# define SET_BINARY_MODE(n) ((void)0)
#endif
