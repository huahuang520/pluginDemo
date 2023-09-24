#include "MTFChartWidget.h"
#include <QApplication>
#include <QtWidgets>
#include <QDebug>

MTFChartWidget::MTFChartWidget(QWidget* parent)
	: QWidget(parent)
{
	m_layout = new QVBoxLayout(this);

	this->initMtf();

	setLayout(m_layout);
	/*connect(chartXMaxButton, SIGNAL(clicked()), this, SLOT(on_chartXMaxButton_clicked()));
	connect(chartXMinButton, SIGNAL(clicked()), this, SLOT(on_chartXMinButton_clicked()));
	connect(resetXButton, SIGNAL(clicked()), this, SLOT(on_resetXButton_clicked()));*/

	//this->setMinimumHeight(300);
}

MTFChartWidget::~MTFChartWidget()
{

}

void MTFChartWidget::updateChartXUnit(int unit)
{
	//unit == 1 ? xAxis_mtf->setTitleText("lp/deg") : xAxis_mtf->setTitleText("lp/mm");
	unit == 1 ? m_mtfChart->setTitle("mtf(lp/deg)") : m_mtfChart->setTitle("mtf(lp/mm)");
}

void MTFChartWidget::on_chartXMaxButton_clicked()
{
	if (lineEditXMax->text().toInt() <= lineEditXMin->text().toInt())
	{
		QMessageBox::warning(this, "warning", "The maximum value of chartx is greater than the minimum value.", QMessageBox::Yes);
		lineEditXMax->setText(QString::number(m_tmpXMax));
		return;
	}

	m_tmpXMax = lineEditXMax->text().toInt();
	m_mtfChart->axisX()->setMax(m_tmpXMax);
}

void MTFChartWidget::on_chartXMinButton_clicked()
{
	if (lineEditXMin->text().toInt() >= lineEditXMax->text().toInt())
	{
		QMessageBox::warning(this, "warning", "The minimum value of chartx is less than the maximum value.", QMessageBox::Yes);
		lineEditXMin->setText(QString::number(m_tmpXMin));
		return;
	}

	m_tmpXMin = lineEditXMin->text().toInt();
	m_mtfChart->axisX()->setMin(m_tmpXMin);
}

void MTFChartWidget::on_resetXButton_clicked()
{
	lineEditXMax->setText(QString::number(100));
	lineEditXMin->setText(QString::number(0));

	m_mtfChart->axisX()->setMin(0);
	m_mtfChart->axisX()->setMax(100);
	m_tmpXMin = 0;
	m_tmpXMax = 100;
}

void MTFChartWidget::initMtf()
{
	m_mtfChart = new QChart();
	m_mtfChart->legend()->hide();
	m_mtfChart->setTitle("mtf");
	xAxis_mtf = new QValueAxis;
	//xAxis_mtf->setRange(m_mtfModel->GetMTFChartXMin(), m_mtfModel->GetMTFChartXMax());
	xAxis_mtf->setRange(0, 100);

	yAxis_mtf = new QValueAxis;
	yAxis_mtf->setRange(0, 100);
	m_mtfChart->addAxis(xAxis_mtf, Qt::AlignBottom);
	m_mtfChart->addAxis(yAxis_mtf, Qt::AlignLeft);
	m_mtfChart->layout()->setContentsMargins(0, 0, 0, 0);
	//QPen red(Qt::red); QPen green(Qt::green); QPen blue(Qt::blue); QPen black(Qt::black);
	for (int i = 0; i < m_ROInum; i++) {
		QLineSeries* s = new QLineSeries();
		m_mtfChart->addSeries(s);
		s->attachAxis(xAxis_mtf);
		s->attachAxis(yAxis_mtf);
		m_mtfSeries.push_back(s);
	}
	QChartView* cView = new QChartView(m_mtfChart);
	cView->setRenderHint(QPainter::Antialiasing);
	m_layout->addWidget(cView);

	m_mtfChart->setTitle("mtf(lp/deg)");
	m_mtfChart->setTitleBrush(QBrush(Qt::red));
}

void MTFChartWidget::updateMTFChart(QList<QList<QPointF>> mtf)
{
    QList<QPointF> list;
    for (int i = 0; i < 4; i++)
    {
        m_mtfSeries[i]->replace(list);
    }
    // static int xMax = 0, yMax = 0;
    for (int i = 0; i < mtf.size(); i++) //iter over 4 ROI
    {
        if (m_mtfSeries[i]->count() == 0) {
            //for (int j = 0; j < mtf.at(i).size(); j++) {
            //	QPointF p = mtf.at(i).at(j);
            //	*m_mtfSeries[i] << p;
            //}
            m_mtfSeries[i]->replace(mtf.at(i));
        }
        else {
            m_mtfSeries[i]->replace(mtf.at(i));
        }
    }
}
