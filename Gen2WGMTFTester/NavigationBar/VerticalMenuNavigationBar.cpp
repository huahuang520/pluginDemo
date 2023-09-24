#include "VerticalMenuNavigationBar.h"

VerticalMenuNavigationBar::VerticalMenuNavigationBar(QStackedWidget* stackedWidget, QWidget* parent) : QWidget(parent)
{
    m_stackedWidget = stackedWidget;
    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);

    menuDialog = new QDialog();
    menuDialog->setStyleSheet("background-color:rgb(0,0,64);");
    menuDialog->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    childDialog = new QDialog();
    childDialog->setStyleSheet("background-color:rgb(0,0,64);");
    childDialog->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    titleButton = new QPushButton();
    titleButton->setMinimumHeight(33);
    titleButton->setMinimumWidth(90);
    titleButton->setMaximumHeight(33);
    titleButton->setMaximumWidth(90);
    titleButton->setContentsMargins(0, 0, 0, 0);
    titleButton->setStyleSheet("QPushButton{background-color:rgb(212,201,201);"
        //"border-image:url(:/Gen2WGMTFTester/images/interfaceSwitch.png);"
        "color:rgb(255,255,255);"
        "font:bold 14px;"
        "text-align:left;"
        "font-family:'Microsoft YaHei';}"
    );

    vLayoutDialog = new QVBoxLayout();
    vLayoutDialog->setContentsMargins(0, 0, 0, 0);
    vLayoutDialog->addWidget(titleButton);
    vLayoutDialog->addWidget(childDialog);
    vLayoutDialog->setSpacing(0);
    menuDialog->setLayout(vLayoutDialog);
    connect(this, SIGNAL(switchWidgetSignal(int, QString)), this, SLOT(switchWidget(int, QString)));

    
    //for (int i = 0; i < 4; i++)
    //{
    //    QToolButton* button = new QToolButton();

    //    button->setStyleSheet("QToolButton{"
    //        "color:rgb(255,255,255);" "font-size: 12px;" "border-radius: 0px;"  "padding-top: 10px;"
    //        "}"
    //        "QToolButton:hover{border:2px groove gray; border-style:outset;}"
    //        "QToolButton:pressed{border-style:inset; border: 1px; }"
    //    );
    //    //"qproperty-icon: url(:/Gen2WGMTFTester/images/MTF.png) center;" "qproperty-iconSize: 40px 40px;"  //set icon
    //    if (i == 0)
    //    {
    //        button->setIcon(QIcon(":/Gen2WGMTFTester/images/MTF.png"));
    //        button->setIconSize(QSize(40,40));
    //        button->setText("MTFMeasure");
    //        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //       
    //    }
    //    else
    //    {
    //        button->setIcon(QIcon(":/Gen2WGMTFTester/images/findFiducial.png"));
    //        button->setIconSize(QSize(40, 40));
    //        button->setText("FindFiducial");
    //        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    //    }
    //   /*
    //   *Note: Setting a fixed width and height can easily cause the components in the layout to overlap, 
    //   *or the components between the layout and the layout to overlap
    //   */
    //    //button->setFixedSize(80, 80);
    //    button->setFixedWidth(80);
    //    vLayout->setSpacing(0);
    //    vLayout->addWidget(button, Qt::AlignLeft);
    //    
    //    button->installEventFilter(this);

    //    QMenu* menu = new QMenu();
    //    menu->setMinimumWidth(90);
    //    menu->setStyleSheet(
    //        "QMenu{"
    //        "background-color:rgb(0,0,64);"
    //        "margin:0px;"
    //        "}"
    //        "QMenu::item "
    //        "{"
    //        "font-size:12px;"
    //        "color:rgb(255,225,255);"
    //        "font-family:Microsoft YaHei;"
    //        "background-color:rgb(52,73,94,20);"
    //        "padding:5px 5px; "
    //        "margin:0px 0px;"
    //        "border-bottom:1px solid rgb(52,73,94,180);"
    //        "}"


    //        "QMenu::item:selected "
    //        "{"
    //        "background-color: rgb(255,189,155);"
    //        "font-size:12px;"
    //        "color:rgb(0,0,0);"
    //        "font-family:Microsoft YaHei;"
    //        "}"

    //        "QMenu::item:hover "
    //        "{"
    //        "font-size:12px;"
    //        "color:rgb(0,0,0);"
    //        "background-color: rgb(155,189,155);"
    //        "font-family:Microsoft YaHei;"
    //        "}");
    //    menu->setParent(childDialog);
    //    
    //    MenuBarListNode* menuBarListNode = new MenuBarListNode();
    //    menuBarListNode->pushbutton = button;
    //   // menuBarListNode->titleStr = QString("%1").arg(i);
    //    if (i ==0 )
    //        menuBarListNode->titleStr = "Switching";
    //    else
    //        menuBarListNode->titleStr = "test";
    //    menuBarListNode->menu = menu;

    //    if (i == 0 || i == 2)
    //    {
    //        menuBarListNode->secondCounts = 2;
    //        for (int j = 0; j < 2; j++)
    //        {
    //            menu->addAction("ShowPag" + QString("%1").arg(i) + QString("%1").arg(j), [=] {
    //                emit SendIndexToMain(QString("%1").arg(i) + QString("%1").arg(j));
    //                hideAndRefresh();
    //                });
    //        }
    //    }
    //    else
    //    {
    //        menuBarListNode->secondCounts = 0;
    //        connect(button, &QToolButton::clicked, [=] {
    //            emit SendIndexToMain(QString("%1").arg(i));
    //            hideAndRefresh();
    //            button->setStyleSheet(
    //                "QToolButton{color:rgb(255,255,255); background-color: rgb(195,195,195); font-size: 12px; border-radius: 0px; padding-top: 10px; }"
    //                "QToolButton:hover{border:2px groove gray; border-style:outset;}"
    //            );
    //            });
    //    }

    //    menuBarList << menuBarListNode;

    //}

    setMouseTracking(true);
    this->parentWidget()->setMouseTracking(true);
  
}

