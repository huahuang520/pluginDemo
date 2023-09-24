#pragma once

#include <QWidget>
#include "ui_GradienterWidget.h"

class GradienterWidget : public QWidget
{
	Q_OBJECT

public:
	GradienterWidget(QWidget *parent = nullptr);
	~GradienterWidget();

public slots:
	void updateConnectStatus(QString);
	void on_open_clicked();
	void on_close_clicked();
	void ShowGradienterData(double xAngle, double yAngle);
	void receiveStatus(QString);

signals:
	void sendOpenSignal(std::string);
	void sendCloseSignal();

private:
	void initUI();
	void GetComPort();
private:
	Ui::GradienterWidgetClass ui;
	QString m_DataStatus;
};
