#include "Gen2WGMTFTester.h"

#include <QHash>
#include <QVariant>
#include "PluginCore.h"
#include "LogPlugin.h"
#include "CollpasePagePlugin.h"
#include "navigationBarPlugin.h"
#include "MTFMeasurePlugin.h"
#include "TabBarPlugin.h"
#include "MenuBarPlugin.h"
#include "common/ml_motion2D.h"
#include "LevelAdjustPlugin.h"
#include "CollimatorPlugin.h"
#include "GradienterPlugin.h"
#include "FilterWheelPlugin.h"

Gen2WGMTFTester::Gen2WGMTFTester(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowIcon(QIcon(":/Gen2WGMTFTester/images/logo/mlico.ico"));
    setMouseTracking(true);
    ui.centralWidget->setMouseTracking(true);
    InitializationInterface();

    
}

Gen2WGMTFTester::~Gen2WGMTFTester()
{
    delete PluginCore::getinstance();
}

void Gen2WGMTFTester::InitializationInterface()
{
    InitLayout();
    CreateMenuBar();
    CreateLeftNavigationBar();
    CreateLogBar();
    InitStatusBar();
    CreateTabBar();
    
}

void Gen2WGMTFTester::CreateMenuBar()
{
    MenuBarPlugin* menuPlugin = PluginCore::getinstance()->GetPlugin<MenuBarPlugin>("MenuBarPlugin");
    if (menuPlugin)
        menuPlugin->SetMenuBar(this);
}

void Gen2WGMTFTester::CreateMenuBarDiscard()
{
    QMenuBar* bar = this->menuBar();
    this->setMenuBar(bar);
    bar->setStyleSheet("QMenuBar{background:#FFFFF5;}"
        "QMenuBar::item:selected{background-color:rgba(150,200,150); color:white; border-radius:10px; border:2px groove gray; border-style:outset;}"
        "QMenuBar::item:pressed{background-color:rgba(85,170,255); border-style:inset;}"
    );

    QMenu* fileMenu = bar->addMenu("File");
    /*fileMenu->setStyleSheet( "QMenu{background:#FFFFF5;}"
        "QMenu::item:selected{background-color:rgba(150,200,150); color:white; border-radius:10px; border:2px groove gray; border-style:outset;}");*/

    QMenu* helpMenu = bar->addMenu("Help");

    QAction* quitAction = fileMenu->addAction(QIcon("./images/close.png"), "close");
    //quitAction->setStatusTip("close widget!");

    connect(quitAction, &QAction::triggered, [=]() {
        this->close();
        });

    
    fileMenu->setToolTipsVisible(true);
    fileMenu->setStyleSheet("QToolTip{border:1px solid rgb(118, 118, 118); background-color: #ffffFF; color:#484848; font-size:12px;font: bold; text-align: center;}");
    QString strText = "---Close Window--- <br><br>Close the current window to exit the program!";
    ///Method I
    if (!strText.isEmpty())
    {
        QRegExp valueRegExp(QString("(%1)").arg("Close Window"));
        valueRegExp.setCaseSensitivity(Qt::CaseInsensitive);
        strText = strText.replace(valueRegExp, "<font style='font-size:13px; background-color:white; color:black; '>\\1</font>");

    }
    quitAction->setToolTip(strText);

    //Method II
   /* QString strBlueColor = QString("<font color=black>%1").arg(QString::fromWCharArray(L" --Close Window--"));
    QString strRedColor = QString("<font color=black>%1").arg(QString::fromWCharArray(L"Close the current window to exit the program!"));
    quitAction->setToolTip(strBlueColor + "<br>" + strRedColor);*/
    
}

void Gen2WGMTFTester::CreateActions()
{
}

void Gen2WGMTFTester::CreateToolBars()
{
}

