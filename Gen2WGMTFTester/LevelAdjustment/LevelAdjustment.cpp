#include "LevelAdjustment.h"
#include "LevelAdjustWidget.h"
#include "./common/ml_motion3D.h"
#include "PluginCore.h"
#include "common/ml_motion2D.h"
#include "CollimatorPlugin.h"
#include "GradienterPlugin.h"
#include "ConfigurationPlugin.h"
#include <stdlib.h>
int LevelAdjustment::adjustTimes = 0;
LevelAdjustment::LevelAdjustment(QObject* parent):QObject(parent)
{
	m_isStopThread = true;
	m_isThreadEnd = false;
	m_AutoAdjustParallelismThread = nullptr;
}

LevelAdjustment::~LevelAdjustment()
{
	DisconnectAll();
	m_isStopThread = true;
	m_isThreadEnd = true;
	if (m_AutoAdjustParallelismThread && m_AutoAdjustParallelismThread->joinable())
	{
		m_AutoAdjustParallelismThread->join();
		delete m_AutoAdjustParallelismThread; //?
		m_AutoAdjustParallelismThread = nullptr;
	}
}

QWidget* LevelAdjustment::GetWidget()
{
	LevelAdjustWidget* widget = new LevelAdjustWidget();
	connect(widget, &LevelAdjustWidget::connectAllSignal, this, &LevelAdjustment::ConnectAll);
	connect(widget, &LevelAdjustWidget::stopAllSignal, this, &LevelAdjustment::DisconnectAll);
	connect(widget, &LevelAdjustWidget::stopAdjustSignal, this, &LevelAdjustment::StopAdjustSlot);
	connect(widget, &LevelAdjustWidget::AutoParallelSignal, this, &LevelAdjustment::AutoAdjustParallelism);
	connect(this, &LevelAdjustment::updateConnectStatus, widget, &LevelAdjustWidget::updateConnectStatus);
	connect(this, &LevelAdjustment::updateAdjustStatus, widget, &LevelAdjustWidget::updateAdjustStatus);
	connect(this, &LevelAdjustment::updateCollimatorShow, widget, &LevelAdjustWidget::updateCollimatorShow);
	connect(this, &LevelAdjustment::updateGradienterShow, widget, &LevelAdjustWidget::updateGradienterShow);
	connect(this, &LevelAdjustment::updateMotion3DPosition, widget, &LevelAdjustWidget::updateMotion3DPosition);
	connect(this, &LevelAdjustment::CollimatorNoSignal_, widget, &LevelAdjustWidget::CollimatorNoSignal);
	
	return widget;
}

bool LevelAdjustment::ConnectAll()
{
	ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
	//connect motion3D
	MLMotion3D* acsPlugin = PluginCore::getinstance()->GetPlugin<MLMotion3D>("ACSPlugin");
	//connect tiptilt
	MLMotion2D* tipTiltPlugin = PluginCore::getinstance()->GetPlugin<MLMotion2D>("TipTiltPlugin");
	//connect Collimator
	CollimatorPlugin* collimatorPlugin = PluginCore::getinstance()->GetPlugin<CollimatorPlugin>("CollimatorPlugin");
	//connect gradienter
	GradienterPlugin* gradienterPlugin = PluginCore::getinstance()->GetPlugin<GradienterPlugin>("GradienterPlugin");

	if (configurationPlugin && acsPlugin && tipTiltPlugin && collimatorPlugin && gradienterPlugin)
	{
		//bool res1 = Motion3D->Connect(configurationPlugin->ReadFromConfig(ConfigMode::Motion3D_Config).motion3DConfig.ipAddress1.c_str());
		bool res2 = tipTiltPlugin->Connect("");
		ML::MLResult res3 = collimatorPlugin->Open();
		std::string comPort = configurationPlugin->ReadFromConfig(ConfigMode::Gradienter_Config).gradienterConfig.ConnectAddress;
		ML::MLResult res4 = gradienterPlugin->Open(comPort.c_str());

		if (res2 && res3.code && res4.code)
		{
			emit updateConnectStatus("Connection succeeded!");
			QObject* obj = dynamic_cast<QObject*>(collimatorPlugin);
			QObject* obj2 = dynamic_cast<QObject*>(gradienterPlugin);
			QObject* obj3 = dynamic_cast<QObject*>(acsPlugin);
			connect(obj, SIGNAL(sendSecAngleSignal(double, double)), this, SLOT(RecvCollimatorSlot(double ,double)));
			connect(obj, SIGNAL(sendStatusSignal(QString)), this, SLOT(CollimatorNoSignal(QString)));
			connect(obj2, SIGNAL(sendAngleSignal(double, double)), this, SLOT(RecvGradienterSlot(double, double)));
			connect(obj3, SIGNAL(updatePosition(int, int, int)), this, SIGNAL(updateMotion3DPosition(int, int, int)));
			m_isStopThread = true;
			m_isThreadEnd = false;
			if (!m_AutoAdjustParallelismThread)
				m_AutoAdjustParallelismThread = new std::thread(&LevelAdjustment::AutoAdjustParallelismThread,this);

			return true;
		}
		else
		{
			emit updateConnectStatus("connection failed!");
			return false;
		}
	}
	else
	{
		emit updateConnectStatus("connection failed!");
		return false;
	}
}

