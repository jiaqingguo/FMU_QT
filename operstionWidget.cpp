#include <QFileDialog>
#include <QDebug>
#include <fstream>
#include <QFile>
#include <qmessagebox.h>

#include "QtGui/private/qzipreader_p.h"
#include "operstionWidget.h"
#include "ui_operstionWidget.h"
#include "addValueDialog.h"
#include "curveShowDialog.h"
#include "widget.h"
#include "qexcel.h"


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

    connect(ui->btn_saveExecl, &QPushButton::clicked, this, &operstionWidget::slot_btnSaveExecl);

    ui->btn_saveExecl->hide();
   
}

operstionWidget::~operstionWidget()
{
    delete ui;
  
    m_mapRelevance.remove(m_iAlgorithmNum);
    m_mapAlgorithmName.remove(m_iAlgorithmNum);
    m_mapOutputPort.remove(m_iAlgorithmNum);
    m_mapAllOutputData.erase(m_iAlgorithmNum);
}

std::vector<double> operstionWidget::get_tableWidgetInput()
{
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
    return vecDoule;
}

std::string operstionWidget::get_fmm_file_path()
{
    return m_fileInfo.absoluteFilePath().toStdString();
}

double operstionWidget::get_fmu_step_size()
{
   return m_stepSize; 
}

std::vector<fmi2ValueReference> operstionWidget::get_input_reference()
{
    return m_vecInputValueReference;
}

std::vector<fmi2ValueReference> operstionWidget::get_output_reference()
{
    return m_vecOutputValueReference;
}

void operstionWidget::get_input_data( std::vector<fmi2Real>& vecInputValue,  std::vector<fmi2ValueReference>& vecInputValueReference)
{
    for (int i = 0; i < ui->tableWidget_input->columnCount(); i++)
    {
        auto pItem = ui->tableWidget_input->item(1, i);
        if (pItem)
        {
            vecInputValue.push_back(pItem->text().toDouble());
        }
        else
        {
            vecInputValue.push_back(0.0);
        }
    }
    vecInputValueReference = m_vecInputValueReference;
}

void operstionWidget::update_tableWidget_out(const std::vector<double>& vecOutputValue)
{
    int count = ui->tableWidget_output->columnCount();
    if (count < vecOutputValue.size())
        return;

    m_iCalculateCount++;
    m_mapAllOutputData[m_iAlgorithmNum][m_iCalculateCount] = vecOutputValue;

    ui->comboBox_countShow->addItem(QString::number(m_iCalculateCount));
    int index = m_iCalculateCount - 1;
    ui->comboBox_countShow->setCurrentIndex(index);
}

std::string operstionWidget::UTF8_To_string(const std::string& str)
{
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
    memset(pwBuf, 0, nwLen * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);

    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr = pBuf;

    delete[]pBuf;
    delete[]pwBuf;

    pBuf = NULL;
    pwBuf = NULL;

    return retStr;
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
    // C++方法;
    //// 打开输入文件
    //std::ifstream inputFile(strFliePath.toStdString(), std::ios::binary);
    //if (!inputFile)
    //{
    //    qDebug() << "无法打开输入文件"<< strFliePath ;
    //    return 1;
    //}

    //// 打开输出文件
    //std::ofstream outputFile(strSavePath.toStdString(), std::ios::binary);
    //if (!outputFile)
    //{
    //    qDebug()  << "无法创建输出文件"<<strSavePath ;
    //    return 1;
    //}

    //// 逐字节复制文件内容
    //char ch;
    //while (inputFile.get(ch))
    //{
    //    outputFile.put(ch);
    //}

    //// 关闭文件
    //inputFile.close();
    //outputFile.close();

    //qDebug()<< "文件复制成功";
    return copyModifyFileFormat(strFliePath, strSavePath);
    return true;
}

bool operstionWidget::copyModifyFileFormat(const QString& strFliePath, const QString& strSavePath)
{
    QFile inputFile(strFliePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开输入文件" << strFliePath;
        return false;
    }

    // 检查目标文件是否存在，如果存在则先删除
     deleteFile(strSavePath);
    // 使用Qt的内建方法复制文件
    if (!QFile::copy(strFliePath,strSavePath)) {
        qDebug() << "复制文件失败: " << inputFile.errorString();
        return false;
    }

    inputFile.close();
    //outputFile.close(); // 实际上不必要，因为QFile的析构函数会自动关闭文件
    return true;
}

