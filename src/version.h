#ifndef FGDF_VERSION_INCLUDED
#define FGDF_VERSION_INCLUDED

#define MAKE_STR_HELPER(a_str) #a_str
#define MAKE_STR(a_str) MAKE_STR_HELPER(a_str)

#define FGDF_VERSION_MAJOR 1
#define FGDF_VERSION_MINOR 1
#define FGDF_VERSION_PATCH 0
#define FGDF_VERSION_BETA 0
#define FGDF_VERSION_VERSTRING   \
	MAKE_STR(FGDF_VERSION_MAJOR) \
	"." MAKE_STR(FGDF_VERSION_MINOR) "." MAKE_STR(FGDF_VERSION_PATCH) "." MAKE_STR(FGDF_VERSION_BETA)

#endif
