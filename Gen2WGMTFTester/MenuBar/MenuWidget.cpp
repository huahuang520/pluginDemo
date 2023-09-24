#include "MenuWidget.h"

#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include "QtCore\QTimer"
#include "QtWidgets\QMainWindow"
#include "QtCore\qcoreevent.h"
#include <Windows.h>
#include <QVBoxLayout>
#include <QHBoxLayout>

const auto MINIMUM_HEIGHT = 62;
/**
 * QRibbon成员封装
 */
struct QRibbonPrivate
{
    QRibbonPrivate(MenuWidget* ribbon)
    {
        // 处理主窗体突然失去焦点时的状态
        _timer = new QTimer();
        QObject::connect(_timer, &QTimer::timeout, ribbon, &MenuWidget::onLostFocus);
        _timer->setInterval(100);
    }

    ~QRibbonPrivate()
    {
        delete _timer;
    }

    // 处理无边框窗体移动事件
    bool        _pressed = false;
    QPoint      _mouseStartPosition;
    QPoint      _origin;
    QRect _originGeometry;

    QMainWindow* _mainWindow = nullptr;

    QTimer* _timer;

    QPropertyAnimation opacityAnimation;
    QPropertyAnimation animationHideBar;
};

MenuWidget::MenuWidget(QWidget *parent)
	: QMenuBar(parent)
    , _ribbon(new QRibbonPrivate(this))
{
	ui.setupUi(this);

    connect(ui.tabWidgetMenuBar, &QTabWidget::tabBarClicked, this, &MenuWidget::clickTab);
    connect(ui.tabWidgetMenuBar, &QTabWidget::currentChanged, this, &MenuWidget::onTabChanged);

    connect(ui.pushButtonFullScreen, &QPushButton::clicked, this, &MenuWidget::toggleMaximized);

    // 隐藏下边线
    connect(&(_ribbon->animationHideBar), &QPropertyAnimation::finished, this, &MenuWidget::onHideTabFinished);

    // Set theme color
    ui.pushButtonStyle->setIcon(QIcon(":/Gen2WGMTFTester/images/routine/themeColor.png"));
    ui.pushButtonStyle->setToolTip("Set theme color");
    ui.pushButtonStyle->setStyleSheet("QToolTip{font-size:14px;font: bold; text-align: center;}");

    _styleMenu = new QMenu(this);
    _styleMenu->setStyleSheet("QMenu::item{ color:black }");
    auto styleActionGroup = new QActionGroup(_styleMenu);
    styleActionGroup->addAction(_styleMenu->addAction("blue", [&]()
        {
            setColor("rgb(79,113,164)");
        }));
    styleActionGroup->addAction(_styleMenu->addAction("green", [&]()
        {
            setColor("rgb(33,115,70)");
        }));
    styleActionGroup->addAction(_styleMenu->addAction("red", [&]()
        {
            setColor("rgb(183, 71, 42)");
        }));
    for (auto a : styleActionGroup->actions())
    {
        a->setCheckable(true);
    }
    ui.pushButtonStyle->setMenu(_styleMenu);

    m_bExpandStaus = false;
    initStyle();
    hideTab();
    isMaxWindow = true;
}

MenuWidget::~MenuWidget()
{
    delete _ribbon;
}

void MenuWidget::install(QMainWindow* window)
{
    // window had install menuWidget
    if (qobject_cast<MenuWidget*>(window->menuBar()))
    {
        return;
    }

    /*auto _menuWidegt = new MenuWidget();
    _menuWidegt->initialize(window);*/
    initialize(window);
}

void MenuWidget::setColor(const QString& colorName)
{
    static QString currentColor = "rgb(43,87,154)";
    QString stylesheet;
    if (!_ribbon->_mainWindow->styleSheet().isEmpty())
    {
        stylesheet += _ribbon->_mainWindow->styleSheet();
        _ribbon->_mainWindow->setStyleSheet("");
    }

    if (!ui.widgetContainer->styleSheet().isEmpty())
    {
        stylesheet += ui.widgetContainer->styleSheet();
        ui.widgetContainer->setStyleSheet("");
    }
    
    if (!qApp->styleSheet().isEmpty())
    {
        stylesheet += qApp->styleSheet();
    }

    //auto&& stylesheet = QString::fromUtf8(ui->widgetContainer->styleSheet().toLocal8Bit());
    stylesheet = stylesheet.replace(currentColor, colorName);
    ui.widgetContainer->setStyleSheet("");

    qApp->setStyleSheet(stylesheet);
    currentColor = colorName;
}

