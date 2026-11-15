#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include "calculate_control_dialog.h"
#include "curveShowDialog.h"
#include "graphdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class thread_pool;
class QExcel;
class UdpClientManager;

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

   // curveShowDialog* get_curve_ptr();
    graphDialog* getGraphDialog();
    QExcel* get_execl_ptr();
    UdpClientManager* getUdpClientManager();
    void init_udp();

    // 收集所有算法最新拍输出数据，拼接为一个double数组通过UDP发送
    void sendCycleDataViaUdp();
protected:
    void closeEvent(QCloseEvent* event);
private slots:
    void slot_widgetCustomContextMenuRequested(const QPoint &pos);

    void create_xml_configuration();

    bool load_xml_configuration();

    void initialize_configuration();

    void slot_update_prot_data(QMap<int, QMap<int, double>> mapNewData);

    void slot_btn_calculate_control();
    void slot_btnClearAll();

    void slot_recv_calculate_control(int flag,int calculate_count,int cycleTime/* = 0*/);

    void slot_fmu_thread_finished(int tab, const  std::vector<double> vecOutputValue);

    void slot_tab_changed(int index);

    void slot_udp_recv_data(const QByteArray& udpData);
private:
    Ui::Widget *ui;
    int m_iAlgorithmNum=0;
    calculate_control_dialog* m_calculate_control_dialog;
    int m_iThreadCount = 10;

    thread_pool* m_pThread_pool;			// �̳߳ض���

    curveShowDialog* m_curve_show_dialog = nullptr;
    graphDialog* m_graphDialog = nullptr;
    QExcel* m_qexecl;

    UdpClientManager* m_pUdpClientManager = nullptr;
};

extern Widget* g_pWidget;
#endif // WIDGET_H
