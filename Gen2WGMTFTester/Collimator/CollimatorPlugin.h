
#ifndef COLLIMATOR_PLUGIN
#define COLLIMATOR_PLUGIN
#include "collimator_global.h"
#include "common/ml.h"
#include <QtPlugin>
#include <QWidget>

class COLLIMATOR_EXPORT CollimatorPlugin
{
public:
    virtual ~CollimatorPlugin() {}
    virtual QWidget* GetWidget() = 0;

    virtual ML::MLResult Open() = 0;
    virtual bool Close() = 0;
    virtual bool isOpen() = 0;
    virtual bool isHasCollimatorData() = 0;

    //signals
    //send Collimator data to external
    virtual void sendSecAngleSignal(double xSecAngle, double ySecAngle) = 0;
    //send collimator status
    virtual void sendStatusSignal(QString) = 0;
};
//Plug-in identifier
#define AbstractInterface_iid "receive collimator data plugin"
//Register plug-in interface with Qt meta-object system
Q_DECLARE_INTERFACE(CollimatorPlugin, AbstractInterface_iid)

#endif // !COLLIMATOR_PLUGIN

