// Removing the __OPTIMIZE__ macro on GCC defined by fmt before
// including stb_image because it causes a compilation error otherwise
// https://stackoverflow.com/questions/70657762/linux-c-compilation-error-emmintrin-h
//#if defined(__GNUC__) && !defined(NDEBUG) && defined(__OPTIMIZE__)
//#undef __OPTIMIZE__
//#endif
//
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//
//#include <stb_image.h>
//#include <stb_image_resize.h>