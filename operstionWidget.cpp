#include <QFileDialog>
#include <QDebug>
#include <fstream>

#include <qmessagebox.h>
#include "QtGui/private/qzipreader_p.h"


#include "operstionWidget.h"
#include "ui_operstionWidget.h"
#include "addValueDialog.h"
#include "curveShowDialog.h"

using namespace fmi4cpp;
const double stepSize = 1;


QMap<int, QMap<int, QMap<int, int>>> operstionWidget::m_mapRelevance = { };
QMap<int,QString> operstionWidget::m_mapAlgorithmName;// 算法名称；
QMap<int,QVector<QString>> operstionWidget::m_mapOutputPort={}; //输出端口;
std::map<int,std::map<int,std::vector<double>>> operstionWidget::m_mapAllOutputData={};



operstionWidget::operstionWidget(const int num,QWidget *parent) :m_iAlgorithmNum(num),
    QWidget(parent),
    ui(new Ui::operstionWidget)
{
    ui->setupUi(this);

    ui->tableWidget_input->setRowCount(2);
    ui->tableWidget_input->setVerticalHeaderLabels(QStringList() << QStringLiteral("端口 ") << QStringLiteral("数值 "));
    ui->tableWidget_input->horizontalHeader()->setHidden(true);
    ui->tableWidget_input->setContextMenuPolicy(Qt::CustomContextMenu);
   // ui->tableWidget_input->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动设置列宽
   // ui->tableWidget_input->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableWidget_input->setHorizontalHeaderLabels(QStringList() << "" << "");
       //设置表格数据区内的所有单元格都不允许编辑
       //TableWidget.setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_output->setRowCount(2);
    ui->tableWidget_output->setVerticalHeaderLabels(QStringList() << QStringLiteral("端口 ") << QStringLiteral("数值 "));
    ui->tableWidget_output->horizontalHeader()->setHidden(true);   // 隐藏表头
    ui->tableWidget_output->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动设置列宽
    ui->tableWidget_output->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置不可编辑;

    connect(ui->btn_choose,&QPushButton::clicked,this,&operstionWidget::slot_btnChooseFile);
    connect(ui->btn_clear_input,&QPushButton::clicked,this,&operstionWidget::slot_btn_clear_input);
    connect(ui->tableWidget_input,&QTableWidget::customContextMenuRequested,this,&operstionWidget::slot_tableWidgetCustomContextMenuRequested);
    connect(ui->tableWidget_input, &QTableWidget::cellDoubleClicked, this, &operstionWidget::slot_tableWidgetCellEntered);
    

   // connect(ui->tableWidget_output, SIGNAL(cellChanged(int ,int )), this, SLOT(slot_tableWigdetCheckedChanged(int , int )));
    connect(ui->btn_calculate,&QPushButton::clicked,this,&operstionWidget::slot_btnCalculate);
    connect(ui->btn_clear,&QPushButton::clicked,this,&operstionWidget::slot_btnClear);
    connect(ui->btn_curve_show,&QPushButton::clicked,this,&operstionWidget::slot_btnCurveShow);
    connect(ui->comboBox_countShow,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comboxPaiNumChanged(int)));
}

operstionWidget::~operstionWidget()
{
    delete ui;
    if (m_thread.joinable())
        m_thread.join();
    qDebug()<<"~operstionWidget()";
    m_mapRelevance.remove(m_iAlgorithmNum);
    m_mapAlgorithmName.remove(m_iAlgorithmNum);
    m_mapOutputPort.remove(m_iAlgorithmNum);
    m_mapAllOutputData.erase(m_iAlgorithmNum);
}

std::string operstionWidget::string_To_UTF8(const std::string& str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr(pBuf);

    delete[]pwBuf;
    delete[]pBuf;

    pwBuf = NULL;
    pBuf = NULL;

    return retStr;
}

