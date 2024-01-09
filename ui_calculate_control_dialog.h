/********************************************************************************
** Form generated from reading UI file 'calculate_control_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALCULATE_CONTROL_DIALOG_H
#define UI_CALCULATE_CONTROL_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_calculate_control_dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpinBox *spinBox;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_start;
    QPushButton *btn_pause;
    QPushButton *btn_stop;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QDialog *calculate_control_dialog)
    {
        if (calculate_control_dialog->objectName().isEmpty())
            calculate_control_dialog->setObjectName(QString::fromUtf8("calculate_control_dialog"));
        calculate_control_dialog->resize(400, 145);
        verticalLayout = new QVBoxLayout(calculate_control_dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(calculate_control_dialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        spinBox = new QSpinBox(calculate_control_dialog);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMaximum(999999999);

        horizontalLayout->addWidget(spinBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        btn_start = new QPushButton(calculate_control_dialog);
        btn_start->setObjectName(QString::fromUtf8("btn_start"));

        horizontalLayout_2->addWidget(btn_start);

        btn_pause = new QPushButton(calculate_control_dialog);
        btn_pause->setObjectName(QString::fromUtf8("btn_pause"));

        horizontalLayout_2->addWidget(btn_pause);

        btn_stop = new QPushButton(calculate_control_dialog);
        btn_stop->setObjectName(QString::fromUtf8("btn_stop"));

        horizontalLayout_2->addWidget(btn_stop);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(calculate_control_dialog);

        QMetaObject::connectSlotsByName(calculate_control_dialog);
    } // setupUi

    void retranslateUi(QDialog *calculate_control_dialog)
    {
        calculate_control_dialog->setWindowTitle(QCoreApplication::translate("calculate_control_dialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("calculate_control_dialog", "\350\256\241\347\256\227\346\254\241\346\225\260\357\274\232", nullptr));
        btn_start->setText(QCoreApplication::translate("calculate_control_dialog", "\345\274\200\345\247\213", nullptr));
        btn_pause->setText(QCoreApplication::translate("calculate_control_dialog", "\346\232\202\345\201\234", nullptr));
        btn_stop->setText(QCoreApplication::translate("calculate_control_dialog", "\345\201\234\346\255\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class calculate_control_dialog: public Ui_calculate_control_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALCULATE_CONTROL_DIALOG_H
