#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(TIPTILT_LIB)
#  define TIPTILT_EXPORT Q_DECL_EXPORT
# else
#  define TIPTILT_EXPORT Q_DECL_IMPORT
# endif
#else
# define TIPTILT_EXPORT
#endif
