#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>

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

private:
    Ui::Widget *ui;
    int m_iAlgorithmNum=0;
};
#endif // WIDGET_H
