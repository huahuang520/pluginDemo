#include "TipTilt.h"
#include "ConfigurationPlugin.h"
#include "PluginCore.h"
#include "PLCControlPlugin.h"
#include <basetsd.h>
#include <QtEndian>
#include <QCoreApplication>
#include "TipTiltWidget.h"
#define GetBit(v,n) ((v) & ((UINT32)1 << (n)))
#define SetBit(v,n) ((v) |= ((UINT32)1 << (n)))
#define ClearBit(v,n) ((v) &= (~(UINT32)1 << (n)))
#define ClearByte(v,n) ((v) &= ((UINT32)0)

TipTilt::TipTilt(QObject* parent): QObject(parent)
{
	m_speed = 1000;  // 1000 pulse
	m_currentConnectStatus = "Disconnect";
}

TipTilt::~TipTilt()
{
	Disconnect();
}

bool TipTilt::Connect(const char* type_name)
{
	if (!IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			bool res = plcControlPlugin->Connect();
			if (res) {
				SetSpeed(m_speed);
				m_currentConnectStatus = "Connecting";
				emit updateConnectStatus(m_currentConnectStatus);
			}
			else
			{
				m_currentConnectStatus = "Disconnect";
				emit updateConnectStatus(m_currentConnectStatus);
			}
			return res;
		}
		m_currentConnectStatus = "Disconnect";
		emit updateConnectStatus(m_currentConnectStatus);
		return false;
	}
	SetSpeed(m_speed);
	m_currentConnectStatus = "Connecting";
	emit updateConnectStatus(m_currentConnectStatus);
	return true;
}

bool TipTilt::Disconnect()
{
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			StopMove();
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				//set enable to false
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Enable"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Enable"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);
			}

			plcControlPlugin->DisConnect();
			m_currentConnectStatus = "Disconnect";
			emit updateConnectStatus(m_currentConnectStatus);
			return true;
		}
	}
	m_currentConnectStatus = "Disconnect";
	emit updateConnectStatus(m_currentConnectStatus);
	return true;
}

bool TipTilt::IsConnected()
{
	PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
	if (plcControlPlugin)
	{
		return plcControlPlugin->IsConnected();
	}
	return false;
}

void TipTilt::MoveByPulse(int posX, int posY)
{
	int tmp = posY;
	posY = posX;
	posX = tmp;
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				//set stop to false
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Stop"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Stop"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);

				//set pos
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Pos"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Pos"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);

				plcControlPlugin->SetInt(byte1, posX);
				plcControlPlugin->SetInt(byte2, posY);

				//set enable to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Enable"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Enable"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//set mov to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Mov"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Mov"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//is moving
				while (IsMoving())
				{
					QCoreApplication::processEvents();
					_sleep(100);
				}

				//set mov to false
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);
			}
		}
	}
}

void TipTilt::MoveRel(int posX, int posY)
{
	int tmp = posY;
	posY = posX;
	posX = tmp;
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				//set stop to false
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Stop"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Stop"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);

				//set pos
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Pos"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Pos"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);

				plcControlPlugin->SetInt(byte1, posX);
				plcControlPlugin->SetInt(byte2, posY);

				//set enable to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Enable"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Enable"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//set mov to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Mov"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Mov"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//is moving
				while (IsMoving())
				{
					QCoreApplication::processEvents();
					_sleep(100);
				}

				//set mov to false
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);
			}
		}
	}
}

void TipTilt::MoveByAngle(double angleX, double angleY)
{
	int tmp = angleX;
	angleX = angleY;
	angleY = tmp;
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				//set stop to false
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Stop"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Stop"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);

				//set pos
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Pos"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Pos"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);

				double anglex = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_PulseConversionAngle"]).toDouble();
				double angley = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_PulseConversionAngle"]).toDouble();
				int posX = (1000 / anglex) * angleX;
				int posY = (1000 / angley) * angleY;
				plcControlPlugin->SetInt(byte1, posX);
				plcControlPlugin->SetInt(byte2, posY);

				//set enable to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Enable"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Enable"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//set mov to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Mov"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Mov"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//is moving
				while (IsMoving())
				{
					QCoreApplication::processEvents();
					_sleep(100);
				}

				//set mov to false
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);
			}
		}
	}
}

