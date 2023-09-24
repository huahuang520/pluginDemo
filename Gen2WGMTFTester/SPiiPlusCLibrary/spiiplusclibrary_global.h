#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SPIIPLUSCLIBRARY_LIB)
#  define SPIIPLUSCLIBRARY_EXPORT Q_DECL_EXPORT
# else
#  define SPIIPLUSCLIBRARY_EXPORT Q_DECL_IMPORT
# endif
#else
# define SPIIPLUSCLIBRARY_EXPORT
#endif
