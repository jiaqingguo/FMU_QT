#ifndef ADDVALUEDIALOG_H
#define ADDVALUEDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class addValueDialog;
}

class addValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addValueDialog(QWidget *parent = nullptr);
    ~addValueDialog();

    void init();

    QString getInputValue();
    void setAlgorithmName(const QMap<int,QString> &mapAlgorithmName);
    void setOutputPortName(const QMap<int,QVector<QString>> &mapOutputPort);
    void setAllOutputData(const std::map<int,std::map<int,std::vector<double>>> & mapAllOutputData);
private slots:
    void slot_algorithmNameChanged(int index);
    void slot_paiNumChanged(int index);
    void slot_portNameChanged(int index);
    void slot_btnOK();
    void slot_btnCancle();
private:
    Ui::addValueDialog *ui;
    QMap<int,QString> m_mapAlgorithmName;// 算法名称；
    QMap<int,QVector<QString>> m_mapOutputPort; //输出端口;
    std::map<int,std::map<int,std::vector<double>>> m_mapAllOutputData; // <<算法顺序，<拍数，输出结果>>;
};

#endif // ADDVALUEDIALOG_H