void Gen2WGMTFTester::CreateLeftNavigationBar_discard()
{
    //mainStackedWidget = new QStackedWidget(ui.centralWidget);
    //mainStackedWidget->addWidget(new QLabel("helloworld"));
    //mainStackedWidget->addWidget(new QLabel("hahahaha"));
    //
    //navigationBarWidget = new QWidget(ui.centralWidget);
    //navigationBarWidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    //navigationBarWidget->setStyleSheet("QWidget{background-color:rgba(20,33,55);}");
    //navigationBarWidget->setMinimumWidth(80);
    //navigationBarWidget->setMaximumWidth(80);


    //ui.centralWidget->layout()->addWidget(navigationBarWidget);
    //ui.centralWidget->layout()->addItem(m_centerQVBoxLayout);
    ////m_centerQVBoxLayout->setSpacing(0);

    //
    //m_centerQVBoxLayout->addWidget(mainStackedWidget);
    //mainStackedWidget->setMouseTracking(true);
    //mainStackedWidget->parentWidget()->setMouseTracking(true);
    //mainStackedWidget->widget(0)->setMouseTracking(true);
    //mainStackedWidget->widget(1)->setMouseTracking(true);

    //menuNavigationBar = new VerticalMenuNavigationBar(mainStackedWidget, navigationBarWidget);
}

void Gen2WGMTFTester::CreateLeftNavigationBar_discard2()
{
    /*mainStackedWidget = new QStackedWidget(ui.centralWidget);

    navigationBarWidget = new QWidget(ui.centralWidget);
    navigationBarWidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    navigationBarWidget->setStyleSheet("QWidget{background-color:rgba(20,33,55);}");
    navigationBarWidget->setMinimumWidth(80);
    navigationBarWidget->setMaximumWidth(80);

    ui.centralWidget->layout()->addWidget(navigationBarWidget);
    ui.centralWidget->layout()->addItem(m_centerQVBoxLayout);

    m_centerQVBoxLayout->addWidget(mainStackedWidget);
    mainStackedWidget->setMouseTracking(true);
    mainStackedWidget->parentWidget()->setMouseTracking(true);
    ui.centralWidget->setMouseTracking(true);

    menuNavigationBar = new VerticalMenuNavigationBar(mainStackedWidget, navigationBarWidget);
    menuNavigationBar->addMainMenu("MTFMeasure", QIcon(":/Gen2WGMTFTester/images/MTF.png"));
    menuNavigationBar->addMenuWidget("MTF", 0, new QLabel("helloworld"));
    menuNavigationBar->addMenuWidget("LED", 0, new QLabel("nihao"));
    menuNavigationBar->addMainMenu("FindFiducial", QIcon(":/Gen2WGMTFTester/images/findFiducial.png"));
    menuNavigationBar->addMenuWidget("FindFiducial", 1, new QLabel("aaaaaaaaaaa"));
    menuNavigationBar->addMainMenu("EyeBoxScan", QIcon(":/Gen2WGMTFTester/images/eyeBoxScan.jpg"));
    menuNavigationBar->addMenuWidget("EyeBoxScan", 2, new QLabel("bbbbbbbbbb"));*/
}

