#include <QMenu>

#include "widget.h"
#include "ui_widget.h"
#include "operstionWidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

     setWindowTitle("FMU算法验证工具");
     setContextMenuPolicy(Qt::CustomContextMenu);

    // 初始化界面;
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    m_iAlgorithmNum++;
    QString strName = "算法"+QString::number(m_iAlgorithmNum);
    operstionWidget *pWidget = new operstionWidget(m_iAlgorithmNum);
    ui->tabWidget->addTab(pWidget,strName);

    connect(this,&Widget::customContextMenuRequested,this,&Widget::slot_widgetCustomContextMenuRequested);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_widgetCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    QAction *add = menu.addAction(tr("添加算法"));
    //menu.addSeparator();
    QAction *del = menu.addAction(tr("删除算法"));


    connect(add, &QAction::triggered, [=]()
    {
        m_iAlgorithmNum++;
        QString strName = "算法"+QString::number(m_iAlgorithmNum);
        operstionWidget *pWidget = new operstionWidget(m_iAlgorithmNum);
        int index =ui->tabWidget->addTab(pWidget,strName);
        ui->tabWidget->setCurrentIndex(index);
    });

    connect(del, &QAction::triggered, [=]()
    {
       QWidget *pWidget = ui->tabWidget->currentWidget();
       ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
       delete  pWidget;
    });



    menu.exec(QCursor::pos());
}

