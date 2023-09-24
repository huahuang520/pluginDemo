#include "MTFSetting.h"

MTFSetting::MTFSetting(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//initUI();
}

MTFSetting::~MTFSetting()
{}

void MTFSetting::initUI()
{
	ui.setMTFMin->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.setMTFMax->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.resetMTF->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.setLSFMin->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.setLSFMax->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.resetLSF->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.setESFMin->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.setESFMax->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");

	ui.resetESF->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;font: bold 17px;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");
}
