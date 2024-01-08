/********************************************************************************
** Form generated from reading UI file 'addValueDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDVALUEDIALOG_H
#define UI_ADDVALUEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_addValueDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox_algorithmName;
    QLabel *label_;
    QComboBox *comboBox_count;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboBox_portName;
    QLabel *label_5;
    QLabel *label_value;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_ok;
    QPushButton *btn_cancle;

    void setupUi(QDialog *addValueDialog)
    {
        if (addValueDialog->objectName().isEmpty())
            addValueDialog->setObjectName(QString::fromUtf8("addValueDialog"));
        addValueDialog->resize(400, 187);
        verticalLayout = new QVBoxLayout(addValueDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(addValueDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        comboBox_algorithmName = new QComboBox(addValueDialog);
        comboBox_algorithmName->setObjectName(QString::fromUtf8("comboBox_algorithmName"));

        horizontalLayout->addWidget(comboBox_algorithmName);

        label_ = new QLabel(addValueDialog);
        label_->setObjectName(QString::fromUtf8("label_"));

        horizontalLayout->addWidget(label_);

        comboBox_count = new QComboBox(addValueDialog);
        comboBox_count->setObjectName(QString::fromUtf8("comboBox_count"));

        horizontalLayout->addWidget(comboBox_count);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(addValueDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboBox_portName = new QComboBox(addValueDialog);
        comboBox_portName->setObjectName(QString::fromUtf8("comboBox_portName"));

        horizontalLayout_2->addWidget(comboBox_portName);

        label_5 = new QLabel(addValueDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_2->addWidget(label_5);

        label_value = new QLabel(addValueDialog);
        label_value->setObjectName(QString::fromUtf8("label_value"));

        horizontalLayout_2->addWidget(label_value);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        btn_ok = new QPushButton(addValueDialog);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));

        horizontalLayout_3->addWidget(btn_ok);

        btn_cancle = new QPushButton(addValueDialog);
        btn_cancle->setObjectName(QString::fromUtf8("btn_cancle"));

        horizontalLayout_3->addWidget(btn_cancle);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(addValueDialog);

        QMetaObject::connectSlotsByName(addValueDialog);
    } // setupUi

    void retranslateUi(QDialog *addValueDialog)
    {
        addValueDialog->setWindowTitle(QCoreApplication::translate("addValueDialog", "\346\267\273\345\212\240\350\276\223\345\205\245\345\200\274&\345\205\263\350\201\224\347\256\227\346\263\225\350\276\223\345\207\272\344\275\215", nullptr));
        label->setText(QCoreApplication::translate("addValueDialog", "\347\256\227\346\263\225\357\274\232", nullptr));
        label_->setText(QCoreApplication::translate("addValueDialog", "\346\213\215\346\225\260\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("addValueDialog", "\350\276\223\345\207\272\344\275\215\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("addValueDialog", "\345\200\274\357\274\232", nullptr));
        label_value->setText(QString());
        btn_ok->setText(QCoreApplication::translate("addValueDialog", "\347\241\256\345\256\232", nullptr));
        btn_cancle->setText(QCoreApplication::translate("addValueDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class addValueDialog: public Ui_addValueDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDVALUEDIALOG_H
