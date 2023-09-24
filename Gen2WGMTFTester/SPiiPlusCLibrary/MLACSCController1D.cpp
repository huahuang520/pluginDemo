#include "MLACSCController1D.h"
#include "LogPlus.h"

MLACSCController1D::MLACSCController1D()
{
    m_IsHome = false;
    m_Enable = false;
    m_IsConnected = false;
    m_CallbackFlag = false;
    m_AcsHandle = ACSC_INVALID;
    direction_ = 0;
    m_State = MLMotionState::kMLStatusDisConnected;
}

MLACSCController1D::~MLACSCController1D()
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

bool MLACSCController1D::Connect(const char* address)
{
    if (m_IsConnected)return true;
    string info(address);
    vector<string> list = MLACSSystemController::GetInstance().Split(address, '/');

    const char* ip = list[0].c_str();

    int port = std::stoi(list[1]);

    R = std::stoi(list[2]);

    m_AcsHandle = MLACSSystemController::GetInstance().Connect(ip,port);
    if (m_AcsHandle == ACSC_INVALID)
    {
        string msg = "ACS 1D motion connect failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());

    }
    bool err = InitialMotionSystem();
    if (!err)
    {
        Disconnect();
        return false;
    }
    m_IsConnected = true;
    m_Enable = true;
    m_State = MLMotionState::kMLStatusConnected;
    return true;
}

bool MLACSCController1D::Disconnect()
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

bool MLACSCController1D::IsConnected()
{
    return m_IsConnected;
}

void MLACSCController1D::SetEnable(bool enable)
{
    if (enable)
    {
        if (!m_Enable)
        {
            if (!acsc_Enable(m_AcsHandle, R, NULL))
            {
                string msg = "ACS 1D motion enable failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
                LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
            }
            acsc_WaitMotorEnabled(m_AcsHandle, R, 1, 10000);
            m_Enable = enable;
        }
    }
    else
    {
        if (m_Enable)
        {
            if (!acsc_Disable(m_AcsHandle, R, NULL))
            {
                string msg = "ACS 1D motion disable failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
                LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
            }
            m_Enable = enable;
        }
    }
}

bool MLACSCController1D::IsEnabled()
{
    return m_Enable;
}

