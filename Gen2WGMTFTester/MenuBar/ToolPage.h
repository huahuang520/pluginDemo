#pragma once

#include <QWidget>
#include <QTabWidget>
#include "ui_ToolPage.h"

class ToolPage : public QWidget
{
	Q_OBJECT

public:
	ToolPage(QWidget *parent = nullptr);
	~ToolPage();

public:
	void setToolBoxState();
private:
	void initToolPage();

signals:
	void updateTabWidgetShow(std::map<QWidget*, bool>);

private slots:
	void showTabWidgetPage(QTabWidget* tabWidget);
	void gradienterStateChanged(int state);
private:
	Ui::ToolPageClass ui;
	std::map<QWidget*, bool> tabWidgetState;
	//from tabBarPlugin
	QTabWidget* m_tabWidget;
};
