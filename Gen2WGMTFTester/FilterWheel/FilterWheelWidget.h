#pragma once

#include <QWidget>
#include "ui_FilterWheelWidget.h"

class FilterWheelWidget : public QWidget
{
	Q_OBJECT

public:
	FilterWheelWidget(QWidget *parent = nullptr);
	~FilterWheelWidget();

public:
	bool Initconfig(std::list<std::string> configPath);

public slots:
	void on_setRxAxis_clicked();

private:
	void initStyle();

private:
	Ui::FilterWheelWidgetClass ui;

	std::vector<std::string> m_NDPosList;
	std::vector<std::string> m_NXYZPosList;
	std::vector<std::string> m_RxPosList;
	std::vector<std::string> m_RxAxisPosList;
};
