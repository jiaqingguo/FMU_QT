#include<QValueAxis>

#include "ChartView.h"

ChartView::ChartView()
{

}

void ChartView::mousePressEvent(QMouseEvent * event)
{
	//if (event->button() == Qt::LeftButton)
	//{
	//	m_lastPoint = event->pos();
	//	m_isPress = true;
	//}
}

void ChartView::mouseMoveEvent(QMouseEvent * event)
{
	/*if (!m_coordItem)
	{
	    m_coordItem = new QGraphicsSimpleTextItem(this->chart());
	    m_coordItem->setZValue(5);
	    m_coordItem->setPos(100, 60);
	    m_coordItem->show();
	}
	const QPoint curPos = event->pos();
	QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
	QString coordStr = QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y());*/
	//m_coordItem->setText(coordStr);
	
	/*if (m_isPress)
	{
		QPoint offset = curPos - m_lastPoint;
		m_lastPoint = curPos;
		if (!m_alreadySaveRange)
		{
			this->saveAxisRange();
			m_alreadySaveRange = true;
		}
		this->chart()->scroll(-offset.x(), offset.y());
	}*/
}

 void ChartView::mouseReleaseEvent(QMouseEvent * event)
 {
	/*m_isPress = false;
	if (event->button() == Qt::RightButton)
	{
		if (m_alreadySaveRange)
		{
			this->chart()->axisX()->setRange(m_xMin, m_xMax);
			this->chart()->axisY()->setRange(m_yMin, m_yMax);
		}
	}*/
}

 //保存原始位置
 void ChartView::saveAxisRange()
 {
	 //QtCharts::QValueAxis * axisX = dynamic_cast<QtCharts::QValueAxis*>(this->chart()->axisX());
	 //    m_xMin = axisX->min();
	 //    m_xMax = axisX->max();
		// QtCharts::QValueAxis * axisY = dynamic_cast<QtCharts::QValueAxis*>(this->chart()->axisY());
	 //    m_yMin = axisY->min();
	 //    m_yMax = axisY->max();
	 }


void ChartView::wheelEvent(QWheelEvent* event)
{
	const QPoint curPos = event->pos();
	QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
	
	if (!m_alreadySaveRange)
	{
	    this->saveAxisRange();
	    m_alreadySaveRange = true;
	}
	const double factor = 1.5;//缩放比例
	if (true)
	{//Y轴
		QtCharts::QValueAxis * axisY = dynamic_cast<QtCharts::QValueAxis*>(this->chart()->axisY());
		const double yMin = axisY->min();
		const double yMax = axisY->max();
		const double yCentral = curVal.y();
		
		double bottomOffset;
		double topOffset;
		if (event->delta() > 0)
		{//放大
			bottomOffset = 1.0 / factor * (yCentral - yMin);
			topOffset = 1.0 / factor * (yMax - yCentral);
		}
		else
		{//缩小
			bottomOffset = 1.0 * factor * (yCentral - yMin);
			topOffset = 1.0 * factor * (yMax - yCentral);
		}
		
		this->chart()->axisY()->setRange(yCentral - bottomOffset, yCentral + topOffset);
	}
	else
	{//X轴
		QtCharts::QValueAxis * axisX = dynamic_cast<QtCharts::QValueAxis*>(this->chart()->axisX());
		const double xMin = axisX->min();
		const double xMax = axisX->max();
		const double xCentral = curVal.x();
		
		double leftOffset;
		double rightOffset;
		if (event->delta() > 0)
		{//放大
			leftOffset = 1.0 / factor * (xCentral - xMin);
			rightOffset = 1.0 / factor * (xMax - xCentral);
		}
		else
		{//缩小
			leftOffset = 1.0 * factor * (xCentral - xMin);
			rightOffset = 1.0 * factor * (xMax - xCentral);
		}
		this->chart()->axisX()->setRange(xCentral - leftOffset, xCentral + rightOffset);
	}
}