void MenuWidget::initialize(QMainWindow* window)
{
    _ribbon->_mainWindow = window;

    // 设置透明度渐变动画
    if (!_ribbon->opacityAnimation.targetObject())
    {
        _ribbon->opacityAnimation.setTargetObject(_ribbon->_mainWindow);
        _ribbon->opacityAnimation.setPropertyName("windowOpacity");
        _ribbon->opacityAnimation.setStartValue(1.0);
        _ribbon->opacityAnimation.setEndValue(1.0);
    }

    QMenuBar* menuBar = _ribbon->_mainWindow->menuBar();
    // menuBar->setCornerWidget(ui.widgetContainer, Qt::Corner::TopLeftCorner);

    if (!menuBar)
    {
        return;
    }

    //_normalGeom = _mainWindow->geometry();

    connect(ui.pushButtonMinimum, &QPushButton::clicked, _ribbon->_mainWindow, &QWidget::showMinimized);
    connect(ui.pushButtonClose, &QPushButton::clicked, _ribbon->_mainWindow, &QWidget::close);

    window->setWindowFlag(Qt::FramelessWindowHint, true);

    QObject::connect(window, &QMainWindow::windowTitleChanged, this, &MenuWidget::setWindowTitle);
    this->setWindowTitle(window->windowTitle());

    //window->installEventFilter(this);

    //window->menuBar()->setParent(0);
    window->setMenuBar(this);

    _styleMenu->actions()[0]->trigger();

    _ribbon->_originGeometry = window->normalGeometry();

    window->menuBar()->layout()->setContentsMargins(0, 0, 0, 0);

   // _ribbon->_mainWindow->setWindowState(Qt::WindowMaximized);
}

void MenuWidget::uninstall()
{
}

void MenuWidget::setStyleButtonVisible(bool visible)
{
    _styleMenu->setVisible(visible);
}

void MenuWidget::toggleMaximized()
{
    /*if (_ribbon->_mainWindow->windowState() == Qt::WindowFullScreen)
    {
        _ribbon->_mainWindow->setWindowState(Qt::WindowActive);
    }
    else if (_ribbon->_mainWindow->windowState() == Qt::WindowMaximized)
    {
        _ribbon->_mainWindow->setWindowState(Qt::WindowActive);
    }
    else
        _ribbon->_mainWindow->setWindowState(Qt::WindowMaximized);*/

    //_ribbon->_mainWindow->setWindowState(_ribbon->_mainWindow->windowState() & (Qt::WindowFullScreen | Qt::WindowMaximized) ? Qt::WindowActive : Qt::WindowActive | Qt::WindowMaximized);

    if (isMaxWindow)
    {
        _ribbon->_mainWindow->setWindowState(Qt::WindowActive);
        isMaxWindow = false;
    }
    else
    {
        _ribbon->_mainWindow->setWindowState(Qt::WindowMaximized);
        int width = GetSystemMetrics(SM_CXFULLSCREEN);
        int height = GetSystemMetrics(SM_CYFULLSCREEN);
        _ribbon->_mainWindow->setFixedSize(width, height);
        isMaxWindow = true;
    }
}

void MenuWidget::expandTab()
{
    _ribbon->animationHideBar.setDirection(QAbstractAnimation::Backward);
    _ribbon->animationHideBar.start();

    m_bExpandStaus = true;
}

void MenuWidget::hideTab()
{
    if (!_ribbon->animationHideBar.targetObject())
    {
        _ribbon->animationHideBar.setTargetObject(this);
        _ribbon->animationHideBar.setPropertyName("minimumHeight");
        _ribbon->animationHideBar.setStartValue(height());
        _ribbon->animationHideBar.setEndValue(MINIMUM_HEIGHT);
        _ribbon->animationHideBar.setDuration(10);
        _ribbon->animationHideBar.setEasingCurve(QEasingCurve::Linear);  // 缓和曲线风格
    }

    if (_ribbon->animationHideBar.state() == QAbstractAnimation::Running)
    {
        _ribbon->animationHideBar.stop();
    }
    //_ribbon->animationHideBar.setDuration(300);
    _ribbon->animationHideBar.setDirection(QAbstractAnimation::Forward);
    _ribbon->animationHideBar.start();

    m_bExpandStaus = false;
}

void MenuWidget::onHideTabFinished()
{
    ui.widgetBottomBar->setMinimumHeight(_ribbon->animationHideBar.direction() == QAbstractAnimation::Forward ? 0 : 1);
}

void MenuWidget::onTabChanged()
{
    if (m_bExpandStaus == false)
    {
        expandTab();
        m_bExpandStaus = true;
    }
}

