#pragma once

#include "leveladjustment_global.h"
#include <QWidget>
#include "LevelAdjustPlugin.h"
#include <thread>

class LEVELADJUSTMENT_EXPORT LevelAdjustment : public QObject, public LevelAdjustPlugin
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(LevelAdjustPlugin)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "LevelAdjustment.json")
public:
    LevelAdjustment(QObject* parent = nullptr);
    ~LevelAdjustment();
    virtual QWidget* GetWidget();
    virtual bool ConnectAll();
    virtual void DisconnectAll();
    virtual bool IsAllConnect();
    virtual void AutoAdjustParallelism();

signals:
    void updateConnectStatus(QString);
    void updateAdjustStatus(QString);
    void updateCollimatorShow(double,double);
    void updateGradienterShow(double,double);
    void updateMotion3DPosition(int, int, int);
    void CollimatorNoSignal_(QString);
private slots:
    void RecvCollimatorSlot(double x, double y);
    void RecvGradienterSlot(double x, double y);
    void StopAdjustSlot();
    void CollimatorNoSignal(QString msg);
private:
    void AutoAdjustParallelismThread();

private:
    std::thread* m_AutoAdjustParallelismThread;
    bool m_isStopThread;
    bool m_isThreadEnd;
    QPointF m_collimatorPoint;
    QPointF m_gradienter;
    static int adjustTimes;
};
