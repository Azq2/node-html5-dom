// Workaround: hide modest build warnings
// Because, it written in C99 and not fully conformant with C++ standarts.
#pragma GCC diagnostic ignored "-Wold-style-declaration"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
#pragma GCC diagnostic ignored "-Wcpp"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"

#define _DEFAULT_SOURCE 1

#if (defined(_WIN32) || defined(_WIN64))
	#define MyCORE_OS_WINDOWS_NT
#endif
