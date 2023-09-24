#pragma once

#include "configuration_global.h"
#include "ConfigurationPlugin.h"
#include <string>
#include <json.hpp>
#include <fstream>

using namespace std;
using json = nlohmann::json;

class CONFIGURATION_EXPORT Configuration : public QObject, public ConfigurationPlugin
{
    Q_OBJECT
        /*
        *Use Q_INTERFACES() macro tells Qt's meta-object system about the interface and tells the meta-object system "I want to use this interface
        */
        Q_INTERFACES(ConfigurationPlugin)
        /*
        * Use Q_ PLUGIN_ METADATA() macro export plug-in
        */
        Q_PLUGIN_METADATA(IID AbstractInterface_iid FILE "Configuration.json")
public:
    Configuration(QObject* parent = nullptr);
    ~Configuration();
    AllConfig ReadFromConfig(ConfigMode configMode);
    void WriteToConfig(AllConfig allConfig, ConfigMode configMode);
    QWidget* GetWidget();
private:
    bool loadJson(ConfigMode configMode);
    bool Load(const char* path);

private:
    //plc config
    PLCControlConfig ReadPLCConfig();
    void WritePLCConfig(PLCControlConfig plcConfig);
    //motion3D config
    Motion3DConfig ReadMotion3DConfig();
    void WriteMotion3DConfig(Motion3DConfig motionConfig);

    //levelAdjust config
    LevelAdjustConfig ReadLevelAdjustConfig();
    void WriteLevelAdjustConfig(LevelAdjustConfig levelAdjustConfig);

    //gradienter config
    GradienterConfig ReadGradienterConfig();
    void WriteGradienterConfig(GradienterConfig gradienterConfig);

private:
    json m_JsonControl;
    std::string m_path;
};
