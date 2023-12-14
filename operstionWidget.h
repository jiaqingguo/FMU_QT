#ifndef OPERSTIONWIDGET_H
#define OPERSTIONWIDGET_H

#include <QWidget>
#include <QFileInfo>
#include <QFile>
#include <QSet>
#include "QtGui/private/qzipreader_p.h"


namespace Ui {
class operstionWidget;
}

class operstionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit operstionWidget(const int num,QWidget *parent = nullptr);
    ~operstionWidget();


    bool modifyFileFormat(const QString &strFliePath,const QString &strSavePath);
    bool decompressingFiles(const QString &strZipPath,const QString &strSavePath);
    bool createFolder(const QString &folderPath);
    bool deleteFile(const QString strFilePath);
    bool deleteDir(const QString strDirPath);
    bool readXML(const QString strXmlPath);

private slots:
    void slot_btnChooseFile();
    void slot_tableWigdetCheckedChanged(int row, int col);
    void slot_addInputValeu();
    void slot_tableWidgetCustomContextMenuRequested(const QPoint &pos);
    void slot_btnCalculate();
    void slot_btnClear();
    void slot_btnCurveShow();
    void slot_comboxPaiNumChanged();
private:
    Ui::operstionWidget *ui;
    int m_iAlgorithmNum=0;// 算法顺序数;
    QFileInfo m_fileInfo;
    QVector<QString> m_vecInputPort;   // 输入端口;
    QSet<int> m_setOutputIndex;

    int m_iCalculateCount =0;


   static QMap<int,QString> m_mapAlgorithmName;// 算法名称；
   static QMap<int,QVector<QString>> m_mapOutputPort; //输出端口;
   static std::map<int,std::map<int,std::vector<double>>> m_mapAllOutputData; // <<算法顺序，<拍数，输出结果>>;
};

#endif // OPERSTIONWIDGET_H
