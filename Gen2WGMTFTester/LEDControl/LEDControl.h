#pragma once

#include "ledcontrol_global.h"
#include <QObject>
#include <QtSerialPort/QSerialPort> // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo> // 提供系统中存在的串口信息
#include "LEDPlugin.h"
using namespace std;

typedef enum LED_COLOR
{
    LED_R,
    LED_G,
    LED_B,
    LED_RGB
};

class LEDCONTROL_EXPORT LEDControl : public QObject, public LEDPlugin
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(LEDPlugin)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "LEDControl.json")
public:
    LEDControl(QObject* parent = nullptr);
    QWidget* GetWidget();
public slots:
    void CloseAllLEDS();
    bool OpenPort(QString portName);
    void updatebrightnessSlot(int brightness);
    void openAllLED_R();
    void openAllLED_G();
    void openAllLED_B();
    void openAllLEDs();
signals:
    void SendStatus(QString);
    void ConnectStatus(QString);
    
private:
    void CommunicationProtocolInit();
    QString GetMsg();
    void SerialPortInit();
    void SendData(QString msg);
private:
    int LEDNumber;
    int m_currentBrightness;
    int led_R[9] = {0};
    int led_G[9] = { 0 };
    int led_B[9] = {0};
    LED_COLOR m_ledColor;
    QString m_CommunicationProtocol;
    QString header;
    QString deviceAddr;
    QString controlCode;
    QString dataLength;
    QString data;
    QString check_bit;
    QString end;

    QSerialPort* m_serialport;
    bool isOpenCom;
    QString comName;
    std::vector<QString> allComName;
    QStringList baudList; //Baud rate
    QStringList parityList; //Check bit
    QStringList dataBitsList; //Data bits
    QStringList stopBitsList; //Stop bit
    QStringList flowControlList; //control flow
};
