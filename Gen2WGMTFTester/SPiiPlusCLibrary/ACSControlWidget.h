#pragma once

#include <QWidget>
#include <QButtonGroup>
#include "ui_ACSControlWidget.h"
#include "./common/ml_motion3D.h"

class ACSControlWidget : public QWidget, public MLMotion3DCallback
{
	Q_OBJECT

public:
	ACSControlWidget(QWidget *parent = nullptr);
	~ACSControlWidget();
public:
	virtual void NotifyMotion3DStateChanged(MLMotionState old_state, MLMotionState new_state);
	virtual void NotifyMotion3DPosition(int x, int y, int z);
private:
	void initUI();

signals:
	void connectSignal();
	void disconnectSignal();
	void stopSignal();
private slots:
	void on3DButtonClicked(int button);
	void on_connect_clicked();
	void on_disconnect_clicked();
	void updateConnectStatusSlot(QString);
	void on_stop_clicked();
	void updateConnectStatus(QString status);
private:
	Ui::ACSControlWidgetClass ui;
	QButtonGroup* m_buttonGroup3D;
};
