#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COLLIMATOR_LIB)
#  define COLLIMATOR_EXPORT Q_DECL_EXPORT
# else
#  define COLLIMATOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define COLLIMATOR_EXPORT
#endif
