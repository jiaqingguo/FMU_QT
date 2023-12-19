#ifndef CURVESHOWDIALOG_H
#define CURVESHOWDIALOG_H

#include <QDialog>
#include <QChart>
#include <QChartView>
#include <QtCharts/QValueAxis>

namespace Ui {
class curveShowDialog;
}

class curveShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit curveShowDialog(QWidget *parent = nullptr);
    ~curveShowDialog();

    void setCurveSHowData(const int paiNum,const QMap<int,QVector<double>> mapData);
private:
    Ui::curveShowDialog *ui;
    QtCharts::QChart *m_pChart;
    QtCharts::QChartView *m_pChartView;
    QtCharts::QValueAxis *m_pAxisX;
    QtCharts::QValueAxis *m_pAxisY;
};

#endif // CURVESHOWDIALOG_H
