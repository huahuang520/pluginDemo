#include "Configuration.h"
#include <QCoreApplication>
#include "ConfigWidget.h"
Configuration::Configuration(QObject* parent) : QObject(parent)
{
}

Configuration::~Configuration()
{
}

AllConfig Configuration::ReadFromConfig(ConfigMode configMode)
{
    AllConfig allConfig;
    if (!loadJson(configMode)) return allConfig;
    switch (configMode)
    {
    case ConfigMode::PLC_Config:
        allConfig.plcControl = ReadPLCConfig();
        break;
    case ConfigMode::Motion3D_Config:
        allConfig.motion3DConfig = ReadMotion3DConfig();
        break;
    case ConfigMode::LevelAdjust_Config:
        allConfig.levelAdjustConfig = ReadLevelAdjustConfig();
        break;
    case ConfigMode::Gradienter_Config:
        allConfig.gradienterConfig = ReadGradienterConfig();
        break;
    }
    return allConfig;
}

void Configuration::WriteToConfig(AllConfig allConfig, ConfigMode configMode)
{
    if (!loadJson(configMode)) return;
    switch (configMode)
    {
    case ConfigMode::PLC_Config:
        WritePLCConfig(allConfig.plcControl);
        break;
    case ConfigMode::Motion3D_Config:
        WriteMotion3DConfig(allConfig.motion3DConfig);
        break;
    case ConfigMode::LevelAdjust_Config:
        WriteLevelAdjustConfig(allConfig.levelAdjustConfig);
        break;
    case ConfigMode::Gradienter_Config:
        WriteGradienterConfig(allConfig.gradienterConfig);
        break;
    }

    std::ofstream os(m_path);
    os << m_JsonControl.dump(4);
    os.close();
}

QWidget* Configuration::GetWidget()
{
    ConfigWidget* widget = new ConfigWidget();
    widget->ReadLevelAdjustConfig(ReadFromConfig(ConfigMode::LevelAdjust_Config));
    qRegisterMetaType<AllConfig>("AllConfig");
    qRegisterMetaType<ConfigMode>("ConfigMode");
    connect(widget, &ConfigWidget::updateAllConfig, this, &Configuration::WriteToConfig);
    return widget;
}

bool Configuration::loadJson(ConfigMode configMode)
{
    QString exePath = QCoreApplication::applicationDirPath();
    //read config from exe path config
    switch (configMode)
    {
    case ConfigMode::PLC_Config:
        exePath += "/config/PLCConfig.json";
        break;
    case ConfigMode::Motion3D_Config:
        exePath += "/config/Motion3DConfig.json";
        break;
    case ConfigMode::LevelAdjust_Config:
        exePath += "/config/LevelAdjustConfig .json";
        break;
    case ConfigMode::Gradienter_Config:
        exePath += "/config/GradienterConfig.json";
        break;
    }

    return Load(exePath.toStdString().c_str());
}

bool Configuration::Load(const char* path)
{
    m_path = path;
    std::ifstream ifs(path, std::fstream::in);

    if (ifs.fail())
    {
        ifs.close();
        return false;
        //throw std::runtime_error("Unable to open MotionConfig File.");
    }

    ifs >> m_JsonControl;
    ifs.close();
    return true;
}

