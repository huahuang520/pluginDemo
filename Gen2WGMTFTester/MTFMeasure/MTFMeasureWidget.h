#pragma once

#include <QWidget>
#include "ui_MTFMeasureWidget.h"
//#include "mtfmeasure_global.h"
#include "MTFChartWidget.h"
#include "ESFChartWidget.h"
#include "LSFChartWidget.h"
#include "MTFSetting.h"
#include <QButtonGroup>
#include "LEDControl.h"

class MTFMeasureWidget : public QWidget
{
	Q_OBJECT

public:
	MTFMeasureWidget(QWidget *parent = nullptr);
	~MTFMeasureWidget();

private:
	void initStyle();
	void initToolWidget();

private slots:
	void on_switchImage_clicked();
	void onButtonClicked(int buttonIndex);
private:
	Ui::MTFMeasureWidgetClass ui;
	MTFChartWidget* m_MTFChartWidget;
	LSFChartWidget* m_LSFChartWidget;
	ESFChartWidget* m_ESFChartWidget;
	MTFSetting* m_MTFSetting;
	QButtonGroup* m_buttonGroup;

	MTFChartWidget* m_9MTFChartWidget[9];
	LSFChartWidget* m_9LSFChartWidget[9];
	ESFChartWidget* m_9ESFChartWidget[9];

	LEDControl* m_LEDControl;
};
