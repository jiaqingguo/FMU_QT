/********************************************************************************
** Form generated from reading UI file 'curveShowDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CURVESHOWDIALOG_H
#define UI_CURVESHOWDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_curveShowDialog
{
public:

    void setupUi(QDialog *curveShowDialog)
    {
        if (curveShowDialog->objectName().isEmpty())
            curveShowDialog->setObjectName(QString::fromUtf8("curveShowDialog"));
        curveShowDialog->resize(650, 600);
        curveShowDialog->setMinimumSize(QSize(650, 600));

        retranslateUi(curveShowDialog);

        QMetaObject::connectSlotsByName(curveShowDialog);
    } // setupUi

    void retranslateUi(QDialog *curveShowDialog)
    {
        curveShowDialog->setWindowTitle(QCoreApplication::translate("curveShowDialog", "\350\276\223\345\207\272\346\233\262\347\272\277\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class curveShowDialog: public Ui_curveShowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CURVESHOWDIALOG_H
