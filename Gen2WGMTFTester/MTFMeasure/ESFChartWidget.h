#pragma once
#include <QGroupBox>
#include <QObject>
#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <vector>
#include <QList>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ESFChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ESFChartWidget(QWidget* parent = Q_NULLPTR);
    ~ESFChartWidget();
private:
    QLabel* label;
    QChart* m_esfChart;
    std::vector<QLineSeries*> m_esfSeries;
    int m_ROInum = 4;
    QHBoxLayout* m_layout;

    QValueAxis* xAxis_esf;
    QValueAxis* yAxis_esf;

    void initEsf();
    //void onModelContextSwitch();

public slots:
    void updateESFChart(QList<QList<QPointF>> esf);
signals:


};
