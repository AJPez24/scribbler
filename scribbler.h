
#ifndef SCRIBBLER_H
#define SCRIBBLER_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QString>
#include <QWidget>

class MouseEvent
{
public:
    enum { Press, Move, Release };

    int action;
    QPointF pos;
    quint64 time;
    double distanceToLast;

    QGraphicsLineItem *line;
    QGraphicsEllipseItem *dot;

    MouseEvent();
    MouseEvent(int _action, QPointF _pos, quint64 _time, double _distanceToLast, QGraphicsLineItem *_line, QGraphicsEllipseItem *_dot);

    QString getActionName();
    QString getPositionString();

    friend QDataStream &operator<<(QDataStream &out, const MouseEvent &evt);
    friend QDataStream &operator>>(QDataStream &out, MouseEvent &evt);
};

class Scribbler : public QGraphicsView
{
    QGraphicsScene scene;
    double lineWidth;
    QPointF lastPoint;
    bool showDotsOnly;
    bool capturing;

    QList<MouseEvent> events;

    QList<QGraphicsLineItem *> linesToDraw;

    QList<QGraphicsEllipseItem *> tabDots;
    QList<QGraphicsLineItem *> tabLines;

    QList<QGraphicsItemGroup *> itemsByTab;

    QGraphicsEllipseItem* drawDot(QPointF _p);
    QGraphicsLineItem* drawLine(QPointF _p);

    void makeItemGroups();

    Q_OBJECT
public:
    Scribbler();

    void drawEventsTab(QList<MouseEvent> &_eventsList);

    void changeOpacity(int tab);

protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

public slots:
    void startCapture();
    void sendEventData();

    void setDotsOnly();
    void setLineSegments();
    void clearScribbler();

signals:
    void emitEventData(QList<MouseEvent> &mouseEventsList);
};

#endif // SCRIBBLER_H
