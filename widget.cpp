#include <QMenu>
#include <QDomDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QTabBar>

#include <qdebug.h>

#include <qmessagebox.h>

#include "widget.h"
#include "ui_widget.h"
#include "operstionWidget.h"
#include "fum_thread.h"
#include "thread_pool.h"

#include "qexcel.h"
#include "UdpClientManager.h"



Widget* g_pWidget = NULL;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

     setWindowTitle(QString::fromLocal8Bit("FMU算法验证工具"));
     setContextMenuPolicy(Qt::CustomContextMenu);
     m_calculate_control_dialog = new calculate_control_dialog(this);
     connect(m_calculate_control_dialog, &calculate_control_dialog::signal_calculate_control, this, &Widget::slot_recv_calculate_control);
    // 初始化界面;
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    m_iAlgorithmNum++;
    QString strName = QString::fromLocal8Bit("算法")+QString::number(m_iAlgorithmNum);
    operstionWidget *pWidget = new operstionWidget(m_iAlgorithmNum);
    connect(pWidget, &operstionWidget::signal_update_port_data, this, &Widget::slot_update_prot_data);
    ui->tabWidget->addTab(pWidget,strName);
 
    connect(this,&Widget::customContextMenuRequested,this,&Widget::slot_widgetCustomContextMenuRequested);
    connect(ui->btn_calculate_control, &QPushButton::clicked, this, &Widget::slot_btn_calculate_control);
    connect(ui->btn_clearAll, &QPushButton::clicked, this, &Widget::slot_btnClearAll);

    connect(m_pThread_pool->instance(), &thread_pool::signal_fmu_thread_finished, this, &Widget::slot_fmu_thread_finished);

    connect(ui->tabWidget->tabBar(), &QTabBar::currentChanged, this, &Widget::slot_tab_changed);

    m_curve_show_dialog = new curveShowDialog();
    g_pWidget = this;
    setMinimumHeight(650);

    m_qexecl = new QExcel();

    m_graphDialog = new graphDialog();

   // m_pUdpClientManager= new UdpClientManager();
    init_udp();
    //m_graphDialog->setupPlot();
   // m_graphDialog->show();
       // 创建QProcess对象

}

Widget::~Widget()
{
    delete ui;
    m_qexecl->quit(); //退出Excel
}


void Widget::slot_fmu_thread_finished(int tab, const std::vector<double> vecOutputValue)
{
    update_algorithm_tableWidget_out(tab, vecOutputValue);
}

void Widget::slot_tab_changed(int index)
{
    if (m_graphDialog->isVisible())
    {
        QWidget* pWidget = ui->tabWidget->widget(index);
        operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
        if (pOperstionWidget)
        {
            pOperstionWidget->update_curve_show_dialog();
        }
    }

}

void Widget::slot_udp_recv_data(const QByteArray& udpData)
{
    QString str = QString::fromUtf8(udpData);
    qDebug() << " udp recv str : " << str;
    // QDataStream stream(message);  // 使用 QByteArray 创建 QDataStream
    QDataStream stream(udpData);
    stream.setByteOrder(QDataStream::LittleEndian); // 设置为小端序
    stream.setVersion(QDataStream::Qt_5_0);  // 设置 QDataStream 版本

    //QDataStream stream(&datagram, QIODevice::ReadOnly);
  

    qint32 command = 0;
    qint32 count = 0;
    qint32 interval = 0;

    // 按顺序读取三个字段
    stream >> command >> count >> interval;

    qDebug() << "\n========== 解析结果 ==========";

    qDebug() << "命令类型:" << command;
    qDebug() << "计算次数:" << count;
    qDebug() << "周期间隔:" << interval << "ms";
    m_calculate_control_dialog->setShow(command, count, interval);
    m_calculate_control_dialog->show();
   // slot_recv_calculate_control(command,count, interval);
}

void Widget::update_algorithm_tableWidget_out(const int& tab, const std::vector<double>& vecOutputValue)
{
    QWidget* pWidget = ui->tabWidget->widget(tab);
    operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
    if (pOperstionWidget)
    {
        pOperstionWidget->update_tableWidget_out(vecOutputValue);
    }
}

void Widget::update_algorithhnum_opersition(const int& tab, const std::vector<double>& vecOutputValue)
{
    QWidget* pWidget = ui->tabWidget->widget(tab);
    operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
    if (pOperstionWidget)
    {
        // 更新输出显示;
        pOperstionWidget->update_tableWidget_out(vecOutputValue);

        // 更新拍数 数据传递

        // 更新关联端口;
        pOperstionWidget->combine_relevacne_port_data(vecOutputValue);
    }
}

