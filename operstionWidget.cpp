#include <QFileDialog>

#include <QDebug>
#include <fstream>
#include <QDomDocument>


#include "operstionWidget.h"
#include "ui_operstionWidget.h"
#include "addValueDialog.h"
#include "curveShowDialog.h"

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
    //ui->tableWidget_input->setHorizontalHeaderLabels(QStringList() << "" << "");
       //设置表格数据区内的所有单元格都不允许编辑
       //TableWidget.setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_output->setRowCount(2);
    ui->tableWidget_output->setVerticalHeaderLabels(QStringList() << QStringLiteral("端口 ") << QStringLiteral("数值 "));
    ui->tableWidget_output->horizontalHeader()->setHidden(true);


    connect(ui->btn_choose,&QPushButton::clicked,this,&operstionWidget::slot_btnChooseFile);
    connect(ui->btn_add_value,&QPushButton::clicked,this,&operstionWidget::slot_addInputValeu);
    connect(ui->tableWidget_input,&QTableWidget::customContextMenuRequested,this,&operstionWidget::slot_tableWidgetCustomContextMenuRequested);

    connect(ui->tableWidget_output, SIGNAL(cellChanged(int ,int )), this, SLOT(slot_tableWigdetCheckedChanged(int , int )));
    connect(ui->btn_calculate,&QPushButton::clicked,this,&operstionWidget::slot_btnCalculate);
    connect(ui->btn_clear,&QPushButton::clicked,this,&operstionWidget::slot_btnClear);
    connect(ui->btn_curve_show,&QPushButton::clicked,this,&operstionWidget::slot_btnCurveShow);
    connect(ui->comboBox_countShow,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comboxPaiNumChanged(int)));
}

operstionWidget::~operstionWidget()
{
    delete ui;
    qDebug()<<"~operstionWidget()";
    m_mapAlgorithmName.remove(m_iAlgorithmNum);
    m_mapOutputPort.remove(m_iAlgorithmNum);
    m_mapAllOutputData.erase(m_iAlgorithmNum);
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
            return-1;
        }

        //2、创建QDomDocument对象，用于解析XML
        QDomDocument doc;
        //通过QFile对象设置QDomDocument的内容
        if(!doc.setContent(&file))
        {
            file.close();
            return -1;
        }
        //关闭文件
        file.close();

        //3、解析XML
        //获取XML根结点

        QDomElement root= doc.documentElement();//class结点
        //遍历每个student结点
        QDomNodeList studentList=root.elementsByTagName("ScalarVariable");//通过标签名获取结点列表

        int listSize =studentList.size();
        for(int i = 0; i < listSize; ++i)
        {
            QDomElement student = studentList.at(i).toElement();    //通过索引获取QDomElement对象
            QString strName=student.attribute("name");                   //获取sex属性值
            QString strCausality =student.attribute("causality");                  //获取age属性值
            if(strCausality=="input")
            {
                m_vecInputPort.append(strName);
            }
            else if(strCausality=="output")
            {
                m_mapOutputPort[m_iAlgorithmNum].append(strName);
            }

        }

        // 界面显示;
        ui->tableWidget_input->setColumnCount(m_vecInputPort.size());

        for(int i=0;i<ui->tableWidget_input->columnCount();i++)
        {
            QTableWidgetItem *pItemPort = new QTableWidgetItem(m_vecInputPort[i]);
            ui->tableWidget_input->setItem(0,i,pItemPort);

            QTableWidgetItem *pItemValue = new QTableWidgetItem("");
            ui->tableWidget_input->setItem(1,i,pItemValue);

        }

        ui->tableWidget_output->setColumnCount(m_mapOutputPort[m_iAlgorithmNum].size());

        for(int i=0;i<ui->tableWidget_output->columnCount();i++)
        {
            QTableWidgetItem *pItemPort = new QTableWidgetItem(m_mapOutputPort[m_iAlgorithmNum].at(i));
            pItemPort->setCheckState(Qt::Checked);
            ui->tableWidget_output->setItem(0,i,pItemPort);

            QTableWidgetItem *pItemValue = new QTableWidgetItem("");
            ui->tableWidget_output->setItem(1,i,pItemValue);

        }

        m_setOutputIndex.clear();
        for(int i=0;i<ui->tableWidget_output->columnCount();i++)
        {
            auto pItem = ui->tableWidget_output->item(0,i);
            if(pItem)
            {
                if(pItem->checkState() == Qt::Checked)
                {
                    m_setOutputIndex.insert(i);
                }
                else
                {
                    m_setOutputIndex.remove(i);
                }
            }

        }
        return 1;
}

void operstionWidget::slot_btnChooseFile()
{

    QString file_full, file_name, current_Path, file_path, file_suffix, complete_suffix, file_baseName, file_completeBaseName ;
    QFileInfo fileinfo; // <<------
    m_fileInfo =QFileDialog::getOpenFileName(this, "Open File", "QCoreApplication::applicationFilePath()",
                                            "AllFile (*.*);;");
    ui->label_algorithm->setText(m_fileInfo.fileName());
    if(m_fileInfo.fileName().isEmpty())
        return;
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
    qDebug()<<"slot_tableWigdetCheckedChanged"<<row<<col;
    auto pItem = ui->tableWidget_output->item(row,col);
    if(pItem==nullptr)
        return;
    if(pItem->checkState() == Qt::Checked)
    {
        m_setOutputIndex.insert(col);
    }
    else
    {
        m_setOutputIndex.remove(col);
    }
}

void operstionWidget::slot_addInputValeu()
{

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
       QString strValue = dialog.getInputValue();
       pItem->setText(strValue);
    }
}

void operstionWidget::slot_btnCalculate()
{
    m_iCalculateCount++;
    ui->comboBox_countShow->addItem(QString::number(m_iCalculateCount));
    int index =m_iCalculateCount-1;
    ui->comboBox_countShow->setCurrentIndex(index);
    //传入输出；
    std::vector<double> vecDoule;
    for(int i=0;i<ui->tableWidget_input->columnCount();i++)
    {
      auto pItem = ui->tableWidget_input->item(1,i);
      if(pItem)
      {
          vecDoule.push_back(pItem->text().toDouble());
      }
      else
      {
          vecDoule.push_back(0.0);
      }
    }

    // 传给算法;

    // 接受算法输出进行显示;
}

void operstionWidget::slot_btnClear()
{
    m_iCalculateCount=0;
    ui->comboBox_countShow->clear();
    m_mapAllOutputData.erase(m_iAlgorithmNum);
}

void operstionWidget::slot_btnCurveShow()
{
    if(m_setOutputIndex.isEmpty())
    {
        qDebug()<< "未勾选显示输出位";
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
    dialog.exec();


}

void operstionWidget::slot_comboxPaiNumChanged()
{
    int iPaiNum = ui->comboBox_countShow->currentText().toInt();
    auto vecData = m_mapAllOutputData[m_iAlgorithmNum][iPaiNum];
    if(vecData.size() < m_mapOutputPort[m_iAlgorithmNum].size())
    {
        qDebug()<<"!!!!!!!输出结果位数可能有问题";
    }
    for(int i=0;i<ui->tableWidget_output->columnCount();i++)
    {
        auto pItem = ui->tableWidget_output->item(0,i);
        if(pItem)
        {
          pItem->setText(QString::number(vecData.at(i)));
        }

    }
}
