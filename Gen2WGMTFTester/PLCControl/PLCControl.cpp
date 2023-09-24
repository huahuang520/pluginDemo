#include "PLCControl.h"
#include <snap7.h>
#include <basetsd.h>
#include "PluginCore.h"
#include "ConfigurationPlugin.h"

#define GetBit(v,n) ((v) & ((UINT32)1 << (n)))
#define SetBit(v,n) ((v) |= ((UINT32)1 << (n)))
#define ClearBit(v,n) ((v) &= (~(UINT32)1 << (n)))

class PLCControl::PLCControllerImpl
{
public:
    PLCControllerImpl()
        : m_DBNumber(1), isConnect(false)
    {
        ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
        if (configurationPlugin)
            m_DBNumber = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config).plcControl.dataBaseNumber;
    };
    ~PLCControllerImpl()
    {
        if (IsConnected())
        {
            isConnect = false;
            Disconnect();
        }

    };

    bool Connect(const char* ip, int rack, int slot)
    {
        m_pClient.ConnectTo(ip, rack, slot);
        isConnect = m_pClient.Connected();
        return isConnect;

    };
    void Disconnect()
    {
        isConnect = false;
        m_pClient.Disconnect();
    };

    bool IsConnected()
    {
        return isConnect;
    }
    void  GetData(void* buffer, int start, int len)
    {
        if (isConnect)
            m_pClient.DBRead(m_DBNumber, start, len, buffer);
    };

    void SetData(void* buffer, int start, int len)
    {
        if (isConnect)
            m_pClient.DBWrite(m_DBNumber, start, len, buffer);
    }

private:
    TS7Client m_pClient;
    int m_DBNumber;
    bool isConnect;
};


PLCControl::PLCControl(QObject* parent) : QObject(parent), m_pImpl(new PLCControllerImpl())
{

}

PLCControl::~PLCControl()
{
	
}

bool PLCControl::Connect()
{
    ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
    if (configurationPlugin)
    {
        AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
        bool res = m_pImpl->Connect(allConfig.plcControl.ipAddress.c_str(), allConfig.plcControl.rock, allConfig.plcControl.slot);
        return res;
    }
    return false;
}

void PLCControl::DisConnect()
{
    m_pImpl->Disconnect();
}

bool PLCControl::IsConnected()
{
    return m_pImpl->IsConnected();
}

void PLCControl::SetData(void* buffer, int start, int len)
{
    m_pImpl->SetData(buffer, start, len);
}

void PLCControl::GetData(void* buffer, int start, int len)
{
    m_pImpl->GetData(buffer, start, len);
}

void PLCControl::SetFloat(int start, float value)
{
    byte MyDB4[2];
    union change
    {
        float d;
        unsigned char dat[2];
    }r1;

    r1.d = value;
   /* MyDB4[3] = r1.dat[0];
    MyDB4[2] = r1.dat[1];
    MyDB4[1] = r1.dat[2];
    MyDB4[0] = r1.dat[3];*/
    MyDB4[1] = r1.dat[0];
    MyDB4[0] = r1.dat[1];
    m_pImpl->SetData(&MyDB4, start, 2);
}

void PLCControl::GetFloat(int start, float& value)
{
    byte MyDB4[2];
    m_pImpl->GetData(MyDB4, start, 2);
    union change
    {
        float d;
        unsigned char dat[2];
    }r1;

   /* r1.dat[0] = MyDB4[3];
    r1.dat[1] = MyDB4[2];
    r1.dat[2] = MyDB4[1];
    r1.dat[3] = MyDB4[0];*/
    r1.dat[0] = MyDB4[1];
    r1.dat[1] = MyDB4[0];
    
    value = r1.d;
}

void PLCControl::SetBool(int start, int bitIndex, bool value)
{
    byte MyDB4[1];
    m_pImpl->GetData(MyDB4, start, 1);
    
    if (value)
    {
        SetBit(MyDB4[0], bitIndex);
    }
    else
    {
        ClearBit(MyDB4[0], bitIndex);
    }
    m_pImpl->SetData(MyDB4, start, 1);
}

void PLCControl::GetBool(int start, int bitIndex, bool& value)
{
    byte MyDB4[1] = {0};
    m_pImpl->GetData(MyDB4, start, 1);
    int iva;
    iva = (MyDB4[bitIndex / 8] >> (bitIndex % 8)) & 1;

    value = iva;
}

void PLCControl::SetInt(int start, int value)
{
    byte MyDB4[2];
    union change
    {
        int d;
        unsigned char dat[2];
    }r1;

    r1.d = value;
    /* MyDB4[3] = r1.dat[0];
     MyDB4[2] = r1.dat[1];
     MyDB4[1] = r1.dat[2];
     MyDB4[0] = r1.dat[3];*/
    MyDB4[1] = r1.dat[0];
    MyDB4[0] = r1.dat[1];
    m_pImpl->SetData(&MyDB4, start, 2);
}

void PLCControl::GetInt(int start, int& value)
{
    byte MyDB4[2];
    m_pImpl->GetData(MyDB4, start, 2);
    union change
    {
        int d;
        unsigned char dat[2];
    }r1;

    r1.dat[0] = MyDB4[1];
    r1.dat[1] = MyDB4[0];

    value = r1.d;
}
