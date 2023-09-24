#pragma once

#include <QWidget>
#include "ui_ConfigWidget.h"
#include "common/configParam.h"

class ConfigWidget : public QWidget
{
	Q_OBJECT

public:
	ConfigWidget(QWidget *parent = nullptr);
	~ConfigWidget();

public:
	void ReadLevelAdjustConfig(AllConfig allConfig);

private slots:
	void updateLevelAdjustConfig(int currentRow, int currentColumn);

signals:
	void updateAllConfig(AllConfig, ConfigMode);

private:
	Ui::ConfigWidgetClass ui;
	AllConfig m_allConfig;
};
