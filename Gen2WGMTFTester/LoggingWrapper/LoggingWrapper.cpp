#include "LoggingWrapper.h"
#include "LogPlus.h"
#include <QDateTime>
//LoggingWrapper* LoggingWrapper::m_loggingwrapper = NULL;

LoggingWrapper::LoggingWrapper(QObject* parent) : QObject(parent)
{
    m_isSetParent = false;

    if (loggingwidget == NULL)
    {
        loggingwidget = new LogWidget();
        //loggingwidget->setMinimumHeight(500);
       // loggingwidget->setStyleSheet("QWidget{background:#F8FFFF;}"); //set background color
    }

}

LoggingWrapper::~LoggingWrapper()
{
   /* delete loggingwidget;
    delete m_loggingwrapper;*/
}

//LoggingWrapper* LoggingWrapper::getinstance()
//{
//    if (NULL == m_loggingwrapper)
//    {
//        m_loggingwrapper = new LoggingWrapper();
//    }
//    return m_loggingwrapper;
//}

QWidget* LoggingWrapper::GetLogWidget()
{
    return loggingwidget;
}

void LoggingWrapper::SetParent(QWidget* widget)
{
    connect(loggingwidget, SIGNAL(changeLogWidgetHeight(int)), widget, SLOT(updateWidgetHeight(int)));
    loggingwidget->setParent(widget);

    loggingwidget->show();
    m_isSetParent = true;
}

void LoggingWrapper::SetLayoutV(QVBoxLayout* qVBoxLayout)
{
    qVBoxLayout->addWidget(loggingwidget);
    m_isSetParent = true;
}

void LoggingWrapper::SetLayoutH(QHBoxLayout* qHBoxLayout)
{
    qHBoxLayout->addWidget(loggingwidget);
    m_isSetParent = true;
}

void LoggingWrapper::SetDockWidget(QDockWidget* qDockWidget)
{
    loggingwidget->setParent(qDockWidget);
    qDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    qDockWidget->setWidget(loggingwidget);
    m_isSetParent = true;
}

void LoggingWrapper::Info(std::string message)
{
    LOG4CPLUS_INFO(LogPlus::getInstance()->logger, message.c_str());

    if (loggingwidget != nullptr && m_isSetParent)
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");
        loggingwidget->message("[" + current_date + "] " + QString::fromStdString(message), "green");
    }
}

void LoggingWrapper::Warn(std::string message)
{
    LOG4CPLUS_WARN(LogPlus::getInstance()->logger, message.c_str());
    if (loggingwidget != nullptr && m_isSetParent)
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");
        loggingwidget->message("[" + current_date + "] " + QString::fromStdString(message), "yellow");
    }
}

void LoggingWrapper::Error(std::string message)
{
    LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, message.c_str());
    if (loggingwidget != nullptr && m_isSetParent)
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");
        loggingwidget->message("[" + current_date + "] " + QString::fromStdString(message), "red");
    }
}
