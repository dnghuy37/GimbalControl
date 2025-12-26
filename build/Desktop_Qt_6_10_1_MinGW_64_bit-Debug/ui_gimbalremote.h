/********************************************************************************
** Form generated from reading UI file 'gimbalremote.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GIMBALREMOTE_H
#define UI_GIMBALREMOTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GimbalRemote
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *zoomIn;
    QPushButton *zoomOut;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QSlider *sliderRoll;
    QSlider *sliderYaw;
    QVBoxLayout *verticalLayout_5;
    QPushButton *rollUp;
    QPushButton *rollDown;
    QSlider *sliderPitch;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pitchUp;
    QPushButton *pitchDown;
    QVBoxLayout *verticalLayout_4;
    QPushButton *yawUp;
    QPushButton *yawDown;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QPushButton *btnTrack;
    QGridLayout *gridLayout_3;
    QLineEdit *editLastX;
    QLineEdit *editLastY;
    QPushButton *goTo;
    QPushButton *btnNeutral;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_7;
    QLineEdit *relaRoll;
    QLineEdit *relaPitch;
    QLineEdit *relaYaw;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *absoRoll;
    QLineEdit *absoPitch;
    QLineEdit *absoYaw;
    QVBoxLayout *verticalLayout_8;
    QLineEdit *absoVelRoll;
    QLineEdit *absoVelPitch;
    QLineEdit *absoVelYaw;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;
    QPlainTextEdit *plainTextEdit_3;
    QPushButton *btnFocus;
    QPushButton *goToB;

    void setupUi(QWidget *GimbalRemote)
    {
        if (GimbalRemote->objectName().isEmpty())
            GimbalRemote->setObjectName("GimbalRemote");
        GimbalRemote->resize(942, 558);
        verticalLayoutWidget = new QWidget(GimbalRemote);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(510, 60, 95, 67));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        zoomIn = new QPushButton(verticalLayoutWidget);
        zoomIn->setObjectName("zoomIn");

        verticalLayout->addWidget(zoomIn);

        zoomOut = new QPushButton(verticalLayoutWidget);
        zoomOut->setObjectName("zoomOut");

        verticalLayout->addWidget(zoomOut);

        gridLayoutWidget = new QWidget(GimbalRemote);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(30, 60, 441, 251));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        sliderRoll = new QSlider(gridLayoutWidget);
        sliderRoll->setObjectName("sliderRoll");
        sliderRoll->setMinimum(-1000);
        sliderRoll->setMaximum(1000);
        sliderRoll->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(sliderRoll, 0, 1, 1, 1);

        sliderYaw = new QSlider(gridLayoutWidget);
        sliderYaw->setObjectName("sliderYaw");
        sliderYaw->setMinimum(-1000);
        sliderYaw->setMaximum(1000);
        sliderYaw->setSingleStep(1);
        sliderYaw->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(sliderYaw, 2, 1, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        rollUp = new QPushButton(gridLayoutWidget);
        rollUp->setObjectName("rollUp");

        verticalLayout_5->addWidget(rollUp);

        rollDown = new QPushButton(gridLayoutWidget);
        rollDown->setObjectName("rollDown");

        verticalLayout_5->addWidget(rollDown);


        gridLayout_2->addLayout(verticalLayout_5, 0, 0, 1, 1);

        sliderPitch = new QSlider(gridLayoutWidget);
        sliderPitch->setObjectName("sliderPitch");
        sliderPitch->setMinimum(-1000);
        sliderPitch->setMaximum(1000);
        sliderPitch->setSingleStep(1);
        sliderPitch->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(sliderPitch, 1, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        pitchUp = new QPushButton(gridLayoutWidget);
        pitchUp->setObjectName("pitchUp");

        verticalLayout_3->addWidget(pitchUp);

        pitchDown = new QPushButton(gridLayoutWidget);
        pitchDown->setObjectName("pitchDown");

        verticalLayout_3->addWidget(pitchDown);


        gridLayout_2->addLayout(verticalLayout_3, 1, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        yawUp = new QPushButton(gridLayoutWidget);
        yawUp->setObjectName("yawUp");

        verticalLayout_4->addWidget(yawUp);

        yawDown = new QPushButton(gridLayoutWidget);
        yawDown->setObjectName("yawDown");

        verticalLayout_4->addWidget(yawDown);


        gridLayout_2->addLayout(verticalLayout_4, 2, 0, 1, 1);

        horizontalLayoutWidget = new QWidget(GimbalRemote);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(510, 160, 265, 91));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        btnTrack = new QPushButton(horizontalLayoutWidget);
        btnTrack->setObjectName("btnTrack");

        verticalLayout_6->addWidget(btnTrack);


        horizontalLayout_2->addLayout(verticalLayout_6);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        editLastX = new QLineEdit(horizontalLayoutWidget);
        editLastX->setObjectName("editLastX");
        editLastX->setEnabled(true);
        editLastX->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);

        gridLayout_3->addWidget(editLastX, 0, 1, 1, 1);

        editLastY = new QLineEdit(horizontalLayoutWidget);
        editLastY->setObjectName("editLastY");

        gridLayout_3->addWidget(editLastY, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_3);

        goTo = new QPushButton(GimbalRemote);
        goTo->setObjectName("goTo");
        goTo->setGeometry(QRect(790, 50, 131, 29));
        btnNeutral = new QPushButton(GimbalRemote);
        btnNeutral->setObjectName("btnNeutral");
        btnNeutral->setGeometry(QRect(630, 60, 93, 29));
        horizontalLayoutWidget_2 = new QWidget(GimbalRemote);
        horizontalLayoutWidget_2->setObjectName("horizontalLayoutWidget_2");
        horizontalLayoutWidget_2->setGeometry(QRect(30, 380, 411, 151));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName("verticalLayout_7");
        relaRoll = new QLineEdit(horizontalLayoutWidget_2);
        relaRoll->setObjectName("relaRoll");

        verticalLayout_7->addWidget(relaRoll);

        relaPitch = new QLineEdit(horizontalLayoutWidget_2);
        relaPitch->setObjectName("relaPitch");

        verticalLayout_7->addWidget(relaPitch);

        relaYaw = new QLineEdit(horizontalLayoutWidget_2);
        relaYaw->setObjectName("relaYaw");

        verticalLayout_7->addWidget(relaYaw);


        horizontalLayout->addLayout(verticalLayout_7);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        absoRoll = new QLineEdit(horizontalLayoutWidget_2);
        absoRoll->setObjectName("absoRoll");

        verticalLayout_2->addWidget(absoRoll);

        absoPitch = new QLineEdit(horizontalLayoutWidget_2);
        absoPitch->setObjectName("absoPitch");

        verticalLayout_2->addWidget(absoPitch);

        absoYaw = new QLineEdit(horizontalLayoutWidget_2);
        absoYaw->setObjectName("absoYaw");

        verticalLayout_2->addWidget(absoYaw);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName("verticalLayout_8");
        absoVelRoll = new QLineEdit(horizontalLayoutWidget_2);
        absoVelRoll->setObjectName("absoVelRoll");

        verticalLayout_8->addWidget(absoVelRoll);

        absoVelPitch = new QLineEdit(horizontalLayoutWidget_2);
        absoVelPitch->setObjectName("absoVelPitch");

        verticalLayout_8->addWidget(absoVelPitch);

        absoVelYaw = new QLineEdit(horizontalLayoutWidget_2);
        absoVelYaw->setObjectName("absoVelYaw");

        verticalLayout_8->addWidget(absoVelYaw);


        horizontalLayout->addLayout(verticalLayout_8);

        plainTextEdit = new QPlainTextEdit(GimbalRemote);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(60, 340, 71, 31));
        plainTextEdit_2 = new QPlainTextEdit(GimbalRemote);
        plainTextEdit_2->setObjectName("plainTextEdit_2");
        plainTextEdit_2->setGeometry(QRect(200, 340, 71, 31));
        plainTextEdit_2->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        plainTextEdit_3 = new QPlainTextEdit(GimbalRemote);
        plainTextEdit_3->setObjectName("plainTextEdit_3");
        plainTextEdit_3->setGeometry(QRect(340, 340, 71, 31));
        btnFocus = new QPushButton(GimbalRemote);
        btnFocus->setObjectName("btnFocus");
        btnFocus->setGeometry(QRect(650, 320, 93, 29));
        goToB = new QPushButton(GimbalRemote);
        goToB->setObjectName("goToB");
        goToB->setGeometry(QRect(790, 90, 131, 31));

        retranslateUi(GimbalRemote);

        QMetaObject::connectSlotsByName(GimbalRemote);
    } // setupUi

    void retranslateUi(QWidget *GimbalRemote)
    {
        GimbalRemote->setWindowTitle(QCoreApplication::translate("GimbalRemote", "Form", nullptr));
        zoomIn->setText(QCoreApplication::translate("GimbalRemote", "Zoom In", nullptr));
        zoomOut->setText(QCoreApplication::translate("GimbalRemote", "Zoom Out", nullptr));
        rollUp->setText(QCoreApplication::translate("GimbalRemote", "Roll Up", nullptr));
        rollDown->setText(QCoreApplication::translate("GimbalRemote", "Roll Down", nullptr));
        pitchUp->setText(QCoreApplication::translate("GimbalRemote", "Pitch Up", nullptr));
        pitchDown->setText(QCoreApplication::translate("GimbalRemote", "Pitch Down", nullptr));
        yawUp->setText(QCoreApplication::translate("GimbalRemote", "Yaw Up", nullptr));
        yawDown->setText(QCoreApplication::translate("GimbalRemote", "Yaw Down", nullptr));
        btnTrack->setText(QCoreApplication::translate("GimbalRemote", "Track", nullptr));
        goTo->setText(QCoreApplication::translate("GimbalRemote", "Go to Point A", nullptr));
        btnNeutral->setText(QCoreApplication::translate("GimbalRemote", "Neutral", nullptr));
        plainTextEdit->setPlainText(QCoreApplication::translate("GimbalRemote", "Relative", nullptr));
        plainTextEdit_2->setPlainText(QCoreApplication::translate("GimbalRemote", "Absolute", nullptr));
        plainTextEdit_3->setPlainText(QCoreApplication::translate("GimbalRemote", "Velocity", nullptr));
        btnFocus->setText(QCoreApplication::translate("GimbalRemote", "Focus", nullptr));
        goToB->setText(QCoreApplication::translate("GimbalRemote", "Go to Point B", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GimbalRemote: public Ui_GimbalRemote {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GIMBALREMOTE_H
