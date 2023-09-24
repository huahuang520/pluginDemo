#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MTFMEASURE_LIB)
#  define MTFMEASURE_EXPORT Q_DECL_EXPORT
# else
#  define MTFMEASURE_EXPORT Q_DECL_IMPORT
# endif
#else
# define MTFMEASURE_EXPORT
#endif
