/*************************************************
Author:jason.huang
Date:2023.2.21
Version:0.0.1
Description:Provide interface control PLC motor
**************************************************/

#ifndef PLC_CONTROL_PLUGIN
#define PLC_CONTROL_PLUGIN
#include "plccontrol_global.h"
#include <QtPlugin>
#include <QObject>

class PLCCONTROL_EXPORT PLCControlPlugin
{
public:
    virtual ~PLCControlPlugin() {}
    virtual bool Connect() = 0;
    virtual void DisConnect() = 0;
    virtual bool IsConnected() = 0;
    virtual void GetFloat(int start, float& value) = 0;
    virtual void SetFloat(int start, float value) = 0;
    virtual void GetBool(int start, int bitIndex, bool& value) = 0;
    virtual void SetBool(int start, int bitIndex, bool value) = 0;
    virtual void SetInt(int start, int value) = 0;
    virtual void GetInt(int start, int& value) = 0;
};
//Plug-in identifier
#define AbstractInterface_iid "plc control plugin"
//Register plug-in interface with Qt meta-object system
Q_DECLARE_INTERFACE(PLCControlPlugin, AbstractInterface_iid)

#endif
