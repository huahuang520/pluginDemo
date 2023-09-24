#include "ACSControlWidget.h"

ACSControlWidget::ACSControlWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
}

ACSControlWidget::~ACSControlWidget()
{}

void ACSControlWidget::NotifyMotion3DStateChanged(MLMotionState old_state, MLMotionState new_state)
{
	if (new_state == MLMotionState::kMLStatusIsMoving)
		ui.moveStatus->setText("Moving...");
	else
		ui.moveStatus->setText("Stop");
}

void ACSControlWidget::NotifyMotion3DPosition(int x, int y, int z)
{
	ui.xLabel->setText(QString::number((double)x / 1000));
	ui.yLabel->setText(QString::number((double)y / 1000));
	ui.zLabel->setText(QString::number((double)z / 1000));
}

void ACSControlWidget::initUI()
{
	QPixmap pixmap(":/Gen2WGMTFTester/images/motionControl/stop.png");
	QPixmap fitpixmap = pixmap.scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.stop->setIcon(QIcon(fitpixmap));
	ui.stop->setIconSize(QSize(32, 32));
	ui.stop->setFlat(true);
	//ui.stop->setStyleSheet("border: 0px"); //Ïû³ı±ß¿ò
	ui.stop->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
											color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(185, 185, 185); border-style: inset; }");

		//ui.stop->setStyleSheet("QPushButton{ border-width: 0px 0px 0px 0px;border-style:solid;color:#ffffff; background:rgba(0,0,0,0);border:0px solid rgba(0,0,0,0);background-image:url(:/Gen2WGMTFTester/images/eyeBoxScan.jpg); }");
	
	/*ui.connect->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);

	ui.disconnect->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);*/
	
	QFont font1("Microsoft YaHei", 10, 50);
	ui.connectStatus->setStyleSheet("color:red;");
	ui.moveStatus->setStyleSheet("color:red;");
	ui.connectStatus->setFont(font1);
	ui.moveStatus->setFont(font1);
	
	QFont font("Microsoft YaHei", 13, 75);
	ui.xLabel->setStyleSheet("color:red;");
	ui.yLabel->setStyleSheet("color:red;");
	ui.zLabel->setStyleSheet("color:red;");
	ui.xLabel->setFont(font);
	ui.yLabel->setFont(font);
	ui.zLabel->setFont(font);

	m_buttonGroup3D = new QButtonGroup(this);
	m_buttonGroup3D->addButton(ui.RelButton, 0);
	m_buttonGroup3D->addButton(ui.AbsButton, 1);
	connect(m_buttonGroup3D, SIGNAL(buttonClicked(int)), this, SLOT(on3DButtonClicked(int)));
	m_buttonGroup3D->button(0)->setChecked(true);
	emit m_buttonGroup3D->buttonClicked(0);
}

void ACSControlWidget::on_connect_clicked()
{
	emit connectSignal();
}

void ACSControlWidget::on_disconnect_clicked()
{
	emit disconnectSignal();
}

void ACSControlWidget::updateConnectStatusSlot(QString)
{
}

void ACSControlWidget::on_stop_clicked()
{
	emit stopSignal();
}

void ACSControlWidget::updateConnectStatus(QString status)
{
}


void ACSControlWidget::on3DButtonClicked(int button)
{
	if (!button)
	{
		ui.reduceX->setEnabled(true);
		ui.RelInputX->setEnabled(true);
		ui.addX->setEnabled(true);
		ui.reduceY->setEnabled(true);
		ui.RelInputY->setEnabled(true);
		ui.addY->setEnabled(true);
		ui.reduceZ->setEnabled(true);
		ui.RelInputZ->setEnabled(true);
		ui.addZ->setEnabled(true);
		ui.AbsInputX->setEnabled(false);
		ui.AbsInputY->setEnabled(false);
		ui.AbsInputZ->setEnabled(false);
		ui.absMove->setEnabled(false);
	}
	else
	{
		ui.reduceX->setEnabled(false);
		ui.RelInputX->setEnabled(false);
		ui.addX->setEnabled(false);
		ui.reduceY->setEnabled(false);
		ui.RelInputY->setEnabled(false);
		ui.addY->setEnabled(false);
		ui.reduceZ->setEnabled(false);
		ui.RelInputZ->setEnabled(false);
		ui.addZ->setEnabled(false);
		ui.AbsInputX->setEnabled(true);
		ui.AbsInputY->setEnabled(true);
		ui.AbsInputZ->setEnabled(true);
		ui.absMove->setEnabled(true);
	}
}