bool operstionWidget::decompressingFiles(const QString &strZipPath, const QString &strSavePath)
{
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

    QFileInfo fileInfo(strFilePath);
    if (fileInfo.exists())
    {
        QFile file(strFilePath);
        return  file.remove();
        
    }
   
    return false;
   
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

bool operstionWidget::is_file_exist(const QString fullFileName)
{
   
    QFileInfo fileInfo(fullFileName);
    if (fileInfo.isFile())
    {
        return true;
    }
    
    return false;
        
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

        QDomElement root= doc.documentElement(); //class结点

        // 获取步长节点;
        QDomNodeList nodeListStep = root.elementsByTagName("DefaultExperiment");//通过标签名获取结点列表
        if (nodeListStep.size() > 0)
        {
            QDomElement element = nodeListStep.at(0).toElement();    //通过索引获取QDomElement对象
            QString strStepSize = element.attribute("stepSize");                   //获取sex属性值
            m_stepSize = element.attribute("stepSize").toDouble();
        }
       
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
    
        // 界面显示;

        ui->tableWidget_input->clearContents();
        ui->tableWidget_input->setColumnCount(m_vecInputPort.size());


        ui->tableWidget_output->clearContents();
        ui->tableWidget_output->setColumnCount(m_mapOutputPort[m_iAlgorithmNum].size());

  
        std::thread thread_(&operstionWidget::load_tableWidget_show, this);

        thread_.detach();
        return 1;
}

// 创建xml配置文件;
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
    QDomAttr stepSize = doc.createAttribute(tr("stepSize"));

    //int iAlgorithmNum = 1;
    num.setValue(QString::number(m_iAlgorithmNum));
    filePath.setValue(m_fileInfo.filePath());
    stepSize.setValue(QString::number(m_stepSize));

    AlgorithmNum.setAttributeNode(filePath);
    AlgorithmNum.setAttributeNode(num);
    AlgorithmNum.setAttributeNode(stepSize);

    // 输入端口相关数据
    const auto& mapRelevance = m_mapRelevance[m_iAlgorithmNum];
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
           
            QString strValue = pItem->text();
            if (strValue.isEmpty())
            {
                value.setValue(QString::number(0.0));
            }
            else
            {
                value.setValue(strValue);
            }
          
        }
        else
        {
            value.setValue(QString::number(0.0));
        }
            
        elment_input.setAttributeNode(value);
        elment_input.setAttributeNode(valueReference);
        elment_input.setAttributeNode(name);
        if (mapRelevance.contains(i))
        {
            auto mapData = mapRelevance[i];
            QDomAttr algorithmNum = doc.createAttribute(tr("algorithmNum"));
            QDomAttr outputIndex = doc.createAttribute(tr("outputIndex"));
            algorithmNum.setValue(QString::number(mapData.firstKey()));
            outputIndex.setValue(QString::number(mapData[mapData.firstKey()]));
            elment_input.setAttributeNode(algorithmNum);
            elment_input.setAttributeNode(outputIndex);
        }

        AlgorithmNum.appendChild(elment_input);
        
    }
    if (mapRelevance.size() <= 0)
    {
        m_mapRelevance.remove(m_iAlgorithmNum);
    }
       
    
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
     
    
     root.appendChild(AlgorithmNum);

    return true;
}

void operstionWidget::load_algorithm_conguration(const QString filePath, const double stepSize)
{
    m_fileInfo = filePath;
    m_stepSize = stepSize;
    if (m_fileInfo.fileName().isEmpty())
    {
        return;
    }
    ui->label_algorithm->setText(m_fileInfo.fileName());
    m_mapAlgorithmName[m_iAlgorithmNum] = m_fileInfo.fileName();

    if (m_pFmu != nullptr)
    {
        delete m_pFmu;
        m_pFmu = nullptr;
    }

    std::string exe_config_paths = m_fileInfo.absoluteFilePath().toStdString();
    const std::string fmu_path = string_To_UTF8(exe_config_paths);

    m_pFmu = new  fmi2::fmu(fmu_path);
   
    auto cs_fmu = m_pFmu->as_cs_fmu();
    auto md = cs_fmu->get_model_description();
    m_pSlvae =(cs_fmu->new_instance());
    m_pSlvae->setup_experiment();
    m_pSlvae->enter_initialization_mode();
    m_pSlvae->exit_initialization_mode();
   

    

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


    ui->tableWidget_output->clearContents();
    ui->tableWidget_output->setColumnCount(m_mapOutputPort[m_iAlgorithmNum].size());

    std::thread thread_(&operstionWidget::load_tableWidget_show, this);
   
    thread_.detach();
}

