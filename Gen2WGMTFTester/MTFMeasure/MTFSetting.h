#pragma once

#include <QWidget>
#include "ui_MTFSetting.h"

class MTFSetting : public QWidget
{
	Q_OBJECT

public:
	MTFSetting(QWidget *parent = nullptr);
	~MTFSetting();

private:
	void initUI();

private:
	Ui::MTFSettingClass ui;
};
