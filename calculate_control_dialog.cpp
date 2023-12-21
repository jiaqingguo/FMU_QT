#include "calculate_control_dialog.h"
#include "ui_calculate_control_dialog.h"

#include <qdebug.h>

calculate_control_dialog::calculate_control_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calculate_control_dialog)
{
    ui->setupUi(this);
    ui->btn_pause->setCheckable(true);
    ui->btn_pause->setEnabled(true);
    ui->btn_stop->setEnabled(true);
    connect(ui->btn_start, &QPushButton::clicked, this, &calculate_control_dialog::slot_btn_start);
    connect(ui->btn_pause, &QPushButton::clicked, this, &calculate_control_dialog::slot_btn_pause);
    connect(ui->btn_stop, &QPushButton::clicked, this, &calculate_control_dialog::slot_btn_stop);

}

calculate_control_dialog::~calculate_control_dialog()
{
    delete ui;
}
void calculate_control_dialog::slot_btn_start()
{
    ui->btn_start->setEnabled(false);
    ui->btn_pause->setEnabled(true);
    ui->btn_stop->setEnabled(true);
    emit signal_calculate_control(1, ui->spinBox->value());
}

void calculate_control_dialog::slot_btn_pause(bool checked)
{
    if (checked)
    {
        ui->btn_pause->setText(QString::fromLocal8Bit("����"));
        emit signal_calculate_control(2, ui->spinBox->value());
    }
    else
    {
        ui->btn_pause->setText(QString::fromLocal8Bit("��ͣ"));
        emit signal_calculate_control(3, ui->spinBox->value());
    }
    qDebug() << "slot_btn_pause()";
   
}

void calculate_control_dialog::slot_btn_stop()
{
    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);
    emit signal_calculate_control(0, ui->spinBox->value());
    qDebug() << "slot_btn_stop()";
}
