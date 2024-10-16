#include "mainwindow.h"

#include <QtWidgets>
#include "scribbler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent){

    QWidget *center = new QWidget();
    setCentralWidget(center);

    QHBoxLayout *mainLayout = new QHBoxLayout(center);

    scribbler = new Scribbler();
    mainLayout->addWidget(scribbler);

    table = new QTableWidget();
    mainLayout->addWidget(table);
    table->setHidden(false);

    tab = new QTabWidget();
    mainLayout->addWidget(tab);
    tab->setHidden(false);



    //=============Menue Bar

    //File Menu
    QAction *resetScribbleAct = new QAction("Reset Scribble");
    connect(resetScribbleAct, &QAction::triggered, this, &MainWindow::resetScribbleSlot);
    resetScribbleAct->setShortcut(Qt::CTRL | Qt::Key_R);

    QAction *openFileAct = new QAction("Open File");
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFileSlot);
    openFileAct->setShortcut(Qt::CTRL | Qt::Key_O);

    QAction *saveFileAct = new QAction("Save File");
    connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveFileSlot);
    saveFileAct->setShortcut(Qt::CTRL | Qt::Key_S);


    QMenu *fileMenu = new QMenu("&File");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(resetScribbleAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveFileAct);




    //Capture Menu
    QAction *startCaptureAct = new QAction("Start Capture");
    connect(startCaptureAct, &QAction::triggered, this, &MainWindow::startCaptureSlot);
    startCaptureAct->setShortcut(Qt::CTRL | Qt::Key_C);

    QAction *endCaptureAct = new QAction("End Capture");
    connect(endCaptureAct, &QAction::triggered, this, &MainWindow::endCaptureSlot);
    endCaptureAct->setShortcut(Qt::CTRL | Qt::Key_E);


    QMenu *captureMenu = new QMenu("&Capture");
    menuBar()->addMenu(captureMenu);
    captureMenu->addAction(startCaptureAct);
    captureMenu->addAction(endCaptureAct);




    //View Menu

    QAction *lineSegmentsAct = new QAction("Lines Only");
    connect(lineSegmentsAct, &QAction::triggered, this, &MainWindow::lineSegmentsSlot);
    lineSegmentsAct->setShortcut(Qt::CTRL | Qt::Key_L);

    QAction *dotsOnlyAct = new QAction("Dots Only");
    connect(dotsOnlyAct, &QAction::triggered, this, &MainWindow::dotsOnlySlot);
    dotsOnlyAct->setShortcut(Qt::CTRL | Qt::Key_D);

    QMenu *viewMenu = new QMenu("&View");
    menuBar()->addMenu(viewMenu);
    viewMenu->addAction(lineSegmentsAct);
    viewMenu->addAction(dotsOnlyAct);












}

MainWindow::~MainWindow() {}

//File Menu
void MainWindow::resetScribbleSlot(){
    scribbler->clearScribbler();
    table->clear();
    table->setHidden(true);
    tab->clear();
    tab->setHidden(true);
}

void MainWindow::openFileSlot(){

}


void MainWindow::saveFileSlot(){

}


//capture Menu
void MainWindow::startCaptureSlot(){

}


void MainWindow::endCaptureSlot(){
    //table->setHidden(false);
    //table->setColumnCount(3);
    //table->setRowCount(scribbler->events.length());
    //qDebug() << scribbler->events.length();



}


//View Menu
void MainWindow::lineSegmentsSlot(){
    scribbler->setLineSegments();
}


void MainWindow::dotsOnlySlot(){
    scribbler->setDotsOnly();
}































