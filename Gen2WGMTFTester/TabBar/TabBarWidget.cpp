#include "TabBarWidget.h"
#include <QTabBar>

TabBarWidget::TabBarWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setupUI();
}

TabBarWidget::~TabBarWidget()
{
}

void TabBarWidget::addWidget(QWidget * widget, QString & title, QIcon & icon)
{
	/*m_tabWidget->insertTab(-1, widget, title);
	int index = m_tabWidget->indexOf(widget);
	m_tabWidget->setTabIcon(index, icon);*/
	m_CustomTabWidget->addTab(widget, icon, title);
}

void TabBarWidget::setupUI()
{
	/*m_tabWidget = new QTabWidget();

	m_tabWidget->tabBar()->setStyleSheet("QTabBar{background-color:rgba(180,180,180,100%);\
                                            color: black;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QTabBar:hover{background-color:rgb(250, 200, 200); color: black;}"
		"QTabBar:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
		"QTabBar::tab:selected{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0.278409 rgba(118, 159, 193, 255), stop:1 rgba(255, 255, 255, 255));}"
	);*/
	m_CustomTabWidget = new CustomTabWidget();
	connect(m_CustomTabWidget, SIGNAL(updateTabWidgetPage(QTabWidget*)), this, SIGNAL(updateTabWidgetPage(QTabWidget*)));
	//menuBarPlugin send signal to this function
	connect(this, SIGNAL(updateTabWidgetShow(std::map<QWidget*, bool>)), m_CustomTabWidget, SLOT(updateTabWidgetShow(std::map<QWidget*, bool>)));

	m_CustomTabWidget->tabBar()->setStyleSheet("QTabBar{background-color:rgba(180,180,180,100%);\
                                            color: black;   border-radius: 15px;  border: 2px groove gray; border-style: outset;}"
		"QTabBar:hover{background-color:rgb(250, 200, 200); color: black;}"
		"QTabBar:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
		"QTabBar::tab:selected{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0.278409 rgba(118, 159, 193, 255), stop:1 rgba(255, 255, 255, 255));}"
	);

	flag = true;
	m_mainSplitter = new QSplitter(Qt::Horizontal, this);
	m_mainSplitter->setFrameStyle(QFrame::NoFrame);
	m_mainSplitter->setHandleWidth(0);

	zoomButton = new QPushButton(this);
	zoomButton->setStyleSheet(
		"QPushButton{ background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0.2 rgba(238, 238,238, 255),stop:0.5 #C3E5E7 ,stop:0.8 rgba(238, 238, 238, 255)); border:none; padding:0px;}"
		"QPushButton:hover{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0.2 rgba(238, 238,238, 255),stop:0.5 #C3E5FF ,stop:0.8 rgba(238, 238, 238, 255)); border:none; padding:0px;border-style: outset;}"
	);
	zoomButton->installEventFilter(this);
	zoomButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	zoomButton->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/right_arrow_disabled.png"));
	zoomButton->setFixedWidth(8);
	zoomButton->setFocusPolicy(Qt::NoFocus);
	//zoomButton->setStyleSheet("background: #E8E8E8; border: none; padding: 0px;");
	//m_mainSplitter->addWidget(m_tabWidget);
	m_mainSplitter->addWidget(m_CustomTabWidget);
	//this->layout()->addItem(new QSpacerItem(0, 160, QSizePolicy::Expanding, QSizePolicy::Fixed));
	this->layout()->addWidget(zoomButton);
	this->layout()->addWidget(m_mainSplitter);

	connect(zoomButton, SIGNAL(clicked()), this, SLOT(onZoomClicked()));
	this->setMaximumWidth(500);
	this->setMinimumWidth(500);
}

bool TabBarWidget::eventFilter(QObject* watched, QEvent* Event)
{
	if (watched == zoomButton && Event->type() == QEvent::HoverMove)
	{
		this->setCursor(Qt::PointingHandCursor);
	}
	else
	{
		this->setCursor(Qt::ArrowCursor);
	}
	return QWidget::eventFilter(watched, Event);
}

void TabBarWidget::onZoomClicked()
{
	if (flag)   // 根据当前的展开状态，进行切换
	{
		flag = false;
		zoomButton->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/left_arrow_disabled.png"));
		//m_tabWidget->setVisible(false);
		m_CustomTabWidget->setVisible(false);
		this->setMaximumWidth(18);
		this->setMinimumWidth(18);
	}
	else
	{
		flag = true;
		zoomButton->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/right_arrow_disabled.png"));
	    //m_tabWidget->setVisible(true);
		m_CustomTabWidget->setVisible(true);
		this->setMinimumWidth(500);
		this->setMaximumWidth(500);
	}
}