#include "MTFMeasure.h"

MTFMeasure::MTFMeasure(QObject* parent) : QObject(parent)
{
	m_MTFMeasureWidget = new MTFMeasureWidget();
	
}

QWidget* MTFMeasure::GetWidget()
{
	return m_MTFMeasureWidget;
}