void MenuWidget::onLostFocus()
{
    if (!_ribbon->_mainWindow)
    {
        return;
    }

    if (_ribbon->_pressed == true)
    {
        if (_ribbon->_mainWindow->isActiveWindow())
        {
            return;
        }
        _ribbon->_pressed = false;
        _ribbon->_mainWindow->setWindowOpacity(1.0);
    }
    _ribbon->_timer->stop();
}

void MenuWidget::clickTab()
{
    if (m_bExpandStaus == false)
    {
        expandTab();
        m_bExpandStaus = true;
    }
    else
    {
        hideTab();
        m_bExpandStaus = false;
    }
}

void MenuWidget::mousePressEvent(QMouseEvent* evt)
{
    if (evt->button() == Qt::LeftButton && evt->y() < ui.labelTitle->height() + 10)
    {
        _ribbon->_pressed = true;

        _ribbon->_timer->start();

        _ribbon->_originGeometry = _ribbon->_mainWindow->normalGeometry();

        //获得鼠标的初始位置
        _ribbon->_mouseStartPosition = evt->globalPos();
        //获得窗口的初始位置
        _ribbon->_origin = _ribbon->_mainWindow->frameGeometry().topLeft();

        if (_ribbon->opacityAnimation.state() == QPropertyAnimation::Running)
        {
            _ribbon->opacityAnimation.stop();
        }

        if (_ribbon->_mainWindow->windowOpacity() != _ribbon->opacityAnimation.endValue().toDouble())
        {
            _ribbon->opacityAnimation.setDuration(250);
            _ribbon->opacityAnimation.setDirection(QAbstractAnimation::Forward);
            _ribbon->opacityAnimation.start();
        }
    }
    QMenuBar::mousePressEvent(evt);
}

void MenuWidget::mouseMoveEvent(QMouseEvent* evt)
{
    if (_ribbon->_pressed)
    {
        //获得鼠标移动的距离
        auto&& offset = evt->globalPos() - _ribbon->_mouseStartPosition;
        //改变窗口的位置

        bool maxOrFull = _ribbon->_mainWindow->windowState() & (Qt::WindowFullScreen | Qt::WindowMaximized);

        if (maxOrFull)
        {
            // 最大化
            if (offset.manhattanLength() > 1)
            {
                auto w = _ribbon->_mainWindow;
                auto geom = _ribbon->_mainWindow->normalGeometry();
                w->mapFromGlobal(evt->globalPos());
                auto localX = geom.width() * (evt->globalX() * 1.0 / w->width());
                geom.moveTopLeft(QPoint(evt->x() - localX, 0));
                toggleMaximized();
                _ribbon->_mainWindow->setGeometry(geom);
                _ribbon->_origin = _ribbon->_mainWindow->frameGeometry().topLeft();
            }
        }
        else
        {
            _ribbon->_mainWindow->move(_ribbon->_origin + offset);
        }
    }
    //QMenuBar::mouseMoveEvent(event);
}

void MenuWidget::mouseReleaseEvent(QMouseEvent* evt)
{
    if (evt->button() == Qt::LeftButton)
    {
        _ribbon->_pressed = false;

        auto moved = (evt->globalPos() - _ribbon->_mouseStartPosition).manhattanLength() > 3;

        bool maxOrFull = _ribbon->_mainWindow->windowState() & (Qt::WindowFullScreen | Qt::WindowMaximized);

        // 移动到屏幕顶部最大化显示
        if (moved && !maxOrFull && evt->globalY() < 3)
        {
            _ribbon->_mainWindow->setGeometry(_ribbon->_originGeometry);
            toggleMaximized();
            //_normalGeom = _beforeGeometry;
        }
        else if (_ribbon->_mainWindow->y() < 0)
        {
            _ribbon->_mainWindow->move(_ribbon->_mainWindow->x(), 0);
        }
    }

    if (_ribbon->opacityAnimation.state() == QPropertyAnimation::Running)
    {
        _ribbon->opacityAnimation.stop();
    }
    if (_ribbon->_mainWindow->windowOpacity() != _ribbon->opacityAnimation.startValue().toDouble())
    {
        _ribbon->opacityAnimation.setDuration(150);
        _ribbon->opacityAnimation.setDirection(QAbstractAnimation::Backward);
        _ribbon->opacityAnimation.start();
    }

    QMenuBar::mouseReleaseEvent(evt);
}

void MenuWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->y() < ui.labelTitle->height() + 10)
    {
        toggleMaximized();
    }

    //QMenuBar::mouseDoubleClickEvent(event);
}

void MenuWidget::paintEvent(QPaintEvent* event)
{
    

}

