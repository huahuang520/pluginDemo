#pragma once

#include <QWidget>
#include "ui_ConfigPage.h"

class ConfigPage : public QWidget
{
	Q_OBJECT

public:
	ConfigPage(QWidget *parent = nullptr);
	~ConfigPage();
	
public slots:
	void on_config_clicked();
	void IsConfigPageClosed(int close);
private:
	void InitUI();

private:
	Ui::ConfigPageClass ui;
	bool m_isConfigPageClosed;
};
