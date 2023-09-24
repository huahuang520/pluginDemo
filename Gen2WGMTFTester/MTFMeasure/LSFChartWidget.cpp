#include "LSFChartWidget.h"
#include <QApplication>
#include <QtWidgets>
#include <QDebug>

LSFChartWidget::LSFChartWidget(QWidget* parent)
	: QWidget(parent)
{
	m_layout = new QHBoxLayout;

	this->initLsf();
	setLayout(m_layout);
	//setMinimumWidth(300);
}

LSFChartWidget::~LSFChartWidget()
{

}

void LSFChartWidget::initLsf()
{
	m_lsfChart = new QChart();
	m_lsfChart->legend()->hide();
	m_lsfChart->setTitle("lsf");
	xAxis_lsf = new QValueAxis;
	xAxis_lsf->setRange(0, 100);
	yAxis_lsf = new QValueAxis;
	yAxis_lsf->setRange(0, 1);
	m_lsfChart->addAxis(xAxis_lsf, Qt::AlignBottom);
	m_lsfChart->addAxis(yAxis_lsf, Qt::AlignLeft);
	m_lsfChart->layout()->setContentsMargins(0,0,0,0);

	for (int i = 0; i < m_ROInum; i++) {
		QLineSeries* s = new QLineSeries();
		m_lsfChart->addSeries(s);
		s->attachAxis(xAxis_lsf);
		s->attachAxis(yAxis_lsf);
		m_lsfSeries.push_back(s);
	}
	QChartView* cView = new QChartView(m_lsfChart);
	cView->setRenderHint(QPainter::Antialiasing);
	m_layout->addWidget(cView);
	m_lsfChart->setTitleBrush(QBrush(Qt::red));
}


void LSFChartWidget::updateLSFChart(QList<QList<QPointF>> lsf)
{
    QList<QPointF> list;
    for (int i = 0; i < 4; i++)
    {
        m_lsfSeries[i]->replace(list);
    }
    static int xMax = 0, yMax = 0;
	for (int i = 0; i < lsf.size(); i++) //iter over 4 ROI
	{
        for (int j = 0; j < lsf.at(i).size(); j++)
        {
            if (xMax < lsf.at(i).at(j).x())
            {
                xMax = lsf.at(i).at(j).x();
            }
            if (yMax < lsf.at(i).at(j).y())
            {
                yMax = lsf.at(i).at(j).y();
            }
        }
        m_lsfChart->axisX()->setRange(0, xMax);
        m_lsfChart->axisY()->setRange(0, yMax);
		if (m_lsfSeries[i]->count() == 0) {
			//for (int j = 0; j < lsf.at(i).size(); j++) {
			//	QPointF p = lsf.at(i).at(j);
			//	*m_lsfSeries[i] << p;
			//}
            m_lsfSeries[i]->replace(lsf.at(i));
		}
		else {
			m_lsfSeries[i]->replace(lsf.at(i));
		}
	}
}