void Gen2WGMTFTester::CreateLogBar()
{
    //AdvancedToolBox* advancedToolBox = new AdvancedToolBox(ui.centralWidget);
    //advancedToolBox->setMouseTracking(true);
    //advancedToolBox->parentWidget()->setMouseTracking(true);

    //advancedToolBox->setItemExpand(1);
   // advancedToolBox->SetLayout(m_centerQVBoxLayout, -1);
    LogPlugin* logPlugin = PluginCore::getinstance()->GetPlugin<LogPlugin>("LogPlugin");
    CollpasePagePlugin* collpasePagePlugin = PluginCore::getinstance()->GetPlugin<CollpasePagePlugin>("CollpasePagePlugin");
    if (logPlugin && collpasePagePlugin)
    {
        collpasePagePlugin->GetWidget()->setParent(ui.centralWidget);
        //collpasePagePlugin->GetWidget()->setMouseTracking(true);
        //collpasePagePlugin->GetWidget()->parentWidget()->setMouseTracking(true);
        //collpasePagePlugin->setItemExpand(1); //Set the folded state to the expanded state
        collpasePagePlugin->SetLayout(m_centerQVBoxLayout, -1); //Put the folded plug-in into the layout
    }

    /*LoggingWrapper::getinstance()->SetParent(advancedToolBox);
    QWidget* widget = LoggingWrapper::getinstance()->GetLogWidget();*/

    if (logPlugin && collpasePagePlugin)
    {
        logPlugin->SetParent(collpasePagePlugin->GetWidget()); //Set Parent Class
        QWidget* widget = logPlugin->GetLogWidget();

        //Put the Logwidget into the folding plug-in
        collpasePagePlugin->addWidget(widget, "Log", QIcon(":/Gen2WGMTFTester/images/routine/log.jpg"));
        logPlugin->Info("be ready...");
    }
    
   
}

void Gen2WGMTFTester::InitStatusBar()
{
    statusBar()->setMaximumHeight(35);
    statusBar()->setStyleSheet(
        "QStatusBar{color:rgb(255,255,255);}"
        "QStatusBar{font-size: 12px;}"
        "QStatusBar{border:2px groove gray;}"
        "QStatusBar{background-color:rgba(246,248,244);}"
        "QStatusBar::item{border:0px}"
    );
    QLabel* text = new QLabel("be ready...");
    text->setStyleSheet(//"QLabel{color:rgb(240,0,0);}"
        "QLabel{font-size:14px;}"
    );

    statusBar()->addWidget(text);
}

void Gen2WGMTFTester::InitLayout()
{
    m_centerQVBoxLayout = new QVBoxLayout();
    m_centerQHBoxLayout = new QHBoxLayout(this);
    ui.centralWidget->setLayout(m_centerQHBoxLayout);
    m_centerQHBoxLayout->setContentsMargins(0, 0, 0, 0);
    ui.centralWidget->layout()->setSpacing(0);
}


void Gen2WGMTFTester::CreateLeftNavigationBar()
{
    NavigationBarPlugin* LeftNavigationBarPlugin = PluginCore::getinstance()->GetPlugin<NavigationBarPlugin>("NavigationBarPlugin");
    MTFMeasurePlugin* mtfMeasurePlugin = PluginCore::getinstance()->GetPlugin<MTFMeasurePlugin>("MTFMeasurePlugin");

    if (LeftNavigationBarPlugin)
    {
        ui.centralWidget->layout()->addWidget(LeftNavigationBarPlugin->GetWidget());
        ui.centralWidget->layout()->addItem(m_centerQVBoxLayout);
        //ui.centralWidget->setMouseTracking(true);

        LeftNavigationBarPlugin->SetParent(ui.centralWidget, m_centerQVBoxLayout);

        LeftNavigationBarPlugin->addMainMenu("MTFMeasure", QIcon(":/Gen2WGMTFTester/images/MTF/MTF.png"));
        if (mtfMeasurePlugin)
            LeftNavigationBarPlugin->addMenuWidget("MTF", 0, mtfMeasurePlugin->GetWidget());
        
        LeftNavigationBarPlugin->addMainMenu("FindFiducial", QIcon(":/Gen2WGMTFTester/images/fiducial/findFiducial.png"));
        LeftNavigationBarPlugin->addMenuWidget("FindFiducial", 1, new QLabel("aaaaaaaaaaa"));
        LeftNavigationBarPlugin->addMainMenu("EyeBoxScan", QIcon(":/Gen2WGMTFTester/images/fiducial/eyeBoxScan.jpg"));
        LeftNavigationBarPlugin->addMenuWidget("EyeBoxScan", 2, new QLabel("bbbbbbbbbb"));
    }
    
}

