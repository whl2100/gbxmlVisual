/********************************************************************************
** Form generated from reading UI file 'test.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_H
#define UI_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_testClass
{
public:
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QPushButton *m_ButtonZoomIn;
    QPushButton *m_ButtonZoomOut;
    QPushButton *m_ButtonLoad_gbXML;
    QLineEdit *GoToFrameLineEdit;
    QPushButton *GoToFrameButton;
    QPushButton *loadKinectDataButton;
    QPushButton *DrawFurnitureButton;
    QPushButton *draw_all_button;
    QLineEdit *draw_all_start;
    QLineEdit *draw_all_end;
    QPushButton *clear_button;
    QPushButton *load_odometry_button;
    QPushButton *load_sonar_data_button;
    QPushButton *generate_2d_button;
    QPushButton *track_button;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *testClass)
    {
        if (testClass->objectName().isEmpty())
            testClass->setObjectName(QStringLiteral("testClass"));
        testClass->resize(904, 588);
        centralWidget = new QWidget(testClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(300, 10, 571, 511));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMinimumSize(QSize(0, 0));
        graphicsView->setLineWidth(1);
        graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(0, 0, 291, 81));
        gridLayout_4 = new QGridLayout(groupBox_7);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        m_ButtonZoomIn = new QPushButton(groupBox_7);
        m_ButtonZoomIn->setObjectName(QStringLiteral("m_ButtonZoomIn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_ButtonZoomIn->sizePolicy().hasHeightForWidth());
        m_ButtonZoomIn->setSizePolicy(sizePolicy1);
        m_ButtonZoomIn->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ZoomIn.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_ButtonZoomIn->setIcon(icon);
        m_ButtonZoomIn->setIconSize(QSize(24, 24));

        gridLayout_3->addWidget(m_ButtonZoomIn, 0, 3, 1, 1);

        m_ButtonZoomOut = new QPushButton(groupBox_7);
        m_ButtonZoomOut->setObjectName(QStringLiteral("m_ButtonZoomOut"));
        sizePolicy1.setHeightForWidth(m_ButtonZoomOut->sizePolicy().hasHeightForWidth());
        m_ButtonZoomOut->setSizePolicy(sizePolicy1);
        m_ButtonZoomOut->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/ZoomOut.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_ButtonZoomOut->setIcon(icon1);
        m_ButtonZoomOut->setIconSize(QSize(24, 24));

        gridLayout_3->addWidget(m_ButtonZoomOut, 0, 5, 1, 1);

        m_ButtonLoad_gbXML = new QPushButton(groupBox_7);
        m_ButtonLoad_gbXML->setObjectName(QStringLiteral("m_ButtonLoad_gbXML"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_ButtonLoad_gbXML->sizePolicy().hasHeightForWidth());
        m_ButtonLoad_gbXML->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(m_ButtonLoad_gbXML, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 1, 1, 1);

        GoToFrameLineEdit = new QLineEdit(centralWidget);
        GoToFrameLineEdit->setObjectName(QStringLiteral("GoToFrameLineEdit"));
        GoToFrameLineEdit->setGeometry(QRect(130, 380, 111, 20));
        GoToFrameButton = new QPushButton(centralWidget);
        GoToFrameButton->setObjectName(QStringLiteral("GoToFrameButton"));
        GoToFrameButton->setGeometry(QRect(10, 380, 81, 21));
        loadKinectDataButton = new QPushButton(centralWidget);
        loadKinectDataButton->setObjectName(QStringLiteral("loadKinectDataButton"));
        loadKinectDataButton->setGeometry(QRect(180, 120, 101, 31));
        DrawFurnitureButton = new QPushButton(centralWidget);
        DrawFurnitureButton->setObjectName(QStringLiteral("DrawFurnitureButton"));
        DrawFurnitureButton->setGeometry(QRect(180, 160, 101, 31));
        draw_all_button = new QPushButton(centralWidget);
        draw_all_button->setObjectName(QStringLiteral("draw_all_button"));
        draw_all_button->setGeometry(QRect(10, 480, 81, 23));
        draw_all_start = new QLineEdit(centralWidget);
        draw_all_start->setObjectName(QStringLiteral("draw_all_start"));
        draw_all_start->setGeometry(QRect(130, 460, 113, 20));
        draw_all_end = new QLineEdit(centralWidget);
        draw_all_end->setObjectName(QStringLiteral("draw_all_end"));
        draw_all_end->setGeometry(QRect(130, 490, 113, 20));
        clear_button = new QPushButton(centralWidget);
        clear_button->setObjectName(QStringLiteral("clear_button"));
        clear_button->setGeometry(QRect(180, 200, 101, 31));
        load_odometry_button = new QPushButton(centralWidget);
        load_odometry_button->setObjectName(QStringLiteral("load_odometry_button"));
        load_odometry_button->setGeometry(QRect(10, 120, 111, 23));
        load_sonar_data_button = new QPushButton(centralWidget);
        load_sonar_data_button->setObjectName(QStringLiteral("load_sonar_data_button"));
        load_sonar_data_button->setGeometry(QRect(10, 150, 111, 23));
        generate_2d_button = new QPushButton(centralWidget);
        generate_2d_button->setObjectName(QStringLiteral("generate_2d_button"));
        generate_2d_button->setGeometry(QRect(180, 240, 101, 31));
        track_button = new QPushButton(centralWidget);
        track_button->setObjectName(QStringLiteral("track_button"));
        track_button->setGeometry(QRect(10, 190, 75, 23));
        testClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(testClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 904, 21));
        testClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(testClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        testClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(testClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        testClass->setStatusBar(statusBar);

        retranslateUi(testClass);

        QMetaObject::connectSlotsByName(testClass);
    } // setupUi

    void retranslateUi(QMainWindow *testClass)
    {
        testClass->setWindowTitle(QApplication::translate("testClass", "test", 0));
        groupBox_7->setTitle(QApplication::translate("testClass", "Scene", 0));
        m_ButtonZoomIn->setText(QApplication::translate("testClass", "Load CSV", 0));
        m_ButtonZoomOut->setText(QApplication::translate("testClass", "Run", 0));
        m_ButtonLoad_gbXML->setText(QApplication::translate("testClass", "Load", 0));
        GoToFrameLineEdit->setText(QApplication::translate("testClass", "Go to frame", 0));
        GoToFrameButton->setText(QApplication::translate("testClass", "Go", 0));
        loadKinectDataButton->setText(QApplication::translate("testClass", "Load Kinect Data", 0));
        DrawFurnitureButton->setText(QApplication::translate("testClass", "Draw Furniture", 0));
        draw_all_button->setText(QApplication::translate("testClass", "Draw all Data", 0));
        draw_all_start->setText(QApplication::translate("testClass", "start", 0));
        draw_all_end->setText(QApplication::translate("testClass", "end", 0));
        clear_button->setText(QApplication::translate("testClass", "Clear", 0));
        load_odometry_button->setText(QApplication::translate("testClass", "Load Odometry", 0));
        load_sonar_data_button->setText(QApplication::translate("testClass", "Load Sonar Returns", 0));
        generate_2d_button->setText(QApplication::translate("testClass", "Generate 2D points", 0));
        track_button->setText(QApplication::translate("testClass", "Track", 0));
    } // retranslateUi

};

namespace Ui {
    class testClass: public Ui_testClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_H
