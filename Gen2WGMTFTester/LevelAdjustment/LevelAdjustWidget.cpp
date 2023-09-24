#include "LevelAdjustWidget.h"
#include <QVBoxLayout>
#include "./common/ml_motion3D.h"
#include "PluginCore.h"
#include <QDialog>
#include "common/ml_motion2D.h"
#include <QDateTime>

LevelAdjustWidget::LevelAdjustWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	isMotion3DDialogClosed = true;
	isTipTiltDialogClosed = true;
}

LevelAdjustWidget::~LevelAdjustWidget()
{}

void LevelAdjustWidget::IsMotion3DDialogClosed(int isClosed)
{
	isMotion3DDialogClosed = !isClosed;
}

void LevelAdjustWidget::IsTipTiltDialogClosed(int isClosed)
{
	isTipTiltDialogClosed = !isClosed;
}

void LevelAdjustWidget::on_AutoParallel_clicked()
{
	emit AutoParallelSignal();
}

void LevelAdjustWidget::on_connectAll_clicked()
{
	emit connectAllSignal();
}

void LevelAdjustWidget::on_closeAll_clicked()
{
	emit stopAllSignal();
}

void LevelAdjustWidget::updateConnectStatus(QString status)
{
	ui.connectStatus->setText(status);
}

void LevelAdjustWidget::updateAdjustStatus(QString status)
{
	ui.adjustStatus->setText(status);
}

void LevelAdjustWidget::on_stop_clicked()
{
	emit stopAdjustSignal();
}

void LevelAdjustWidget::updateCollimatorShow(double x, double y)
{
	ui.CollimatorX->setText(QString::number(x));
	ui.CollimatorY->setText(QString::number(y));
}

void LevelAdjustWidget::updateGradienterShow(double x, double y)
{
	ui.GradienterX->setText(QString::number(x));
	ui.GradienterY->setText(QString::number(y));
}

void LevelAdjustWidget::updateMotion3DPosition(int x, int y, int z)
{
	ui.motion3DX->setText(QString::number((double)x / 1000));
	ui.motion3DY->setText(QString::number((double)y / 1000));
	ui.motion3DZ->setText(QString::number((double)z / 1000));
}

void LevelAdjustWidget::CollimatorNoSignal(QString msg)
{
	ui.CollimatorX->setText(msg);
	ui.CollimatorY->setText(msg);
}

void LevelAdjustWidget::initUI()
{
	QFont font1("Microsoft YaHei", 10, 50);

	ui.connectStatus->setStyleSheet("color:red;");
	ui.adjustStatus->setStyleSheet("color:red;");
	ui.connectStatus->setFont(font1);
	ui.adjustStatus->setFont(font1);

	QFont font("Microsoft YaHei", 13, 75);

	ui.motion3DX->setStyleSheet("color:red;");
	ui.motion3DY->setStyleSheet("color:red;");
	ui.motion3DZ->setStyleSheet("color:red;");
	ui.CollimatorX->setStyleSheet("color:green;");
	ui.CollimatorY->setStyleSheet("color:green;");
	ui.GradienterX->setStyleSheet("color:green;");
	ui.GradienterY->setStyleSheet("color:green;");
	ui.motion3DX->setFont(font);
	ui.motion3DY->setFont(font);
	ui.motion3DZ->setFont(font);
	ui.CollimatorX->setFont(font);
	ui.CollimatorY->setFont(font);
	ui.GradienterX->setFont(font);
	ui.GradienterY->setFont(font);

	/*ui.AutoParallel->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");
	ui.stop->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");*/

	ui.AutoParallel->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);
	ui.stop->setStyleSheet(
		"QPushButton{background-color: rgba(194, 194, 194);border-radius: 4px;border: none;width: 30px;height: 23px; color: rgb(180, 0, 0);font: bold 18px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);
}

void LevelAdjustWidget::on_motion3DManual_clicked()
{
	MLMotion3D* acsPlugin = PluginCore::getinstance()->GetPlugin<MLMotion3D>("ACSPlugin");

	if (acsPlugin && isMotion3DDialogClosed)
	{
		QDialog* dialog = new QDialog();
		isMotion3DDialogClosed = false;
		connect(dialog, SIGNAL(finished(int)), this, SLOT(IsMotion3DDialogClosed(int)));
		//dialog->setAttribute(Qt::WA_DeleteOnClose); 
		dialog->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
		QVBoxLayout* layout = new QVBoxLayout(dialog);
		dialog->setLayout(layout);
		layout->setMargin(0);
		layout->addWidget(acsPlugin->GetWidget());

		dialog->setWindowTitle(acsPlugin->GetWidget()->windowTitle());
		dialog->resize(400, 250);
		dialog->show();
	}

}


void LevelAdjustWidget::on_TipTiltManual_clicked()
{
	MLMotion2D* tipTiltPlugin = PluginCore::getinstance()->GetPlugin<MLMotion2D>("TipTiltPlugin");
	if (tipTiltPlugin && isTipTiltDialogClosed)
	{
		QDialog* dialog = new QDialog();
		isTipTiltDialogClosed = false;
		connect(dialog, SIGNAL(finished(int)), this, SLOT(IsTipTiltDialogClosed(int)));
		//dialog->setAttribute(Qt::WA_DeleteOnClose); 
		dialog->setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
		QVBoxLayout* layout = new QVBoxLayout(dialog);
		dialog->setLayout(layout);
		layout->setMargin(0);
		layout->addWidget(tipTiltPlugin->GetWidget());

		dialog->setWindowTitle(tipTiltPlugin->GetWidget()->windowTitle());
		dialog->resize(400, 250);
		dialog->show();
	}
}
