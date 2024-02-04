#ifndef CURVESHOWDIALOG_H
#define CURVESHOWDIALOG_H

#include <QDialog>
#include <QChart>
#include <QChartView>
#include <QtCharts/QValueAxis>

#include "ChartView.h"

namespace Ui {
class curveShowDialog;
}

class curveShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit curveShowDialog(QWidget *parent = nullptr);
    ~curveShowDialog();

    void setCurveSHowData(const QString& title, const int paiNum,const QMap<int,QVector<double>> mapData);

    void clear_show();

  
private:
    Ui::curveShowDialog *ui;
    QtCharts::QChart *m_pChart;
   ChartView *m_pChartView;
   // QtCharts::QChartView* m_pChartView;
    QtCharts::QValueAxis *m_pAxisX;
    QtCharts::QValueAxis *m_pAxisY;
};

#endif // CURVESHOWDIALOG_H
