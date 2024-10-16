
#ifndef SCRIBBLER_H
#define SCRIBBLER_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>


class MouseEvent {
public:
    enum {
        Press,
        Move,
        Release
    };

    int action;
    QPointF pos;
    quint64 time;

    MouseEvent(int _action, QPointF _pos, quint64 _time);

    friend QDataStream &operator<<(QDataStream &out, const MouseEvent &evt);
    friend QDataStream &operator>>(QDataStream &out, MouseEvent &evt);
};

class Scribbler : public QGraphicsView
{
    QGraphicsScene scene;
    double lineWidth;
    QPointF lastPoint;
    bool showDotsOnly;


    QList<MouseEvent> events;
    //QList<QGraphicsLineItem> lines;
    //QList<QGraphicsEllipseItem> dots;

    Q_OBJECT
public:
    Scribbler();

    void setDotsOnly();
    void setLineSegments();
    void clearScribbler();

protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

    void sendEventData();
};

#endif // SCRIBBLER_H
