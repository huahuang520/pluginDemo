/*************************************************
Author:jason.huang
Date:2023.3.2
Version:0.0.1
Description:LED control
**************************************************/

#ifndef LED_PLUGIN
#define LED_PLUGIN
#include "ledcontrol_global.h"
#include <QtPlugin>
#include <QWidget>

class LEDCONTROL_EXPORT LEDPlugin
{
public:
    virtual ~LEDPlugin() {}
    virtual QWidget* GetWidget() = 0;
};
//Plug-in identifier
#define AbstractInterface_iid "led control plugin"
//Register plug-in interface with Qt meta-object system
Q_DECLARE_INTERFACE(LEDPlugin, AbstractInterface_iid)

#endif
