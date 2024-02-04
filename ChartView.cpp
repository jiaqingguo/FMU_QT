#include<QValueAxis>

#include "ChartView.h"

ChartView::ChartView()
{
	setRenderHint(QPainter::Antialiasing);
}

void ChartView::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		m_isTouching = true;
		m_lastMousePos = event->pos();
		event->accept();
	}
	else {
		QChartView::mousePressEvent(event);
	}
}

void ChartView::mouseMoveEvent(QMouseEvent * event)
{
	if (m_isTouching) { // 如果是左键按下状态
		auto dx = event->pos().x() - m_lastMousePos.x();
		auto dy = event->pos().y() - m_lastMousePos.y();
		chart()->scroll(-dx, dy);
		m_lastMousePos = event->pos();
		event->accept();
	}
	else {
		QChartView::mouseMoveEvent(event);
	}
}

 void ChartView::mouseReleaseEvent(QMouseEvent * event)
 {
	 if (event->button() == Qt::LeftButton && m_isTouching) {
		 m_isTouching = false;
		 event->accept();
	 }
	 else {
		 QChartView::mouseReleaseEvent(event);
	 }
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
	qreal factor = 1.1; // 设置缩放的速度
	if (event->angleDelta().y() > 0) { // 滚轮向上滚动
		chart()->zoom(factor);
	}
	else { // 滚轮向下滚动
		chart()->zoom(1 / factor);
	}
}
