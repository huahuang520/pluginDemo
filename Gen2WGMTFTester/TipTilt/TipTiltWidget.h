#pragma once

#include <QWidget>
#include "ui_TipTiltWidget.h"

class TipTiltWidget : public QWidget
{
	Q_OBJECT

public:
	TipTiltWidget(QWidget *parent = nullptr);
	~TipTiltWidget();

public slots:
	void on_connect_clicked();
	void on_disconnect_clicked();
	void on_stop_clicked();
	void on_setStep_clicked();
	void on_setSpeed_clicked();
	void on_top_clicked();
	void on_down_clicked();
	void on_left_clicked();
	void on_right_clicked();
	void updateSpeedShow(int);
	void updateConnectSlot(QString status);
	void updateMoveSlot(QString status);
	void on_coordinate_clicked();
signals:
	void connectSignal(const char* type_name);
	void disconnectSignal();
	void moveSignal(int, int);
	void stopSignal();
	void speedSignal(int);
private:
	void initUI();

private:
	Ui::TipTiltWidgetClass ui;
	int currentStep;
	int currentSpeed;
};
