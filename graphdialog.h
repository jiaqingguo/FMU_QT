#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>

#include "qcustomplot.h"

namespace Ui {
class graphDialog;
}

class graphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit graphDialog(QWidget *parent = nullptr);
    ~graphDialog();
    void setDataShow(const QString& title, const int paiNum, const QMap<int, QVector<double>> mapData);
    void setupPlot();
   

private slots:
    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);
private:
    Ui::graphDialog *ui;

    QList<QColor> m_colors;
    QCPTextElement* m_title;
};

#endif // GRAPHDIALOG_H