void TipTilt::MoveBySecond(double secondX, double secondY)
{
	int tmp = secondX;
	secondX = secondY;
	secondY = tmp;
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				//set stop to false
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Stop"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Stop"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);

				//set pos
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Pos"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Pos"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);

				double anglex = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_PulseConversionAngle"]).toDouble();
				double angley = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_PulseConversionAngle"]).toDouble();
				int posX = (1000 / anglex) * (secondX / 3600);
				int posY = (1000 / angley) * (secondY / 3600);
				plcControlPlugin->SetInt(byte1, posX);
				plcControlPlugin->SetInt(byte2, posY);

				//set enable to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Enable"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Enable"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//set mov to true
				axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Mov"]);
				axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Mov"]);
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);

				//is moving
				while (IsMoving())
				{
					QCoreApplication::processEvents();
					_sleep(100);
				}

				//set mov to false
				plcControlPlugin->SetBool(byte1, bit1, false);
				plcControlPlugin->SetBool(byte2, bit2, false);
			}
		}
	}
}

void TipTilt::SetSpeed(int speed)
{
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Speed"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Speed"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);
				plcControlPlugin->SetInt(byte1, speed);
				plcControlPlugin->SetInt(byte2, speed);
				m_speed = speed;
				emit updateSpeedShowSignal(speed);
			}
		}
	}
}

int TipTilt::GetSpeed()
{
	return m_speed;
}

bool TipTilt::IsMoving()
{
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Busy"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Busy"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);

				bool isMove1, isMove2;
				plcControlPlugin->GetBool(byte1, bit1, isMove1);
				plcControlPlugin->GetBool(byte2, bit2, isMove2);

				if (isMove1 || isMove2)
				{
					emit updateMoveStatus("moving...");
					return true;
				}
				else
				{
					emit updateMoveStatus("stop");
					return false;
				}
			}
		}
	}
	emit updateMoveStatus("stop");
	return false;
}

void TipTilt::StopMove()
{
	if (IsConnected())
	{
		PLCControlPlugin* plcControlPlugin = PluginCore::getinstance()->GetPlugin<PLCControlPlugin>("PLCControlPlugin");
		if (plcControlPlugin)
		{
			ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
			if (configurationPlugin)
			{
				AllConfig allConfig = configurationPlugin->ReadFromConfig(ConfigMode::PLC_Config);
				QString axis1_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis1_offset_Stop"]);
				QString axis2_speed_offset = QString::fromStdString(allConfig.plcControl.tip_tilt["Axis2_offset_Stop"]);
				int byte1, bit1, byte2, bit2;
				SplitOffset(axis1_speed_offset, byte1, bit1);
				SplitOffset(axis2_speed_offset, byte2, bit2);

				plcControlPlugin->SetBool(byte1, bit1, true);
				plcControlPlugin->SetBool(byte2, bit2, true);
			}
		}
	}
}

QWidget* TipTilt::GetWidget()
{
	TipTiltWidget* widget = new TipTiltWidget();
	connect(widget, &TipTiltWidget::connectSignal, this, &TipTilt::Connect);
	connect(widget, &TipTiltWidget::disconnectSignal, this, &TipTilt::Disconnect);
	connect(widget, &TipTiltWidget::stopSignal, this, &TipTilt::StopMove);
	connect(widget, &TipTiltWidget::moveSignal, this, &TipTilt::MoveByPulse);
	connect(widget, &TipTiltWidget::speedSignal, this, &TipTilt::SetSpeed);
	connect(this, &TipTilt::updateSpeedShowSignal, widget, &TipTiltWidget::updateSpeedShow);
	connect(this, &TipTilt::updateConnectStatus, widget, &TipTiltWidget::updateConnectSlot);
	connect(this, &TipTilt::updateMoveStatus, widget, &TipTiltWidget::updateMoveSlot);
	emit updateConnectStatus(m_currentConnectStatus);
	return widget;
}

void TipTilt::SplitOffset(QString& offset, int& off_byte, int& off_bit)
{
	QStringList offsets = offset.split(".");
	off_byte = offsets.at(0).toInt();
	off_bit = offsets.at(1).toInt();
}