#include "TipTiltWidget.h"
#include <QFileInfo>

TipTiltWidget::TipTiltWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	currentStep = 1000;
	currentSpeed = 1000;
	ui.stepInput->setText(QString::number(currentStep));
	ui.speedInput->setText(QString::number(currentSpeed));
}

TipTiltWidget::~TipTiltWidget()
{}

void TipTiltWidget::on_connect_clicked()
{
	emit connectSignal("");
}

void TipTiltWidget::on_disconnect_clicked()
{
	emit disconnectSignal();
}

void TipTiltWidget::on_stop_clicked()
{
	emit stopSignal();
}

void TipTiltWidget::on_setStep_clicked()
{
	currentStep = ui.stepInput->text().toInt();
}

void TipTiltWidget::on_setSpeed_clicked()
{
	currentSpeed = ui.speedInput->text().toInt();
	emit speedSignal(currentSpeed);
}

void TipTiltWidget::on_top_clicked()
{
	int y = ui.stepInput->text().toInt();
	//emit moveSignal(x, 0);
	emit moveSignal(0, y);
}

void TipTiltWidget::on_down_clicked()
{
	int y = ui.stepInput->text().toInt();
	//emit moveSignal(-x, 0);
	emit moveSignal(0, -y);
}

void TipTiltWidget::on_left_clicked()
{
	int x = ui.stepInput->text().toInt();
	//emit moveSignal(0, -y);
	emit moveSignal(-x, 0);
}

void TipTiltWidget::on_right_clicked()
{
	int x = ui.stepInput->text().toInt();
	//emit moveSignal(0, y);
	emit moveSignal(x, 0);
}

void TipTiltWidget::updateSpeedShow(int speed)
{
	currentSpeed = speed;
	ui.speedInput->setText(QString::number(currentSpeed));
}

void TipTiltWidget::updateConnectSlot(QString status)
{
	ui.connectStatus->setText(status);
}

void TipTiltWidget::updateMoveSlot(QString status)
{
	ui.moveStatus->setText(status);
}

void TipTiltWidget::on_coordinate_clicked()
{
	QLabel* labelImage = new QLabel(this, Qt::Dialog | Qt::WindowCloseButtonHint);
	labelImage->setWindowTitle("Motor coordinate system");
	QString imagePath = ":/Gen2WGMTFTester/images/motionControl/tipTilt.png";

	QFileInfo file(imagePath);
	QImage image;
	image.load(imagePath);
	labelImage->setFixedSize(400, 400);
	labelImage->setPixmap(QPixmap::fromImage(image.scaled(400, 400, Qt::KeepAspectRatio)));
	labelImage->show();
}

void TipTiltWidget::initUI()
{
	ui.connect->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);

	ui.disconnect->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);
	ui.stop->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);

	ui.coordinate->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);

	QFont font("Microsoft YaHei", 10, 50);

	ui.connectStatus->setStyleSheet("color:red;");
	ui.moveStatus->setStyleSheet("color:red;");
	ui.connectStatus->setFont(font);
	ui.moveStatus->setFont(font);

	//top
	QPixmap pixmap(":/Gen2WGMTFTester/images/motionControl/top.png");
	QPixmap fitpixmap = pixmap.scaled(20, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.top->setIcon(QIcon(fitpixmap));
	ui.top->setIconSize(QSize(30, 80));
	ui.top->setFlat(true);
	ui.top->setToolTip("Y+");
	
	ui.top->setStyleSheet("border: 0px");

	ui.top->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
		"QToolTip{font-size:15px;}");

	//down
	QPixmap pixmap1(":/Gen2WGMTFTester/images/motionControl/down.png");
	QPixmap fitpixmap1 = pixmap1.scaled(20, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.down->setIcon(QIcon(fitpixmap1));
	ui.down->setIconSize(QSize(30, 80));
	ui.down->setFlat(true);
	ui.down->setToolTip("Y-");
	ui.down->setStyleSheet("border: 0px");
	ui.down->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
		"QToolTip{font-size:15px;}");

	//left
	QPixmap pixmap2(":/Gen2WGMTFTester/images/motionControl/left.png");
	QPixmap fitpixmap2 = pixmap2.scaled(80, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.left ->setIcon(QIcon(fitpixmap2));
	ui.left->setIconSize(QSize(80, 30));
	ui.left->setFlat(true);
	ui.left->setToolTip("X-");
	ui.left->setStyleSheet("border: 0px");
	ui.left->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
		"QToolTip{font-size:15px;}");

	//right
	QPixmap pixmap3(":/Gen2WGMTFTester/images/motionControl/right.png");
	QPixmap fitpixmap3 = pixmap3.scaled(80, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.right->setIcon(QIcon(fitpixmap3));
	ui.right->setIconSize(QSize(80, 30));
	ui.right->setFlat(true);
	ui.right->setToolTip("X+");
	ui.right->setStyleSheet("border: 0px");
	ui.right->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
		"QToolTip{font-size:15px;}");

}