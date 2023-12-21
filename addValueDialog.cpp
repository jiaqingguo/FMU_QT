#include "addValueDialog.h"
#include "ui_addValueDialog.h"

#include <QtDebug>

addValueDialog::addValueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addValueDialog)
{
    ui->setupUi(this);

    connect(ui->comboBox_portName,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_portNameChanged(int)));
    connect(ui->comboBox_algorithmName,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_algorithmNameChanged(int)));
    connect(ui->comboBox_count,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_paiNumChanged(int)));
    connect(ui->btn_ok, &QPushButton::clicked, this, &addValueDialog::slot_btnOK);
    connect(ui->btn_cancle, &QPushButton::clicked, this, &addValueDialog::slot_btnCancle);
}

addValueDialog::~addValueDialog()
{
    delete ui;
}

void addValueDialog::init()
{

    for(  auto itor = m_mapAlgorithmName.begin();itor!=m_mapAlgorithmName.end();itor++)
    {
        ui->comboBox_algorithmName->addItem(itor.value(),itor.key());
    }
    int iAlgorithmNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();

    // 初始化端口；
    if (m_mapOutputPort.find(iAlgorithmNum) == m_mapOutputPort.end())
    {
        ui->comboBox_portName->clear();
    }
    else
    {
        auto& vecPort = m_mapOutputPort[iAlgorithmNum];
        ui->comboBox_portName->clear();
        for (auto& protData : vecPort)
        {
            ui->comboBox_portName->addItem(protData);
        }
    }

    // 初始化拍数；
    if (m_mapAllOutputData.find(iAlgorithmNum) == m_mapAllOutputData.end())
    {
        ui->comboBox_count->clear();
        ui->label_value->setText("");
    }
    else
    {
        auto& vecOutputData = m_mapAllOutputData.at(iAlgorithmNum);
        ui->comboBox_count->clear();
        for (auto& paiNum : vecOutputData)
        {
            ui->comboBox_count->addItem(QString::number(paiNum.first));
        }

        // 初始化显示数值;
        int paiNum = ui->comboBox_count->currentText().toInt();
        if (vecOutputData.find(paiNum) == vecOutputData.end())
        {
            ui->label_value->setText("");
        }
        else
        {
            double value = vecOutputData[paiNum].at(ui->comboBox_portName->currentIndex());
            ui->label_value->setText(QString::number(value));
        }
    }
  
   
}

void addValueDialog::setAlgorithmName(const QMap<int, QString> &mapAlgorithmName)
{
    m_mapAlgorithmName = mapAlgorithmName;
}

void addValueDialog::setOutputPortName(const QMap<int, QVector<QString> > &mapOutputPort)
{
    m_mapOutputPort=mapOutputPort;
}

void addValueDialog::setAllOutputData(const std::map<int, std::map<int, std::vector<double> > > &mapAllOutputData)
{
    m_mapAllOutputData= mapAllOutputData;
}

QString addValueDialog::getInputValue()
{
    return ui->label_value->text();
}

void addValueDialog::getRelevanceData(int& algorithNum, int& outputIndex, QString& value)
{
    algorithNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();
    outputIndex = ui->comboBox_portName->currentIndex();
    value= ui->label_value->text().toDouble();
}

void addValueDialog::slot_algorithmNameChanged(int index)
{
    if (index < 0)
        return;
    int iAlgorithmNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();

    // 初始化端口；
    if (m_mapOutputPort.find(iAlgorithmNum) == m_mapOutputPort.end())
    {
        ui->comboBox_portName->clear();
    }
    else
    {
        ui->comboBox_portName->clear();
        auto& vecPort = m_mapOutputPort[iAlgorithmNum];
        for (auto& protData : vecPort)
        {
            ui->comboBox_portName->addItem(protData);
        }
    }

    // 初始化拍数；
    if (m_mapAllOutputData.find(iAlgorithmNum) == m_mapAllOutputData.end())
    {
        ui->comboBox_count->clear();
        ui->label_value->setText("");
    }
    else
    {
        auto& vecOutputData = m_mapAllOutputData.at(iAlgorithmNum);
        ui->comboBox_count->clear();
        for (auto& paiNum : vecOutputData)
        {
            ui->comboBox_count->addItem(QString::number(paiNum.first));
        }

        // 初始化显示数值;
        int paiNum = ui->comboBox_count->currentText().toInt();
        if (vecOutputData.find(paiNum) == vecOutputData.end())
        {
            ui->label_value->setText("");
        }
        else
        {
            double value = vecOutputData[paiNum].at(ui->comboBox_portName->currentIndex());
            ui->label_value->setText(QString::number(value));
        }
       
    }


    // 初始化拍数；
 /*    auto &vecOutputData=m_mapAllOutputData.at(iAlgorithmNum);
    ui->comboBox_count->clear();
    for(auto & paiNum : vecOutputData)
    {
        ui->comboBox_count->addItem(QString::number(paiNum.first));
    }*/

    //// 初始化端口
    //auto &vecPort =m_mapOutputPort[iAlgorithmNum];
    //ui->comboBox_portName->clear();
    //for(auto & protData : vecPort)
    //{
    //    ui->comboBox_portName->addItem(protData);
    //}

    //ui->label_value->clear();
    //if(vecPort.isEmpty())
    //    return;
    //if(vecOutputData.size()<=0)
    //    return;
    //// 初始化显示数值;
    //int paiNum = ui->comboBox_count->currentIndex() + 1;
    //int protIndex = ui->comboBox_portName->currentIndex();
    //if (protIndex > vecOutputData[paiNum].size())
    //    return;
    //double value = vecOutputData[paiNum].at(protIndex);
   
    //ui->label_value->setText(QString::number(value));
}

void addValueDialog::slot_paiNumChanged(int index)
{
    if (index < 0)
        return;
    int iAlgorithmNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();

    if (m_mapAllOutputData.find(iAlgorithmNum) == m_mapAllOutputData.end())
    {
        ui->label_value->setText("");
    }
    else
    {
        auto& vecOutputData = m_mapAllOutputData.at(iAlgorithmNum);

        int paiNum = ui->comboBox_count->currentIndex() + 1;
        int protIndex = ui->comboBox_portName->currentIndex();

        if (vecOutputData.find(paiNum) == vecOutputData.end())
        {
            ui->label_value->setText("");
        }
        else
        {
            double value = vecOutputData[paiNum].at(protIndex);
            ui->label_value->setText(QString::number(value));
        }
    }
    
}

void addValueDialog::slot_portNameChanged(int index)
{
    if (index < 0)
        return;
    int iAlgorithmNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();

    if (m_mapAllOutputData.find(iAlgorithmNum) == m_mapAllOutputData.end())
    {
        ui->label_value->setText("");
    }
    else
    {
        auto& vecOutputData = m_mapAllOutputData.at(iAlgorithmNum);

        int paiNum = ui->comboBox_count->currentIndex() + 1;
        int protIndex = ui->comboBox_portName->currentIndex();

        if (vecOutputData.find(paiNum) == vecOutputData.end())
        {
            ui->label_value->setText("");
        }
        else
        {
            double value = vecOutputData[paiNum].at(protIndex);
            ui->label_value->setText(QString::number(value));
        }

    }

}

void addValueDialog::slot_btnOK()
{
    this->accept();
}

void addValueDialog::slot_btnCancle()
{
    this->close();
}
