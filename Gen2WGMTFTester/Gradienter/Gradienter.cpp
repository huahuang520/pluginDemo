#include "Gradienter.h"
#include "LogPlus.h"
#include "GradienterWidget.h"
#include "ConfigurationPlugin.h"
#include "PluginCore.h"

Gradienter::Gradienter(QObject* parent) : QObject(parent)
{
	m_serialPort = new QSerialPort();
	m_isOpen = false;
}

Gradienter::~Gradienter()
{
	delete m_serialPort;
}

QWidget* Gradienter::GetWidget()
{
	GradienterWidget* gradWidget = new GradienterWidget();

	connect(this, &Gradienter::updateConnectStatus, gradWidget, &GradienterWidget::updateConnectStatus);
	connect(gradWidget, &GradienterWidget::sendOpenSignal, this, &Gradienter::Open);
	connect(gradWidget, &GradienterWidget::sendCloseSignal, this, &Gradienter::Close);
	connect(this, &Gradienter::sendStatusSignal, gradWidget, &GradienterWidget::receiveStatus);
	connect(this, &Gradienter::sendAngleSignal, gradWidget, &GradienterWidget::ShowGradienterData);

	return gradWidget;
}

ML::MLResult Gradienter::Open(std::string name)
{
	ConfigurationPlugin* configurationPlugin = PluginCore::getinstance()->GetPlugin<ConfigurationPlugin>("ConfigurationPlugin");
	std::string comPort = configurationPlugin->ReadFromConfig(ConfigMode::Gradienter_Config).gradienterConfig.ConnectAddress;
	if (comPort != name)
	{
		m_isOpen = false;

		emit updateConnectStatus("Open Failed");

		LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, "Gradienter Open Failed!!");

		return ML::MLResult{ ML::MLStatus::ML_STATUS_FAILED,"Gradienter Open Failed!!" };
	}
	if (m_serialPort->isOpen())
	{
		m_serialPort->clear();
		m_serialPort->close();

		m_isOpen = true;

		emit updateConnectStatus("Already Open");

		LOG4CPLUS_WARN(LogPlus::getInstance()->logger, "Gradienter is already Open!!");

		return ML::MLResult{ ML::MLStatus::ML_STATUS_OK,"" };
	}

	QList<QSerialPortInfo> serial_list = QSerialPortInfo::availablePorts();

	foreach(const QSerialPortInfo & info, serial_list)
	{
		m_serialPortName << info.portName();
		if (QString::fromStdString(name) == info.portName())
		{
			m_serialPort->setPortName(info.portName());
			break;
		}
	}
	if (!m_serialPort->open(QIODevice::ReadWrite))
	{
		m_isOpen = false;

		emit updateConnectStatus("Open Failed");

		LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, "Gradienter Open Failed!!");

		return ML::MLResult{ ML::MLStatus::ML_STATUS_FAILED,"Gradienter Open Failed!!" };
	}

	m_serialPort->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections); // Set baud rate and read/write direction
	m_serialPort->setDataBits(QSerialPort::Data8);		// 8 Data bits
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//ÎÞÁ÷¿ØÖÆ
	m_serialPort->setParity(QSerialPort::NoParity);	// No flow control
	m_serialPort->setStopBits(QSerialPort::OneStop); // One stop bit

	//
	connect(m_serialPort, &QIODevice::readyRead, this, &Gradienter::receiveInfo);

	m_isOpen = true;

	emit updateConnectStatus("Open Success");

	LOG4CPLUS_INFO(LogPlus::getInstance()->logger, "Gradienter Open Success!!");

	return ML::MLResult{ ML::MLStatus::ML_STATUS_OK,"" };
}

bool Gradienter::Close()
{
	if (m_serialPort->isOpen())
	{
		m_serialPort->clear();
		m_serialPort->close();

		m_isOpen = false;
		emit updateConnectStatus("Close Success");

		LOG4CPLUS_INFO(LogPlus::getInstance()->logger, "Gradienter Close Success!!");

		return true;
	}
	else
	{
		m_isOpen = true;

		emit updateConnectStatus("Close Failed");

		LOG4CPLUS_INFO(LogPlus::getInstance()->logger, "Gradienter Close Failed!!");

		return false;
	}
}

bool Gradienter::isOpen()
{
	return m_isOpen;
}

void Gradienter::receiveInfo()
{
	QByteArray info = m_serialPort->readLine();
	m_serialPort->waitForReadyRead(10);
	if (info.size() == 30)
	{
		QString str = info;
		QStringList qlist = str.split("\r\n");
		QStringList xyAngle_list = qlist[0].split(";");
		if (xyAngle_list.size() >= 2)
		{
			QString xstr = xyAngle_list[0].mid(2);
			QString ystr = xyAngle_list[1].mid(2);
			double xAngle = xstr.toDouble();
			double yAngle = ystr.toDouble();

			emit sendStatusSignal("OK");
			emit sendAngleSignal(xAngle, yAngle);
		}
		else
		{
			emit sendStatusSignal("Invalid_Data");
			return;
		}
	}
	else
	{
		emit sendStatusSignal("Invalid_Data");
		return;
	}
}
