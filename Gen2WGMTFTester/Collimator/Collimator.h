#pragma once

#include "collimator_global.h"
#include <QWidget>
#include "CollimatorPlugin.h"
#include <qtimer.h>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

class COLLIMATOR_EXPORT Collimator : public QObject ,public CollimatorPlugin
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(CollimatorPlugin)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "Collimator.json")
public:
    Collimator(QObject* parent = nullptr);
    ~Collimator();

    QWidget* GetWidget();
    bool isOpen();

private slots:
    ML::MLResult Open();
    bool Close();
    bool isHasCollimatorData();
    void receiveInfo();
    void updateData();
signals:
    //send collimator data to external
    void sendSecAngleSignal(double xSecAngle, double ySecAngle) override;
    //send collimator status
    void sendStatusSignal(QString) override;

    void updateConnectStatus(QString);

private:
    SOCKADDR_IN addrClient;
    char recvBuf[1000];
    SOCKET sockSrv;

    int len;
    bool m_isOpen;
    bool m_isHasCollimatorData;
    bool m_isUpdateData;
    QTimer* m_Timer;
    std::thread* m_thread;
    bool m_isRecvDataStop;
    bool m_isRecvDataEnd;
};