void LevelAdjustment::DisconnectAll()
{
	//disconnect motion3D
	MLMotion3D* acsPlugin = PluginCore::getinstance()->GetPlugin<MLMotion3D>("ACSPlugin");
	//disconnect tiptilt
	MLMotion2D* tipTiltPlugin = PluginCore::getinstance()->GetPlugin<MLMotion2D>("TipTiltPlugin");
	//disconnect Collimator
	CollimatorPlugin* collimatorPlugin = PluginCore::getinstance()->GetPlugin<CollimatorPlugin>("CollimatorPlugin");
	//disconnect gradienter
	GradienterPlugin* gradienterPlugin = PluginCore::getinstance()->GetPlugin<GradienterPlugin>("GradienterPlugin");

	if (acsPlugin && tipTiltPlugin && collimatorPlugin && gradienterPlugin)
	{
		//Motion3D->Disconnect();
		bool res1 = tipTiltPlugin->Disconnect();
		bool res2 = collimatorPlugin->Close();
		bool res3 = gradienterPlugin->Close();
		if (res1 && res2 && res3)
		{
			QObject* obj = dynamic_cast<QObject*>(collimatorPlugin);
			QObject* obj2 = dynamic_cast<QObject*>(gradienterPlugin);
			disconnect(obj, SIGNAL(sendSecAngleSignal(double, double)), this, SLOT(RecvCollimatorSlot(double, double)));
			disconnect(obj2, SIGNAL(sendAngleSignal(double, double)), this, SLOT(RecvGradienterSlot(double, double)));
			emit updateConnectStatus("Disconnected successfully!");
		}
		else
		{
			emit updateConnectStatus("Disconnected failed!");
		}
		if (m_AutoAdjustParallelismThread && m_AutoAdjustParallelismThread->joinable())
		{
			m_isStopThread = true;
			m_isThreadEnd = true;
			m_AutoAdjustParallelismThread->join();
			delete m_AutoAdjustParallelismThread;
			m_AutoAdjustParallelismThread = nullptr;
		}
	}
}

bool LevelAdjustment::IsAllConnect()
{
	MLMotion3D* acsPlugin = PluginCore::getinstance()->GetPlugin<MLMotion3D>("ACSPlugin");
	MLMotion2D* tipTiltPlugin = PluginCore::getinstance()->GetPlugin<MLMotion2D>("TipTiltPlugin");
	CollimatorPlugin* collimatorPlugin = PluginCore::getinstance()->GetPlugin<CollimatorPlugin>("CollimatorPlugin");
	GradienterPlugin* gradienterPlugin = PluginCore::getinstance()->GetPlugin<GradienterPlugin>("GradienterPlugin");
	
	if (acsPlugin && tipTiltPlugin && collimatorPlugin && gradienterPlugin)
	{
		//bool isConnect1 = Motion3D->IsConnected();
		bool isConnect2 = tipTiltPlugin->IsConnected();
		bool isConnect3 = collimatorPlugin->isOpen();
		bool isConnect4 = gradienterPlugin->isOpen();
		if (isConnect2 && isConnect3 && isConnect4) return true;
		else
			return false;
	}
	else
		return false;
}

void LevelAdjustment::AutoAdjustParallelism()
{
	m_isStopThread = false;
	m_isThreadEnd = false;
}

