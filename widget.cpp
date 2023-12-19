#include <QMenu>
#include <QDomDocument>
#include <QTextStream>
#include <qdebug.h>

#include <qmessagebox.h>

#include "widget.h"
#include "ui_widget.h"
#include "operstionWidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

     setWindowTitle(QString::fromLocal8Bit("FMU算法验证工具"));
     setContextMenuPolicy(Qt::CustomContextMenu);

    // 初始化界面;
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    m_iAlgorithmNum++;
    QString strName = QString::fromLocal8Bit("算法")+QString::number(m_iAlgorithmNum);
    operstionWidget *pWidget = new operstionWidget(m_iAlgorithmNum);
    ui->tabWidget->addTab(pWidget,strName);

    connect(this,&Widget::customContextMenuRequested,this,&Widget::slot_widgetCustomContextMenuRequested);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::create_xml_configuration()
{
   
    QString str_xml_path = QCoreApplication::applicationDirPath() + "/" + "configuration.xml";
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

    QFile file(QCoreApplication::applicationDirPath() + "/" + "configuration.xml");
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
            ui->tabWidget->addTab(pWidget, strName);

            pWidget->load_algorithm_conguration(algorithmNum_path);

            std::vector<unsigned int> vecInputValueReference;

            std::vector<fmi2ValueReference> vecOutputValueReference;
            QVector<QString> vecInputPort;   // 输入端口;
            QVector<QString> vecOutputPort;   // 输出端口;
            QVector<double> vecInputValue;  // 输入值;

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
                    }
                    else if (strType == "output")
                    {
                        vecOutputPort.append(strName);
                        vecOutputValueReference.push_back(strValueReference.toInt());
                    }
                }
            }

            pWidget->load_data_conguration(vecInputPort, vecInputValueReference, vecInputValue, vecOutputPort, vecOutputValueReference);



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
            initialize_configuration();
           
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

