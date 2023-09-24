#pragma once

#include "mtfmeasure_global.h"
#include <QObject>
#include "MTFMeasurePlugin.h"
#include "MTFMeasureWidget.h"

class MTFMEASURE_EXPORT MTFMeasure : public QObject, public MTFMeasurePlugin
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(MTFMeasurePlugin)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "MTFMeasure.json")
public:
    MTFMeasure(QObject* parent = nullptr);

public:
    QWidget* GetWidget();

private:
    MTFMeasureWidget* m_MTFMeasureWidget;
};
