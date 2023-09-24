#pragma once

#include <QWidget>
#include "ui_LevelAdjustWidget.h"

class LevelAdjustWidget : public QWidget
{
	Q_OBJECT

public:
	LevelAdjustWidget(QWidget *parent = nullptr);
	~LevelAdjustWidget();

public slots:
	void on_motion3DManual_clicked();
	void on_TipTiltManual_clicked();
	void IsMotion3DDialogClosed(int isClosed);
	void IsTipTiltDialogClosed(int isClosed);
	void on_AutoParallel_clicked();
	void on_connectAll_clicked();
	void on_closeAll_clicked();
	void updateConnectStatus(QString status);
	void updateAdjustStatus(QString status);
	void on_stop_clicked();
	void updateCollimatorShow(double x, double y);
	void updateGradienterShow(double x, double y);
	void updateMotion3DPosition(int x, int y, int z);
	void CollimatorNoSignal(QString msg);
signals:
	void AutoParallelSignal();
	void connectAllSignal();
	void stopAllSignal();
	void stopAdjustSignal();
private:
	void initUI();

private:
	Ui::LevelAdjustWidgetClass ui;
	bool isMotion3DDialogClosed;
	bool isTipTiltDialogClosed;
};
