#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "scribbler.h"

class QTabWidget;
class QTableWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT

    Scribbler *scribbler;
    QTableWidget *table;
    QTabWidget *tab;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void resetScribbleSlot();
    void openFileSlot();
    void saveFileSlot();

    void startCaptureSlot();
    void endCaptureSlot();

    void lineSegmentsSlot();
    void dotsOnlySlot();

};

#endif // MAINWINDOW_H
