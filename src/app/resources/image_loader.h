#pragma once

#define STB_IMAGE_IMPLEMENTATION

#ifdef MINIG_PLATFORM_WINDOWS
#  pragma warning (push)
#  pragma warning (disable: 4244)
#endif /* MINIG_PLATFORM_WINDOWS */

#include <stb/stb_image.h>

#ifdef MINIG_PLATFORM_WINDOWS
#  pragma warning (pop)
#endif /* MINIG_PLATFORM_WINDOWS */