std::vector<double> Widget::get_algorithm_tableWidget_input(const int& tab)
{
    QWidget* pWidget = ui->tabWidget->widget(tab);
    operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
    if (pOperstionWidget)
    {
        return pOperstionWidget->get_tableWidgetInput();
    }
    return std::vector<double>();
}

void Widget::calculate_control(int count)
{
    for (int i = 0; i < 2; i++)
    {
        int count = ui->tabWidget->count();
        for (int i = 0; i < count; i++)
        {
            QWidget* pWidget = ui->tabWidget->widget(i);
            operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
            pOperstionWidget->slot_btnCalculate();
        }
        qDebug() << "calculate_control()" << i;
    }
}

void Widget::reset_control_btns()
{
    m_calculate_control_dialog->reset_btns();
}

//curveShowDialog* Widget::get_curve_ptr()
//{
//    return m_curve_show_dialog;
//}

graphDialog* Widget::getGraphDialog()
{
    return m_graphDialog;
}

QExcel* Widget::get_execl_ptr()
{
    return m_qexecl;
}

void Widget::init_udp()
{
    m_pUdpClientManager = new UdpClientManager;
    m_pUdpClientManager->startListen(36520);

    //QHostAddress local_ip = QHostAddress("192.168.0.129");
    //int  local_port = 56660;  // 本地端口号
    //m_pUdpClientManager->sendData("发送测试", local_ip, local_port);
    QObject::connect(m_pUdpClientManager, &UdpClientManager::msgRecevied,this,&Widget::slot_udp_recv_data);
    //QObject::connect(m_pUdpClientManager, &UdpClientManager::msgRecevied,
    //    [this](const QByteArray& message)
    //    {
    //        QString str = QString::fromUtf8(message);
    //        qDebug() << " udp recv str : " << str;
    //        // QDataStream stream(message);  // 使用 QByteArray 创建 QDataStream
    //        QDataStream stream(message);
    //        stream.setByteOrder(QDataStream::LittleEndian); // 设置为小端序
    //        stream.setVersion(QDataStream::Qt_5_0);  // 设置 QDataStream 版本

    //        stPlcData data;
    //        stream >> data.Q1;
    //        stream >> data.Q2;
    //        stream >> data.M;
    //        for (int i = 0; i < 7; i++) {
    //            stream >> data.P[i];
    //        }



    //        QByteArray byteArray;
    //        QDataStream dataStream(&byteArray, QIODevice::WriteOnly);

    //        // 将自定义结构体的属性写入QDataStream
    //        dataStream << data.Q1 << data.Q2 << data.M;
    //        for (int i = 0; i < 7; i++) {
    //            stream << data.P[i];
    //        }

    //        //QHostAddress local_ip = QHostAddress::LocalHost;
    //        //    int  local_port = 56660;  // 本地端口号

    //        //    m_pUdpClientManager->sendData(message, local_ip, local_port);

    //    });
}

void Widget::closeEvent(QCloseEvent* event)
{
    if (m_curve_show_dialog)
    {
        delete m_curve_show_dialog;
    }
    QWidget::closeEvent(event);
}


void Widget::create_xml_configuration()
{
    QString str_xml_path = QFileDialog::getSaveFileName(this,
        tr("Save Config"),
        "",
        tr("Config Files (*.xml)"));

    if (str_xml_path.isNull())
    {
        //点的是取消
        return ;
    }
   
    QFile file(str_xml_path);

    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if (!ret)
    {
        qDebug() << "打开文件失败";
        return ;
    }

    //创建文档
    QDomDocument doc;

    //添加处理指令
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml",
        "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement(tr("configuration"));


    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        QWidget* pWidget = ui->tabWidget->widget(i);
        operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
        if (pOperstionWidget)
        {
            pOperstionWidget->create_xml_configuration(file, doc, root);
        }
    }
   
    //添加根节点;
    doc.appendChild(root);

    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

}

