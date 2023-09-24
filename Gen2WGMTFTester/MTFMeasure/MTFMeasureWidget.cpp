#include "MTFMeasureWidget.h"
#include "CollpasePagePlugin.h"
#include "PluginCore.h"
#include <QLabel>
#include "LEDPlugin.h"
#include "./common/ml_motion3D.h"
MTFMeasureWidget::MTFMeasureWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initStyle();
	initToolWidget();
}

MTFMeasureWidget::~MTFMeasureWidget()
{
	delete m_MTFChartWidget;
	delete m_LSFChartWidget;
	delete m_ESFChartWidget;
	for (int i = 0; i < 9; i++)
	{
		delete m_9MTFChartWidget[i];
		delete m_9LSFChartWidget[i];
		delete m_9ESFChartWidget[i];
	}
}

void MTFMeasureWidget::initStyle()
{
	ui.open->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}" 
		"QPushButton:hover{background-color:white; color: black;}"  
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");
	ui.open->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/camera_open.png"));

	ui.close->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");
	ui.close->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/camera_close.png"));

	ui.switchImage->setStyleSheet("QPushButton{background-color:rgba(180,180,180,100%);\
                                            color: white;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QPushButton:hover{background-color:white; color: black;}"
		"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");
	ui.switchImage->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/switch.png"));
}

void MTFMeasureWidget::initToolWidget()
{
	CollpasePagePlugin* collpasePagePlugin = PluginCore::getinstance()->GetPlugin<CollpasePagePlugin>("CollpasePagePlugin", "MTFTool");
	if (collpasePagePlugin)
	{
		m_MTFSetting = new MTFSetting(this);

		collpasePagePlugin->GetWidget()->setParent(ui.groupBox);
		collpasePagePlugin->GetWidget()->setMouseTracking(true);
		collpasePagePlugin->GetWidget()->parentWidget()->setMouseTracking(true);
		collpasePagePlugin->SetLayout((QVBoxLayout*)ui.groupBox->layout(), -1);

		//ui.groupBox->layout()->addWidget(label);
		collpasePagePlugin->addWidget(m_MTFSetting, "MTFSetting", QIcon(":/Gen2WGMTFTester/images/MTF/MTFsetting.png"));
	}
	
	ui.groupBox->setMinimumWidth(600);

	QHBoxLayout* qHBoxLayout = new QHBoxLayout(this);
	ui.groupBox_2->setLayout(qHBoxLayout);
	qHBoxLayout->setContentsMargins(0,0,0,0);
	qHBoxLayout->setSpacing(0);
	//qHBoxLayout->addStretch();

	//motion3D control
	MLMotion3D* acsPlugin = PluginCore::getinstance()->GetPlugin<MLMotion3D>("ACSPlugin");
	if (acsPlugin)
	{
		qHBoxLayout->addWidget(acsPlugin->GetWidget());
		//motionControlPlugin->getWidget()->setMinimumHeight(300);
		acsPlugin->GetWidget()->setMaximumWidth(400);
	}

	//init charts
	m_MTFChartWidget = new MTFChartWidget(this);
	m_LSFChartWidget = new LSFChartWidget(this);
	m_ESFChartWidget = new ESFChartWidget(this);
	qHBoxLayout->addWidget(m_MTFChartWidget);
	qHBoxLayout->addWidget(m_LSFChartWidget);
	qHBoxLayout->addWidget(m_ESFChartWidget);

	//qHBoxLayout->addStretch(1);

	//Nine Palace Button Group
	m_buttonGroup = new QButtonGroup(this);
	m_buttonGroup->addButton(ui.imageShow, 0);
	m_buttonGroup->addButton(ui.mtfShow, 1);
	m_buttonGroup->addButton(ui.lsfShow, 2);
	m_buttonGroup->addButton(ui.esfShow, 3);
	connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onButtonClicked(int)));
	m_buttonGroup->button(0)->setChecked(true);
	emit m_buttonGroup->buttonClicked(0);

	for (int i = 0; i < 9; i++)
	{
		m_9MTFChartWidget[i] = new MTFChartWidget(this);
		m_9LSFChartWidget[i] = new LSFChartWidget(this);
		m_9ESFChartWidget[i] = new ESFChartWidget(this);
		switch (i)
		{
		case 0:
			ui.stackedWidget_2->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_2->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_2->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 1:
			ui.stackedWidget_4->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_4->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_4->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 2:
			ui.stackedWidget_3->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_3->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_3->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 3:
			ui.stackedWidget_8->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_8->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_8->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 4:
			ui.stackedWidget_9->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_9->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_9->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 5:
			ui.stackedWidget_10->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_10->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_10->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 6:
			ui.stackedWidget_6->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_6->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_6->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 7:
			ui.stackedWidget_5->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_5->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_5->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		case 8:
			ui.stackedWidget_7->widget(1)->layout()->addWidget(m_9MTFChartWidget[i]);
			ui.stackedWidget_7->widget(2)->layout()->addWidget(m_9LSFChartWidget[i]);
			ui.stackedWidget_7->widget(3)->layout()->addWidget(m_9ESFChartWidget[i]);
			break;
		}
	}

	//LED control
	LEDPlugin* ledPlugin = PluginCore::getinstance()->GetPlugin<LEDPlugin>("LEDPlugin");
	if (collpasePagePlugin && ledPlugin)
	{
		collpasePagePlugin->addWidget(ledPlugin->GetWidget(), "LEDControl", QIcon(":/Gen2WGMTFTester/images/MTF/LED.jpg"));
		collpasePagePlugin->setItemExpand(0, false);
		collpasePagePlugin->setItemExpand(1, false);
	}
}

