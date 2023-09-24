#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Gen2WGMTFTester.h"
#include <QLabel>
#include <QEvent>
#include <QToolTip>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QApplication>

class Gen2WGMTFTester : public QMainWindow
{
    Q_OBJECT

public:
    Gen2WGMTFTester(QWidget *parent = nullptr);
    ~Gen2WGMTFTester();

public:
    void InitializationInterface();
    void CreateMenuBarDiscard();
    void CreateMenuBar();
    void CreateActions();
    void CreateToolBars();
    void CreateLeftNavigationBar_discard();
    void CreateLeftNavigationBar_discard2();
    void CreateLogBar();
    void InitStatusBar();
    void InitLayout();
    void CreateLeftNavigationBar();
    void CreateTabBar();
public slots:

protected:
    //bool eventFilter(QObject* watched, QEvent* event);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private:
    Ui::Gen2WGMTFTesterClass ui;
    QVBoxLayout* m_centerQVBoxLayout;
    QHBoxLayout* m_centerQHBoxLayout;

    /*VerticalMenuNavigationBar* menuNavigationBar;
    QWidget* navigationBarWidget;
    QStackedWidget* mainStackedWidget;*/

};
