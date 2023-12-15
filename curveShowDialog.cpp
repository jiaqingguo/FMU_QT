#include <QGridLayout>
#include <QtCharts/QSplineSeries>

#include "curveShowDialog.h"
#include "ui_curveShowDialog.h"

curveShowDialog::curveShowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curveShowDialog)
{
    ui->setupUi(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(0);
    setLayout(layout);

    m_pChartView= new QtCharts::QChartView(this);
    layout->addWidget(m_pChartView);

    m_pChart = new QtCharts::QChart();
    m_pChartView->setChart(m_pChart);
    // 创建xY轴
    m_pAxisX = new QtCharts::QValueAxis();
    m_pAxisY = new QtCharts::QValueAxis();
    // 设置显示xy轴
    m_pChart->addAxis(m_pAxisX,Qt::AlignBottom);
    m_pChart->addAxis(m_pAxisY,Qt::AlignLeft);

    //m_pAxisX->setTickCount(5); // 分成多少格;
    m_pAxisX->setTitleText(QString::fromLocal8Bit("拍数"));
    m_pAxisX->setMinorTickCount(0);  // 禁用次要刻度
    m_pAxisX->setTickType(QtCharts::QValueAxis::TickType::TicksDynamic);  // 动态刻度
    m_pAxisX->setTickInterval(2);  // 设置刻度间隔为2
    m_pAxisX->setRange(0, 10);

    m_pAxisY->setTitleText(QString::fromLocal8Bit("输出值"));
    m_pAxisY->setRange(0, 20);

}

curveShowDialog::~curveShowDialog()
{
    delete ui;
}

void curveShowDialog::setCurveSHowData(const int paiNUm,const QMap<int, QVector<double> > mapData)
{

    m_pAxisX->setRange(0,paiNUm>10? paiNUm:10);
    for(  auto itor = mapData.begin();itor!=mapData.end();itor++)
    {
        //创建线
       QtCharts::QLineSeries *ser = new QtCharts::QLineSeries;

       //设置名字
       QString strName ="["+QString::number(itor.key())+"]";
       ser->setName(strName);

       // 线条添加数据;
       for(int i =0;i< paiNUm;i++)
       {
           ser->append(i,itor.value().at(i));
       }

       //放入charts里
       m_pChart->addSeries(ser);
       //将线条放入表中
       m_pChart->setAxisX(m_pAxisX,ser);
       m_pChart->setAxisY(m_pAxisY,ser);

    }
}