void operstionWidget::load_relevance_conguration(const QMap<int, QMap<int, int>>& mapRelevance)
{
    m_mapRelevance.insert(m_iAlgorithmNum, mapRelevance);
}

int operstionWidget::get_algorithm_num()
{
    return m_iAlgorithmNum;
}

void operstionWidget::load_tableWidget_show()
{
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
   

    disconnect(ui->tableWidget_output, SIGNAL(cellChanged(int, int)), this, SLOT(slot_tableWigdetCheckedChanged(int, int)));
    for (int i = 0; i < ui->tableWidget_output->columnCount(); i++)
    {
        QTableWidgetItem* pItemPort = new QTableWidgetItem(m_mapOutputPort[m_iAlgorithmNum].at(i));
        pItemPort->setCheckState(Qt::Unchecked);
        ui->tableWidget_output->setItem(0, i, pItemPort);

        QTableWidgetItem* pItemValue = new QTableWidgetItem("");
        ui->tableWidget_output->setItem(1, i, pItemValue);

    }
    m_setOutputIndex.clear();
    connect(ui->tableWidget_output, SIGNAL(cellChanged(int, int)), this, SLOT(slot_tableWigdetCheckedChanged(int, int)));

   
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
   
  //  const std::string fmu_path = string_To_UTF8(exe_config_paths); 
    const std::string fmu_path =QTextCodec::codecForName("gb18030")->fromUnicode(m_fileInfo.absoluteFilePath()).data();

   /* fmi2::fmu fmu(fmu_path);
    auto cs_fmu = fmu.as_cs_fmu();
    auto md = cs_fmu->get_model_description();
    auto slave1 = cs_fmu->new_instance();
    m_pSlvae->setup_experiment();
    m_pSlvae->enter_initialization_mode();
    m_pSlvae->exit_initialization_mode();*/

   
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


    if (!m_pSlvae->write_real(m_vecInputValueReference, wef))
    {
        return;
    }

    if (!m_pSlvae->step(m_stepSize))
    {
        return;
    }

    if (!m_pSlvae->read_real(m_vecOutputValueReference, ref))
    {
        int a;
        return;
    }

    // 接受算法输出进行显示;
    m_mapAllOutputData[m_iAlgorithmNum][m_iCalculateCount] = ref;
    //qDebug() << "end----use_fmu_caculate()"
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
}

void operstionWidget::combine_relevacne_port_data(const std::vector<double> vecNewData)
{
    //auto vecNewData = m_mapAllOutputData[m_iAlgorithmNum][m_iCalculateCount];
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
}

bool operstionWidget::is_curve_show_dialog()
{
    //if (g_pWidget->get_curve_ptr())
    //{
    //    return g_pWidget->get_curve_ptr()->isVisible();
    //}
    if (g_pWidget->getGraphDialog())
    {
        return g_pWidget->getGraphDialog()->isVisible();
    }
    return false;
}

void operstionWidget::update_curve_show_dialog()
{
    if (g_pWidget->getGraphDialog() == nullptr)
        return;
   
    const auto& mapData = m_mapAllOutputData[m_iAlgorithmNum];

    QMap<int, QVector<double>> mapShowData;
    for (const auto& outIndex : m_setOutputIndex)
    {
        for (auto& vecPaidata : mapData)
        {
            mapShowData[outIndex].append(vecPaidata.second.at(outIndex));
        }
    }

    QString title = QString::fromLocal8Bit("算法") + QString::number(m_iAlgorithmNum);
    g_pWidget->getGraphDialog()->setDataShow(title,m_iCalculateCount, mapShowData);
    g_pWidget->getGraphDialog()->resize(1000, 650);
       
    g_pWidget->getGraphDialog()->show();
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

    if (m_pFmu != nullptr)
    {
        delete m_pFmu;
        m_pFmu = nullptr;
    }

    std::string exe_config_paths = m_fileInfo.absoluteFilePath().toStdString();
  //  const std::string fmu_path = string_To_UTF8(exe_config_paths);
    const std::string fmu_path = QTextCodec::codecForName("gb18030")->fromUnicode(m_fileInfo.absoluteFilePath()).data();

    m_pFmu = new  fmi2::fmu(fmu_path);

    auto cs_fmu = m_pFmu->as_cs_fmu();
    auto md = cs_fmu->get_model_description();
    m_pSlvae = (cs_fmu->new_instance());
    m_pSlvae->setup_experiment();
    m_pSlvae->enter_initialization_mode();
    m_pSlvae->exit_initialization_mode();
}

