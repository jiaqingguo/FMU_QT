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
    // 初始化拍数；
     auto &vecOutputData=m_mapAllOutputData[iAlgorithmNum];

    for(auto & paiNum : vecOutputData)
    {
        ui->comboBox_count->addItem(QString::number(paiNum.first));
    }

    // 初始化端口
    auto &vecPort =m_mapOutputPort[iAlgorithmNum];
    if(vecPort.isEmpty())
        return;
    for(auto & protData : vecPort)
    {
        ui->comboBox_portName->addItem(protData);
    }
    if(vecPort.isEmpty())
        return;
    if(vecOutputData.size()<=0)
        return;
    // 初始化显示数值;
    int paiNum = ui->comboBox_count->currentIndex();
    double value = vecOutputData[paiNum].at(ui->comboBox_portName->currentIndex());
    ui->label_value->setText(QString::number(value));
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

void addValueDialog::slot_algorithmNameChanged(int index)
{
    int iAlgorithmNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();
    // 初始化拍数；
     auto &vecOutputData=m_mapAllOutputData[iAlgorithmNum];
    ui->comboBox_count->clear();
    for(auto & paiNum : vecOutputData)
    {
        ui->comboBox_count->addItem(QString::number(paiNum.first));
    }

    // 初始化端口
    auto &vecPort =m_mapOutputPort[iAlgorithmNum];
    ui->comboBox_portName->clear();
    for(auto & protData : vecPort)
    {
        ui->comboBox_portName->addItem(protData);
    }

    ui->label_value->clear();
    if(vecPort.isEmpty())
        return;
    if(vecOutputData.size()<=0)
        return;
    // 初始化显示数值;
    int paiNum = ui->comboBox_count->currentIndex();
    double value = vecOutputData[paiNum].at(ui->comboBox_portName->currentIndex());
    ui->label_value->setText(QString::number(value));
}

void addValueDialog::slot_paiNumChanged(int index)
{
    int iAlgorithmNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();

    auto &vecOutputData=m_mapAllOutputData[iAlgorithmNum];

    ui->label_value->clear();

    if(vecOutputData.size()<=0)
        return;
    // 改变显示数值;
    int paiNum = ui->comboBox_count->currentIndex();
    double value = vecOutputData[paiNum].at(ui->comboBox_portName->currentIndex());
    ui->label_value->setText(QString::number(value));
}

void addValueDialog::slot_portNameChanged(int index)
{
    int iAlgorithmNum = ui->comboBox_algorithmName->currentData(Qt::UserRole).toInt();

    auto &vecOutputData=m_mapAllOutputData[iAlgorithmNum];
    ui->label_value->clear();

    if(vecOutputData.size()<=0)
        return;
    // 改变显示数值;
    int paiNum = ui->comboBox_count->currentIndex();
    double value = vecOutputData[paiNum].at(ui->comboBox_portName->currentIndex());
    ui->label_value->setText(QString::number(value));

}
