#include "CollimatorWidget.h"

CollimatorWidget::CollimatorWidget(QWidget* parent) :QWidget(parent)
{
	ui.setupUi(this);
	initUI();
}

CollimatorWidget::~CollimatorWidget()
{
}

void CollimatorWidget::on_open_clicked()
{
	emit sendOpenSignal();
}

void CollimatorWidget::on_close_clicked()
{
	emit sendCloseSignal();
}

void CollimatorWidget::ShowCollimatorData(double xSecAngle, double ySecAngle)
{
	if (m_DataStatus == "OK")
	{
		ui.CollimatorX->setText(QString::number(-ySecAngle));
		ui.CollimatorY->setText(QString::number(xSecAngle));
	}
	else if (m_DataStatus == "Invaild_Data")
	{
		ui.CollimatorY->setText("NULL");
		ui.CollimatorX->setText("NULL");
	}
}

void CollimatorWidget::receiveStatus(QString status)
{
	m_DataStatus = status;
}

void CollimatorWidget::updateConnectStatus(QString status)
{
	ui.connectStatus->setText(status);
}

void CollimatorWidget::initUI()
{
	QFont font1("Microsoft YaHei", 10, 50);

	ui.connectStatus->setStyleSheet("color:red;");
	ui.connectStatus->setFont(font1);

	QFont font("Microsoft YaHei", 13, 75);

	ui.CollimatorX->setStyleSheet("color:green;");
	ui.CollimatorY->setStyleSheet("color:green;");
	ui.CollimatorX->setFont(font);
	ui.CollimatorY->setFont(font);
}