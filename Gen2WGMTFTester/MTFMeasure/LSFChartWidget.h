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

class LSFChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LSFChartWidget(QWidget* parent = Q_NULLPTR);
    ~LSFChartWidget();
private:
    QChart* m_lsfChart;
    std::vector<QLineSeries*> m_lsfSeries;
    int m_ROInum = 4;
    QHBoxLayout* m_layout;

    QValueAxis* xAxis_lsf;
    QValueAxis* yAxis_lsf;

    void initLsf();
    //void onModelContextSwitch();

public slots:
    void updateLSFChart(QList<QList<QPointF>> lsf);
signals:


};
