#pragma once
#include <QGroupBox>
#include <QObject>
#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <vector>
#include <QList>
#include <QLineEdit>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MTFChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MTFChartWidget(QWidget* parent = Q_NULLPTR);
    ~MTFChartWidget();
private:
    QLabel* label;
    QChart* m_mtfChart;
    std::vector<QLineSeries*> m_mtfSeries;
    int m_ROInum = 4;
    QVBoxLayout* m_layout;

    QValueAxis* xAxis_mtf;
    QValueAxis* yAxis_mtf;

    QLineEdit* lineEditXMax;
    QLineEdit* lineEditXMin;

    QPushButton* chartXMaxButton;
    QPushButton* chartXMinButton;
    QPushButton* resetXButton;

    int m_tmpXMax;
    int m_tmpXMin;
    int m_unit;
    void initMtf();
    //void onModelContextSwitch();

public slots:
    //void updateMTFChart(QVector<MLROI_RESULT> mtf);
    void updateMTFChart(QList<QList<QPointF>> mtf);
    void on_chartXMaxButton_clicked();
    void on_chartXMinButton_clicked();
    void on_resetXButton_clicked();
    void updateChartXUnit(int);
};
