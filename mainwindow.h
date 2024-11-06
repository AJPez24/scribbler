#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>

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

    QString lastDir;
    QString loadFileName;

    QTableWidget makeTable(QList<MouseEvent> _eventsList);
    void setSelectedTab();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int getSelectedTab();

public slots:
    void resetMainSlot();

    void saveFileSlot();
    void openFileSlot();

    void addTab(QList<MouseEvent> _eventsList);
};

#endif // MAINWINDOW_H
