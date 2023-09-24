#include "ESFChartWidget.h"
#include <QApplication>
#include <QtWidgets>
#include <QDebug>

ESFChartWidget::ESFChartWidget(QWidget* parent)
	: QWidget(parent)
{
	m_layout = new QHBoxLayout;
	this->initEsf();
	setLayout(m_layout);
	//setMinimumWidth(300);
	//setMinimumHeight(250);
}

ESFChartWidget::~ESFChartWidget()
{

}


void ESFChartWidget::initEsf()
{
	m_esfChart = new QChart();
	m_esfChart->legend()->hide();
	m_esfChart->setTitle("esf");
	xAxis_esf = new QValueAxis;
	xAxis_esf->setRange(0, 100);
	yAxis_esf = new QValueAxis;
	yAxis_esf->setRange(0, 1);
	m_esfChart->addAxis(xAxis_esf, Qt::AlignBottom);
	m_esfChart->addAxis(yAxis_esf, Qt::AlignLeft);
	m_esfChart->layout()->setContentsMargins(0, 0, 0, 0);

	for (int i = 0; i < m_ROInum; i++) {
		QLineSeries* s = new QLineSeries();
		m_esfChart->addSeries(s);
		s->attachAxis(xAxis_esf);
		s->attachAxis(yAxis_esf);
		m_esfSeries.push_back(s);
	}
	QChartView* cView = new QChartView(m_esfChart);
	cView->setRenderHint(QPainter::Antialiasing);
	m_layout->addWidget(cView);
	m_esfChart->setTitleBrush(QBrush(Qt::red));
}


void ESFChartWidget::updateESFChart(QList<QList<QPointF>> esf)
{
    QList<QPointF> list;
    for (int i = 0; i < 4; i++)
    {
        m_esfSeries[i]->replace(list);
    }
	int xMax = 0;
	int yMax = 0;
	for (int i = 0; i < esf.size(); i++) //iter over 4 ROI
	{
        for (int j = 0; j < esf.at(i).size(); j++)
        {
            if (xMax < esf.at(i).at(j).x())
            {
                xMax = esf.at(i).at(j).x();
            }
            if (yMax < esf.at(i).at(j).y())
            {
                yMax = esf.at(i).at(j).y();
            }
        }
        m_esfChart->axisX()->setRange(0, xMax);
        //m_esfChart->axisY()->setRange(0, yMax);
		if (m_esfSeries[i]->count() == 0) {
			//for (int j = 0; j < esf.at(i).size(); j++) {
			//	QPointF p = esf.at(i).at(j);
			//	*m_esfSeries[i] << p;
			//}
            m_esfSeries[i]->replace(esf.at(i));
		}
		else {
			m_esfSeries[i]->replace(esf.at(i));
		}
	}
}
