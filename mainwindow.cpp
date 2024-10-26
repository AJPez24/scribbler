#include "mainwindow.h"

#include "scribbler.h"
#include <QtWidgets>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tabCount(0){

    QWidget *center = new QWidget();
    setCentralWidget(center);

    QHBoxLayout *mainLayout = new QHBoxLayout(center);

    scribbler = new Scribbler();
    mainLayout->addWidget(scribbler, 1);


    tab = new QTabWidget();
    mainLayout->addWidget(tab, 1);
    tab->setHidden(true);


    connect(tab, &QTabWidget::currentChanged, this, &MainWindow::setSelectedTab);

    QSettings settings("Pezza Productions", "Scribbler Midterm");
    lastDir = settings.value("lastDir", "").toString();



    //=============Menue Bar

    //File Menu
    QAction *resetScribbleAct = new QAction("Reset Scribble");
    connect(resetScribbleAct, &QAction::triggered, this, &MainWindow::resetMainSlot);
    connect(resetScribbleAct, &QAction::triggered, scribbler, &Scribbler::clearScribbler);
    resetScribbleAct->setShortcut(Qt::CTRL | Qt::Key_R);


    QAction *saveFileAct = new QAction("Save File");
    connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveFileSlot);
    saveFileAct->setShortcut(Qt::CTRL | Qt::Key_S);


    QAction *openFileAct = new QAction("Open File");
    connect(openFileAct, &QAction::triggered, scribbler, &Scribbler::clearScribbler);
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFileSlot);
    openFileAct->setShortcut(Qt::CTRL | Qt::Key_O);



    QMenu *fileMenu = new QMenu("&File");
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(resetScribbleAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveFileAct);




    //Capture Menu
    QAction *startCaptureAct = new QAction("Start Capture");
    connect(startCaptureAct, &QAction::triggered, scribbler, &Scribbler::startCapture);
    startCaptureAct->setShortcut(Qt::CTRL | Qt::Key_C);

    QAction *endCaptureAct = new QAction("End Capture");
    connect(endCaptureAct, &QAction::triggered, scribbler, &Scribbler::sendEventData);
    endCaptureAct->setShortcut(Qt::CTRL | Qt::Key_E);

    connect(scribbler, &Scribbler::emitEventData, this, &MainWindow::addTab);

    QMenu *captureMenu = new QMenu("&Capture");
    menuBar()->addMenu(captureMenu);
    captureMenu->addAction(startCaptureAct);
    captureMenu->addAction(endCaptureAct);




    //View Menu

    QAction *lineSegmentsAct = new QAction("Lines Only");
    connect(lineSegmentsAct, &QAction::triggered, scribbler, &Scribbler::setLineSegments);
    lineSegmentsAct->setShortcut(Qt::CTRL | Qt::Key_L);

    QAction *dotsOnlyAct = new QAction("Dots Only");
    connect(dotsOnlyAct, &QAction::triggered, scribbler, &Scribbler::setDotsOnly);
    dotsOnlyAct->setShortcut(Qt::CTRL | Qt::Key_D);

    QMenu *viewMenu = new QMenu("&View");
    menuBar()->addMenu(viewMenu);
    viewMenu->addAction(lineSegmentsAct);
    viewMenu->addAction(dotsOnlyAct);




}

MainWindow::~MainWindow() {
    QSettings settings("Pezza Productions", "Scribbler Midterm");
    settings.setValue("lastDir", lastDir);
}

//File Menu
void MainWindow::resetMainSlot(){
    tab->clear();
    tab->setHidden(true);
    tabCount = 0;
}



void MainWindow::addTab(QList<MouseEvent> _eventsList){
    QTableWidget *currentTable = new QTableWidget();
    tab->addTab(currentTable, QString("Tab %1").arg(++tabCount));
    tab->setHidden(false);
    tab->setCurrentIndex(tab->count() - 1);

    currentTable->setColumnCount(4);
    currentTable->setRowCount(_eventsList.length());

    QStringList headers;
    headers << "Action" << "Position" << "Time" << "Distance";
    currentTable->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < _eventsList.length(); ++i){
        QTableWidgetItem *nameItem = new QTableWidgetItem();
        nameItem->setData(Qt::DisplayRole, _eventsList[i].getActionName());
        currentTable->setItem(i, 0, nameItem);

        QTableWidgetItem *positionItem = new QTableWidgetItem();
        positionItem->setData(Qt::DisplayRole, _eventsList[i].getPositionString());
        currentTable->setItem(i, 1, positionItem);

        QTableWidgetItem *timeItem = new QTableWidgetItem();
        timeItem->setData(Qt::DisplayRole, _eventsList[i].time);
        currentTable->setItem(i, 2, timeItem);

        QTableWidgetItem *distanceItem = new QTableWidgetItem();
        distanceItem->setData(Qt::DisplayRole, _eventsList[i].distanceToLast);
        currentTable->setItem(i, 3, distanceItem);
    }

    setSelectedTab();

}


void MainWindow::saveFileSlot(){
    QString outName = QFileDialog::getSaveFileName(this, "Save", lastDir);

    QFile outFile(outName);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QMessageBox::information(this, "Error", QString("Can't write to file \"%1\"").arg(outName));
        return;
    }

    QDataStream out (&outFile);

    QList<QList<MouseEvent>> _eventsListList;
    _eventsListList = scribbler->getEventsListList();

    out << _eventsListList;



}


void MainWindow::openFileSlot(){
    resetMainSlot();

    loadFileName = QFileDialog::getOpenFileName(this, "Open File", lastDir);
    QFile inFile(loadFileName);
    if (loadFileName.isEmpty()) return;

    lastDir = QFileInfo(loadFileName).absolutePath();


    if(!inFile.open(QIODevice::ReadOnly)){
        QMessageBox::information(this, "Error", QString("Can't open file \"%1\"").arg(loadFileName));
        return;
    }

    else{

        QDataStream in (&inFile);

        QList<QList<MouseEvent>> inListList;

        in >> inListList;

        for (int i = 0; i < inListList.length(); ++i){
            addTab(inListList[i]);
        }
        scribbler->drawLoadedFile(inListList);
    }
}

void MainWindow::setSelectedTab(){
    selectedTab = tab->currentIndex();
    qDebug() << selectedTab;
    scribbler->changeOpacity(selectedTab);
}

int MainWindow::getSelectedTab(){
    return selectedTab;
}



