bool operstionWidget::modifyFileFormat(const QString &strFliePath, const QString &strSavePath)
{
    // 打开输入文件
    std::ifstream inputFile(strFliePath.toStdString(), std::ios::binary);
    if (!inputFile)
    {
        qDebug() << "无法打开输入文件"<< strFliePath ;
        return 1;
    }

    // 打开输出文件
    std::ofstream outputFile(strSavePath.toStdString(), std::ios::binary);
    if (!outputFile)
    {
        qDebug()  << "无法创建输出文件"<<strSavePath ;
        return 1;
    }

    // 逐字节复制文件内容
    char ch;
    while (inputFile.get(ch))
    {
        outputFile.put(ch);
    }

    // 关闭文件
    inputFile.close();
    outputFile.close();

    qDebug()<< "文件复制成功";
    return true;
}

bool operstionWidget::decompressingFiles(const QString &strZipPath, const QString &strSavePath)
{
    //QZipReader zipReader(strZipPath);

    createFolder(strSavePath);
    QZipReader zipreader(strZipPath);
    //reader.extractAll(path); // //可加可不加没有什么影响


    for(auto fileInfo : zipreader.fileInfoList())
    {
       if(fileInfo.isFile)
       {
           QFile file(strSavePath + "/" + fileInfo.filePath);
           file.open(QIODevice::WriteOnly);
           //注意编码问题
           QByteArray dt = fileInfo.filePath.toUtf8();
           QString strtemp = QString::fromLocal8Bit(dt);


           QByteArray array = zipreader.fileData(strtemp);
           file.write(array);
           file.close();
       }
       else if(fileInfo.isDir)
       {
           createFolder(strSavePath + "/" + fileInfo.filePath);
       }
    }

    zipreader.close();
    return true;
}

bool operstionWidget::createFolder(const QString &folderPath)
{
    bool state = false;
        //创建文件夹
    QDir dir(folderPath);
    if (!dir.exists())
    {
        return dir.mkdir(folderPath);
    }
    else
    {
        state = true;
    }
    return state;
}

bool operstionWidget::deleteFile(const QString strFilePath)
{
    QFile file(strFilePath);

    return  file.remove();
}

