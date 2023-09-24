#include "MLACSCController3D.h"
#include "LogPlus.h"
#include "ACSControlWidget.h"

MLACSCController3D::MLACSCController3D()
{
    m_IsHome = false;
    m_Enable = false;
    m_IsConnected = false;
    m_CallbackFlag = false;
    m_TotalAxes = 0;
    m_AcsHandle = ACSC_INVALID;
    m_State = MLMotionState::kMLStatusDisConnected;

}

MLACSCController3D::~MLACSCController3D()
{
    if (m_IsConnected) 
    {
        Disconnect();
        m_State = MLMotionState::kMLStatusDisConnected;
    }
    if (m_CallbackFlag)
    {
        m_CallbackFlag = false;
        m_CallbackHandle.join();
    }
    m_Callbacks.clear();
}

bool MLACSCController3D::Connect(const char* address)
{
    if (m_IsConnected) {
        m_currentConnectStatus = "Connecting";
        emit updateConnectStatus(m_currentConnectStatus);
        return true;
    }
    string info(address);
    vector<string> list = MLACSSystemController::GetInstance().Split(address, '/');

    const char* ip = list[0].c_str();

    int port = std::stoi(list[1]);

    X = std::stoi(list[2]);
    Y = std::stoi(list[3]);
    Z = std::stoi(list[4]);
    m_AcsHandle = MLACSSystemController::GetInstance().Connect(ip,port);
    if (m_AcsHandle == ACSC_INVALID)
    {
        string msg = "ACS 3D motion connect failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        
    }
    bool err = InitialMotionSystem();
    if (!err)
    {
        Disconnect();
        m_currentConnectStatus = "connection failed!";
        emit updateConnectStatus(m_currentConnectStatus);
        return false;
    }
    m_IsConnected = true;
    m_Enable = true;
    m_State = MLMotionState::kMLStatusConnected;
    m_currentConnectStatus = "Connecting";
    emit updateConnectStatus(m_currentConnectStatus);
    return true;
}

bool MLACSCController3D::Disconnect()
{
    if (m_AcsHandle != ACSC_INVALID)
    {
        SetEnable(false);
        if (!m_Enable)
        {
            m_IsConnected = false;
            m_State = MLMotionState::kMLStatusDisConnected;
            m_AcsHandle = ACSC_INVALID;
        }
        return !m_Enable;
    }
}

bool MLACSCController3D::IsConnected()
{
    return m_IsConnected;
}

void MLACSCController3D::SetEnable(bool enable)
{
    if (enable)
    {
        if (!m_Enable) 
        {
            int AxisList[] = { Y, X, Z, -1 };
            if (!acsc_EnableM(m_AcsHandle, AxisList, NULL))
            {
                string msg = "ACS 3D motion enable failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
                LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
            }
            acsc_WaitMotorEnabled(m_AcsHandle, Z, 1, 3000);
            m_Enable = enable;
        }
    }
    else
    {
        if (m_Enable)
        {
            int AxisList[] = { Y, X, Z, -1 };
            if (!acsc_DisableM(m_AcsHandle, AxisList, NULL))
            {
                string msg = "ACS 3D motion disable failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
                LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
            }
            m_Enable = enable;
        }
    }

    
}

bool MLACSCController3D::IsEnabled()
{
    return m_Enable;
}

