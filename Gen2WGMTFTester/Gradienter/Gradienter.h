#pragma once

#include "gradienter_global.h"
#include <QWidget>
#include "GradienterPlugin.h"
#include <qserialportinfo.h>
#include <qserialport.h>
#include <QtCore/qiodevice.h>

class GRADIENTER_EXPORT Gradienter : public QObject, public GradienterPlugin
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(GradienterPlugin)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "Gradienter.json")
public:
    Gradienter(QObject* parent = nullptr);
    ~Gradienter();

    QWidget* GetWidget();
    bool isOpen();

public slots:
    ML::MLResult Open(std::string name);
    bool Close();
    void receiveInfo();

signals:
    //send Gradienter data to external
    void sendAngleSignal(double xAngle, double yAngle) override;
    //send Gradienter status
    void sendStatusSignal(QString) override;

    void updateConnectStatus(QString);

private:
    bool m_isOpen;
    QStringList m_serialPortName;
    QSerialPort* m_serialPort;
};