void MTFMeasureWidget::onButtonClicked(int buttonIndex)
{
	if (buttonIndex == 0)
	{
		ui.stackedWidget_2->setCurrentIndex(0);
		ui.stackedWidget_4->setCurrentIndex(0);
		ui.stackedWidget_3->setCurrentIndex(0);
		ui.stackedWidget_8->setCurrentIndex(0);
		ui.stackedWidget_9->setCurrentIndex(0);
		ui.stackedWidget_10->setCurrentIndex(0);
		ui.stackedWidget_6->setCurrentIndex(0);
		ui.stackedWidget_5->setCurrentIndex(0);
		ui.stackedWidget_7->setCurrentIndex(0);
	}
	else if (buttonIndex == 1)
	{
		ui.stackedWidget_2->setCurrentIndex(1);
		ui.stackedWidget_4->setCurrentIndex(1);
		ui.stackedWidget_3->setCurrentIndex(1);
		ui.stackedWidget_8->setCurrentIndex(1);
		ui.stackedWidget_9->setCurrentIndex(1);
		ui.stackedWidget_10->setCurrentIndex(1);
		ui.stackedWidget_6->setCurrentIndex(1);
		ui.stackedWidget_5->setCurrentIndex(1);
		ui.stackedWidget_7->setCurrentIndex(1);
	}
	else if (buttonIndex == 2)
	{
		ui.stackedWidget_2->setCurrentIndex(2);
		ui.stackedWidget_4->setCurrentIndex(2);
		ui.stackedWidget_3->setCurrentIndex(2);
		ui.stackedWidget_8->setCurrentIndex(2);
		ui.stackedWidget_9->setCurrentIndex(2);
		ui.stackedWidget_10->setCurrentIndex(2);
		ui.stackedWidget_6->setCurrentIndex(2);
		ui.stackedWidget_5->setCurrentIndex(2);
		ui.stackedWidget_7->setCurrentIndex(2);
	}
	else
	{
		ui.stackedWidget_2->setCurrentIndex(3);
		ui.stackedWidget_4->setCurrentIndex(3);
		ui.stackedWidget_3->setCurrentIndex(3);
		ui.stackedWidget_8->setCurrentIndex(3);
		ui.stackedWidget_9->setCurrentIndex(3);
		ui.stackedWidget_10->setCurrentIndex(3);
		ui.stackedWidget_6->setCurrentIndex(3);
		ui.stackedWidget_5->setCurrentIndex(3);
		ui.stackedWidget_7->setCurrentIndex(3);
	}
}

void MTFMeasureWidget::on_switchImage_clicked()
{
	static bool flag = false;
	if (flag)
	{
		ui.stackedWidget->setCurrentIndex(0);
		flag = false;
	}
	else
	{
		ui.stackedWidget->setCurrentIndex(1);
		flag = true;
	}
}