//bool MenuWidget::eventFilter(QObject* tgt, QEvent* e)
//{
//    if (tgt == _ribbon->_mainWindow && e->type() == QEvent::WindowStateChange)
//    {
//        ui.pushButtonFullScreen->setText(_ribbon->_mainWindow->windowState() & (Qt::WindowFullScreen | Qt::WindowMaximized) ? "66" : "99");
//    }
//
//    return QMenuBar::eventFilter(tgt, e);
//}

void MenuWidget::initStyle()
{
    ui.widgetBottomBar->setStyleSheet("QWidget{ border: 1px solid red; }");
    //ui.pushButtonFullScreen->setIcon(QPixmap(":/Gen2WGMTFTester/images/routine/maxWindow.png").scaled(ui.pushButtonFullScreen->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    //this->setAutoFillBackground(true);
    //ui.widgetContainer->setAutoFillBackground(true);    // 这句要加上, 否则可能显示不出背景图.
    //QPalette palette = ui.widgetContainer->palette();
    ////palette.setColor(QPalette::Window, Qt::red);  // 设置背景色
    ////palette.setBrush(this->backgroundRole(), Qt::black);// 设置背景色
    //palette.setBrush(QPalette::Window,
    //    QBrush(QPixmap(":/Gen2WGMTFTester/images/logo/ShowBackground.png").scaled(   
    //        ui.widgetContainer->size(),
    //        Qt::IgnoreAspectRatio,
    //        Qt::SmoothTransformation)));    // 使用平滑的缩放方式
    //ui.widgetContainer->setPalette(palette);
    //ui.widgetContainer->update();
    //ui.widgetContainer->show();

    /*ui.widgetContainer->setAttribute(Qt::WA_StyledBackground);
    ui.widgetContainer->setStyleSheet("background-image:url(:/Gen2WGMTFTester/images/logo/ShowBackground.png);");*/

    ui.toolButton_4->setStyleSheet("QToolButton{background:transparent;border:0px solid gray;color: rgb(10, 10, 10);margin:0;padding-bottom:10px;}"
        "QToolButton:hover{border:1px solid rgba(100,100,100,100);border-radius:2px;background:rgb(198,198,198);}"
        "QToolButton:checked{border:1px solid rgba(100,100,100,100);border-radius:2px;background:rgb(198,198,198);}"
        /* 16px width + 4px for border = 20px allocated above */
        "QToolButton::menu-button{width: 16px;subcontrol-origin:margin;subcontrol-position: bottom center;bottom:0px;left:0px;height:10px;width:100%}"
    );
    ui.toolButton_5->setStyleSheet("QToolButton{background:transparent;border:0px solid gray;color: rgb(10, 10, 10);margin:0;padding-bottom:10px;}"
        "QToolButton:hover{border:1px solid rgba(100,100,100,100);border-radius:2px;background:rgb(198,198,198);}"
        "QToolButton:checked{border:1px solid rgba(100,100,100,100);border-radius:2px;background:rgb(198,198,198);}"
        /* 16px width + 4px for border = 20px allocated above */
        "QToolButton::menu-button{width: 16px;subcontrol-origin:margin;subcontrol-position: bottom center;bottom:0px;left:0px;height:10px;width:100%}"
    );
    ui.toolButton_7->setStyleSheet("QToolButton{background:transparent;border:0px solid gray;color: rgb(10, 10, 10);margin:0;padding-bottom:10px;}"
        "QToolButton:hover{border:1px solid rgba(100,100,100,100);border-radius:2px;background:rgb(198,198,198);}"
        "QToolButton:checked{border:1px solid rgba(100,100,100,100);border-radius:2px;background:rgb(198,198,198);}"
        /* 16px width + 4px for border = 20px allocated above */
        "QToolButton::menu-button{width: 16px;subcontrol-origin:margin;subcontrol-position: bottom center;bottom:0px;left:0px;height:10px;width:100%}"
    );
}

void MenuWidget::on_pushButtonMaxTab_clicked()
{
    if (!m_bExpandStaus)
    {
        m_bExpandStaus = true;
        expandTab();
    }

}

void MenuWidget::on_pushButtonMinTab_clicked()
{
    if (m_bExpandStaus)
    {
        m_bExpandStaus = false;
        hideTab();
    }
}

void MenuWidget::setWindowTitle(const QString& title)
{
    QSize s = ui.labelIcon->size();
    int w = s.width() - 6;
    int h = s.height() - 6;
    ui.labelIcon->setPixmap(QPixmap(":/Gen2WGMTFTester/images/logo/mlico.png").scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui.labelTitle->setText(title);
}