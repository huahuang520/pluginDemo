/*************************************************
Author:jason.huang
Date:2023.2.22
Version:0.0.1
Description:Obtain and modify all configuration parameters
**************************************************/

#ifndef CONFIGURATION_PLUGIN
#define CONFIGURATION_PLUGIN
#include "configuration_global.h"
#include <QtPlugin>
#include <QObject>
#include <QWidget>
#include "./common/configParam.h"

class CONFIGURATION_EXPORT ConfigurationPlugin
{
public:
    virtual ~ConfigurationPlugin() {}
    virtual AllConfig ReadFromConfig(ConfigMode configMode) = 0;
    virtual void WriteToConfig(AllConfig allConfig, ConfigMode configMode) = 0;
    virtual QWidget* GetWidget() = 0;
};
//Plug-in identifier
#define AbstractInterface_iid "configuration plugin"
//Register plug-in interface with Qt meta-object system
Q_DECLARE_INTERFACE(ConfigurationPlugin, AbstractInterface_iid)

#endif
