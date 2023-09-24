#ifndef MTFMEASURE_PLUGIN
#define MTFMEASURE_PLUGIN
#include "mtfmeasure_global.h"
#include <QtPlugin>
#include <QWidget>

class MTFMEASURE_EXPORT MTFMeasurePlugin
{
public:
    virtual ~MTFMeasurePlugin() {}

    virtual QWidget* GetWidget() = 0;
};
//Plug-in identifier
#define AbstractInterface_iid "mtf measure plugin"
//Register plug-in interface with Qt meta-object system
Q_DECLARE_INTERFACE(MTFMeasurePlugin, AbstractInterface_iid)


#endif