bool Widget::load_xml_configuration()
{
    QString file_full, file_name, current_Path, file_path, file_suffix, complete_suffix, file_baseName, file_completeBaseName;
    QFileInfo fileinfo;
    QString strFilePath = QFileDialog::getOpenFileName(this, "Open File", "QCoreApplication::applicationFilePath()",
        "AllFile (*.xml);;");

    if (strFilePath.isEmpty())
    {
        return false;
    }
    // 先清除配置;
    initialize_configuration();

    QFile file(strFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    m_iAlgorithmNum = 0;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    //返回根节点及其子节点的元素标记名
    //返回根元素
    QDomElement rootElem = doc.documentElement();
    //返回根节点的第一个子节点
    QDomNode node_AlgorithmNum = rootElem.firstChild();

    
    QMap<int, QMap<int, int>> mapRelevance;
    while (!node_AlgorithmNum.isNull())
    {
        //如果子节点是元素
        if (node_AlgorithmNum.isElement())
        {
            //将其转换为元素
            QDomElement e_AlgorithmNum = node_AlgorithmNum.toElement();
           // ui->listWidget->addItem(e.tagName() + e.attribute(tr("编号")));

            QString algorithmNum_path = e_AlgorithmNum.attribute("filePath");
            double algorithmStepSize = e_AlgorithmNum.attribute("stepSize").toDouble();
            m_iAlgorithmNum = e_AlgorithmNum.attribute("num").toInt();
            QString strName = QString::fromLocal8Bit("算法") + QString::number(m_iAlgorithmNum);
            operstionWidget* pWidget = new operstionWidget(m_iAlgorithmNum);
            connect(pWidget, &operstionWidget::signal_update_port_data, this, &Widget::slot_update_prot_data);

            ui->tabWidget->addTab(pWidget, strName);

            pWidget->load_algorithm_conguration(algorithmNum_path, algorithmStepSize);

            std::vector<unsigned int> vecInputValueReference;

            std::vector<fmi2ValueReference> vecOutputValueReference;
            QVector<QString> vecInputPort;   // 输入端口;
            QVector<QString> vecOutputPort;   // 输出端口;
            QVector<double> vecInputValue;  // 输入值;

            int inputPortIndex = 0;

            QDomNodeList list = e_AlgorithmNum.childNodes();
            for (int i = 0; i < list.count(); i++)
            {
                QDomNode node = list.at(i);
                if (node.isElement())
                {
                    QDomElement element_port = node.toElement();
                 
                    QString strType = element_port.tagName();
                    QString strName = element_port.attribute("name");                   //获取sex属性值
                    QString strValueReference = element_port.attribute("valueReference");
                    
                    if (strType == "input")
                    {
                        double  value = element_port.attribute("value").toDouble();
                        vecInputPort.append(strName);
                        vecInputValueReference.push_back(strValueReference.toInt());
                        vecInputValue.append(value);

                        // 关联算法端口读取处理;
                        if (element_port.hasAttribute("algorithmNum"))
                        {
                            int sendAlgorithNum = element_port.attribute("algorithmNum").toInt();
                            int sendOutputPortIndex = element_port.attribute("outputIndex").toInt();
                            mapRelevance[inputPortIndex].insert(sendAlgorithNum, sendOutputPortIndex);
                        }
                        inputPortIndex++;
                    }
                    else if (strType == "output")
                    {
                        vecOutputPort.append(strName);
                        vecOutputValueReference.push_back(strValueReference.toInt());
                    }
                }
            }

            pWidget->load_data_conguration(vecInputPort, vecInputValueReference, vecInputValue, vecOutputPort, vecOutputValueReference);
            if (mapRelevance.size() > 0)
            {
                pWidget->load_relevance_conguration(mapRelevance);
                mapRelevance.clear();
            }
        }

        node_AlgorithmNum = node_AlgorithmNum.nextSibling();
    }

    return true;
}

void Widget::initialize_configuration()
{
    int count = ui->tabWidget->count();
    for (int i = 0; i < count; i++)
    {
        QWidget* pWidget = ui->tabWidget->widget(0);
        ui->tabWidget->removeTab(0);
        delete  pWidget;
    }
    m_iAlgorithmNum = 0;
}


void Widget::slot_update_prot_data(QMap<int, QMap<int, double>> mapNewData)
{
    for (auto groupIt = mapNewData.begin(); groupIt != mapNewData.end(); ++groupIt) // 遍历算法数据;
    {
        int srcAlgorithNum = groupIt.key();

        int count = ui->tabWidget->count();
        for (int i = 0; i < count; i++)  // 遍历算法界面;
        {
            QWidget* pWidget = ui->tabWidget->widget(i);
            operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
        
            if (srcAlgorithNum == 4)
            {
                int a = 0;
            }
            if (pOperstionWidget->get_algorithm_num() == srcAlgorithNum)
            {
                auto newData = mapNewData[srcAlgorithNum];


                pOperstionWidget->update_prot_data(newData);
                break;
            }
        }


    }

}

void Widget::slot_btn_calculate_control()
{
    m_calculate_control_dialog->exec();

    //if (m_pThread_pool->instance()->get_thread_size() > 0)
    //{
    //    QMessageBox msgBox;
    //    msgBox.setText(QString::fromLocal8Bit("提示"));
    //    msgBox.setInformativeText(QString::fromLocal8Bit("确认退出,终止计算?"));
    //    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    //    msgBox.setDefaultButton(QMessageBox::Ok);
    //    // 将原本显示“Yes”的按钮改为显示“是”
    //    msgBox.button(QMessageBox::Ok)->setText(QString::fromLocal8Bit("确定"));
    //    // 将原本显示“No”的按钮改为显示“否”
    //    msgBox.button(QMessageBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
    //   
    //    int ret = msgBox.exec();
    //    if (ret == QMessageBox::Ok) 
    //    {
    //        //若用户点击确认，结束线程池;
    //        m_calculate_control_dialog->slot_btn_stop();
    //    }
    //    else 
    //    {
    //        //若用户点击取消，
    //        m_calculate_control_dialog->exec();
    //    }
    //}  
}

void Widget::slot_btnClearAll()
{
    int  count = ui->tabWidget->count();
    for (int i = 0; i < count; i++)
    {
        QWidget* pWidget = ui->tabWidget->widget(i);
        operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);

       
        pOperstionWidget->slot_btnClear();

    }
}

