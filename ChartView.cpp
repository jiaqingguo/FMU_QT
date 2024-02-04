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
	if (m_isTouching) { // ������������״̬
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

 //����ԭʼλ��
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
	qreal factor = 1.1; // �������ŵ��ٶ�
	if (event->angleDelta().y() > 0) { // �������Ϲ���
		chart()->zoom(factor);
	}
	else { // �������¹���
		chart()->zoom(1 / factor);
	}
}
