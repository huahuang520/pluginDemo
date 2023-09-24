#include "LEDControlWidget.h"

LEDControlWidget::LEDControlWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
}

LEDControlWidget::~LEDControlWidget()
{
}

void LEDControlWidget::initUI()
{
	ui.openAllLED->setStyleSheet(
		"QPushButton{background-color: rgba(184, 180, 165);border-radius: 4px;border: none;width: 35px;height: 35px; color: rgb(180, 0, 0);font: bold 25px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);

	ui.closeAllLED->setStyleSheet(
		"QPushButton{background-color: rgba(184, 180, 165);border-radius: 4px;border: none;width: 35px;height: 35px; color: rgb(180, 0, 0);font: bold 25px;}"
		"QPushButton:hover{background: rgb(144, 140, 123);}"
		"QPushButton:pressed {background: rgb(125, 125, 125); }"
	);

	ui.RLED->setStyleSheet(
		"QPushButton{color: rgb(255, 255, 255);background-color: rgba(255, 0, 0, 125);border:1px outset rgb(255, 255, 255);border-radius:8px;text-align: center center;width: 25px;height: 25px;font: bold 17px;}"
		"QPushButton:hover{background-color: rgba(255, 0, 0, 80%);border:2px outset rgba(36, 36, 36, 0);}"
		"QPushButton:pressed {background-color: rgba(255, 0, 0, 90%);border:4px outset rgba(36, 36, 36, 0);}"
	);

	ui.GLED->setStyleSheet(
		"QPushButton{color: rgb(255, 255, 255);background-color: rgba(0, 255, 0, 125);border:1px outset rgb(255, 255, 255);border-radius:8px;text-align: center center;width: 25px;height: 25px;font: bold 17px;}"
		"QPushButton:hover{background-color: rgba(0, 255, 0, 80%);border:2px outset rgba(36, 36, 36, 0);}"
		"QPushButton:pressed {background-color: rgba(0, 255, 0, 90%);border:4px outset rgba(36, 36, 36, 0);}"
	);

	ui.BLED->setStyleSheet(
		"QPushButton{color: rgb(255, 255, 255);background-color: rgba(0, 0, 255, 125);border:1px outset rgb(255, 255, 255);border-radius:8px;text-align: center center;width: 25px;height: 25px;font: bold 17px;}"
		"QPushButton:hover{background-color: rgba(0, 0, 255, 80%);border:2px outset rgba(36, 36, 36, 0);}"
		"QPushButton:pressed {background-color: rgba(0, 0, 255, 90%);border:4px outset rgba(36, 36, 36, 0);}"
	);
}