bool operstionWidget::deleteDir(const QString strDirPath)
{
    if (strDirPath.isEmpty())
    {
              return false;
        }
    QDir dir(strDirPath);
    if(!dir.exists())
    {
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList)
    { //遍历文件信息
        if (file.isFile())
        { // 是文件，删除
            bool isDelete = file.dir().remove(file.fileName());
            if(!isDelete)
            {
                 qDebug() << "删除失败= " <<isDelete << " filename= " << file.fileName();
            }
        }
        else
        { // 递归删除
            deleteDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹

}

bool operstionWidget::readXML(const QString strXmlPath)
{
    //1、创建QFile对象，指定要打开的XML文件

        QFile file(strXmlPath);

        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"xml File open faild!";
            return false;
        }

        //2、创建QDomDocument对象，用于解析XML
        QDomDocument doc;
        //通过QFile对象设置QDomDocument的内容
        if(!doc.setContent(&file))
        {
            file.close();
            return false;
        }
        //关闭文件
        file.close();

        //3、解析XML
        //获取XML根结点

        QDomElement root= doc.documentElement();//class结点
        //遍历每个student结点
        QDomNodeList nodeList=root.elementsByTagName("ScalarVariable");//通过标签名获取结点列表

        m_vecInputValueReference.clear();
        m_vecOutputValueReference.clear();
        m_vecInputPort.clear();
        if (m_mapOutputPort.contains(m_iAlgorithmNum))
        {
            m_mapOutputPort[m_iAlgorithmNum].clear();
        }

        int listSize = nodeList.size();
        for(int i = 0; i < listSize; ++i)
        {
            QDomElement element = nodeList.at(i).toElement();    //通过索引获取QDomElement对象
            QString strName= element.attribute("name");                   //获取sex属性值

            QString strValueReference = element.attribute("valueReference");
            QString strCausality = element.attribute("causality");                  //获取age属性值
            if(strCausality=="input")
            {
                m_vecInputPort.append(strName);
                m_vecInputValueReference.push_back(strValueReference.toInt());
            }
            else if(strCausality=="output")
            {
                m_mapOutputPort[m_iAlgorithmNum].append(strName);
                m_vecOutputValueReference.push_back(strValueReference.toInt());
            }

        }
        // QTableWidgetItem *pItemValue = new QTableWidgetItem("");
        // 界面显示;

        //if (!m_thread.joinable())
           // m_thread = std::thread(&operstionWidget::load_tableWidget_show, this);
       
       ui->tableWidget_input->clearContents();
        ui->tableWidget_input->setColumnCount(m_vecInputPort.size());
       /* QTableWidgetItem* pItem;
        for(int i=0;i<ui->tableWidget_input->columnCount();i++)
        {
            pItem = new QTableWidgetItem(m_vecInputPort[i]);
            ui->tableWidget_input->setItem(0,i, pItem);

          
            pItem = new QTableWidgetItem("");
            ui->tableWidget_input->setItem(1,i, pItem);
        }*/

        ui->tableWidget_output->clearContents();
        ui->tableWidget_output->setColumnCount(m_mapOutputPort[m_iAlgorithmNum].size());

      /*  for(int i=0;i<ui->tableWidget_output->columnCount();i++)
        {
            QTableWidgetItem *pItemPort = new QTableWidgetItem(m_mapOutputPort[m_iAlgorithmNum].at(i));
            pItemPort->setCheckState(Qt::Unchecked);
            ui->tableWidget_output->setItem(0,i,pItemPort);

            QTableWidgetItem *pItemValue = new QTableWidgetItem("");
            ui->tableWidget_output->setItem(1,i,pItemValue);

        }*/
        std::thread thread_(&operstionWidget::load_tableWidget_show, this);
        thread_.join();
       
        return 1;
}

bool operstionWidget::create_xml_configuration(QFile& file, QDomDocument& doc, QDomElement& root)
{

    if (m_vecInputPort.size() <= 0 || (!m_mapOutputPort.contains(m_iAlgorithmNum)))
    {
       qDebug() << m_iAlgorithmNum << "此算法不保存配置！";
        return false;
    }
       
    const auto& vecOutputData = m_mapOutputPort[m_iAlgorithmNum];
    if (vecOutputData.size() <= 0)
    {
        qDebug() << m_iAlgorithmNum << "此算法不保存配置！";
        return false;
    }
       

    //添加第一个book元素及其子元素
    QDomElement AlgorithmNum = doc.createElement(tr("Algorithm"));
    QDomAttr num = doc.createAttribute(tr("num"));
    QDomAttr filePath = doc.createAttribute(tr("filePath"));

    //int iAlgorithmNum = 1;
    num.setValue(QString::number(m_iAlgorithmNum));
    filePath.setValue(m_fileInfo.filePath());

    AlgorithmNum.setAttributeNode(filePath);
    AlgorithmNum.setAttributeNode(num);
   

    // 输入端口相关数据
 
    for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    {
        QDomElement elment_input = doc.createElement(tr("input"));

        // 属性;
        QDomAttr name = doc.createAttribute(tr("name"));
        QDomAttr valueReference = doc.createAttribute(tr("valueReference")); 
        QDomAttr value = doc.createAttribute(tr("value")); 

        name.setValue(m_vecInputPort.at(i));
        valueReference.setValue(QString::number(m_vecInputValueReference.at(i)));
        auto pItem = ui->tableWidget_input->item(1, i);
        if (pItem)
        {
            value.setValue(pItem->text());
        }
        else
        {
            value.setValue(QString::number(0.0));
        }
            
        elment_input.setAttributeNode(value);
        elment_input.setAttributeNode(valueReference);
        elment_input.setAttributeNode(name);
            

        AlgorithmNum.appendChild(elment_input);
        
}
    //else
    //{
    //    //QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("!"));
    //    qDebug() << m_iAlgorithmNum << " 输入端口数量为0，此算法不保存配置！";
    //    return false;
    //}
    
    // 输出端口相关数据;
  //  const auto & vecOutputData = m_mapOutputPort[m_iAlgorithmNum];
   
    for (int i = 0; i < vecOutputData.size(); i++)
    {
        QDomElement elment_output = doc.createElement(tr("output"));

        // 属性;
        QDomAttr name = doc.createAttribute(tr("name"));
        QDomAttr valueReference = doc.createAttribute(tr("valueReference"));


        name.setValue(vecOutputData.at(i));
        valueReference.setValue(QString::number(m_vecOutputValueReference.at(i)));

        elment_output.setAttributeNode(valueReference);
        elment_output.setAttributeNode(name);
             

        AlgorithmNum.appendChild(elment_output);
    }
     
     //else
     //{
     //    qDebug() << m_iAlgorithmNum << " 输出端口数量为0，此算法不保存配置！";
     //    return false;
     //}
    
     root.appendChild(AlgorithmNum);



  /*  QDomElement title = doc.createElement(tr("书名"));
    QDomElement author = doc.createElement(tr("作者"));
    QDomText text;
   
    AlgorithmNum.setAttributeNode(num);
    AlgorithmNum.setAttributeNode(filePath);
    text = doc.createTextNode("Qt");
    title.appendChild(text);
    text = doc.createTextNode("shiming");
    author.appendChild(text);*/
    //AlgorithmNum.appendChild(title);
    //AlgorithmNum.appendChild(author);
   

    //添加第二个book元素及其子元素
    //AlgorithmNum = doc.createElement(tr("图书"));
    //num = doc.createAttribute(tr("编号"));
    //title = doc.createElement(tr("书名"));
    //author = doc.createElement(tr("作者"));
    //num.setValue("2");
    //AlgorithmNum.setAttributeNode(num);
    //text = doc.createTextNode("Linux");
    //title.appendChild(text);
    //text = doc.createTextNode("LiMing");
    //author.appendChild(text);
    //AlgorithmNum.appendChild(title);
    //AlgorithmNum.appendChild(author);
   // root.appendChild(AlgorithmNum);

    //QTextStream out(&file);
    //doc.save(out, 4);
    //file.close();

    return true;
}

void operstionWidget::load_algorithm_conguration(const QString filePath)
{
    m_fileInfo = filePath;
    if (m_fileInfo.fileName().isEmpty())
    {
        return;
    }
    ui->label_algorithm->setText(m_fileInfo.fileName());
    m_mapAlgorithmName[m_iAlgorithmNum] = m_fileInfo.fileName();
   
}

void operstionWidget::load_data_conguration(const QVector<QString>& vecInputPort, std::vector<fmi2ValueReference>& vecInputValueReference, const QVector<double>& vecInputValue, const QVector<QString>& vecOutputPort, std::vector<fmi2ValueReference>& vecOutputValueReference)
{
    m_vecInputPort = vecInputPort;
    m_vecInputValueReference = vecInputValueReference;
    m_mapOutputPort[m_iAlgorithmNum] = vecOutputPort;
    m_vecOutputValueReference = vecOutputValueReference;

    m_vecInputValue = vecInputValue;

    // 界面显示;
    ui->tableWidget_input->clearContents();
    ui->tableWidget_input->setColumnCount(m_vecInputPort.size());

    //for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    //{
    //    QTableWidgetItem* pItemPort = new QTableWidgetItem(m_vecInputPort[i]);
    //    ui->tableWidget_input->setItem(0, i, pItemPort);

    //    QTableWidgetItem* pItemValue = new QTableWidgetItem(QString::number(vecInputValue.at(i)));
    //    ui->tableWidget_input->setItem(1, i, pItemValue);

    //}

    ui->tableWidget_output->clearContents();
    ui->tableWidget_output->setColumnCount(m_mapOutputPort[m_iAlgorithmNum].size());

    //for (int i = 0; i < ui->tableWidget_output->columnCount(); i++)
    //{
    //    QTableWidgetItem* pItemPort = new QTableWidgetItem(m_mapOutputPort[m_iAlgorithmNum].at(i));
    //    pItemPort->setCheckState(Qt::Unchecked);
    //    ui->tableWidget_output->setItem(0, i, pItemPort);

    //    QTableWidgetItem* pItemValue = new QTableWidgetItem("");
    //    ui->tableWidget_output->setItem(1, i, pItemValue);

    //}

    std::thread thread_(&operstionWidget::load_tableWidget_show, this);
    thread_.join();

}

int operstionWidget::get_algorithm_num()
{
    return m_iAlgorithmNum;
}

//void operstionWidget::set_port_relevance(int srcOutIndex, int srcAlgorithNum, int dstInputIndex)
//{
//    /*auto &multMap = m_mapMultPort[dstInputIndex];
//    multMap.remove(srcAlgorithNum, srcOutIndex);
//    multMap.insert(srcAlgorithNum, dstInputIndex);*/
//   
//}

void operstionWidget::load_tableWidget_show()
{

   // ui->tableWidget_input->clearContents();
   // ui->tableWidget_input->setColumnCount(m_vecInputPort.size());
   
    int count = ui->tableWidget_input->columnCount();
    if (m_vecInputValue.size() == m_vecInputPort.size())
    {
      
        for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
        {
            QTableWidgetItem* pItemPort = new QTableWidgetItem(m_vecInputPort[i]);
            ui->tableWidget_input->setItem(0, i, pItemPort);

            QTableWidgetItem* pItemValue = new QTableWidgetItem(QString::number(m_vecInputValue.at(i)));
            ui->tableWidget_input->setItem(1, i, pItemValue);

        }
    }
    else
    {
        QTableWidgetItem* pItem;
        for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
        {
            pItem = new QTableWidgetItem(m_vecInputPort[i]);
            ui->tableWidget_input->setItem(0, i, pItem);


            pItem = new QTableWidgetItem("");
            ui->tableWidget_input->setItem(1, i, pItem);
        }
    }
   

   // ui->tableWidget_output->clearContents();
    //ui->tableWidget_output->setColumnCount(m_mapOutputPort[m_iAlgorithmNum].size());

    for (int i = 0; i < ui->tableWidget_output->columnCount(); i++)
    {
        QTableWidgetItem* pItemPort = new QTableWidgetItem(m_mapOutputPort[m_iAlgorithmNum].at(i));
        pItemPort->setCheckState(Qt::Unchecked);
        ui->tableWidget_output->setItem(0, i, pItemPort);

        QTableWidgetItem* pItemValue = new QTableWidgetItem("");
        ui->tableWidget_output->setItem(1, i, pItemValue);

    }
}

void operstionWidget::use_fmu_caculate()
{
    //传入输出；
    std::vector<double> vecDoule;
    for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    {
        auto pItem = ui->tableWidget_input->item(1, i);
        if (pItem)
        {
            vecDoule.push_back(pItem->text().toDouble());
        }
        else
        {
            vecDoule.push_back(0.0);
        }
    }

    // 传给算法;
    //std::string exe_config_paths = "D:\\CS\\atspaceMicroTimer.fmu";//m_fileInfo.filePath().toStdString();
    std::string exe_config_paths = m_fileInfo.absoluteFilePath().toStdString();
    const std::string fmu_path = string_To_UTF8(exe_config_paths);
    fmi2::fmu fmu(fmu_path);
    auto cs_fmu = fmu.as_cs_fmu();
    auto md = cs_fmu->get_model_description();
    auto slave1 = cs_fmu->new_instance();
    slave1->setup_experiment();
    slave1->enter_initialization_mode();
    slave1->exit_initialization_mode();


    std::vector<fmi2Real> wef(m_vecInputValueReference.size());
    std::vector<fmi2Real> ref(m_vecOutputValueReference.size());


    for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    {
        auto pItem = ui->tableWidget_input->item(1, i);
        if (pItem)
        {
            wef[i] = pItem->text().toDouble();
        }
        else
        {
            wef[i] = 0.0;
        }
    }


    if (!slave1->write_real(m_vecInputValueReference, wef))
    {
        return;
    }

    if (!slave1->step(stepSize))
    {
        return;
    }

    if (!slave1->read_real(m_vecOutputValueReference, ref))
    {
        return;
    }

    // 接受算法输出进行显示;
    m_mapAllOutputData[m_iAlgorithmNum][m_iCalculateCount] = ref;

 /*   ui->comboBox_countShow->addItem(QString::number(m_iCalculateCount));
    int index = m_iCalculateCount - 1;
    ui->comboBox_countShow->setCurrentIndex(index);*/
}

void operstionWidget::get_relevacne_port_data()
{
    auto vecNewData = m_mapAllOutputData[m_iAlgorithmNum][m_iCalculateCount];
    // <算法，端口，要更新的值>
    QMap<int, QMap<int, double>> mapNewData;
   // QMap<int, QMap<int, QMap<int, int>>>
    for (auto groupIt = m_mapRelevance.begin(); groupIt != m_mapRelevance.end(); ++groupIt)
    {
        int recvAlgorithNum = groupIt.key();

        // 内层循环遍历第二层键值对
        for (auto input_itor = groupIt.value().begin(); input_itor != groupIt.value().end(); ++input_itor)
        {
            int recvInput = input_itor.key();
            int i = 0;
            for (auto dstNumIt = input_itor.value().begin(); dstNumIt != input_itor.value().end(); ++dstNumIt)
            {
                int sendAlgorithmNum = dstNumIt.key();
                if (sendAlgorithmNum == m_iAlgorithmNum)
                {

                    // 获取端口的值;
                    auto sendPortIndex = dstNumIt.value();
                    double value = vecNewData[sendPortIndex];
                    mapNewData[recvAlgorithNum][recvInput] = value;
                    break;
                }
                i++;
            }
            
        }
    }
    if (mapNewData.size() > 0)
    {
         emit signal_update_port_data(mapNewData);
    }
   
   /* for (const auto& dstAlgorithmDataData : m_mapRelevance)
    {
        for (auto& srcInputData: dstAlgorithmDataData)
        {
            int iInputIndx = srcInputData.first();
            for (auto srcAlgorithmData : srcInputData)
            {
                if (srcAlgorithmData。 == m_iAlgorithmNum)
                {

                }
            }
        }
       */
    //}


}

void operstionWidget::update_prot_data(QMap<int, double> portData)
{
    for (auto groupIt = portData.begin(); groupIt != portData.end(); ++groupIt)
    {
        auto pItem = ui->tableWidget_input->item(1, groupIt.key());
        if (pItem)
        {
            pItem->setText(QString::number(groupIt.value()));
        }
    }

   /* for (int i = 0; i < portData.size(); i++)
    {
        auto pItem = ui->tableWidget_input->item(1, portData.key());
        if (pItem)
        {
            pItem->setText(QString::number(portData[i]));
        }
    }*/
}



void operstionWidget::slot_btnChooseFile()
{
    
    QString file_full, file_name, current_Path, file_path, file_suffix, complete_suffix, file_baseName, file_completeBaseName ;
    QFileInfo fileinfo; 
    m_fileInfo =QFileDialog::getOpenFileName(this, "Open File", "QCoreApplication::applicationFilePath()",
                                            "AllFile (*.fmu);;");
    
    if (m_fileInfo.fileName().isEmpty())
    {
        return;
    }
    m_mapRelevance.remove(m_iAlgorithmNum);
 

    ui->label_algorithm->setText(m_fileInfo.fileName());
    m_mapAlgorithmName[m_iAlgorithmNum]=m_fileInfo.fileName();
    qDebug()<< m_fileInfo.fileName()<<m_fileInfo.filePath();


    QString strZipPath =m_fileInfo.filePath().split(".").first() +".zip";
    qDebug()<<"strZipPath:"<<strZipPath;
    // 另存为zip；
    modifyFileFormat(m_fileInfo.filePath(),strZipPath);
    // 解压zip
    QString strDecompressingDir = strZipPath.split(".").first();
    decompressingFiles(strZipPath,strDecompressingDir);
    // 读取xml
    QString strXmlPath = strDecompressingDir+"/modelDescription.xml";
    readXML(strXmlPath);
    // 删除解压目录;
    deleteDir(strDecompressingDir);

    // 删除zip;
    deleteFile(strZipPath);
}

void operstionWidget::slot_tableWigdetCheckedChanged(int row, int col)
{
    //qDebug()<<"slot_tableWigdetCheckedChanged"<<row<<col;
    //auto pItem = ui->tableWidget_output->item(row,col);
    //if(pItem==nullptr)
    //    return;
    //if(pItem->checkState() == Qt::Checked)
    //{
    //    m_setOutputIndex.insert(col);
    //}
    //else
    //{
    //    m_setOutputIndex.remove(col);
    //}
}

void operstionWidget::slot_btn_clear_input()
{
    for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    {
        auto pItem = ui->tableWidget_input->item(1, i);
        if (pItem)
        {
            pItem->setText("");
        }

    }
}

void operstionWidget::slot_tableWidgetCustomContextMenuRequested(const QPoint &pos)
{
    auto pItem =ui->tableWidget_input->itemAt(pos);
    if(pItem==nullptr)
        return;

    addValueDialog dialog;
    dialog.setAlgorithmName(m_mapAlgorithmName);
    dialog.setAllOutputData(m_mapAllOutputData);
    dialog.setOutputPortName(m_mapOutputPort);
    dialog.init();
    if(dialog.exec() ==QDialog::Accepted)
    {
       //QString strValue = dialog.getInputValue();

        QString value;
       int sendAlgorithmNum; 
       int sendOutputIndex;
       dialog.getRelevanceData(sendAlgorithmNum, sendOutputIndex, value);
       pItem->setText(value);
       if (sendAlgorithmNum != m_iAlgorithmNum)
       {
           int col = pItem->column();
           // 建立端口关联关系;
           //emit signal_port_relevance(srcAlgorithmNum, srcOutputIndex, m_iAlgorithmNum, pItem->column());
           auto& data = m_mapRelevance[m_iAlgorithmNum];
           data.remove(col);
           data[col][sendAlgorithmNum] = sendOutputIndex;
       }
       else
       {
           // 自己的端口是否关联;
       }

       

    }
}

void operstionWidget::slot_btnCalculate()
{
    if (m_mapAlgorithmName[m_iAlgorithmNum].isEmpty())
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选择算法!"));
        return;
    }

    if (m_vecInputValueReference.size() <= 0 || m_vecOutputValueReference.size() <= 0)
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("输入输出端口不正常,无法计算!"));
        return;
    }

    m_iCalculateCount++;

    std::thread thread_(&operstionWidget::use_fmu_caculate, this);
    thread_.join();
   
    ////传入输出；
    //std::vector<double> vecDoule;
    //for(int i=0;i<ui->tableWidget_input->columnCount();i++)
    //{
    //  auto pItem = ui->tableWidget_input->item(1,i);
    //  if(pItem)
    //  {
    //      vecDoule.push_back(pItem->text().toDouble());
    //  }
    //  else
    //  {
    //      vecDoule.push_back(0.0);
    //  }
    //}

    //// 传给算法;
    ////std::string exe_config_paths = "D:\\CS\\atspaceMicroTimer.fmu";//m_fileInfo.filePath().toStdString();
    //std::string exe_config_paths = m_fileInfo.absoluteFilePath().toStdString();
    //const std::string fmu_path = string_To_UTF8(exe_config_paths);
    //fmi2::fmu fmu(fmu_path);
    //auto cs_fmu = fmu.as_cs_fmu();
    //auto md = cs_fmu->get_model_description();
    //auto slave1 = cs_fmu->new_instance();
    //slave1->setup_experiment();
    //slave1->enter_initialization_mode();
    //slave1->exit_initialization_mode();


    //std::vector<fmi2Real> wef(m_vecInputValueReference.size());
    //std::vector<fmi2Real> ref(m_vecOutputValueReference.size());
   

    //for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    //{
    //    auto pItem = ui->tableWidget_input->item(1, i);
    //    if (pItem)
    //    {
    //        wef[i]=pItem->text().toDouble();
    //    }
    //    else
    //    {
    //        wef[i] = 0.0;
    //    }
    //}


    //if (!slave1->write_real(m_vecInputValueReference, wef))
    //{
    //    return;
    //}
   
    //if (!slave1->step(stepSize))
    //{
    //    return;
    //}
    //
    //if (!slave1->read_real(m_vecOutputValueReference, ref))
    //{
    //    return;
    //}
   
    //// 接受算法输出进行显示;
    //m_mapAllOutputData[m_iAlgorithmNum][m_iCalculateCount] = ref;

    ui->comboBox_countShow->addItem(QString::number(m_iCalculateCount));
    int index = m_iCalculateCount - 1;
    ui->comboBox_countShow->setCurrentIndex(index);
   

    get_relevacne_port_data();
}

