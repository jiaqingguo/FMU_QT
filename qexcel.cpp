#include "qexcel.h"

QExcel::QExcel(QObject* parent) :QObject(parent)
{
    excel = nullptr;
    workBooks = nullptr;
    workBook = nullptr;
    sheets = nullptr;
    sheet = nullptr;
}

QExcel::~QExcel()
{

}

/***************************************************************************/
/* �ļ�����                                                                 */
/**************************************************************************/

void QExcel::open(QString FileName)
{
    if (excel == nullptr)
    {
        excel = new QAxObject();
        excel->setControl("Excel.Application");                            //����Excel�ؼ�
        excel->setProperty("DisplayAlerts", false);                        //��ֹ��ʾ����
        workBooks = excel->querySubObject("Workbooks");                    //��ȡ����������
        QFile file(FileName);
        if (file.exists())
        {
            workBooks->dynamicCall("Open(const QString&)", FileName);      //��ָ���ļ�
            workBook = excel->querySubObject("ActiveWorkBook");            //��ȡ��Ծ������
            sheets = workBook->querySubObject("WorkSheets");               //��ȡ��������
            sheet = workBook->querySubObject("ActiveSheet");               //��ȡ��Ծ������
        }
    }
    else
    {
        workBooks = excel->querySubObject("Workbooks");                    //��ȡ����������
        QFile file(FileName);
        if (file.exists())
        {
            workBooks->dynamicCall("Open(const QString&)", FileName);      //��ָ���ļ�
            workBook = excel->querySubObject("ActiveWorkBook");            //��ȡ��Ծ������
            sheets = workBook->querySubObject("WorkSheets");               //��ȡ��������
            sheet = workBook->querySubObject("ActiveSheet");               //��ȡ��Ծ������
        }
    }
}

bool QExcel::createFile(QString FileName)
{
    if (m_mapExeclFile.contains(FileName))
    {
        return false;
    }
    m_mapExeclFile[FileName]=m_fileCount;
    m_fileCount++;
    deleteFile(FileName);

    if (excel == nullptr)
    {
        excel = new QAxObject();
        excel->setControl("Excel.Application");                            //����Excel�ؼ�
        excel->setProperty("DisplayAlerts", false);                        //��ֹ��ʾ����
        workBooks = excel->querySubObject("Workbooks");                    //��ȡ����������
        QFile file(FileName);
        if (!file.exists())
        {
            workBooks->dynamicCall("Add");                                 //�½��ļ�
            workBook = excel->querySubObject("ActiveWorkBook");              //��ȡ��Ծ������
            workBook->dynamicCall("SaveAs(const QString&)", FileName);      //��ָ���ļ��������ļ�
            sheets = workBook->querySubObject("WorkSheets");               //��ȡ��������
            sheet = workBook->querySubObject("ActiveSheet");               //��ȡ��Ծ������
              // ���ù����������
            sheet->setProperty("Name", "default");
        }
    }
    else
    {
        workBooks = excel->querySubObject("Workbooks");                    //��ȡ����������
        QFile file(FileName);
        if (!file.exists())
        {
            workBooks->dynamicCall("Add");                                 //�½��ļ�
            workBook = excel->querySubObject("ActiveWorkBook");              //��ȡ��Ծ������
            workBook->dynamicCall("SaveAs(const QString&)", FileName);      //��ָ���ļ��������ļ�
            sheets = workBook->querySubObject("WorkSheets");               //��ȡ��������
            sheet = workBook->querySubObject("ActiveSheet");               //��ȡ��Ծ������
              // ���ù����������
            sheet->setProperty("Name", "default");
        }
    }
}

void QExcel::save() //����Excel�ļ�
{
    workBook->dynamicCall("Save()");
    qDebug() << "�ļ��ѱ���";
}

void QExcel::quit() //�˳�Excel
{
    excel->dynamicCall("Quit()");

    delete sheet;
    delete sheets;
    delete workBook;
    delete workBooks;
    delete excel;

    excel = nullptr;
    workBooks = nullptr;
    workBook = nullptr;
    sheets = nullptr;
    sheet = nullptr;

    qDebug() << "�˳�Excel";
}

/**************************************************************************/
/* ���������                                                              */
/*************************************************************************/

void QExcel::selectSheet(const QString& sheetName) //��������ѡ������
{
    sheet = sheets->querySubObject("Item(const QString&)", sheetName);
}

void QExcel::selectSheet(int sheetIndex) //�����±�����ѡ�������±��1��ʼ
{
    sheet = sheets->querySubObject("Item(int)", sheetIndex);
}

void QExcel::deleteSheet(const QString& sheetName) //��������ɾ��������
{
    QAxObject* a = sheets->querySubObject("Item(const QString&)", sheetName);
    a->dynamicCall("delete");
}

void QExcel::deleteSheet(int sheetIndex) //�����±�����ɾ���������±��1��ʼ
{
    QAxObject* a = sheets->querySubObject("Item(int)", sheetIndex);
    a->dynamicCall("delete");
}

