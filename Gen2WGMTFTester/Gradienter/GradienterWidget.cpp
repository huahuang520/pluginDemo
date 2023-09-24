#include "GradienterWidget.h"
#include <qserialportinfo.h>
#include "ConfigurationPlugin.h"
#include "PluginCore.h"

GradienterWidget::GradienterWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	GetComPort();
}

GradienterWidget::~GradienterWidget()
{
}

void GradienterWidget::updateConnectStatus(QString status)
{
	ui.connectStatus->setText(status);
}

void GradienterWidget::on_open_clicked()
{
	QString comPort = ui.ComPort->currentText();

	emit sendOpenSignal(comPort.toStdString());
}

void GradienterWidget::on_close_clicked()
{
	emit sendCloseSignal();
}

void GradienterWidget::ShowGradienterData(double xAngle, double yAngle)
{
	if (m_DataStatus == "OK")
	{
		ui.GradienterX->setText(QString::number(xAngle));
		ui.GradienterY->setText(QString::number(yAngle));
	}
	else if (m_DataStatus == "Invalid_Data")
	{
		ui.GradienterX->setText("NULL");
		ui.GradienterY->setText("NULL");
	}
}

void GradienterWidget::receiveStatus(QString status)
{
	m_DataStatus = status;
}

void GradienterWidget::initUI()
{
	QFont font1("Microsoft YaHei", 10, 50);

	ui.connectStatus->setStyleSheet("color:red;");
	ui.connectStatus->setFont(font1);

	QFont font("Microsoft YaHei", 13, 75);

	ui.GradienterX->setStyleSheet("color:green;");
	ui.GradienterY->setStyleSheet("color:green;");
	ui.GradienterX->setFont(font);
	ui.GradienterY->setFont(font);
}

void GradienterWidget::GetComPort()
{
	ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
	std::string comPort = configurationPlugin->ReadFromConfig(ConfigMode::Gradienter_Config).gradienterConfig.ConnectAddress;
	ui.ComPort->addItem(QString::fromStdString(comPort));
}

