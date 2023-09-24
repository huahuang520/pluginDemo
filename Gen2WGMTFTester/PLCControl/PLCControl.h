#pragma once

#include "plccontrol_global.h"
#include "PLCControlPlugin.h"

class PLCCONTROL_EXPORT PLCControl : public QObject, public PLCControlPlugin
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(PLCControlPlugin)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "PLCControl.json")
public:
    PLCControl(QObject* parent = nullptr);
    ~PLCControl();

public:
    bool Connect();
    void DisConnect();
    bool IsConnected();
    
    void SetFloat(int start, float value);
    void GetFloat(int start, float& value);
    void SetBool(int start, int bitIndex, bool value);
    void GetBool(int start, int bitIndex, bool& value);
    void SetInt(int start,int value);
    void GetInt(int start, int&value);

private:
    void SetData(void* buffer, int start, int len);
    void GetData(void* buffer, int start, int len);

private:
    class PLCControllerImpl;
    std::unique_ptr<PLCControllerImpl> m_pImpl;
};
