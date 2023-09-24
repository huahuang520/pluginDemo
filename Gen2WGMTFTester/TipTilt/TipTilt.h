#pragma once

#include "tiptilt_global.h"
#include <QObject>
#include <QWidget>
//#include "TipTiltPlugin.h"
#include "./common/ml_motion2D.h"
class TIPTILT_EXPORT TipTilt : public QObject, public MLMotion2D
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(MLMotion2D)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "TipTilt.json")
public:
    TipTilt(QObject* parent = nullptr);
    ~TipTilt();
    virtual bool Connect(const char* type_name);
    virtual bool Disconnect();
    virtual bool IsConnected();
    //input pulse number, X:1000 pulse corresponds to 1.041413755 degrees, Y:1000 pulse corresponds to 0.70303334 degrees
    virtual void MoveByPulse(int posX, int posY);
    //input pulse number
    virtual void MoveRel(int posX, int posY);
    //input angle
    virtual void MoveByAngle(double angleX, double angleY);
    //input second
    virtual void MoveBySecond(double secondX, double secondY);
    virtual void SetSpeed(int speed); //input pulse number
    virtual int GetSpeed(); //get pulse number
    virtual bool IsMoving();
    virtual void StopMove();
    virtual QWidget* GetWidget();

    // not use
    virtual void SetEnable(bool enable) {};
    virtual bool IsEnabled() { return false; };
    virtual void MoveRelAsync(int posX, int posY) {};
    virtual void MoveAbsAsync(int pos, int posY) {};
    virtual void MoveAbs(int posX, int posY) {};
    virtual void Homing() {};
    virtual bool IsHome() { return false; };
    virtual ML_Point GetPosition() { return ML_Point{0, 0}; };
    virtual MLMotionState GetState() { return MLMotionState::kMLStatusOk; };
    virtual void Subscribe(MLMotionEvent event, MLMotionCallback* callback) {};
    virtual void Unsubscribe(MLMotionEvent event, MLMotionCallback* callback) {};
    virtual void clearAlarm() {};
signals:
    void updateSpeedShowSignal(int);
    void updateConnectStatus(QString);
    void updateMoveStatus(QString);
private:
    void SplitOffset(QString& offset, int& off_byte, int& off_bit);
    int m_speed;
    QString m_currentConnectStatus;
};