void operstionWidget::slot_btnClear()
{
    m_iCalculateCount=0;
    ui->comboBox_countShow->clear();
    m_mapAllOutputData.erase(m_iAlgorithmNum);
    for (int i = 0; i < ui->tableWidget_output->columnCount(); i++)
    {
        auto pItem = ui->tableWidget_output->item(1, i);
        if (pItem)
        {
            pItem->setText("");
        }

    }
}

void operstionWidget::slot_btnCurveShow()
{
    m_setOutputIndex.clear();
    for (int i = 0; i < ui->tableWidget_output->columnCount(); i++)
    {
        auto pItem = ui->tableWidget_output->item(0, i);
        if (pItem)
        {
            if (pItem->checkState() == Qt::Checked)
            {
                m_setOutputIndex.insert(i);
            }
            else
            {
                m_setOutputIndex.remove(i);
            }
        }

    }
    if(m_setOutputIndex.isEmpty())
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先勾选要显示的输出位!"));
        return;
    }
    const auto &mapData=m_mapAllOutputData[m_iAlgorithmNum];

    QMap<int,QVector<double>> mapShowData;
    for(const auto &outIndex: m_setOutputIndex)
    {
        for(auto &vecPaidata : mapData)
        {
            mapShowData[outIndex].append(vecPaidata.second.at(outIndex));
        }
    }
    curveShowDialog dialog;
    dialog.setCurveSHowData(m_iCalculateCount,mapShowData);
    dialog.resize(1000, 900);
    dialog.exec();

}

void operstionWidget::slot_comboxPaiNumChanged(int index)
{
    int iPaiNum = ui->comboBox_countShow->currentText().toInt();
    auto vecData = m_mapAllOutputData[m_iAlgorithmNum][iPaiNum];
    if (vecData.size() <= 0)
        return;
    if(vecData.size() < m_mapOutputPort[m_iAlgorithmNum].size())
    {
        qDebug()<<"!!!!!!!输出结果位数可能有问题";
    }
    for(int i=0;i<ui->tableWidget_output->columnCount();i++)
    {
        auto pItem = ui->tableWidget_output->item(1,i);
        if(pItem)
        {
          pItem->setText(QString::number(vecData.at(i)));
        }

    }
}

void operstionWidget::slot_tableWidgetCellEntered(int row, int column)
{
    qDebug() << "slot_tableWidgetCellEntered " << row << column;
}
