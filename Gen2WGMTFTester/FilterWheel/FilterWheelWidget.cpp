#include "FilterWheelWidget.h"
#include "json.hpp"
#include <fstream>
#include <QListView>

using json = nlohmann::json;

FilterWheelWidget::FilterWheelWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    initStyle();
}

FilterWheelWidget::~FilterWheelWidget()
{}

bool FilterWheelWidget::Initconfig(std::list<std::string> configPath)
{
    m_NDPosList.clear();
    m_NXYZPosList.clear();
    m_RxPosList.clear();
    m_RxAxisPosList.clear();
    for (auto it : configPath)
    {
        std::ifstream ifs(it.c_str(), std::fstream::in);

        if (ifs.fail())
        {
            ifs.close();
            return false;
            //throw std::runtime_error("Unable to open MotionConfig File.");
        }
        json JsonControl;
        ifs >> JsonControl;
        ifs.close();

        auto filterConfig = JsonControl["FilterMap"];
        for (auto& it : filterConfig.items())
        {
            std::string filterName = it.value()["Name"].get<std::string>();
            auto positionList = it.value()["PositionList"];
            for (auto& it : positionList.items())
            {
                std::string posName = it.value()["Name"].get<std::string>();
                if (filterName.find("ND") != -1)
                    m_NDPosList.push_back(posName);
                else if (filterName.find("XYZ") != -1)
                    m_NXYZPosList.push_back(posName);
                else if (filterName.find("Rx") != -1 || filterName.find("RX") != -1)
                    m_RxPosList.push_back(posName);
            }
        }
    }

    for (auto it : m_NDPosList)
        ui.NDList->addItem(it.c_str());

    for (auto it : m_NXYZPosList)
        ui.XYZList->addItem(it.c_str());

    for (auto it : m_RxPosList)
        ui.RxList->addItem(it.c_str());

    for (auto it : m_RxAxisPosList)
        //ui.RxAxisList->addItem(it.c_str());

	return true;
}

void FilterWheelWidget::initStyle()
{
    /**
    *     //效果：comboBox表头显示黑色，字体白色
    *     QComboBox{
    *         border-left:1px solid rgb(195,195,195);
    *         border-right:1px solid rgb(195,195,195);
    *         background:rgb(50,50,50);
    *         color:white;
    *         selection-background-color:rgb(80,52,158);
    *         combobox-popup:0;
    *     }
    * 
    *     //在combxBox可编辑状态时，编辑字体为白色，20px，需设置comboBox->setEditable(true)，一般用不到;
    *     QComboBox:editable{
    *         color:white;
    *         font:20px;
    *     }
    * 
    *     //
    *     QComboBox:enabled{
    *           border:1pxsolidrgba(195,195,195,0);
    *           selection-background-color:rgb(80,52,158);
    *     }
    * 
    *     //下拉框的显示框效果
    *     QComboBox QAbstractItemView{
    *           border:1px solid rgb(195, 195, 195);
    *           border:2px;
    *       }
    * 
    */

    //下拉框以listWidget的格式显示
    ui.NDList->setView(new QListView());
    ui.XYZList->setView(new QListView());
    ui.RxList->setView(new QListView());
    //设置滚动条的重点在这
    //ui.NDList->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui.setRxAxis->setStyleSheet(
        "QPushButton{color: rgb(255, 255, 255);background-color: rgba(255, 0, 0, 30%);border:1px outset rgb(255, 255, 255);border-radius:5px;text-align: center center;width: 100px;height: 25px;font: bold 15px;}"
        "QPushButton:hover{background-color: rgba(255, 0, 0, 50%);border:2px outset rgba(36, 36, 36, 0);}"
        "QPushButton:pressed {background-color: rgba(255, 0, 0, 90%);border:4px outset rgba(36, 36, 36, 0);}"
        "QPushButton:checked {background-color: rgba(255, 0, 0, 100%);border:4px outset rgba(36, 36, 36, 0);}"
    );

    ui.openPort->setStyleSheet(
        "QPushButton{background-color: rgba(184, 180, 165);border-radius: 4px;border: none;width: 30px;height: 30px; color: rgb(180, 0, 0);font: bold 25px;}"
        "QPushButton:hover{background: rgb(144, 140, 123);}"
        "QPushButton:pressed {background: rgb(125, 125, 125); }"
    );

    ui.closePort->setStyleSheet(
        "QPushButton{background-color: rgba(184, 180, 165);border-radius: 4px;border: none;width: 30px;height: 30px; color: rgb(180, 0, 0);font: bold 25px;}"
        "QPushButton:hover{background: rgb(144, 140, 123);}"
        "QPushButton:pressed {background: rgb(125, 125, 125); }"
    );

    ui.axisEdit->setPlaceholderText("degree:0-180");
}

void FilterWheelWidget::on_setRxAxis_clicked()
{
    if (ui.axisEdit->text() == "")
        return;
    int degree = ui.axisEdit->text().toInt();
}