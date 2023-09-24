#pragma once

#include <QWidget>
#include "ui_CollimatorWidget.h"

class CollimatorWidget : public QWidget
{
	Q_OBJECT

public:
	CollimatorWidget(QWidget* parent = nullptr);
	~CollimatorWidget();

public slots:
	void updateConnectStatus(QString);
	void on_open_clicked();
	void on_close_clicked();
	void ShowCollimatorData(double xSecAngle, double ySecAngle);
	void receiveStatus(QString);

signals:
	void sendOpenSignal();
	void sendCloseSignal();

private:
	void initUI();

private:
	Ui::CollimatorWidgetClass ui;
	QString m_DataStatus;
};