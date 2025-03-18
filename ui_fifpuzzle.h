/********************************************************************************
** Form generated from reading UI file 'fifpuzzle.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIFPUZZLE_H
#define UI_FIFPUZZLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fifpuzzle
{
public:
    QWidget *centralWidget;
    QPushButton *square7;
    QPushButton *square8;
    QPushButton *square4;
    QPushButton *square10;
    QPushButton *square3;
    QPushButton *square9;
    QPushButton *square2;
    QPushButton *square1;
    QPushButton *square5;
    QPushButton *square6;
    QPushButton *square14;
    QPushButton *square13;
    QPushButton *square12;
    QPushButton *square11;
    QPushButton *square15;
    QPushButton *square16;
    QLCDNumber *timecount;
    QProgressBar *progressbar;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QFrame *line;
    QFrame *frame;
    QFrame *frame_2;
    QLCDNumber *stepcount;
    QFrame *frame_3;
    QFrame *frame_4;
    QLabel *label_6;
    QFrame *line_2;
    QPushButton *solveandshow;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *fifpuzzle)
    {
        if (fifpuzzle->objectName().isEmpty())
            fifpuzzle->setObjectName(QStringLiteral("fifpuzzle"));
        fifpuzzle->resize(1000, 800);
        fifpuzzle->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(fifpuzzle);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        square7 = new QPushButton(centralWidget);
        square7->setObjectName(QStringLiteral("square7"));
        square7->setGeometry(QRect(500, 270, 121, 121));
        QFont font;
        font.setFamily(QStringLiteral("Arial Rounded MT Bold"));
        font.setPointSize(25);
        square7->setFont(font);
        square8 = new QPushButton(centralWidget);
        square8->setObjectName(QStringLiteral("square8"));
        square8->setGeometry(QRect(620, 270, 121, 121));
        square8->setFont(font);
        square4 = new QPushButton(centralWidget);
        square4->setObjectName(QStringLiteral("square4"));
        square4->setGeometry(QRect(620, 150, 121, 121));
        square4->setFont(font);
        square10 = new QPushButton(centralWidget);
        square10->setObjectName(QStringLiteral("square10"));
        square10->setGeometry(QRect(380, 390, 121, 121));
        square10->setFont(font);
        square3 = new QPushButton(centralWidget);
        square3->setObjectName(QStringLiteral("square3"));
        square3->setGeometry(QRect(500, 150, 121, 121));
        square3->setFont(font);
        square9 = new QPushButton(centralWidget);
        square9->setObjectName(QStringLiteral("square9"));
        square9->setGeometry(QRect(260, 390, 121, 121));
        square9->setFont(font);
        square2 = new QPushButton(centralWidget);
        square2->setObjectName(QStringLiteral("square2"));
        square2->setGeometry(QRect(380, 150, 121, 121));
        square2->setFont(font);
        square1 = new QPushButton(centralWidget);
        square1->setObjectName(QStringLiteral("square1"));
        square1->setGeometry(QRect(260, 150, 121, 121));
        square1->setFont(font);
        square5 = new QPushButton(centralWidget);
        square5->setObjectName(QStringLiteral("square5"));
        square5->setGeometry(QRect(260, 270, 121, 121));
        square5->setFont(font);
        square6 = new QPushButton(centralWidget);
        square6->setObjectName(QStringLiteral("square6"));
        square6->setGeometry(QRect(380, 270, 121, 121));
        square6->setFont(font);
        square14 = new QPushButton(centralWidget);
        square14->setObjectName(QStringLiteral("square14"));
        square14->setGeometry(QRect(380, 510, 121, 121));
        square14->setFont(font);
        square13 = new QPushButton(centralWidget);
        square13->setObjectName(QStringLiteral("square13"));
        square13->setGeometry(QRect(260, 510, 121, 121));
        square13->setFont(font);
        square12 = new QPushButton(centralWidget);
        square12->setObjectName(QStringLiteral("square12"));
        square12->setGeometry(QRect(620, 390, 121, 121));
        square12->setFont(font);
        square11 = new QPushButton(centralWidget);
        square11->setObjectName(QStringLiteral("square11"));
        square11->setGeometry(QRect(500, 390, 121, 121));
        square11->setFont(font);
        square15 = new QPushButton(centralWidget);
        square15->setObjectName(QStringLiteral("square15"));
        square15->setGeometry(QRect(500, 510, 121, 121));
        square15->setFont(font);
        square16 = new QPushButton(centralWidget);
        square16->setObjectName(QStringLiteral("square16"));
        square16->setGeometry(QRect(620, 510, 121, 121));
        square16->setFont(font);
        timecount = new QLCDNumber(centralWidget);
        timecount->setObjectName(QStringLiteral("timecount"));
        timecount->setGeometry(QRect(450, 70, 121, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Bodoni MT"));
        timecount->setFont(font1);
        timecount->setStyleSheet(QStringLiteral("coler:red"));
        timecount->setFrameShape(QFrame::NoFrame);
        timecount->setFrameShadow(QFrame::Plain);
        timecount->setSmallDecimalPoint(false);
        timecount->setMode(QLCDNumber::Dec);
        timecount->setSegmentStyle(QLCDNumber::Flat);
        progressbar = new QProgressBar(centralWidget);
        progressbar->setObjectName(QStringLiteral("progressbar"));
        progressbar->setGeometry(QRect(270, 90, 121, 21));
        progressbar->setValue(24);
        progressbar->setTextVisible(false);
        progressbar->setInvertedAppearance(false);
        progressbar->setTextDirection(QProgressBar::TopToBottom);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(280, 40, 121, 31));
        QFont font2;
        font2.setPointSize(9);
        label->setFont(font2);
        label->setStyleSheet(QStringLiteral("color:green"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(450, 40, 121, 31));
        label_2->setFont(font2);
        label_2->setStyleSheet(QStringLiteral("color:red"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(620, 40, 121, 31));
        label_3->setFont(font2);
        label_3->setStyleSheet(QStringLiteral("color:blue"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(620, 60, 121, 31));
        QFont font3;
        font3.setPointSize(12);
        label_4->setFont(font3);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 40, 161, 31));
        label_5->setFont(font2);
        label_5->setAlignment(Qt::AlignCenter);
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(210, 40, 31, 571));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(260, 30, 141, 91));
        frame->setStyleSheet(QStringLiteral("color:green"));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(3);
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(430, 30, 141, 91));
        QFont font4;
        font4.setBold(false);
        font4.setWeight(50);
        frame_2->setFont(font4);
        frame_2->setStyleSheet(QStringLiteral("color:red;"));
        frame_2->setFrameShape(QFrame::WinPanel);
        frame_2->setFrameShadow(QFrame::Plain);
        frame_2->setLineWidth(3);
        stepcount = new QLCDNumber(centralWidget);
        stepcount->setObjectName(QStringLiteral("stepcount"));
        stepcount->setGeometry(QRect(650, 80, 91, 31));
        stepcount->setFont(font1);
        stepcount->setStyleSheet(QStringLiteral("coler:red"));
        stepcount->setFrameShape(QFrame::NoFrame);
        stepcount->setFrameShadow(QFrame::Plain);
        stepcount->setSmallDecimalPoint(false);
        stepcount->setMode(QLCDNumber::Dec);
        stepcount->setSegmentStyle(QLCDNumber::Flat);
        frame_3 = new QFrame(centralWidget);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(600, 30, 141, 91));
        frame_3->setStyleSheet(QStringLiteral("color:blue"));
        frame_3->setFrameShape(QFrame::WinPanel);
        frame_3->setFrameShadow(QFrame::Plain);
        frame_3->setLineWidth(3);
        frame_4 = new QFrame(centralWidget);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(30, 90, 161, 51));
        frame_4->setStyleSheet(QStringLiteral("color:brown"));
        frame_4->setFrameShape(QFrame::WinPanel);
        frame_4->setFrameShadow(QFrame::Plain);
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(0, 10, 161, 31));
        QFont font5;
        font5.setPointSize(10);
        label_6->setFont(font5);
        label_6->setAlignment(Qt::AlignCenter);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(750, 40, 51, 571));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        solveandshow = new QPushButton(centralWidget);
        solveandshow->setObjectName(QStringLiteral("solveandshow"));
        solveandshow->setGeometry(QRect(430, 680, 141, 41));
        fifpuzzle->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(fifpuzzle);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 17));
        fifpuzzle->setMenuBar(menuBar);
        statusBar = new QStatusBar(fifpuzzle);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        fifpuzzle->setStatusBar(statusBar);

        retranslateUi(fifpuzzle);

        QMetaObject::connectSlotsByName(fifpuzzle);
    } // setupUi

    void retranslateUi(QMainWindow *fifpuzzle)
    {
        fifpuzzle->setWindowTitle(QApplication::translate("fifpuzzle", "fifpuzzle", Q_NULLPTR));
        square7->setText(QApplication::translate("fifpuzzle", "7", Q_NULLPTR));
        square8->setText(QApplication::translate("fifpuzzle", "8", Q_NULLPTR));
        square4->setText(QApplication::translate("fifpuzzle", "4", Q_NULLPTR));
        square10->setText(QApplication::translate("fifpuzzle", "10", Q_NULLPTR));
        square3->setText(QApplication::translate("fifpuzzle", "3", Q_NULLPTR));
        square9->setText(QApplication::translate("fifpuzzle", "9", Q_NULLPTR));
        square2->setText(QApplication::translate("fifpuzzle", "2", Q_NULLPTR));
        square1->setText(QApplication::translate("fifpuzzle", "1", Q_NULLPTR));
        square5->setText(QApplication::translate("fifpuzzle", "5", Q_NULLPTR));
        square6->setText(QApplication::translate("fifpuzzle", "6", Q_NULLPTR));
        square14->setText(QApplication::translate("fifpuzzle", "14", Q_NULLPTR));
        square13->setText(QApplication::translate("fifpuzzle", "13", Q_NULLPTR));
        square12->setText(QApplication::translate("fifpuzzle", "12", Q_NULLPTR));
        square11->setText(QApplication::translate("fifpuzzle", "11", Q_NULLPTR));
        square15->setText(QApplication::translate("fifpuzzle", "15", Q_NULLPTR));
        square16->setText(QString());
        label->setText(QApplication::translate("fifpuzzle", "Progress", Q_NULLPTR));
        label_2->setText(QApplication::translate("fifpuzzle", "Time", Q_NULLPTR));
        label_3->setText(QApplication::translate("fifpuzzle", "Step", Q_NULLPTR));
        label_4->setText(QString());
        label_5->setText(QApplication::translate("fifpuzzle", "Playin' as", Q_NULLPTR));
        label_6->setText(QApplication::translate("fifpuzzle", "Jack Lu", Q_NULLPTR));
        solveandshow->setText(QApplication::translate("fifpuzzle", "SolveNShow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class fifpuzzle: public Ui_fifpuzzle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIFPUZZLE_H
