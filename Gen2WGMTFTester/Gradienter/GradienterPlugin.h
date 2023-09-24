/*************************************************
Author:jason.huang
Date:2023.2.13
Version:0.0.1
Description:Level adjustment
**************************************************/

#ifndef GRADIENTER_PLUGIN
#define GRADIENTER_PLUGIN
#include "gradienter_global.h"
#include <QtPlugin>
#include <QWidget>
#include <QLayout>
#include <common/ml.h>

class GRADIENTER_EXPORT GradienterPlugin
{
public:
    virtual ~GradienterPlugin() {}
    virtual QWidget* GetWidget() = 0;

    virtual ML::MLResult Open(std::string name) = 0;
    virtual bool Close() = 0;
    virtual bool isOpen() = 0;

    //signals
    //send Gradienter data to external
    virtual void sendAngleSignal(double xAngle,double yAngle) = 0;
    //send Gradienter status
    virtual void sendStatusSignal(QString) = 0;
};
//Plug-in identifier
#define AbstractInterface_iid "Level adjustment plugin"
//Register plug-in interface with Qt meta-object system
Q_DECLARE_INTERFACE(GradienterPlugin, AbstractInterface_iid)

#endif