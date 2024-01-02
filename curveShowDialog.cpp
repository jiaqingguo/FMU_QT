#include <QGridLayout>
#include <QtCharts/QSplineSeries>

#include "curveShowDialog.h"
#include "ui_curveShowDialog.h"

curveShowDialog::curveShowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curveShowDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(0);
    setLayout(layout);

    //m_pChartView= new QtCharts::QChartView(this);

    m_pChartView = new ChartView();


    //m_pChartView->setRenderHint(QPainter::Antialiasing);

    //// 启用滚轮缩放
    //m_pChartView->setRenderHint(QPainter::Antialiasing, true);
    //m_pChartView->setRenderHint(QPainter::SmoothPixmapTransform, true);
    //m_pChartView->setRubberBand(QtCharts::QChartView::NoRubberBand);

    //// 设置缩放的中心点为鼠标位置
    //m_pChartView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //:setRubberBand(QChartView::RectangleRubberBand)
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
    m_pAxisX->setTickInterval(1);  // 设置刻度间隔为1
    m_pAxisX->setRange(0, 10);

    m_pAxisY->setTitleText(QString::fromLocal8Bit("输出值"));
    m_pAxisY->setRange(0, 20);

}

curveShowDialog::~curveShowDialog()
{
    delete ui;
}

void curveShowDialog::setCurveSHowData(const QString& title, const int paiNum,const QMap<int, QVector<double> > mapData)
{
    clear_show();
    m_pChart->setTitle(title);

    // 确定x轴 y轴的最大值最小值;

    double min = 0;
    double max = 10;

    m_pAxisX->setRange(0, paiNum >2? paiNum :2);
    for(  auto itor = mapData.begin();itor!=mapData.end();itor++)
    {
        //创建线
       QtCharts::QSplineSeries*ser = new QtCharts::QSplineSeries;

       //设置名字
       QString strName ="["+QString::number(itor.key())+"]";
       ser->setName(strName);
       ser->setPointsVisible(true);
       int x = 0;
       // 线条添加数据;
       for(int i =0;i< paiNum;i++)
       {
           x=i + 1;
           double value = itor.value().at(i);
           ser->append(x,itor.value().at(i));

           if (max < itor.value().at(i))
           {
               max = itor.value().at(i);
           }
           if (min > itor.value().at(i))
           {
               min = itor.value().at(i);
           }
       }

       m_pAxisY->setRange(min, max);
       //放入charts里
       m_pChart->addSeries(ser);
       //将线条放入表中
       m_pChart->setAxisX(m_pAxisX,ser);
       m_pChart->setAxisY(m_pAxisY,ser);

    }
}

void curveShowDialog::clear_show()
{
    m_pChart->removeAllSeries();
}
