#pragma once

#include <QWidget>
#include "ui_LEDControlWidget.h"

class LEDControlWidget : public QWidget
{
	Q_OBJECT

public:
	LEDControlWidget(QWidget *parent = nullptr);
	~LEDControlWidget();

private:
	void initUI();

private:
	Ui::LEDControlWidgetClass ui;
};
