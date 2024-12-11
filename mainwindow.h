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
    QTabWidget *tab;
    int tabCount;
    int selectedTab;
    QList<QTableWidget*> tables;

    QString lastDir;
    QString loadFileName;

    QTableWidget makeTable(QList<MouseEvent> _eventsList);
    void setSelectedTab();
    void changeSelectedRows();

    QList<QList<MouseEvent>> eventsListList;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int getSelectedTab();


public slots:
    void resetMainSlot();

    void saveFileSlot();
    void openFileSlot();

    void addTab(QList<MouseEvent> _eventsList);
    void newCaptureData(QList<MouseEvent> _eventsList);
};

#endif // MAINWINDOW_H