void VerticalMenuNavigationBar::hideAndRefresh()
{
    for (int i = 0; i < menuBarList.length(); i++)
    {
        menuBarList.at(i)->pushbutton->setStyleSheet(
            "QToolButton{color:rgb(255,255,255);  font-size: 12px; border-radius: 0px; padding-top: 10px; }"
        );

    }

    menuDialog->hide();
}

void VerticalMenuNavigationBar::addMainMenu(QString title, QIcon icon)
{
    QToolButton* button = new QToolButton();

    button->setStyleSheet("QToolButton{"
        "color:rgb(255,255,255);" "font-size: 12px;" "border-radius: 0px;"  "padding-top: 10px;"
        "}"
        "QToolButton:hover{border:2px groove gray; border-style:outset;}"
        "QToolButton:pressed{border-style:inset; border: 1px; }"
    );
    //"qproperty-icon: url(:/Gen2WGMTFTester/images/MTF.png) center;" "qproperty-iconSize: 40px 40px;"  //set icon

    button->setIcon(icon);
    button->setIconSize(QSize(40, 40));
    button->setText(title);
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    /*
    *Note: Setting a fixed width and height can easily cause the components in the layout to overlap,
    *or the components between the layout and the layout to overlap
    */
    //button->setFixedSize(80, 80);
    button->setFixedWidth(80);
    vLayout->setSpacing(0);
    vLayout->addWidget(button, Qt::AlignLeft);

    button->installEventFilter(this);

    QMenu* menu = new QMenu();
    menu->setMinimumWidth(90);
    menu->setStyleSheet(
        "QMenu{"
        "background-color:rgb(0,0,64);"
        "margin:0px;"
        "}"
        "QMenu::item "
        "{"
        "font-size:12px;"
        "color:rgb(255,225,255);"
        "font-family:Microsoft YaHei;"
        "background-color:rgb(52,73,94,20);"
        "padding:5px 5px; "
        "margin:0px 0px;"
        "border-bottom:1px solid rgb(52,73,94,180);"
        "}"


        "QMenu::item:selected "
        "{"
        "background-color: rgb(255,189,155);"
        "font-size:12px;"
        "color:rgb(0,0,0);"
        "font-family:Microsoft YaHei;"
        "}"

        "QMenu::item:hover "
        "{"
        "font-size:12px;"
        "color:rgb(0,0,0);"
        "background-color: rgb(155,189,155);"
        "font-family:Microsoft YaHei;"
        "}");
    menu->setParent(childDialog);

    MenuBarListNode* menuBarListNode = new MenuBarListNode();
    menuBarListNode->pushbutton = button;
    menuBarListNode->titleStr = "";
    menuBarListNode->menu = menu;
    menuBarListNode->secondCounts = 0;
    menuBarList.push_back(menuBarListNode);
}

