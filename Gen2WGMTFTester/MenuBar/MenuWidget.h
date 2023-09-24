#pragma once

#include <QMenuBar>
#include "ui_MenuWidget.h"
#include <QMouseEvent>
#include <QEvent>
#include <QTabWidget>
#include <QPaintEvent>

class QMainWindow;
class QMouseEvent;
class QEvent;

struct QRibbonPrivate;

class MenuWidget : public QMenuBar
{
	Q_OBJECT

public:
	MenuWidget(QWidget *parent = nullptr);
	~MenuWidget();

public:
    void install(QMainWindow* window);

    void setColor(const QString& colorName);

public:

    /**
     * 安装本实例至QMainWindow实例，
     * 自动根据QMainWindow的菜单栏创建Ribbon内容
     */
    void initialize(QMainWindow* window);

    /**
     * 卸载
     * TBD
     */
    void uninstall();

    void setStyleButtonVisible(bool visible);

public slots:

    /**
     * 设置标题
     */
    void setWindowTitle(const QString& title);

    /**
     * 切换全屏
     */
    void toggleMaximized();

private slots:

    /**
     * 展开标签页
     */
    void expandTab();

    /**
     * 隐藏标签页
     */
    void hideTab();

    /**
     * 隐藏标签页动画结束
     */
    void onHideTabFinished();

    /**
     * 标签页变化
     */
    void onTabChanged();

    /**
     * 失去焦点时
     */
    void onLostFocus();

    /**
     * tab被点击
     */
    void clickTab();

public slots:
    void on_pushButtonMaxTab_clicked();
    void on_pushButtonMinTab_clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
   // bool eventFilter(QObject* tgt, QEvent* e) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void initStyle();

private:
	Ui::MenuWidgetClass ui;

    friend struct QRibbonPrivate;
    QRibbonPrivate* _ribbon;

    QMenu* _styleMenu;

    bool   m_bExpandStaus;

    bool isMaxWindow;
};
