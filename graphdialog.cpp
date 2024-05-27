#include "graphdialog.h"
#include "ui_graphdialog.h"

graphDialog::graphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("�㷨�������ͼ"));
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
    // ���ù��ּ��϶�;
    ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->plot->xAxis->setLabel(QString::fromLocal8Bit("����"));
    ui->plot->yAxis->setLabel(QString::fromLocal8Bit("���ֵ"));

    // initialize axis range (and scroll bar positions via signals we just connected):
   // ui->plot->xAxis->setRange(0, 10, Qt::AlignCenter);
   // ui->plot->yAxis->setRange(0, 10, Qt::AlignCenter);
   // 
    // ����������ͼ��ı���
    m_title = new QCPTextElement(ui->plot);
    m_title->setText(QString::fromLocal8Bit("����ͼ����"));
  //  m_title->setFont(QFont("sans", 12, QFont::Bold));
    ui->plot->plotLayout()->insertRow(0); // �ڵ�һ�в������
    ui->plot->plotLayout()->addElement(0, 0, m_title); // ������Ԫ����ӵ�������

     // ����ͼ��
    ui->plot->legend->setVisible(true); // ����ʾ�ߵ����ƣ�

    // ����һ���㹻�����ɫ���ϣ�ȷ����ɫ�����ظ�
    for (int i = 0; i < 256 * 256 * 256; ++i)
    {
        m_colors << QColor(QRandomGenerator::global()->generate());
    }
    // ������ɫ�б���ȷ�������
    std::shuffle(m_colors.begin(), m_colors.end(), std::default_random_engine(QRandomGenerator::global()->generate()));
}

graphDialog::~graphDialog()
{
    delete ui;
}

void graphDialog::setDataShow(const QString& title, const int paiNum, const QMap<int, QVector<double>> &mapData)
{
    QList<QColor> colors=m_colors;
    // ֱ���������ͼ��
    ui->plot->clearGraphs();
    ui->plot->replot(); // �ػ�ͼ���Ը�����ͼ
   
   // ui->plot->xAxis->setRange(0, paiNum > 2 ? paiNum : 2, Qt::AlignBottom);
    m_title->setText(title);

    double minValue = 0;

    double maxValue = 2;

    bool ret = false;
    // ����mapData�е�ÿһ����ֵ�ԣ�Ϊÿһ��������һ������
    for (auto it = mapData.begin(); it != mapData.end(); ++it) 
    {
        int key = it.key();
        QVector<double> y = it.value();
        QVector<double> x(y.size());
        std::iota(x.begin(), x.end(), 1); // ��0��ʼΪÿ��yֵ����һ��xֵ

        // ��������
        QCPGraph* newGraph = ui->plot->addGraph();
        ui->plot->graph()->setData(x, y);
        QString strName = "[" + QString::number(key) + "]";
        ui->plot->graph()->setName(strName); // �������ߵ�����

  
      
        QPen pen;
        pen.setColor(QColor(colors.takeFirst())); 
        pen.setWidth(2); // ���������������Ϊ2����
        ui->plot->graph()->setPen(pen);
        // ��ɫ����ʽ�����ã����Ը�����Ҫ����
       // newGraph->setLineStyle(QCPGraph::lsLine); // ����
       // newGraph->setScatterStyle(QCPScatterStyle::ssCircle); // ����

        int min = *std::min_element(it.value().constBegin(), it.value().constEnd());
        int max= *std::max_element(it.value().constBegin(), it.value().constEnd());
        // ��ȡ��Сֵ
         minValue = min<minValue? min : minValue;

        // ��ȡ���ֵ
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