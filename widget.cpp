#include <QMenu>
#include <QDomDocument>
#include <QTextStream>
#include <QFileDialog>
#include <qdebug.h>

#include <qmessagebox.h>

#include "widget.h"
#include "ui_widget.h"
#include "operstionWidget.h"
#include "fum_thread.h"
#include "thread_pool.h"

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
    /*int tabIndex=*/ ui->tabWidget->addTab(pWidget,strName);
   /* myUseData *pData=new myUseData;
    pData->iAlgorithmNum = m_iAlgorithmNum;*/
   // ui->tabWidget->setUserData(tabIndex, pData);
    connect(this,&Widget::customContextMenuRequested,this,&Widget::slot_widgetCustomContextMenuRequested);
    connect(ui->btn_calculate_control, &QPushButton::clicked, this, &Widget::slot_btn_calculate_control);

    connect(m_pThread_pool->instance(), &thread_pool::signal_fmu_thread_finished, this, &Widget::slot_fmu_thread_finished);

    g_pWidget = this;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_thread_finished()
{
    
}

void Widget::slot_fmu_thread_finished(int tab, const std::vector<double> vecOutputValue)
{
    update_algorithm_tableWidget_out(tab, vecOutputValue);
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

        // 更新关联端口
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

void Widget::update_relevance_tableWidget_input(const int& tab, const QVector<double>& outputValue)
{
    QWidget* pWidget = ui->tabWidget->widget(tab);
    operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
    if (pOperstionWidget)
    {
      // pOperstionWidget->combine_relevacne_port_data(outputValue);
    }

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
   
   // QString str_xml_path = QCoreApplication::applicationDirPath() + "/" + "configuration.xml";
    //QString strNameTemp = Win0rLinuxFilePath(strName);
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
   
    //添加根元素
    doc.appendChild(root);


    ////添加第一个book元素及其子元素
    //QDomElement AlgorithmNum = doc.createElement(tr("AlgorithmNum"));
    //QDomAttr num = doc.createAttribute(tr("num"));
    //QDomAttr filePath = doc.createAttribute(tr("filePath"));

    //int iAlgorithmNum = 1;
    //num.setValue(QString::number(iAlgorithmNum));
    //filePath.setValue(m_fileInfo.filePath());

    //AlgorithmNum.setAttributeNode(filePath);
    //AlgorithmNum.setAttributeNode(num);


    //// 输入端口相关数据
    //if (m_vecInputPort.size() > 0)
    //{
    //    for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    //    {
    //        QDomElement elment_input = doc.createElement(tr("input"));

    //        // 属性;
    //        QDomAttr name = doc.createAttribute(tr("name"));
    //        QDomAttr valueReference = doc.createAttribute(tr("valueReference"));
    //        QDomAttr value = doc.createAttribute(tr("value"));

    //        name.setValue(m_vecInputPort.at(i));
    //        valueReference.setValue(QString::number(m_vecInputValueReference.at(i)));
    //        auto pItem = ui->tableWidget_input->item(1, i);
    //        if (pItem)
    //        {
    //            value.setValue(pItem->text());
    //        }
    //        else
    //        {
    //            value.setValue(QString::number(0.0));
    //        }

    //        elment_input.setAttributeNode(value);
    //        elment_input.setAttributeNode(valueReference);
    //        elment_input.setAttributeNode(name);


    //        AlgorithmNum.appendChild(elment_input);
    //    }
    //}
    //else
    //{
    //    //QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("!"));
    //    qDebug() << iAlgorithmNum << " 输入端口数量为0，此算法不保存配置！";
    //    return false;
    //}

    //// 输出端口相关数据;
    //const auto& vecOutputData = m_mapOutputPort[m_iAlgorithmNum];
    //if (vecOutputData.size() > 0)
    //{
    //    for (int i = 0; i < vecOutputData.size(); i++)
    //    {
    //        QDomElement elment_output = doc.createElement(tr("output"));

    //        // 属性;
    //        QDomAttr name = doc.createAttribute(tr("name"));
    //        QDomAttr valueReference = doc.createAttribute(tr("valueReference"));


    //        name.setValue(vecOutputData.at(i));
    //        valueReference.setValue(QString::number(m_vecOutputValueReference.at(i)));

    //        elment_output.setAttributeNode(valueReference);
    //        elment_output.setAttributeNode(name);


    //        AlgorithmNum.appendChild(elment_output);
    //    }
    //}
    //else
    //{
    //    qDebug() << iAlgorithmNum << " 输出端口数量为0，此算法不保存配置！";
    //    return false;
    //}
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

   // QFile file(QCoreApplication::applicationDirPath() + "/" + "configuration.xml");
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
            m_iAlgorithmNum = e_AlgorithmNum.attribute("num").toInt();
            QString strName = QString::fromLocal8Bit("算法") + QString::number(m_iAlgorithmNum);
            operstionWidget* pWidget = new operstionWidget(m_iAlgorithmNum);
            connect(pWidget, &operstionWidget::signal_update_port_data, this, &Widget::slot_update_prot_data);

            ui->tabWidget->addTab(pWidget, strName);

            pWidget->load_algorithm_conguration(algorithmNum_path);

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
}

void Widget::slot_recv_calculate_control(int flag, int calculate_count/* = 0*/)
{

    //for (int i = 0; i < m_iThreadCount; i++)
    //{
    //    int count = ui->tabWidget->count();
    //    for (int i = 0; i < count; i++)
    //    {
    //        QWidget* pWidget = ui->tabWidget->widget(i);
    //        operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
    //        pOperstionWidget->slot_btnCalculate();

    //    }

    if (flag == 1)
    {
        int thread_count = 0;
        for (int iCycle = 0; iCycle < calculate_count; iCycle++)
        {
            int count = ui->tabWidget->count();
            for (int i = 0; i < count; i++)
            {
                QWidget* pWidget = ui->tabWidget->widget(i);
                operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
               
                fum_thread* pThread = new fum_thread;
                pThread->set_fmu_file(pOperstionWidget->get_fmm_file_path());
                
                pThread->set_cur_tab(i);
                pThread->set_thread_number(thread_count);
                //pThread->set_input_value(pOperstionWidget->get_tableWidgetInput());
                pThread->set_input_reference(pOperstionWidget->get_input_reference());
                pThread->set_output_reference(pOperstionWidget->get_output_reference());
                
              
                m_pThread_pool->instance()->add_thread(pThread, thread_count);

                thread_count++;

            }
        }
        
        QWidget* pWidget = ui->tabWidget->widget(0);
        operstionWidget* pOperstionWidget = dynamic_cast<operstionWidget*>(pWidget);
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