void Gen2WGMTFTester::CreateTabBar()
{
    TabBarPlugin* tabBarPlugin = PluginCore::getinstance()->GetPlugin<TabBarPlugin>("TabBarPlugin");
    if (tabBarPlugin)
        ui.centralWidget->layout()->addWidget(tabBarPlugin->getWidget());

    LevelAdjustPlugin* levelAdjustPlugin = PluginCore::getinstance()->GetPlugin<LevelAdjustPlugin>("LevelAdjustPlugin");
    if (levelAdjustPlugin && tabBarPlugin)
        tabBarPlugin->addWidget(levelAdjustPlugin->GetWidget(), "LevelAdjust", QIcon());

    MLMotion2D* tipTiltPlugin = PluginCore::getinstance()->GetPlugin<MLMotion2D>("TipTiltPlugin");
    if (tipTiltPlugin && tabBarPlugin)
        tabBarPlugin->addWidget(tipTiltPlugin->GetWidget(), "TipTilt", QIcon());

    CollimatorPlugin* collPlugin = PluginCore::getinstance()->GetPlugin<CollimatorPlugin>("CollimatorPlugin");
    if (collPlugin && tabBarPlugin)
        tabBarPlugin->addWidget(collPlugin->GetWidget(), "Collimator", QIcon());

    GradienterPlugin* gradPlugin = PluginCore::getinstance()->GetPlugin<GradienterPlugin>("GradienterPlugin");
    if (gradPlugin && tabBarPlugin)
        tabBarPlugin->addWidget(gradPlugin->GetWidget(), "Gradienter", QIcon());

    FilterWheelPlugin* filterWheelPlugin = PluginCore::getinstance()->GetPlugin<FilterWheelPlugin>("FilterWheelPlugin");
    if (filterWheelPlugin && tabBarPlugin)
    {
        std::list<std::string> configPath;
        configPath.push_back("E:\\pluginDemo\\Gen2WGMTFTester\\FilterWheel\\FilterConfig.json");
        configPath.push_back("E:\\pluginDemo\\Gen2WGMTFTester\\FilterWheel\\RXFilterConfig.json");
        filterWheelPlugin->InitConfig(configPath);
        tabBarPlugin->addWidget(filterWheelPlugin->GetWidget(), "Gradienter", QIcon());
    }
}

void Gen2WGMTFTester::mousePressEvent(QMouseEvent* event)
{
   /* if (!menuNavigationBar->menuDialog->isHidden() && ((QCursor::pos().x() < menuNavigationBar->menuDialog->mapToGlobal(QPoint(0, 0)).x() || QCursor::pos().x() > menuNavigationBar->menuDialog->mapToGlobal(QPoint(0, 0)).x() + menuNavigationBar->menuDialog->rect().width()) || (QCursor::pos().y() < menuNavigationBar->menuDialog->mapToGlobal(QPoint(0, 0)).y() || QCursor::pos().y() > menuNavigationBar->menuDialog->mapToGlobal(QPoint(0, 0)).y() + menuNavigationBar->menuDialog->rect().height())))
        menuNavigationBar->hideAndRefresh();*/
}

void Gen2WGMTFTester::mouseMoveEvent(QMouseEvent* event)
{
    NavigationBarPlugin* LeftNavigationBarPlugin = PluginCore::getinstance()->GetPlugin<NavigationBarPlugin>("NavigationBarPlugin");
    if (LeftNavigationBarPlugin) LeftNavigationBarPlugin->hideAndRefresh();
    //menuNavigationBar->hideAndRefresh();
    //ui.centralWidget->setCursor(Qt::SizeVerCursor);
  
}

void Gen2WGMTFTester::paintEvent(QPaintEvent* event)
{
}

void Gen2WGMTFTester::resizeEvent(QResizeEvent* event)
{
    
}

void Gen2WGMTFTester::mouseReleaseEvent(QMouseEvent* event)
{

}