#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CONFIGURATION_LIB)
#  define CONFIGURATION_EXPORT Q_DECL_EXPORT
# else
#  define CONFIGURATION_EXPORT Q_DECL_IMPORT
# endif
#else
# define CONFIGURATION_EXPORT
#endif