void QExcel::insertSheet(QString sheetName) //�½�������
{
    sheets->querySubObject("Add()");
    QAxObject* a = sheets->querySubObject("Item(int)", 1);
    a->setProperty("Name", sheetName);
}

int QExcel::getSheetsCount() //��ȡ����������
{
    return sheets->property("Count").toInt();
}

QString QExcel::getSheetName() //��ȡ��ǰ��Ծ�����������
{
    return sheet->property("Name").toString();
}

QString QExcel::getSheetName(int sheetIndex) //�����±�������ȡ����������
{
    QAxObject* a = sheets->querySubObject("Item(int)", sheetIndex);
    return a->property("Name").toString();
}

/***************************************************************************/
/* ��Ԫ�����                                                               */
/**************************************************************************/

void QExcel::mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn) //�������б�źϲ���Ԫ��
{
    QString cell;
    cell.append(QChar(topLeftColumn - 1 + 'A'));
    cell.append(QString::number(topLeftRow));
    cell.append(":");
    cell.append(QChar(bottomRightColumn - 1 + 'A'));
    cell.append(QString::number(bottomRightRow));

    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);//xlCenter
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);
}

void QExcel::mergeCells(const QString& cell) //������ĸ��źϲ���Ԫ��
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);//xlCenter
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);
}

void QExcel::setCellString(int row, int column, const QString& value) //�������б�����õ�Ԫ������
{
    QAxObject* range = sheet->querySubObject("Cells(int,int)", row, column);
    range->dynamicCall("SetValue(const QString&)", value);
}

void QExcel::setCellString(const QString& cell, const QString& value) //������ĸ������õ�Ԫ������
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("SetValue(const QString&)", value);
}

bool QExcel::setRowData(int row, int colum_start, int column_end, QVariant vData) //����д��һ������
{
    bool op = false;
    QString start, end;
    start = getRange_fromColumn(colum_start);
    end = getRange_fromColumn(column_end);
    QVariant qstrRange = start + QString::number(row, 10) + ":" + end + QString::number(row, 10);
    QAxObject* range = sheet->querySubObject("Range(const QString&)", qstrRange);
    if (range)
    {
        range->dynamicCall("SetValue(const QVariant&)", QVariant(vData)); //�޸ĵ�Ԫ�������
        op = true;
    }
    else
    {
        op = false;
    }

    delete range;
    return op;
}

QString QExcel::getRange_fromColumn(int column) //������ֵ�������ĸֵ
{
    if (column <= 0) //��ֵ������ڵ���1
        return "";

    QString ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString result = "";
    QVector<int> pos;
    if (column >= 1 && column <= 26)
    {
        result += ABC[column - 1];
        return result;
    }
    else
    {
        int high = column;
        int low;
        int last_high;
        while (high > 0)
        {
            last_high = high;
            high = high / 26;
            low = last_high % 26;
            if (low == 0 && high != 0)
            {
                low = 26;
                high = high - 1;
            }
            pos.push_front(low);
        }

        for (int i = 0; i < pos.size(); i++)
        {
            result += ABC[pos[i] - 1];
        }
    }
    return result;
}

QVariant QExcel::getCellValue(int row, int column) //�������б�Ż�ȡ��Ԫ����ֵ
{
    QAxObject* range = sheet->querySubObject("Cells(int,int)", row, column);
    return range->property("Value2");
}

QVariant QExcel::getCellValue(const QString& cell) //������ĸ��Ż�ȡ��Ԫ����ֵ
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    return range->property("Value2");
}

void QExcel::clearCell(int row, int column) //�������б����յ�Ԫ��
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("ClearContents()");
}

void QExcel::clearCell(const QString& cell) //������ĸ�����յ�Ԫ��
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("ClearContents()");
}

void QExcel::setCellFontBold(int row, int column, bool isBold) //�������б�����õ�Ԫ�������Ƿ�Ӵ�
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);
}

void QExcel::setCellFontBold(const QString& cell, bool isBold) //������ĸ������õ�Ԫ�������Ƿ�Ӵ�
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);
}

void QExcel::setCellFontSize(int row, int column, int size) //�������б�����õ�Ԫ�������С
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Size", size);
}

void QExcel::setCellFontSize(const QString& cell, int size) //������ĸ������õ�Ԫ�������С
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Size", size);
}

void QExcel::setCellTextCenter(int row, int column) //�������б�����õ�Ԫ�����ݾ���
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);//xlCenter
}

void QExcel::setCellTextCenter(const QString& cell) //������ĸ������õ�Ԫ�����ݾ���
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);//xlCenter
}

void QExcel::setCellTextWrap(int row, int column, bool isWrap) //�������б�����õ�Ԫ�������Ƿ���
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("WrapText", isWrap);
}

void QExcel::setCellTextWrap(const QString& cell, bool isWrap) //������ĸ������õ�Ԫ�������Ƿ���
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("WrapText", isWrap);
}

