#include "graphdialog.h"
#include "ui_graphdialog.h"

graphDialog::graphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("算法输出曲线图"));
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
   // setupPlot();
    resize(1000, 500);
    // configure scroll bars:
    // Since scroll bars only support integer values, we'll set a high default range of -500..500 and
    // divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
    // axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
    // just increase the minimum/maximum values of the scroll bars as needed.
    ui->horizontalScrollBar->setRange(-500, 500);
    ui->verticalScrollBar->setRange(-500, 500);

    // create connection between axes and scroll bars:
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    // 设置滚轮及拖动;
    ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->plot->xAxis->setLabel(QString::fromLocal8Bit("拍数"));
    ui->plot->yAxis->setLabel(QString::fromLocal8Bit("输出值"));

    // initialize axis range (and scroll bar positions via signals we just connected):
   // ui->plot->xAxis->setRange(0, 10, Qt::AlignCenter);
   // ui->plot->yAxis->setRange(0, 10, Qt::AlignCenter);
   // 
    // 创建并设置图表的标题
    m_title = new QCPTextElement(ui->plot);
    m_title->setText(QString::fromLocal8Bit("曲线图标题"));
  //  m_title->setFont(QFont("sans", 12, QFont::Bold));
    ui->plot->plotLayout()->insertRow(0); // 在第一行插入标题
    ui->plot->plotLayout()->addElement(0, 0, m_title); // 将标题元素添加到布局中

     // 启用图例
    ui->plot->legend->setVisible(true); // 来显示线的名称；

    // 定义一个足够大的颜色集合，确保颜色不会重复
    for (int i = 0; i < 256 * 256 * 256; ++i)
    {
        m_colors << QColor(QRandomGenerator::global()->generate());
    }
    // 打乱颜色列表以确保随机性
    std::shuffle(m_colors.begin(), m_colors.end(), std::default_random_engine(QRandomGenerator::global()->generate()));
}

graphDialog::~graphDialog()
{
    delete ui;
}

void graphDialog::setDataShow(const QString& title, const int paiNum, const QMap<int, QVector<double>> &mapData)
{
    QList<QColor> colors=m_colors;
    // 直接清除所有图形
    ui->plot->clearGraphs();
    ui->plot->replot(); // 重绘图表以更新视图
   
   // ui->plot->xAxis->setRange(0, paiNum > 2 ? paiNum : 2, Qt::AlignBottom);
    m_title->setText(title);

    double minValue = 0;

    double maxValue = 2;

    bool ret = false;
    // 遍历mapData中的每一个键值对，为每一个键创建一条曲线
    for (auto it = mapData.begin(); it != mapData.end(); ++it) 
    {
        int key = it.key();
        QVector<double> y = it.value();
        QVector<double> x(y.size());
        std::iota(x.begin(), x.end(), 1); // 从0开始为每个y值创建一个x值

        // 创建曲线
        QCPGraph* newGraph = ui->plot->addGraph();
        ui->plot->graph()->setData(x, y);
        QString strName = "[" + QString::number(key) + "]";
        ui->plot->graph()->setName(strName); // 设置曲线的名称

  
      
        QPen pen;
        pen.setColor(QColor(colors.takeFirst())); 
        pen.setWidth(2); // 这里设置线条宽度为2像素
        ui->plot->graph()->setPen(pen);
        // 颜色、样式等设置，可以根据需要调整
       // newGraph->setLineStyle(QCPGraph::lsLine); // 线型
       // newGraph->setScatterStyle(QCPScatterStyle::ssCircle); // 点型

        int min = *std::min_element(it.value().constBegin(), it.value().constEnd());
        int max= *std::max_element(it.value().constBegin(), it.value().constEnd());
        // 获取最小值
         minValue = min<minValue? min : minValue;

        // 获取最大值
         maxValue = max > maxValue ? max : maxValue;
         ui->plot->graph()->rescaleAxes(ret);
         ret = true;
    }
 //   ui->plot->yAxis->setRange(minValue, maxValue, Qt::AlignLeft);
  //*  ui->plot->xAxis->setScaleType(QCPAxis::stLinear);
    ui->plot->replot();

   
}

void graphDialog::setupPlot()
{
    // The following plot setup is mostly taken from the plot demos:
    ui->plot->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::blue));
    ui->plot->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->plot->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::red));
    ui->plot->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::red));
    ui->plot->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::yellow));
    ui->plot->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::red));
    ui->plot->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::red));
    QVector<double> x(50000), y0(50000), y1(50000), y2(50000), y3(50000), y4(50000), y5(50000);
    for (int i = 0; i < 50000; ++i)
    {
        x[i] = i;
        y0[i] = i * 0.1;
        y1[i] = i * 0.2;
        y2[i] = i * 0.3;
        y3[i] = i * 0.4;
        y4[i] = i * 0.5;
        y5[i] = i * 0.6;
    }
    ui->plot->graph(0)->setData(x, y0);
    ui->plot->graph(1)->setData(x, y1);
    ui->plot->graph(2)->setData(x, y2);
    ui->plot->graph(3)->setData(x, y3);
    ui->plot->graph(4)->setData(x, y4);
    ui->plot->graph(5)->setData(x, y5);

    ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void graphDialog::horzScrollBarChanged(int value)
{
    if (qAbs(ui->plot->xAxis->range().center() - value / 100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        ui->plot->xAxis->setRange(value / 100.0, ui->plot->xAxis->range().size(), Qt::AlignCenter);
        ui->plot->replot();
    }
}

void graphDialog::vertScrollBarChanged(int value)
{
    if (qAbs(ui->plot->yAxis->range().center() + value / 100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        ui->plot->yAxis->setRange(-value / 100.0, ui->plot->yAxis->range().size(), Qt::AlignCenter);
        ui->plot->replot();
    }
}

void graphDialog::xAxisChanged(QCPRange range)
{
    ui->horizontalScrollBar->setValue(qRound(range.center() * 100.0)); // adjust position of scroll bar slider
    ui->horizontalScrollBar->setPageStep(qRound(range.size() * 100.0)); // adjust size of scroll bar slider
}

void graphDialog::yAxisChanged(QCPRange range)
{
    ui->verticalScrollBar->setValue(qRound(-range.center() * 100.0)); // adjust position of scroll bar slider
    ui->verticalScrollBar->setPageStep(qRound(range.size() * 100.0)); // adjust size of scroll bar slider
}