#include "LEDControl.h"
#include <QDebug>
#include "LEDControlWidget.h"
LEDControl::LEDControl(QObject* parent):QObject(parent)
{
	LEDNumber = 9;
	isOpenCom = false;
	m_currentBrightness = 255;
	m_ledColor = LED_COLOR::LED_RGB;
	CommunicationProtocolInit();
}

QWidget* LEDControl::GetWidget()
{
	LEDControlWidget* widget = new LEDControlWidget();
	return widget;
}

bool LEDControl::OpenPort(QString portName)
{
	m_serialport->setPortName(portName);
	if (!m_serialport->open(QIODevice::ReadWrite))
	{
		emit ConnectStatus("connect failed!");
		isOpenCom = false;
		return false;
	}
	emit ConnectStatus("connect successful!");
	isOpenCom = true;
	return true;
}

void LEDControl::updatebrightnessSlot(int brightness)
{
	m_currentBrightness = brightness;
	
	if (m_ledColor == LED_COLOR::LED_R)
	{
		for (int i = 0; i < LEDNumber; i++)
		{
			data.append(to_string(i * 3 + 1).c_str()).append(",").append(to_string(m_currentBrightness).c_str()).append(",").append(to_string(i * 3 + 2).c_str()).append(",0,").append(to_string(i * 3 + 3).c_str()).append(",0,");
		}
	}
	else if (m_ledColor == LED_COLOR::LED_G)
	{
		for (int i = 0; i < LEDNumber; i++)
		{
			data.append(to_string(i * 3 + 1).c_str()).append(",0,").append(to_string(i * 3 + 2).c_str()).append(",").append(to_string(m_currentBrightness).c_str()).append(",").append(to_string(i * 3 + 3).c_str()).append(",0,");
		}
	}
	else if (m_ledColor == LED_COLOR::LED_B)
	{
		for (int i = 0; i < LEDNumber; i++)
		{
			data.append(to_string(i * 3 + 1).c_str()).append(",0,").append(to_string(i * 3 + 2).c_str()).append(",0,").append(to_string(i * 3 + 3).c_str()).append(",").append(to_string(m_currentBrightness).c_str()).append(",");
		}
	}
	else if (m_ledColor == LED_COLOR::LED_RGB)
	{
		for (int i = 0; i < LEDNumber; i++)
		{
			data.append(to_string(i * 3 + 1).c_str()).append(",").append(to_string(m_currentBrightness).c_str()).append(",").append(to_string(i * 3 + 2).c_str()).append(",").append(to_string(m_currentBrightness).c_str()).append(",").append(to_string(i * 3 + 3).c_str()).append(",").append(to_string(m_currentBrightness).c_str()).append(",");
		}
	}
	
	SendData(GetMsg());
}

void LEDControl::openAllLED_R()
{
	m_ledColor == LED_COLOR::LED_R;
	updatebrightnessSlot(m_currentBrightness);
}

void LEDControl::openAllLED_G()
{
	m_ledColor == LED_COLOR::LED_G;
	updatebrightnessSlot(m_currentBrightness);
}

void LEDControl::openAllLED_B()
{
	m_ledColor == LED_COLOR::LED_B;
	updatebrightnessSlot(m_currentBrightness);
}

void LEDControl::openAllLEDs()
{
	m_ledColor == LED_COLOR::LED_RGB;
	updatebrightnessSlot(m_currentBrightness);
}

void LEDControl::CommunicationProtocolInit()
{
	header = "200,";
	deviceAddr = "1,";
	controlCode = "1,";
	dataLength = "54,";
	data = "";
	for (int i = 0; i < LEDNumber; i++)
	{
		data.append(to_string(i * 3 + 1).c_str()).append(",0,").append(to_string(i * 3 + 2).c_str()).append(",0,").append(to_string(i * 3 + 3).c_str()).append(",0,");
	}
	check_bit = "0,0,";
	end = "201";
}

QString LEDControl::GetMsg()
{
	return (header + deviceAddr + controlCode + dataLength + data + check_bit + end);
}

void LEDControl::SerialPortInit()
{
	QList<QSerialPortInfo> list;

	list = QSerialPortInfo::availablePorts(); //get available Ports
	allComName.clear();
	for (int i = 0; i < list.size(); i++)
	{
		comName = list.at(i).portName();
		allComName.push_back(comName);
		qDebug() << list.at(i).portName();
		qDebug() << list.at(i).description();//port description
	}

	m_serialport = new QSerialPort(comName);

	m_serialport->setBaudRate(QSerialPort::Baud9600);

	//m_serialport->setDataBits(QSerialPort::Data8);//Set the data format to 8bit

	//m_serialport->setParity(QSerialPort::NoParity);//no Parity

	//m_serialport->setStopBits(QSerialPort::OneStop);//One stop bit

	//m_serialport->setFlowControl(QSerialPort::NoFlowControl);//Do not use flow control
}

void LEDControl::SendData(QString msg)
{
	if (m_serialport != NULL && m_serialport->isOpen())
	{
		m_serialport->write(msg.toLatin1());

		//m_serialport->waitForBytesWritten(500);//在子线程这句不可少，否则虽然发送出去了，对方接收不到；

		//QSerialPort 在子线程中调用write后并未等待数据发送出去子线程就被休眠导致数据丢失，从而我们需要在子线程中调用waitForBytesWritten等待write数据发送完毕。
		emit SendStatus("send successful!");
	}
	else
	{
		emit SendStatus("send failed!");
	}
}

void LEDControl::CloseAllLEDS()
{
	for (int i = 0; i < LEDNumber; i++)
	{
		data.append(to_string(i * 3 + 1).c_str()).append(",0,").append(to_string(i * 3 + 2).c_str()).append(",0,").append(to_string(i * 3 + 3).c_str()).append(",0,");
	}
	SendData(GetMsg());
}