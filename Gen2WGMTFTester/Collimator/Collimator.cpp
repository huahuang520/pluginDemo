#include "Collimator.h"
#include "CollimatorWidget.h"

Collimator::Collimator(QObject* parent) : QObject(parent)
{
    m_isOpen = false;
    m_isHasCollimatorData = false;
    len = 0;
    m_isRecvDataStop = true;
    m_isRecvDataEnd = true;
    m_thread = nullptr;
    m_isUpdateData = false;
    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(updateData()));
    
} 

Collimator::~Collimator()
{
    Close();
    delete m_Timer;
}

QWidget* Collimator::GetWidget()
{
    CollimatorWidget* collWidget = new CollimatorWidget();
    connect(collWidget, &CollimatorWidget::sendOpenSignal, this, &Collimator::Open);
    connect(collWidget, &CollimatorWidget::sendCloseSignal, this, &Collimator::Close);
    connect(this, &Collimator::updateConnectStatus, collWidget, &CollimatorWidget::updateConnectStatus);
    connect(this, &Collimator::sendStatusSignal, collWidget, &CollimatorWidget::receiveStatus);
    connect(this, &Collimator::sendSecAngleSignal, collWidget, &CollimatorWidget::ShowCollimatorData);
	return collWidget;
}

void Collimator::updateData()
{
    if (m_isUpdateData)
    {
        m_isHasCollimatorData = true;
        m_isUpdateData = false;
    }
    else
    {
        m_isHasCollimatorData = false;
        emit sendStatusSignal("Invaild_Data");
    }
    Sleep(10);
}

ML::MLResult Collimator::Open()
{
    WSADATA wsaData;
    SOCKADDR_IN addrSrv;

    WORD wVersionRequested = MAKEWORD(1, 1);

    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) 
    {
        m_isOpen = false;
        emit updateConnectStatus("Connect Failed");
        return ML::MLResult{ ML::MLStatus::ML_STATUS_FAILED,"" };
    }

    if (LOBYTE(wsaData.wVersion) != 1 ||
        HIBYTE(wsaData.wVersion) != 1) 
    {
        m_isOpen = false;
        WSACleanup();
        
        emit updateConnectStatus("Connect Failed");
        return ML::MLResult{ ML::MLStatus::ML_STATUS_FAILED,"" };
    }
    sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

    //set UPD blocking
   /* unsigned long ul = 1;
    int rt = ioctlsocket(sockSrv, FIONBIO, (unsigned long*)&ul);
    if (rt == SOCKET_ERROR)
    {
        err = WSAGetLastError();
        closesocket(sockSrv);
        sockSrv = NULL;
        return ML::MLResult{ ML::MLStatus::ML_STATUS_FAILED,"" };
    }*/
    

    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(5888);

    bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

    len = sizeof(SOCKADDR);

    m_isOpen = true;
    emit updateConnectStatus("Connect Success");

    m_isRecvDataEnd = false;
    m_isRecvDataStop = false;
    
    if (m_Timer->isActive() == false)
    {
        m_Timer->start(500);
    }

    if (!m_thread)
        m_thread = new std::thread(&Collimator::receiveInfo, this);

    return ML::MLResult{ML::MLStatus::ML_STATUS_OK,""};
}

bool Collimator::Close()
{
    if (m_isOpen)
    {
        if (m_Timer->isActive() == true)
        {
            m_Timer->stop();
        }
        closesocket(sockSrv);
        WSACleanup();

        m_isOpen = false;
        emit updateConnectStatus("Close Success");

        if (m_thread && m_thread->joinable())
        {
            m_isRecvDataEnd = true;
            m_isRecvDataStop = true;
            m_thread->join();
            delete m_thread;
            m_thread = nullptr;
        }

        return true;
    }
    return true;
}

bool Collimator::isOpen()
{
	return m_isOpen;
}

bool Collimator::isHasCollimatorData()
{
	return m_isHasCollimatorData;
}

void Collimator::receiveInfo()
{
    while (true)
    {
        if (m_isRecvDataEnd)
        {
            m_isRecvDataStop = true;
            break;
        }

        if (m_isOpen && !m_isRecvDataStop)
        {
            if (m_Timer->isActive() == false)
            {
                m_Timer->start(1000);
            }

            int rt = recvfrom(sockSrv, recvBuf, 1000, 0, (SOCKADDR*)&addrClient, &len); //When there are two pipes, it must be greater than or equal to 1685?
            m_isUpdateData = true;

            /*if (rt == -1)
            {
                emit sendStatusSignal("Error");
                m_isHasCollimatorData = false;
                Sleep(10);
                continue;
            }*/

            if (strlen(recvBuf) <= 30)
            {
                std::string str = recvBuf;
                QStringList qlist = QString::fromStdString(str).split("$");
                QStringList qlist2 = qlist[1].split("*");
                QStringList xyAngle_list = qlist2[0].split(",");

                double xSecAngle = xyAngle_list[0].toDouble();
                double ySecAngle = xyAngle_list[1].toDouble();

                //m_isHasCollimatorData = true;
                emit sendStatusSignal("OK");
                emit sendSecAngleSignal(xSecAngle, ySecAngle);
            }
            //else
            //{
            //    //m_isHasCollimatorData = false;
            //    emit sendStatusSignal("Invaild_Data");
            //}
            
        }
        Sleep(10);
    }
    
}
