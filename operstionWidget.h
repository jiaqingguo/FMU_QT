#ifndef OPERSTIONWIDGET_H
#define OPERSTIONWIDGET_H

#include <QWidget>
#include <QFileInfo>
#include <QFile>
#include <QSet>
#include <QDomDocument>
#include <QMultiMap>

#include<thread>

#include "fmi4cpp/fmi4cpp.hpp"

namespace Ui {
class operstionWidget;
}

class operstionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit operstionWidget(const int num,QWidget *parent = nullptr);
    ~operstionWidget();

    std::string string_To_UTF8(const std::string& str);
    bool modifyFileFormat(const QString &strFliePath,const QString &strSavePath);
    bool decompressingFiles(const QString &strZipPath,const QString &strSavePath);
    bool createFolder(const QString &folderPath);
    bool deleteFile(const QString strFilePath);
    bool deleteDir(const QString strDirPath);
    bool readXML(const QString strXmlPath);

    bool create_xml_configuration(QFile& file, QDomDocument& doc, QDomElement& root);

    void load_algorithm_conguration(const QString filePath);
    void load_data_conguration(const QVector<QString> &vecInputPort, std::vector<fmi2ValueReference> &vecInputValueReference,const QVector<double>& vecInputValue, const QVector<QString>& vecOutputPort, std::vector<fmi2ValueReference>& vecOutputValueReference );
    void load_relevance_conguration(const QMap<int, QMap<int, int>>& mapRelevance);
    int get_algorithm_num();
    //void set_port_relevance(int srcOutIndex, int dstSrcAlgorithNum, int dstInputIndex);
    // 更新端口关联数据;
    void update_prot_data(QMap<int, double> portData);
private:

    void load_tableWidget_show();
    void use_fmu_caculate();

    //获得关联端口数据;
    void get_relevacne_port_data();

signals:
    // 端口关联信号;
   // void signal_port_relevance(int srcAlgorithNum, int srcOutIndex, int dstSrcAlgorithNum, int dstInputIndex);

    void signal_update_port_data(QMap<int, QMap<int, double>> mapNewData);
public slots:
    void slot_btnCalculate();
private slots:
    void slot_btnChooseFile();
    void slot_tableWigdetCheckedChanged(int row, int col);
    void slot_btn_clear_input();
    void slot_tableWidgetCustomContextMenuRequested(const QPoint &pos);
   
    void slot_btnClear();
    void slot_btnCurveShow();

    void slot_comboxPaiNumChanged(int index);
    void slot_tableWidgetCellEntered(int row, int column);
private:
    Ui::operstionWidget *ui;
    int m_iAlgorithmNum=0;// 算法顺序数;
    QFileInfo m_fileInfo;
    QVector<QString> m_vecInputPort;   // 输入端口;
    QSet<int> m_setOutputIndex;

    
    int m_iCalculateCount =0;
    std::thread							m_thread;
    std::vector<fmi2ValueReference> m_vecInputValueReference;

    std::vector<fmi2ValueReference> m_vecOutputValueReference; 
    QVector<double> m_vecInputValue;
     //m_mapMultPort;// < 算法num <自身输入端口位，<算法num，关联的输出端口>>
    static QMap<int, QMap<int, QMap<int, int>>> m_mapRelevance;
   static QMap<int,QString> m_mapAlgorithmName;// 算法名称；
   static QMap<int,QVector<QString>> m_mapOutputPort; //输出端口;
   static std::map<int,std::map<int,std::vector<double>>> m_mapAllOutputData; // <<算法顺序，<拍数，输出结果>>;
};

#endif // OPERSTIONWIDGET_H