void QExcel::setCellBackground(int row, int column, QColor color) //�������б�����õ�Ԫ�񱳾���ɫ
{
    QAxObject* range = sheet->querySubObject("Cells(int,int)", row, column);
    QAxObject* interior = range->querySubObject("Interior");
    interior->setProperty("Color", color);
}

void QExcel::setCellBackground(const QString& cell, QColor color) //������ĸ������õ�Ԫ�񱳾���ɫ
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    QAxObject* interior = range->querySubObject("Interior");
    interior->setProperty("Color", color);
}

void QExcel::setCellFontColor(int row, int column, QColor color) //�������б�����õ�Ԫ��������ɫ
{
    QAxObject* range = sheet->querySubObject("Cells(int,int)", row, column);
    QAxObject* font = range->querySubObject("Font");
    font->setProperty("Color", color);
}

void QExcel::setCellFontColor(const QString& cell, QColor color) //������ĸ������õ�Ԫ��������ɫ
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    QAxObject* font = range->querySubObject("Font");
    font->setProperty("Color", color);
}

void QExcel::setCellBorderColor(int row, int column, QColor color) //�������б�����õ�Ԫ��߿���ɫ
{
    QAxObject* range = sheet->querySubObject("Cells(int,int)", row, column);
    QAxObject* border = range->querySubObject("Borders");
    border->setProperty("Color", color);
}

void QExcel::setCellBorderColor(const QString& cell, QColor color) //������ĸ������õ�Ԫ��߿���ɫ
{
    QAxObject* range = sheet->querySubObject("Range(const QString&)", cell);
    QAxObject* border = range->querySubObject("Borders");
    border->setProperty("Color", color);
}

void QExcel::getUsedRange(int* topLeftRow, int* topLeftColumn, int* bottomRightRow, int* bottomRightColumn) //ȡ�ù�������ʹ�÷�Χ
{
    QAxObject* usedRange = sheet->querySubObject("UsedRange");
    *topLeftRow = usedRange->property("Row").toInt();
    *topLeftColumn = usedRange->property("Column").toInt();

    QAxObject* rows = usedRange->querySubObject("Rows");
    *bottomRightRow = *topLeftRow + rows->property("Count").toInt() - 1;

    QAxObject* columns = usedRange->querySubObject("Columns");
    *bottomRightColumn = *topLeftColumn + columns->property("Count").toInt() - 1;
}

void QExcel::setColumnWidth(int column, int width) //�����п�
{
    QString columnName;
    columnName.append(QChar(column - 1 + 'A'));
    columnName.append(":");
    columnName.append(QChar(column - 1 + 'A'));

    QAxObject* col = sheet->querySubObject("Columns(const QString&)", columnName);
    col->setProperty("ColumnWidth", width);
}

void QExcel::setRowHeight(int row, int height) //�����и�
{
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject* r = sheet->querySubObject("Rows(const QString &)", rowsName);
    r->setProperty("RowHeight", height);
}

void QExcel::setAutoFitRow(int row) //��������Ӧ�и�
{
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject* rows = sheet->querySubObject("Rows(const QString &)", rowsName);
    rows->dynamicCall("AutoFit()");
}

void QExcel::mergeSerialSameCellsInAColumn(int column, int topRow) //�ϲ�һ������ͬ�����ĵ�Ԫ��
{
    int a, b, c, rowsCount;
    getUsedRange(&a, &b, &rowsCount, &c);

    int aMergeStart = topRow, aMergeEnd = topRow + 1;

    QString value;
    while (aMergeEnd <= rowsCount)
    {
        value = getCellValue(aMergeStart, column).toString();
        while (value == getCellValue(aMergeEnd, column).toString())
        {
            clearCell(aMergeEnd, column);
            aMergeEnd++;
        }
        aMergeEnd--;
        mergeCells(aMergeStart, column, aMergeEnd, column);

        aMergeStart = aMergeEnd + 1;
        aMergeEnd = aMergeStart + 1;
    }
}

int QExcel::getUsedRowsCount() //��ȡ������
{
    QAxObject* usedRange = sheet->querySubObject("UsedRange");
    int topRow = usedRange->property("Row").toInt();
    QAxObject* rows = usedRange->querySubObject("Rows");
    int bottomRow = topRow + rows->property("Count").toInt() - 1;
    return bottomRow;
}

int QExcel::getUsedColumnCount() //��ȡ������
{
    QAxObject* usedRange = sheet->querySubObject("UsedRange");
    int leftColumn = usedRange->property("Column").toInt();
    QAxObject* columns = usedRange->querySubObject("Columns");
    int rightColumn = leftColumn + columns->property("Count").toInt() - 1;
    return rightColumn;
}

bool QExcel::deleteFile(const QString strFilePath)
{
    QFile file(strFilePath);

    return  file.remove();
}

QAxObject* QExcel::getWorkBooks()
{
    return workBooks;
}

QAxObject* QExcel::getWorkBook()
{
    return workBook;
}

QAxObject* QExcel::getWorkSheets()
{
    return sheets;
}

QAxObject* QExcel::getWorkSheet()
{
    return sheet;
}

