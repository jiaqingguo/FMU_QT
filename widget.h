#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include "calculate_control_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    struct myUseData :QObjectUserData 
    {
        int iAlgorithmNum;
    };
private slots:
    void  slot_widgetCustomContextMenuRequested(const QPoint &pos);

    void create_xml_configuration();

    bool load_xml_configuration();

    void initialize_configuration();

   // void slot_setRelevance(int srcAlgorithNum, int srcOutIndex, int dstSrcAlgorithNum, int dstInputIndex);

    void slot_update_prot_data(QMap<int, QMap<int, double>> mapNewData);

    void slot_btn_calculate_control();

    void slot_recv_calculate_control(int flag,int count/* = 0*/);

    
private:
    Ui::Widget *ui;
    int m_iAlgorithmNum=0;
    calculate_control_dialog* m_calculate_control_dialog;
    int m_iThreadCount = 10;
};
#endif // WIDGET_H