void VerticalMenuNavigationBar::addMenuWidget(QString widgetTitle, int mainMenuIndex, QWidget* widget)
{
    if (menuBarList.size() > mainMenuIndex)
    {
        menuBarList[mainMenuIndex]->widgetTitle.push_back(widgetTitle);
        menuBarList[mainMenuIndex]->secondCounts++;
        static int stackedWidgetNum = -1;
        m_stackedWidget->addWidget(widget);
        stackedWidgetNum++;
        m_stackedWidget->widget(stackedWidgetNum)->setMouseTracking(true);

        if (menuBarList[mainMenuIndex]->secondCounts > 1)
        {
            for (int i = 0; i < menuBarList[mainMenuIndex]->secondCounts; i++)
            {
                
                QAction* action = menuBarList[mainMenuIndex]->menu->addAction(menuBarList[mainMenuIndex]->widgetTitle[i]);
                //menuBarList[mainMenuIndex]->menu->addAction(menuBarList[mainMenuIndex]->widgetTitle[i], [=] {
                //    //emit SendIndexToMain(widgetTitle);
                //    emit switchWidgetSignal(mainMenuIndex, menuBarList[mainMenuIndex]->widgetTitle[i]);
                //    hideAndRefresh();
                //    });

                connect(action, &QAction::triggered, [=] {
                    emit switchWidgetSignal(mainMenuIndex, menuBarList[mainMenuIndex]->widgetTitle[i]);
                    hideAndRefresh();
                    });
            }
            
        }
        else
        {
            connect(menuBarList[mainMenuIndex]->pushbutton, &QToolButton::clicked, [=]{
                int beforeWidgetNum = 0;
                for (int i = -1; i < mainMenuIndex; i++)
                {
                    for (int j = 0; j < menuBarList[i + 1]->widgetTitle.size(); j++)
                    {
                        if (i + 1 == mainMenuIndex && menuBarList[i + 1]->widgetTitle[j] == menuBarList[mainMenuIndex]->widgetTitle[0]) break;
                        beforeWidgetNum++;
                    }

                }
                m_stackedWidget->setCurrentIndex(beforeWidgetNum);
                });
        }
    }
}

void VerticalMenuNavigationBar::switchWidget(int mainMenuIndex, QString widgetTitle)
{
    int beforeWidgetNum = 0;
    for (int i = -1; i < mainMenuIndex; i++)
    {
        for (int j = 0; j < menuBarList[i + 1]->widgetTitle.size(); j++)
        {
            if (i + 1 == mainMenuIndex && menuBarList[i + 1]->widgetTitle[j] == widgetTitle) break;
            beforeWidgetNum++;
        }
        
    }
    m_stackedWidget->setCurrentIndex(beforeWidgetNum);
}

bool VerticalMenuNavigationBar::eventFilter(QObject* obj, QEvent* event)
{
    for (int i = 0; i < menuBarList.length(); i++)
    {
        if (obj == menuBarList.at(i)->pushbutton && (event->type() == QEvent::Enter || event->type() == QEvent::MouseButtonPress))
        {
            if (menuBarList.at(i)->secondCounts > 1)
                menuDialog->show();

            menuBarList.at(i)->pushbutton->setStyleSheet(
                "QToolButton{color:rgb(255,255,255); background-color: rgb(195,195,195); font-size: 12px; border-radius: 0px; padding-top: 10px;}"

                "QToolButton:hover{border:2px groove gray; border-style:outset;}"
                "QToolButton:pressed{border-style:inset; border: 1px;}"
                );
            for (int j = 0; j < menuBarList.length(); j++)
            {
                if (i == j)
                {
                    titleButton->setText(menuBarList.at(j)->titleStr);
                    if (menuBarList.at(j)->secondCounts > 1)
                        menuBarList.at(j)->menu->show();
                    else
                    {
                        menuBarList.at(j)->menu->hide();
                        //titleButton->hide();
                        menuDialog->hide();
                    }
                }
                else
                {
                    //if (menuBarList.at(j)->secondCounts > 1)
                    menuBarList.at(j)->menu->hide();
                    menuBarList.at(j)->pushbutton->setStyleSheet(
                        "QToolButton{color:rgb(255,255,255);  font-size: 12px; border-radius: 0px; padding-top: 10px;}"

                        "QToolButton:hover{border:2px groove gray; border-style:outset;}"
                        "QToolButton:pressed{border-style:inset; border: none; }"
                        );
                }
            }
            if (menuBarList.at(i)->menu != NULL)
            {
                childDialog->resize(90, menuBarList.at(i)->menu->rect().height());
                menuDialog->resize(90, titleButton->minimumHeight() + menuBarList.at(i)->menu->rect().height());
            }
            
            QPoint p = menuBarList.at(i)->pushbutton->mapToGlobal(QPoint(0, 0));
            menuDialog->move(QPoint(p.x() + menuBarList.at(i)->pushbutton->width(), p.y()));
            if (menuBarList.at(i)->secondCounts > 1)
                return true;
            else
                return false;
        }
      
    }
    return QObject::eventFilter(obj, event);
}

void VerticalMenuNavigationBar::mouseReleaseEvent(QMouseEvent* event)
{
    
}

void VerticalMenuNavigationBar::mouseMoveEvent(QMouseEvent* event)
{
    int a = 0;
}