void MLACSCController3D::MoveRelAsync(int posX, int posY, int posZ)
{
    
    if (IsMoving())
    {
        string msg = "The relative moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    int AxisList[] = { Y, X, Z, -1 };
    double PosList[] = { posY / m_Scale ,posX / m_Scale ,posZ / m_Scale ,-1};
    if (!acsc_ToPointM(m_AcsHandle, ACSC_AMF_RELATIVE, AxisList, PosList, NULL)) 
    {
        string msg = "ACS 3D motion relative moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

void MLACSCController3D::MoveAbsAsync(int posX, int posY, int posZ)
{
    if (IsMoving())
    {
        string msg = "The absolute moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    int AxisList[] = { Y, X, Z, -1 };
    double PosList[] = { posY / m_Scale ,posX / m_Scale ,posZ / m_Scale ,-1 };
    if (!acsc_ToPointM(m_AcsHandle, 0, AxisList, PosList, NULL))
    {
        string msg = "ACS 3D motion absolute moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

void MLACSCController3D::MoveRelAsync(const char* axis, int pos)
{
    int ax;
    if (strcmp (axis,"X") == 0)
    {
        ax = X;
    }
    else if (strcmp(axis, "Y") == 0)
    {
        ax = Y;
    }
    else
    {
        ax = Z;
    }
    int State;
    acsc_GetMotorState(m_AcsHandle, ax, &State, NULL);
    // If motor is moving state, 
    if (State & ACSC_MST_MOVE)
    {
        string msg = "[" + string(axis) + "] axis is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    double position = pos / m_Scale;
    if (!acsc_ToPoint(m_AcsHandle, ACSC_AMF_RELATIVE, ax, position, NULL))
    {
        string msg = "ACS 3D motion " + string(axis) + " axis relative moving failed : " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }

}

void MLACSCController3D::MoveAbsAsync(const char* axis, int pos)
{
    int ax;
    if (strcmp(axis, "X") == 0)
    {
        ax = X;
    }
    else if (strcmp(axis, "Y") == 0)
    {
        ax = Y;
    }
    else
    {
        ax = Z;
    }
    int State;
    acsc_GetMotorState(m_AcsHandle, ax, &State, NULL);
    // If motor is moving state, 
    if (State & ACSC_MST_MOVE)
    {
        string msg = "[" + string(axis) + "] axis is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }

    double position = pos / m_Scale;
    // Seconds arguement '0', it means absolute position (no option)
    if (!acsc_ToPoint(m_AcsHandle, 0, ax, position, NULL))
    {
        string msg = "ACS 3D motion " + string(axis) + " axis absolute moving failed : " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

void MLACSCController3D::MoveRel(int posX, int posY, int posZ)
{
    if (IsMoving())
    {
        string msg = "The relative moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    int AxisList[] = { Y, X, Z, -1 };
    double PosList[] = { posY / m_Scale ,posX / m_Scale ,posZ / m_Scale ,-1 };
    if (!acsc_ToPointM(m_AcsHandle, ACSC_AMF_RELATIVE, AxisList, PosList, NULL))
    {
        string msg = "ACS 3D motion relative moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
    acsc_WaitMotionEnd(m_AcsHandle, X, 10000);
    acsc_WaitMotionEnd(m_AcsHandle, Y, 10000);
    acsc_WaitMotionEnd(m_AcsHandle, Z, 10000);
}

void MLACSCController3D::MoveAbs(int posX, int posY, int posZ)
{
    if (IsMoving())
    {
        string msg = "The absolute moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    int AxisList[] = { Y, X, Z, -1 };
    double PosList[] = { posY / m_Scale ,posX / m_Scale ,posZ / m_Scale ,-1 };
    if (!acsc_ToPointM(m_AcsHandle, 0, AxisList, PosList, NULL))
    {
        string msg = "ACS 3D motion absolute moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
    acsc_WaitMotionEnd(m_AcsHandle, X, 10000);
    acsc_WaitMotionEnd(m_AcsHandle, Y, 10000);
    acsc_WaitMotionEnd(m_AcsHandle, Z, 10000);
}

void MLACSCController3D::StopMove()
{
    int AxisList[] = { Y, X, Z, -1 };

    if (!acsc_HaltM(m_AcsHandle, AxisList, NULL))
    {
        string msg = "ACS 3D motion stop moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

void MLACSCController3D::Homing()
{
    if (m_IsConnected) 
    {
        //for (int i = 0; i < m_TotalAxes;i++) 
        //{
        //    std::string buf = "HOME "+std::to_string(i)+",1,10";
        //    char* command = const_cast<char*>(buf.c_str());
        //    if (!acsc_Command(m_AcsHandle, command,buf.size() , NULL)) return ACSCErrorExecFailed;
        //} 
        bool err = InitialMotionSystem();
        if (!err) 
        {
            string msg = "ACS 3D motion homing failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
            LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
            m_IsHome = false;
            return;
        }
        m_IsHome = true;
    }
}

bool MLACSCController3D::IsHome()
{
    return m_IsHome;
}

void MLACSCController3D::SetSpeed(int speed)
{
    int s = speed / m_Scale;
    if (!acsc_SetVelocity(m_AcsHandle, X, s, NULL))
    {
        string msg = "ACS 3D motion set X axis Velocity failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
    if (!acsc_SetVelocity(m_AcsHandle, Y, s, NULL))
    {
        string msg = "ACS 3D motion set Y axis Velocity failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
    if (!acsc_SetVelocity(m_AcsHandle, Z, s, NULL))
    {
        string msg = "ACS 3D motion set Z axis Velocity failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

int MLACSCController3D::GetSpeed()
{
    double speed;
    if(!acsc_GetVelocity(m_AcsHandle, X, &speed, NULL))
    {
        string msg = "ACS 3D motion get velocity failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return 0;
    }
    else
    {
        return int(speed * m_Scale);
    }
}

bool MLACSCController3D::IsMoving()
{
    int state;
    bool moving = false;
    if (acsc_GetAxisState(m_AcsHandle, X, &state, NULL))
    {
        if (state & ACSC_MST_MOVE) 
        {
            moving = true;
        }
    }
    if (acsc_GetAxisState(m_AcsHandle, Y, &state, NULL))
    {
        if (state & ACSC_MST_MOVE)
        {
            moving = true;
        }
    }
    if (acsc_GetAxisState(m_AcsHandle, Z, &state, NULL))
    {
        if (state & ACSC_MST_MOVE)
        {
            moving = true;
        }
    }
    return moving;
}

ML_Point3D MLACSCController3D::GetPosition()
{
    ML_Point3D positions;
    double pos;
    if (acsc_GetFPosition(m_AcsHandle, X, &pos, NULL))
    {
        positions.x = round(pos * m_Scale);
    }
    else
    {
        string msg = "ACS 1D motion get X position failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        positions.x = 0;
    }

    if (acsc_GetFPosition(m_AcsHandle, Y, &pos, NULL))
    {
        positions.y = round(pos * m_Scale);
    }
    else
    {
        string msg = "ACS 1D motion get Y position failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        positions.y = 0;
    }

    if (acsc_GetFPosition(m_AcsHandle, Z, &pos, NULL))
    {
        positions.z = round(pos * m_Scale);
    }
    else
    {
        string msg = "ACS 1D motion get Z position failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        positions.z = 0;
    }

    return positions;

    //if (!acsc_ReadReal(m_AcsHandle, -1, const_cast <char*>("FPOS"), 0, m_TotalAxes - 1, -1, -1, m_FPosition, NULL))
    //{
    //    string msg = "ACS 3D motion get positions failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
    //    LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    //    return ML_Point3D();
    //}
    //else
    //{
    //    ML_Point3D pos{ m_FPosition[X] * m_Scale, m_FPosition[Y] * m_Scale, m_FPosition[Z] * m_Scale };
    //    return pos;
    //}
}

MLMotionState MLACSCController3D::GetState()
{
    if (IsMoving())
    {
        m_State = MLMotionState::kMLStatusIsMoving;
    }
    else
    {
        m_State = MLMotionState::kMLStatusStationary;
    }
    return m_State;
    
}

//ACSCErrorType MLACSCController3D::GetState(int axis,MLMotionState& state)
//{
//    int State;
//    if (!acsc_GetMotorState(m_AcsHandle, axis, &State, NULL))
//    {
//        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, GetErrorMessage().c_str());
//        return ACSCErrorExecFailed;
//    }
//    switch(State)
//    {
//    case ACSC_MST_ENABLE:
//        state = Enabble;
//        break;
//    case ACSC_MST_INPOS:
//        state = InPosition;
//        break;
//    case ACSC_MST_MOVE:
//        state = Moving;
//        break;
//    case ACSC_MST_ACC:
//        state = Accelerating;
//        break;
//    default:
//        state = Disconnected;
//        break;
//    }
//    return ACSCErrorSuccess;
//}

void MLACSCController3D::Subscribe(MLMotionEvent event, MLMotion3DCallback* callback)
{
    if (!m_Callbacks.count(event)) 
    {
        m_Callbacks[event] = callback;
        if (!m_CallbackFlag) 
        {
            m_CallbackHandle = std::thread(&MLACSCController3D::threadFunc, this);
            m_CallbackFlag = true;
        }
    }
}

void MLACSCController3D::Unsubscribe(MLMotionEvent event, MLMotion3DCallback* callback)
{
    if(m_Callbacks.count(event))
    {
        m_Callbacks.erase(event);
        if (m_CallbackFlag && m_Callbacks.empty())
        {
            m_CallbackFlag = false;
            m_CallbackHandle.join();
        }
    }
}

QWidget* MLACSCController3D::GetWidget()
{
    ACSControlWidget* widget = new ACSControlWidget();
    connect(this, SIGNAL(updateConnectStatus(QString)), widget, SLOT(updateConnectStatus(QString)));

    return widget;
}

void MLACSCController3D::threadFunc()
{
    while (m_CallbackFlag) 
    {
        if (m_IsConnected)
        {
            if (m_Callbacks.count(kStateChanged))
            {
                //if(acsc_ReadInteger(m_AcsHandle, -1, const_cast<char*>("MST"), 0, m_TotalAxes - 1, -1, -1, m_MotorStatus, NULL))
                m_Callbacks[kStateChanged]->NotifyMotion3DStateChanged(m_State, GetState());
            }

            if (m_Callbacks.count(kPositionChanged))
            {
                ML_Point3D pos = GetPosition();
                m_Callbacks[kPositionChanged]->NotifyMotion3DPosition(pos.x,pos.y,pos.z);
                emit updatePosition(pos.x, pos.y, pos.z);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool MLACSCController3D::InitialMotionSystem()
{
    //double SysInfo = 0.0;
    //acsc_SysInfo(m_AcsHandle, 10, &SysInfo, NULL);
    //int bufferSize = (int)SysInfo;
    if (!acsc_RunBuffer(m_AcsHandle, 1, NULL, NULL))
    {
        string msg = "ACS 3D motion Initialize failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());;
        return false;
    }
    acsc_WaitProgramEnd(m_AcsHandle, 1, 60000);
    return true;
}
