#ifndef ACS_CONTROLLER_1D_H_
#define ACS_CONTROLLER_1D_H_
#include "spiiplusclibrary_global.h"
#include <QObject>
#include <QObject>
#include "common\ml_motion.h"
#include <thread>
#include "MLACSSystemController.h"

#ifdef ACSC_EXPORTS
#define ACSC_API __declspec(dllexport)
#else
#define ACSC_API __declspec(dllimport)
#endif

using std::map;
using std::thread;

class SPIIPLUSCLIBRARY_EXPORT MLACSCController1D : public QObject, public MLMotion
{
    Q_OBJECT
public:
    MLACSCController1D();
    ~MLACSCController1D();

public:
    virtual bool Connect(const char* type_name);

    virtual bool Disconnect();

    virtual bool IsConnected();

    virtual void SetEnable(bool enable);

    virtual bool IsEnabled();

    virtual void MoveRelAsync(int step);

    virtual void MoveAbsAsync(int step);

    virtual void MoveRel(int step);

    virtual void MoveAbs(int step);

    virtual void StopMove();

    virtual void Homing();

    virtual bool IsHome();

    virtual void SetSpeed(int speed);

    virtual int GetSpeed();

    virtual bool IsMoving();

    virtual int GetPosition();

    virtual MLMotionState GetState();

    virtual void Subscribe(MLMotionEvent event, MLMotionCallback* callback);

    virtual void Unsubscribe(MLMotionEvent event, MLMotionCallback* callback);

private:
    void threadFunc();
    bool InitialMotionSystem();


private:
    int R;
    const double m_Scale = 1000.0;
    bool m_IsConnected;
    bool m_Enable;
    HANDLE m_AcsHandle;
    map<MLMotionEvent, MLMotionCallback*> m_Callbacks;
    std::thread m_CallbackHandle;
    bool m_CallbackFlag;
    bool m_IsHome;
    MLMotionState m_State;
};

#endif // !ACS_CONTROLLER_1D_H_