void Widget::slot_recv_calculate_control(int flag, int calculate_count, int cycleTime/* = 0*/)
{
    if (flag == 1)
    {
        int count = 0;
       
       
            count = ui->tabWidget->count();
            for (int i = 0; i < count; i++)
            {
                QWidget* pWidget = ui->tabWidget->widget(i);
                operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
                if (pOperstionWidget->is_available())
                {
                    fum_thread* pThread = new fum_thread;
                    pThread->set_fmu_file(pOperstionWidget->get_fmm_file_path(), pOperstionWidget->get_fmu_step_size(), calculate_count);

                    pThread->set_cur_tab(i);
                    pThread->set_thread_number(i);
                    //pThread->set_input_value(pOperstionWidget->get_tableWidgetInput());
                    pThread->set_input_reference(pOperstionWidget->get_input_reference());
                    pThread->set_output_reference(pOperstionWidget->get_output_reference());

                    m_pThread_pool->instance()->add_thread(pThread, i);
                }
               
              
            }
        
        
        QWidget* pWidget = ui->tabWidget->widget(0);
        operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);

        m_pThread_pool->instance()->setCylclyData(count, cycleTime);
        m_pThread_pool->instance()->start_thread(0, pOperstionWidget->get_tableWidgetInput());
      
    }
    else
    {
        m_pThread_pool->instance()->run_contral(flag);
    }
}

void Widget::slot_widgetCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    QAction *add = menu.addAction(QString::fromLocal8Bit("添加算法"));
  
    QAction *del = menu.addAction(QString::fromLocal8Bit("删除算法"));
    menu.addSeparator();
    QAction *save_configuration = menu.addAction(QString::fromLocal8Bit("保存配置"));
    QAction* load_configuration = menu.addAction(QString::fromLocal8Bit("加载配置"));
    menu.addSeparator();
    QAction* initialize = menu.addAction(QString::fromLocal8Bit("初始化配置"));


    connect(add, &QAction::triggered, [=]()
    {
        m_iAlgorithmNum++;
        QString strName = QString::fromLocal8Bit("算法")+QString::number(m_iAlgorithmNum);
        operstionWidget *pWidget = new operstionWidget(m_iAlgorithmNum);
        connect(pWidget, &operstionWidget::signal_update_port_data, this, &Widget::slot_update_prot_data);
        int index =ui->tabWidget->addTab(pWidget,strName);
        ui->tabWidget->setCurrentIndex(index);
    });

    connect(del, &QAction::triggered, [=]()
    {
       QWidget *pWidget = ui->tabWidget->currentWidget();
       ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
       delete  pWidget;
    });


    connect(save_configuration, &QAction::triggered, [=]()
    {
            create_xml_configuration();
    });
    connect(load_configuration, &QAction::triggered, [=]()
    {
           
            load_xml_configuration();

    });
    connect(initialize, &QAction::triggered, [=]()
    {
            initialize_configuration();
            m_iAlgorithmNum++;
            QString strName = QString::fromLocal8Bit("算法") + QString::number(m_iAlgorithmNum);
            operstionWidget* pWidget = new operstionWidget(m_iAlgorithmNum);
            ui->tabWidget->addTab(pWidget, strName);
    });

    menu.exec(QCursor::pos());
}

