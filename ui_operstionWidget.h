/********************************************************************************
** Form generated from reading UI file 'operstionWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPERSTIONWIDGET_H
#define UI_OPERSTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_operstionWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_algorithm;
    QPushButton *btn_choose;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_input;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_clear_input;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QTableWidget *tableWidget_input;
    QGroupBox *groupBox_output;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btn_calculate;
    QPushButton *btn_clear;
    QLabel *label_2;
    QComboBox *comboBox_countShow;
    QPushButton *btn_saveExecl;
    QSpacerItem *horizontalSpacer_4;
    QTableWidget *tableWidget_output;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_curve_show;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *operstionWidget)
    {
        if (operstionWidget->objectName().isEmpty())
            operstionWidget->setObjectName(QString::fromUtf8("operstionWidget"));
        operstionWidget->resize(446, 422);
        verticalLayout_2 = new QVBoxLayout(operstionWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(operstionWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        label_algorithm = new QLabel(operstionWidget);
        label_algorithm->setObjectName(QString::fromUtf8("label_algorithm"));
        label_algorithm->setMinimumSize(QSize(60, 0));

        horizontalLayout->addWidget(label_algorithm);

        btn_choose = new QPushButton(operstionWidget);
        btn_choose->setObjectName(QString::fromUtf8("btn_choose"));

        horizontalLayout->addWidget(btn_choose);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        groupBox_input = new QGroupBox(operstionWidget);
        groupBox_input->setObjectName(QString::fromUtf8("groupBox_input"));
        groupBox_input->setMinimumSize(QSize(0, 180));
        groupBox_input->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_3 = new QVBoxLayout(groupBox_input);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btn_clear_input = new QPushButton(groupBox_input);
        btn_clear_input->setObjectName(QString::fromUtf8("btn_clear_input"));

        horizontalLayout_2->addWidget(btn_clear_input);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));

        horizontalLayout_2->addLayout(horizontalLayout_5);


        verticalLayout_3->addLayout(horizontalLayout_2);

        tableWidget_input = new QTableWidget(groupBox_input);
        tableWidget_input->setObjectName(QString::fromUtf8("tableWidget_input"));
        tableWidget_input->setMinimumSize(QSize(0, 70));
        tableWidget_input->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_3->addWidget(tableWidget_input);


        verticalLayout_2->addWidget(groupBox_input);

        groupBox_output = new QGroupBox(operstionWidget);
        groupBox_output->setObjectName(QString::fromUtf8("groupBox_output"));
        groupBox_output->setMinimumSize(QSize(0, 180));
        verticalLayout = new QVBoxLayout(groupBox_output);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btn_calculate = new QPushButton(groupBox_output);
        btn_calculate->setObjectName(QString::fromUtf8("btn_calculate"));

        horizontalLayout_4->addWidget(btn_calculate);

        btn_clear = new QPushButton(groupBox_output);
        btn_clear->setObjectName(QString::fromUtf8("btn_clear"));

        horizontalLayout_4->addWidget(btn_clear);

        label_2 = new QLabel(groupBox_output);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        comboBox_countShow = new QComboBox(groupBox_output);
        comboBox_countShow->setObjectName(QString::fromUtf8("comboBox_countShow"));

        horizontalLayout_4->addWidget(comboBox_countShow);

        btn_saveExecl = new QPushButton(groupBox_output);
        btn_saveExecl->setObjectName(QString::fromUtf8("btn_saveExecl"));

        horizontalLayout_4->addWidget(btn_saveExecl);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        tableWidget_output = new QTableWidget(groupBox_output);
        tableWidget_output->setObjectName(QString::fromUtf8("tableWidget_output"));
        tableWidget_output->setMinimumSize(QSize(0, 70));

        verticalLayout->addWidget(tableWidget_output);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btn_curve_show = new QPushButton(groupBox_output);
        btn_curve_show->setObjectName(QString::fromUtf8("btn_curve_show"));

        horizontalLayout_3->addWidget(btn_curve_show);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox_output);


        retranslateUi(operstionWidget);

        QMetaObject::connectSlotsByName(operstionWidget);
    } // setupUi

    void retranslateUi(QWidget *operstionWidget)
    {
        operstionWidget->setWindowTitle(QCoreApplication::translate("operstionWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("operstionWidget", "\347\256\227\346\263\225:", nullptr));
        label_algorithm->setText(QString());
        btn_choose->setText(QCoreApplication::translate("operstionWidget", "\351\200\211\346\213\251", nullptr));
        groupBox_input->setTitle(QCoreApplication::translate("operstionWidget", "\350\276\223\345\205\245", nullptr));
        btn_clear_input->setText(QCoreApplication::translate("operstionWidget", "\346\270\205\347\251\272\350\276\223\345\205\245", nullptr));
        groupBox_output->setTitle(QCoreApplication::translate("operstionWidget", "\350\276\223\345\207\272", nullptr));
        btn_calculate->setText(QCoreApplication::translate("operstionWidget", "\346\211\247\350\241\214\350\256\241\347\256\227", nullptr));
        btn_clear->setText(QCoreApplication::translate("operstionWidget", "\346\270\205\351\231\244\346\211\200\346\234\211\350\276\223\345\207\272", nullptr));
        label_2->setText(QCoreApplication::translate("operstionWidget", "\350\276\223\345\207\272\346\213\215\346\225\260\357\274\232", nullptr));
        comboBox_countShow->setCurrentText(QString());
        btn_saveExecl->setText(QCoreApplication::translate("operstionWidget", "\345\257\274\345\207\272EXECl", nullptr));
        btn_curve_show->setText(QCoreApplication::translate("operstionWidget", "\346\237\245\347\234\213\350\276\223\345\207\272\346\233\262\347\272\277\346\230\276\347\244\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class operstionWidget: public Ui_operstionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPERSTIONWIDGET_H
