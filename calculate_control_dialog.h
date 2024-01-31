#ifndef CALCULATE_CONTROL_DIALOG_H
#define CALCULATE_CONTROL_DIALOG_H

#include <QDialog>

namespace Ui {
class calculate_control_dialog;
}

class calculate_control_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit calculate_control_dialog(QWidget *parent = nullptr);
    ~calculate_control_dialog();

    void reset_btns();
signals:
    void signal_calculate_control(int flag,int count, int cycleTime);
public slots:
    void slot_btn_start();
    void slot_btn_pause(bool checked);
    void slot_btn_stop();

private:
    Ui::calculate_control_dialog *ui;

};

#endif // CALCULATE_CONTROL_DIALOG_H