void operstionWidget::slot_tableWigdetCheckedChanged(int row, int col)
{
    if (row != 0)
        return;
    auto pItem = ui->tableWidget_output->item(row, col);
    if (pItem == nullptr)
        return;
    if (pItem->checkState() == Qt::Checked)
    {
        m_setOutputIndex.insert(col);
    }
    else
    {
        m_setOutputIndex.remove(col);
    }
    if (g_pWidget->getGraphDialog()->isVisible())
    {
        qDebug() << "slot_tableWigdetCheckedChanged" << row << col;

        const auto& mapData = m_mapAllOutputData[m_iAlgorithmNum];

        QMap<int, QVector<double>> mapShowData;
        for (const auto& outIndex : m_setOutputIndex)
        {
            for (auto& vecPaidata : mapData)
            {
                mapShowData[outIndex].append(vecPaidata.second.at(outIndex));
            }
        }
        QString title = QString::fromLocal8Bit("算法") + QString::number(m_iAlgorithmNum);
        g_pWidget->getGraphDialog()->setDataShow(title,m_iCalculateCount, mapShowData);
    }
   
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
       if (value.isEmpty())
       {
           pItem->setText(QString::number(0.0));
       }
       else
       {
           pItem->setText(value);
       }
      
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
    if (!(is_file_exist(m_fileInfo.absoluteFilePath())))
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("fmu文件不存在,无法计算!"));
        return;
    }
    m_iCalculateCount++;

    std::thread thread_(&operstionWidget::use_fmu_caculate, this);
    thread_.join();
   

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
    mapShowData.clear();
    for(const auto &outIndex: m_setOutputIndex)
    {
        for(auto &vecPaidata : mapData)
        {
            mapShowData[outIndex].append(vecPaidata.second.at(outIndex));
        }
    }
    QString title = QString::fromLocal8Bit("算法") + QString::number(m_iAlgorithmNum);
    g_pWidget->getGraphDialog()->setDataShow(title,m_iCalculateCount,mapShowData);
    g_pWidget->getGraphDialog()->resize(1000, 650);
   
    g_pWidget->getGraphDialog()->show();

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

    // 解除关联端口;
    auto pItem = ui->tableWidget_input->item(row, column);
    if (pItem)
    {
        //pItem->
    }

    if (m_mapRelevance.contains(m_iAlgorithmNum))
    {
        m_mapRelevance[m_iAlgorithmNum].remove(column);
    }
}

void operstionWidget::slot_btnSaveExecl()
{
    QSet<int> saveIndex;
    saveIndex.clear();
    bool bSave = true;
    QVector<QString> outPortName;
    outPortName.clear();
    for (int i = 0; i < ui->tableWidget_output->columnCount(); i++)
    {
        auto pItem = ui->tableWidget_output->item(0, i);
        if (pItem)
        {
            if (pItem->checkState() == Qt::Checked)
            {
                saveIndex.insert(i);
                outPortName.append(pItem->text());

                bSave = false;
            }
          
        }

    }
    if (bSave)
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先勾选要导出的输出位!"));
        return;
    }

    QString str_execl_path = QFileDialog::getSaveFileName(this,
        tr("Save Config"),
        "",
        tr("Config Files (*.xls)"));

    if (str_execl_path.isNull())
    {
        //点的是取消
        return;
    }
    if (is_file_exist(str_execl_path))
    {
        if (!deleteFile(str_execl_path))
        {
            QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("无法操作，文件可能已经打开!"));

            return;
        }
    }
    
    str_execl_path=str_execl_path.replace("/", "\\");
    if (!g_pWidget->get_execl_ptr()->createFile(str_execl_path))//创建一个Excel文件
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("无法操作，文件可能已经打开!"));

        return;
    }
    // 插入端口名称;
    int iRow = 1;
    int cloumn = 1;
    for (int i = 0; i < outPortName.size(); i++)
    {
        g_pWidget->get_execl_ptr()->setCellString(iRow, cloumn, outPortName.at(i));
        g_pWidget->get_execl_ptr()->setCellBackground(iRow, cloumn, Qt::yellow);
        cloumn++;
    }
    iRow++;
  


    const auto& mapData = m_mapAllOutputData[m_iAlgorithmNum];


    for (const auto& paiData : mapData)
    {

        int i = 1;
        for (auto portIndex : saveIndex)
        {
            g_pWidget->get_execl_ptr()->setCellString(iRow, i,QString::number(paiData.second.at(portIndex)));
            i++;
        }
        iRow++;
    }


    g_pWidget->get_execl_ptr()->save(); //保存文件
}