void MLACSCController1D::MoveRelAsync(int step)
{
    if (IsMoving())
    {
        string msg = "The relative moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    double position = step / m_Scale;
    if (!acsc_ToPoint(m_AcsHandle, ACSC_AMF_RELATIVE, R, position, NULL))
    {
        string msg = "ACS 1D motion relative moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

void MLACSCController1D::MoveAbsAsync(int step)
{
    if (IsMoving())
    {
        string msg = "The absolute moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    double position = step / m_Scale;
    if (!acsc_ToPoint(m_AcsHandle, 0, R, position, NULL))
    {
        string msg = "ACS 1D motion absolute moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

void MLACSCController1D::MoveRel(int step)
{
    if (IsMoving())
    {
        string msg = "The relative moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    double position = step / m_Scale;
    if (!acsc_ToPoint(m_AcsHandle, ACSC_AMF_RELATIVE, R, position, NULL))
    {
        string msg = "ACS 1D motion relative moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
    acsc_WaitMotionEnd(m_AcsHandle, R, 10000);
}

void MLACSCController1D::MoveAbs(int step)
{
    if (IsMoving())
    {
        string msg = "The absolute moving failed: the motion is moving!";
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return;
    }
    double position = step / m_Scale;
    if (!acsc_ToPoint(m_AcsHandle, 0, R, position, NULL))
    {
        string msg = "ACS 1D motion absolute moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
    acsc_WaitMotionEnd(m_AcsHandle, R, 10000);
}

void MLACSCController1D::StopMove()
{
    if (!acsc_Halt(m_AcsHandle, R, NULL))
    {
        string msg = "ACS 1D motion stop moving failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

void MLACSCController1D::Homing()
{
    if (m_IsConnected)
    {
        //    std::string buf = "HOME "+std::to_string(R)+",1,10";
        //    char* command = const_cast<char*>(buf.c_str());
        //    if (!acsc_Command(m_AcsHandle, command,buf.size() , NULL)) return ACSCErrorExecFailed;
        bool err = InitialMotionSystem();
        if (!err)
        {
            string msg = "ACS 1D motion homing failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
            LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
            m_IsHome = false;
            return;
        }
        m_IsHome = true;
    }
}

bool MLACSCController1D::IsHome()
{
    return m_IsHome;
}

void MLACSCController1D::SetSpeed(int speed)
{
    int s = speed / m_Scale;
    if (!acsc_SetVelocity(m_AcsHandle, R, s, NULL))
    {
        string msg = "ACS 1D motion set velocity failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
    }
}

int MLACSCController1D::GetSpeed()
{
    double speed;
    if (!acsc_GetVelocity(m_AcsHandle, R, &speed, NULL))
    {
        string msg = "ACS 1D motion get velocity failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return 0;
    }
    else
    {
        return int(speed * m_Scale);
    }
}

bool MLACSCController1D::IsMoving()
{
    int state;
    if (!acsc_GetAxisState(m_AcsHandle, R, &state, NULL))
    {
        string msg = "ACS 1D motion get moving state failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return false;
    }
    else
    {
        if (state & ACSC_MST_MOVE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

int MLACSCController1D::GetPosition()
{
    double pos;
    if (!acsc_GetFPosition(m_AcsHandle, R, &pos, NULL))
    {
        string msg = "ACS 1D motion get positions failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        return 0;
    }
    else
    {
        return round(pos * m_Scale);
    }
}

MLMotionState MLACSCController1D::GetState()
{
    int state;
    if (!acsc_GetAxisState(m_AcsHandle, R, &state, NULL))
    {
        string msg = "ACS 1D motion get moving state failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
        LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());
        m_State = MLMotionState::kMLStatusFailed;
    }
    else
    {
        if (state & ACSC_MST_MOVE)
        {
            m_State = MLMotionState::kMLStatusIsMoving;
        }
        else
        {
            m_State = MLMotionState::kMLStatusStationary;
        }
    }
    return m_State;
}

void MLACSCController1D::Subscribe(MLMotionEvent event, MLMotionCallback* callback)
{
    if (!m_Callbacks.count(event))
    {
        m_Callbacks[event] = callback;
        if (!m_CallbackFlag)
        {
            m_CallbackHandle = std::thread(&MLACSCController1D::threadFunc, this);
            m_CallbackFlag = true;
        }
    }
}

void MLACSCController1D::Unsubscribe(MLMotionEvent event, MLMotionCallback* callback)
{
    if (m_Callbacks.count(event))
    {
        m_Callbacks.erase(event);
        if (m_CallbackFlag && m_Callbacks.empty())
        {
            m_CallbackFlag = false;
            m_CallbackHandle.join();
        }
    }
}

void MLACSCController1D::threadFunc()
{
    while (m_CallbackFlag)
    {
        if (m_IsConnected) 
        {
            if (m_Callbacks.count(kStateChanged))
            {
                m_Callbacks[kStateChanged]->NotifyMotionStateChanged(m_State, GetState());
            }

            if (m_Callbacks.count(kPositionChanged))
            {
                m_Callbacks[kPositionChanged]->NotifyMotionPosition(GetPosition());
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool MLACSCController1D::InitialMotionSystem()
{
    if (direction_)
    {
        if (!acsc_RunBuffer(m_AcsHandle, 2, NULL, NULL))
        {
            string msg = "ACS 1D motion Initialize failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
            LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());;
            return false;
        }
    }
    else
    {
        if (!acsc_RunBuffer(m_AcsHandle, 3, NULL, NULL))
        {
            string msg = "ACS 1D motion Initialize failed: " + MLACSSystemController::GetInstance().GetErrorMessage();
            LOG4CPLUS_ERROR(LogPlus::getInstance()->logger, msg.c_str());;
            return false;
        }
    }

    acsc_WaitProgramEnd(m_AcsHandle,2,15000);
    return true;
}
