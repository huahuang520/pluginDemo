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
     * ��װ��ʵ����QMainWindowʵ����
     * �Զ�����QMainWindow�Ĳ˵�������Ribbon����
     */
    void initialize(QMainWindow* window);

    /**
     * ж��
     * TBD
     */
    void uninstall();

    void setStyleButtonVisible(bool visible);

public slots:

    /**
     * ���ñ���
     */
    void setWindowTitle(const QString& title);

    /**
     * �л�ȫ��
     */
    void toggleMaximized();

private slots:

    /**
     * չ����ǩҳ
     */
    void expandTab();

    /**
     * ���ر�ǩҳ
     */
    void hideTab();

    /**
     * ���ر�ǩҳ��������
     */
    void onHideTabFinished();

    /**
     * ��ǩҳ�仯
     */
    void onTabChanged();

    /**
     * ʧȥ����ʱ
     */
    void onLostFocus();

    /**
     * tab�����
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
