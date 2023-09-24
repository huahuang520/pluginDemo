#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(LEDCONTROL_LIB)
#  define LEDCONTROL_EXPORT Q_DECL_EXPORT
# else
#  define LEDCONTROL_EXPORT Q_DECL_IMPORT
# endif
#else
# define LEDCONTROL_EXPORT
#endif
