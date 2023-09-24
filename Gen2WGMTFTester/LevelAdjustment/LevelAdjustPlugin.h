/*************************************************
Author:jason.huang
Date:2023.2.13
Version:0.0.1
Description:Level adjustment
**************************************************/

#ifndef LEVEL_ADJUST_PLUGIN
#define LEVEL_ADJUST_PLUGIN
#include "leveladjustment_global.h"
#include <QtPlugin>
#include <QWidget>
#include <QLayout>

class LEVELADJUSTMENT_EXPORT LevelAdjustPlugin
{
public:
    virtual ~LevelAdjustPlugin() {}
    virtual QWidget* GetWidget() = 0;
    virtual bool ConnectAll() = 0;
    virtual void DisconnectAll() = 0;
    virtual bool IsAllConnect() = 0;
    virtual void AutoAdjustParallelism() = 0;
};
//Plug-in identifier
#define AbstractInterface_iid "Level adjustment plugin"
//Register plug-in interface with Qt meta-object system
Q_DECLARE_INTERFACE(LevelAdjustPlugin, AbstractInterface_iid)

#endif