void LevelAdjustment::AutoAdjustParallelismThread()
{
	MLMotion3D* acsPlugin = PluginCore::getinstance()->GetPlugin<MLMotion3D>("ACSPlugin");
	MLMotion2D* tipTiltPlugin = PluginCore::getinstance()->GetPlugin<MLMotion2D>("TipTiltPlugin");
	CollimatorPlugin* collimatorPlugin = PluginCore::getinstance()->GetPlugin<CollimatorPlugin>("CollimatorPlugin");
	GradienterPlugin* gradienterPlugin = PluginCore::getinstance()->GetPlugin<GradienterPlugin>("GradienterPlugin");
	ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");

	while (true)
	{
		if (m_isThreadEnd)
		{
			adjustTimes = 0;
			m_isStopThread = true;
			break;
		}

		if (!m_isStopThread)
		{
			if (adjustTimes == configurationPlugin->ReadFromConfig(ConfigMode::LevelAdjust_Config).levelAdjustConfig.adjustTimesMax)
			{
				m_isStopThread = true;
				adjustTimes = 0;
				emit updateAdjustStatus("reach max adjust times!");
				continue;
			}
			
			QString msg = "start " + QString::number(adjustTimes + 1) + " time horizontal adjustment";
			emit updateAdjustStatus(msg);
			double x = configurationPlugin->ReadFromConfig(ConfigMode::LevelAdjust_Config).levelAdjustConfig.motion3DX;
			double y = configurationPlugin->ReadFromConfig(ConfigMode::LevelAdjust_Config).levelAdjustConfig.motion3DY;
			double z = configurationPlugin->ReadFromConfig(ConfigMode::LevelAdjust_Config).levelAdjustConfig.motion3DZ;
			double angleRange = configurationPlugin->ReadFromConfig(ConfigMode::LevelAdjust_Config).levelAdjustConfig.angleRange;
			double pulseConversionAngle_X = configurationPlugin->ReadFromConfig(ConfigMode::LevelAdjust_Config).levelAdjustConfig.pulseConversionAngle_X;
			double pulseConversionAngle_Y = configurationPlugin->ReadFromConfig(ConfigMode::LevelAdjust_Config).levelAdjustConfig.pulseConversionAngle_Y;
			//3DMotor move to position start to adjust
			//Motion3D->MoveAbs(x * 1000, y * 1000, z * 1000);
			if (collimatorPlugin->isHasCollimatorData())
			{
				//tipTiltPlugin->MoveRel(-m_collimatorPoint.x(), m_collimatorPoint.y());
				int pulseX = ((double)1000 / (pulseConversionAngle_X * 3600)) * (m_collimatorPoint.y());
				int pulseY = ((double)1000 / (pulseConversionAngle_Y * 3600)) * (m_collimatorPoint.x());
				tipTiltPlugin->MoveRel(pulseX, -pulseY);

				while (tipTiltPlugin->IsMoving())
				{
					_sleep(10);
					//continue;
				}
				_sleep(1000);
				if ((fabs(m_collimatorPoint.x()) <= angleRange) && (fabs(m_collimatorPoint.y()) <= angleRange))
				{
					m_isStopThread = true;
					adjustTimes = 0;
					emit updateAdjustStatus("horizontal adjustment succeeded!");
					_sleep(10);
					continue;
				}
			}
			else
			{
				//tipTiltPlugin->MoveRel(-m_gradienter.y(), -m_gradienter.x());
				
				int pulseX = ((double)1000 / pulseConversionAngle_X) * (m_gradienter.x());
				int pulseY = ((double)1000 / pulseConversionAngle_Y) * (m_gradienter.y());
				tipTiltPlugin->MoveRel(-pulseX, -pulseY);
			}
			adjustTimes++;
		}

		_sleep(100);
	}
}

void LevelAdjustment::RecvCollimatorSlot(double x, double y)
{
	m_collimatorPoint.setX(x);
	m_collimatorPoint.setY(y);

	emit updateCollimatorShow(-y, x);
	
}

void LevelAdjustment::RecvGradienterSlot(double x, double y)
{
	m_gradienter.setX(x);
	m_gradienter.setY(y);
	emit updateGradienterShow(x, y);
}

void LevelAdjustment::StopAdjustSlot()
{
	MLMotion2D* tipTiltPlugin = PluginCore::getinstance()->GetPlugin<MLMotion2D>("TipTiltPlugin");
	if (tipTiltPlugin)
		tipTiltPlugin->StopMove();
	m_isStopThread = true;
	adjustTimes = 0;
	emit updateAdjustStatus("Stop horizontal adjustment!");
}

void LevelAdjustment::CollimatorNoSignal(QString msg)
{
	if (msg == "Invaild_Data")
		emit CollimatorNoSignal_("NULL");
}
