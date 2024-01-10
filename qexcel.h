#ifndef QEXCEL_H
#define QEXCEL_H

#include <QString>
#include <QColor>
#include <QVariant>
#include <QAxObject>
#include <QFile>
#include <QStringList>
#include <QDebug>

class QAxObject;

class QExcel : public QObject
{
public:
    QExcel(QObject* parent = nullptr);
    ~QExcel();

public:
    QAxObject* getWorkBooks();
    QAxObject* getWorkBook();
    QAxObject* getWorkSheets();
    QAxObject* getWorkSheet();


    /***************************************************************************/
    /* �ļ�����                                                                 */
    /**************************************************************************/
private:
    void open(QString FileName);                   //���ļ�  �������� ��Ϊ˽�� ��ͣʹ��;
public:
    bool createFile(QString FileName);             //�����ļ�
    void save();                                   //����Excel�ļ�
    void quit();                                  //�˳�Excel

    /**************************************************************************/
    /* ���������                                                              */
    /*************************************************************************/
    void selectSheet(const QString& sheetName);   //��������ѡ������
    void selectSheet(int sheetIndex);             //�����±�����ѡ�������±��1��ʼ
    void deleteSheet(const QString& sheetName);   //��������ɾ��������
    void deleteSheet(int sheetIndex);             //�����±�����ɾ���������±��1��ʼ
    void insertSheet(QString sheetName);          //�½�������
    int getSheetsCount();                         //��ȡ����������
    QString getSheetName();                       //��ȡ��ǰ��Ծ�����������
    QString getSheetName(int sheetIndex);         //�����±�������ȡ����������

    /***************************************************************************/
    /* ��Ԫ�����                                                               */
    /**************************************************************************/
    void mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn); //�������б�źϲ���Ԫ��
    void mergeCells(const QString& range);                         //������ĸ��źϲ���Ԫ������(A5:C7)
    void setCellString(int row, int column, const QString& value); //�������б�����õ�Ԫ�����ݣ�����(1,1,"xxx")
    void setCellString(const QString& cell, const QString& value); //������ĸ������õ�Ԫ�����ݣ�����("A5","xxx")
    bool setRowData(int row, int colum_start, int column_end, QVariant vData);  //����д��һ������
    QString getRange_fromColumn(int column);                                 //������ֵ�������ĸֵ
    QVariant getCellValue(int row, int column);                    //�������б�Ż�ȡ��Ԫ����ֵ
    QVariant getCellValue(const QString& cell);                    //������ĸ��Ż�ȡ��Ԫ����ֵ
    void clearCell(int row, int column);                           //�������б����յ�Ԫ��
    void clearCell(const QString& cell);                           //������ĸ�����յ�Ԫ��
    void setCellFontBold(int row, int column, bool isBold);        //�������б�����õ�Ԫ�������Ƿ�Ӵ�
    void setCellFontBold(const QString& cell, bool isBold);        //������ĸ������õ�Ԫ�������Ƿ�Ӵ�
    void setCellFontSize(int row, int column, int size);           //�������б�����õ�Ԫ�������С
    void setCellFontSize(const QString& cell, int size);           //������ĸ������õ�Ԫ�������С
    void setCellTextCenter(int row, int column);                   //�������б�����õ�Ԫ�����ݾ���
    void setCellTextCenter(const QString& cell);                   //������ĸ������õ�Ԫ�����ݾ���
    void setCellTextWrap(int row, int column, bool isWrap);        //�������б�����õ�Ԫ�������Ƿ���
    void setCellTextWrap(const QString& cell, bool isWrap);        //������ĸ������õ�Ԫ�������Ƿ���
    void setCellBackground(int row, int column, QColor color);     //�������б�����õ�Ԫ�񱳾�ɫ
    void setCellBackground(const QString& cell, QColor color);     //������ĸ������õ�Ԫ�񱳾�ɫ
    void setCellFontColor(int row, int column, QColor color);      //�������б�����õ�Ԫ��������ɫ
    void setCellFontColor(const QString& cell, QColor color);      //������ĸ������õ�Ԫ��������ɫ
    void setCellBorderColor(int row, int column, QColor color);    //�������б�����õ�Ԫ��߿���ɫ
    void setCellBorderColor(const QString& cell, QColor color);    //������ĸ������õ�Ԫ��߿���ɫ

    void getUsedRange(int* topLeftRow, int* topLeftColumn, int* bottomRightRow, int* bottomRightColumn); //ȡ�ù�������ʹ�÷�Χ
    void setColumnWidth(int column, int width);                    //�����п�
    void setRowHeight(int row, int height);                        //�����и�
    void setAutoFitRow(int row);                                   //��������Ӧ�и�
    void mergeSerialSameCellsInAColumn(int column, int topRow);    //�ϲ�һ������ͬ�����ĵ�Ԫ��
    int getUsedRowsCount();                                        //��ȡ������
    int getUsedColumnCount();                                      //��ȡ������



    // �����ļ�����;
    bool deleteFile(const QString strFilePath);
    int m_fileCount= 1;
    QMap<QString, int> m_mapExeclFile;

private:
    QAxObject* excel;         //Excelָ��
    QAxObject* workBooks;     //����������
    QAxObject* workBook;      //������
    QAxObject* sheets;        //��������
    QAxObject* sheet;         //������
};

#endif
