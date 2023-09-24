#include "Gen2WGMTFTester.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <Windows.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyleSheet("file:///:/Gen2WGMTFTester/qss/style.qss");

    // Show startup image
    QPixmap startup(":/Gen2WGMTFTester/images/routine/startup.png");
    QSplashScreen splash(startup);
    splash.show();

    Gen2WGMTFTester w;
    splash.finish(&w);
    w.setWindowState(Qt::WindowMaximized);
    int width = GetSystemMetrics(SM_CXFULLSCREEN);
    int height = GetSystemMetrics(SM_CYFULLSCREEN);
    w.setFixedSize(width, height);

    w.show();

    
    return a.exec();
}
