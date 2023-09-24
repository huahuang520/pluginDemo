#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(GRADIENTER_LIB)
#  define GRADIENTER_EXPORT Q_DECL_EXPORT
# else
#  define GRADIENTER_EXPORT Q_DECL_IMPORT
# endif
#else
# define GRADIENTER_EXPORT
#endif