PLCControlConfig Configuration::ReadPLCConfig()
{
    //---------read pcl config-----------
    PLCControlConfig plcControl;
    plcControl.ipAddress = m_JsonControl["Connection"]["IpAddress"].get<std::string>();
    plcControl.rock = m_JsonControl["Connection"]["Rock"].get<int>();
    plcControl.slot = m_JsonControl["Connection"]["Solt"].get<int>();
    plcControl.dataBaseNumber = m_JsonControl["DataBaseNumber"].get<int>();
    plcControl.tip_tilt.clear();
    plcControl.tip_tilt.insert(pair<string, string>("Axis1_offset_Enable", m_JsonControl["tip/tilt"]["Axis1_offset_Enable"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis1_offset_Stop", m_JsonControl["tip/tilt"]["Axis1_offset_Stop"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis1_offset_Speed", m_JsonControl["tip/tilt"]["Axis1_offset_Speed"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis1_offset_Mov", m_JsonControl["tip/tilt"]["Axis1_offset_Mov"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis1_offset_Pos", m_JsonControl["tip/tilt"]["Axis1_offset_Pos"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis1_offset_Busy", m_JsonControl["tip/tilt"]["Axis1_offset_Busy"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis2_offset_Enable", m_JsonControl["tip/tilt"]["Axis2_offset_Enable"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis2_offset_Stop", m_JsonControl["tip/tilt"]["Axis2_offset_Stop"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis2_offset_Speed", m_JsonControl["tip/tilt"]["Axis2_offset_Speed"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis2_offset_Mov", m_JsonControl["tip/tilt"]["Axis2_offset_Mov"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis2_offset_Pos", m_JsonControl["tip/tilt"]["Axis2_offset_Pos"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis2_offset_Busy", m_JsonControl["tip/tilt"]["Axis2_offset_Busy"].get<std::string>()));
   /* plcControl.tip_tilt.insert(pair<string, string>("Axis1_PulseConversionAngle", m_JsonControl["tip/tilt"]["Axis1_PulseConversionAngle"].get<std::string>()));
    plcControl.tip_tilt.insert(pair<string, string>("Axis2_PulseConversionAngle", m_JsonControl["tip/tilt"]["Axis2_PulseConversionAngle"].get<std::string>()));*/
    return plcControl;
}

void Configuration::WritePLCConfig(PLCControlConfig plcConfig)
{
    //----------write to pcl config---------------
    m_JsonControl["Connection"]["IpAddress"] = plcConfig.ipAddress;
    m_JsonControl["Connection"]["Rock"] = plcConfig.rock;
    m_JsonControl["Connection"]["Solt"] = plcConfig.slot;
    m_JsonControl["DataBaseNumber"] = plcConfig.dataBaseNumber;
}

Motion3DConfig Configuration::ReadMotion3DConfig()
{
    Motion3DConfig config;
    config.ipAddress1 = m_JsonControl["Motion3D_load"]["IP"].get<std::string>();
    config.type1 = m_JsonControl["Motion3D_load"]["Type"].get<std::string>();
    config.ipAddress2 = m_JsonControl["Motion3D_scan"]["IP"].get<std::string>();
    config.type2 = m_JsonControl["Motion3D_scan"]["Type"].get<std::string>();
    return config;
}

void Configuration::WriteMotion3DConfig(Motion3DConfig motionConfig)
{
    m_JsonControl["Motion3D_load"]["IP"] = motionConfig.ipAddress1;
    m_JsonControl["Motion3D_load"]["Type"] = motionConfig.type1;
    m_JsonControl["Motion3D_scan"]["IP"] = motionConfig.ipAddress2;
    m_JsonControl["Motion3D_scan"]["Type"] = motionConfig.type2;
}

LevelAdjustConfig Configuration::ReadLevelAdjustConfig()
{
    LevelAdjustConfig levelAdjustConfig;
    levelAdjustConfig.motion3DX = m_JsonControl["Motion3D_load"]["X"].get<double>();
    levelAdjustConfig.motion3DY = m_JsonControl["Motion3D_load"]["Y"].get<double>();
    levelAdjustConfig.motion3DZ = m_JsonControl["Motion3D_load"]["Z"].get<double>();
    levelAdjustConfig.angleRange = m_JsonControl["AngleRange"].get<double>();
    levelAdjustConfig.adjustTimesMax = m_JsonControl["AdjustTimesMax"].get<int>();
    levelAdjustConfig.pulseConversionAngle_X = m_JsonControl["PulseConversionAngle_X"].get<double>();
    levelAdjustConfig.pulseConversionAngle_Y = m_JsonControl["PulseConversionAngle_Y"].get<double>();
    return levelAdjustConfig;
}

void Configuration::WriteLevelAdjustConfig(LevelAdjustConfig levelAdjustConfig)
{
    m_JsonControl["Motion3D_load"]["X"] = levelAdjustConfig.motion3DX;
    m_JsonControl["Motion3D_load"]["Y"] = levelAdjustConfig.motion3DY;
    m_JsonControl["Motion3D_load"]["Z"] = levelAdjustConfig.motion3DZ;
    m_JsonControl["AngleRange"] = levelAdjustConfig.angleRange;
    m_JsonControl["AdjustTimesMax"] = levelAdjustConfig.adjustTimesMax;
    m_JsonControl["PulseConversionAngle_X"] = levelAdjustConfig.pulseConversionAngle_X;
    m_JsonControl["PulseConversionAngle_Y"] = levelAdjustConfig.pulseConversionAngle_Y;
}

GradienterConfig Configuration::ReadGradienterConfig()
{
    GradienterConfig config;
    config.ConnectAddress = m_JsonControl["Gradienter"]["ConnectAddress"].get<std::string>();
    config.Type = m_JsonControl["Gradienter"]["Type"].get<string>();
    return config;
}

void Configuration::WriteGradienterConfig(GradienterConfig gradienterConfig)
{
    m_JsonControl["Gradienter"]["ConnectAddress"] = gradienterConfig.ConnectAddress;
    m_JsonControl["Gradienter"]["Type"] = gradienterConfig.Type;
}
