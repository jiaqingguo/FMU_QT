#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include "calculate_control_dialog.h"
#include "curveShowDialog.h"
//


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class thread_pool;
class QExcel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    
    void update_algorithm_tableWidget_out(const int& tab, const  std::vector<double> & vecOutputValue);

    void update_algorithhnum_opersition(const int& tab, const  std::vector<double>& vecOutputValue);
    std::vector<double>  get_algorithm_tableWidget_input(const int& tab);

    void calculate_control(int count);

    void reset_control_btns();

    curveShowDialog* get_curve_ptr();
    QExcel* get_execl_ptr();
protected:
    void closeEvent(QCloseEvent* event);
private slots:
    void slot_widgetCustomContextMenuRequested(const QPoint &pos);

    void create_xml_configuration();

    bool load_xml_configuration();

    void initialize_configuration();

    void slot_update_prot_data(QMap<int, QMap<int, double>> mapNewData);

    void slot_btn_calculate_control();

    void slot_recv_calculate_control(int flag,int calculate_count,int cycleTime/* = 0*/);

    void slot_fmu_thread_finished(int tab, const  std::vector<double> vecOutputValue);

    void slot_tab_changed(int index);
private:
    Ui::Widget *ui;
    int m_iAlgorithmNum=0;
    calculate_control_dialog* m_calculate_control_dialog;
    int m_iThreadCount = 10;

    thread_pool* m_pThread_pool;			// 线程池对象

    curveShowDialog* m_curve_show_dialog = nullptr;
    QExcel* m_qexecl;
};

extern Widget* g_pWidget;
#endif // WIDGET_H
