#ifndef ACSCONTROLLER_3D_H_
#define ACSCONTROLLER_3D_H_
#include "spiiplusclibrary_global.h"
#include <QObject>
#include <map>
#include <thread>
#include "common\ml_motion3D.h"
#include "MLACSSystemController.h"


#ifdef ACSC_EXPORTS
#define ACSC_API __declspec(dllexport)
#else
#define ACSC_API __declspec(dllimport)
#endif

using std::map;
using std::thread;

class SPIIPLUSCLIBRARY_EXPORT MLACSCController3D: public QObject, public MLMotion3D
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(MLMotion3D)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "SPiiPlusCLibrary.json")
public:
    MLACSCController3D();
    ~MLACSCController3D();

public:
    virtual bool Connect(const char* type_name);

    virtual bool Disconnect();

    virtual bool IsConnected();

    virtual void SetEnable(bool enable);

    virtual bool IsEnabled();

    virtual void MoveRelAsync(int posX, int posY, int posZ);

    virtual void MoveAbsAsync(int pos, int posY, int posZ);

    virtual void MoveRelAsync(const char* axis, int pos);

    virtual void MoveAbsAsync(const char* axis, int pos);

    virtual void MoveRel(int posX, int posY, int posZ);

    virtual void MoveAbs(int posX, int posY, int posZ);

    virtual void StopMove();

    virtual void Homing();

    virtual bool IsHome();

    virtual void SetSpeed(int speed);

    virtual int GetSpeed();

    virtual bool IsMoving();

    virtual ML_Point3D GetPosition();

    virtual MLMotionState GetState();

    virtual void Subscribe(MLMotionEvent event, MLMotion3DCallback* callback);

    virtual void Unsubscribe(MLMotionEvent event, MLMotion3DCallback* callback);

    virtual void clearAlarm() {};

    virtual QWidget* GetWidget();
private:
    void threadFunc();
    bool InitialMotionSystem();

signals:
    void updateConnectStatus(QString);
    void updateMoveStatus(QString);
    void updatePosition(int x, int y, int z);
private:
    int X;
    int Y;
    int Z;
    const double m_Scale = 1000.0;
    bool m_IsConnected;
    bool m_Enable;
    MLMotionState m_State;
    int m_TotalAxes;
    HANDLE m_AcsHandle;
    map<MLMotionEvent, MLMotion3DCallback*> m_Callbacks;
    std::thread m_CallbackHandle;
    bool m_CallbackFlag;
    bool m_IsHome;
    QString m_currentConnectStatus;
};

#endif // !ACSCONTROLLER_3D_H_