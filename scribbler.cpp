
#include "scribbler.h"
#include <QtWidgets>

MouseEvent::MouseEvent(int _action, QPointF _pos, quint64 _time)
    :action(_action),pos(_pos),time(_time) { }

MouseEvent::MouseEvent(){}

QString MouseEvent::getActionName(){
    switch (action){
        case 0:
            return QString("Press");
            break;
        case 1:
            return QString("Move");
            break;
        case 2:
            return QString("Release");
            break;
        default:
            return QString("Error");
    }
}

QString MouseEvent::getPositionString(){
    return QString("(%1, %2)").arg(pos.x()).arg(pos.y());
}

QDataStream &operator<<(QDataStream &out, const MouseEvent &evt) {
    return out << evt.action << evt.pos << evt.time;
}

QDataStream &operator>>(QDataStream &in, MouseEvent &evt) {
    return in >> evt.action >> evt.pos >> evt.time;
}

//========== Scribbler
Scribbler::Scribbler()
    :lineWidth(4.0), capturing(false) {
    setScene(&scene);
    setSceneRect(QRectF(0.0, 0.0, 800.0, 600.0));
    setMinimumSize(QSize(400, 300));
    setRenderHint(QPainter::Antialiasing, true);
    setBackgroundBrush(Qt::white);

    scene.addRect(sceneRect());
}


QGraphicsEllipseItem* Scribbler::drawDot(QPointF _p){
    QGraphicsEllipseItem *_currentDot = new QGraphicsEllipseItem(QRectF(_p - QPointF(0.5*lineWidth, 0.5*lineWidth), QSizeF(lineWidth, lineWidth)));
    _currentDot->setBrush(Qt::black);
    _currentDot->setPen(Qt::NoPen);

    scene.addItem(_currentDot);

    return _currentDot;
}

QGraphicsLineItem* Scribbler::drawLine(QPointF _p){
    QGraphicsLineItem *_currentLine = new QGraphicsLineItem(QLineF(lastPoint, _p));
    _currentLine->setPen(QPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap));
    linesToDraw.append(_currentLine);
    scene.addItem(_currentLine);

    return _currentLine;

}


void Scribbler::mousePressEvent(QMouseEvent *evt) {
    QGraphicsView::mousePressEvent(evt);

    QPointF p = mapToScene(evt->pos());
    lastPoint = p;

    QGraphicsEllipseItem *currentDot = drawDot(p);

    if (capturing){
        events << MouseEvent(MouseEvent::Press, p, evt->timestamp());
        tabDots.append(currentDot);
    }
}

void Scribbler::mouseMoveEvent(QMouseEvent *evt) {
    QGraphicsView::mouseMoveEvent(evt);

    QPointF p = mapToScene(evt->pos());

    QGraphicsLineItem *currentLine = drawLine(p);

    if (showDotsOnly){
        currentLine->setVisible(false);
    }


    QGraphicsEllipseItem *currentDot = drawDot(p);

    lastPoint = p;

    if (capturing){
        events << MouseEvent(MouseEvent::Move, p, evt->timestamp());
        tabDots.append(currentDot);
        tabLines.append(currentLine);
    }
}

void Scribbler::mouseReleaseEvent(QMouseEvent *evt) {
    QGraphicsView::mouseReleaseEvent(evt);
    QPointF p = mapToScene(evt->pos());

    if (capturing){
        events << MouseEvent(MouseEvent::Release, p, evt->timestamp());
    }
}



void Scribbler::setDotsOnly(){
    showDotsOnly = true;
    for (int i = 0; i < linesToDraw.length(); ++i){
        linesToDraw[i]->setVisible(false);
    }
}

void Scribbler::setLineSegments(){
    showDotsOnly = false;
    for (int i = 0; i < linesToDraw.length(); ++i){
        linesToDraw[i]->setVisible(true);
    }
}

void Scribbler::clearScribbler(){
    scene.clear();
    events.clear();
    linesToDraw.clear();
    tabDots.clear();
    eventsListList.clear();
    capturing = false;
}

void Scribbler::startCapture(){
    capturing = true;
}




void Scribbler::makeItemGroups(){
    QGraphicsItemGroup *currentGroup = new QGraphicsItemGroup;
    scene.addItem(currentGroup);

    for (int i = 0; i < tabDots.length(); i++){
        currentGroup->addToGroup(tabDots[i]);
    }
    tabDots.clear();

    for (int i = 0; i < tabLines.length(); i++){
        currentGroup->addToGroup(tabLines[i]);
    }
    tabLines.clear();

    itemsByTab.append(currentGroup);
}


//end capture clicked

void Scribbler::sendEventData(){
    if (events.isEmpty()){
        QMessageBox::information(this, "Error", QString("You have yet to start a capture"));
        return;
    }

    makeItemGroups();

    emit emitEventData(events);
    eventsListList.append(events);
    events.clear();
    capturing = false;

}




QList<QList<MouseEvent>>  Scribbler::getEventsListList(){
    return eventsListList;
}


void Scribbler::drawEventsTab(QList<MouseEvent> _eventsList){
    for (int i = 0; i < _eventsList.length(); ++i){  
        if (_eventsList[i].action == 0){
            QPointF p = _eventsList[i].pos;
            lastPoint = p;

            tabDots.append(drawDot(p));
        }

        else if (_eventsList[i].action == 1){
            QPointF p = _eventsList[i].pos;

            QGraphicsLineItem *currentLine = drawLine(p);

            if (showDotsOnly){
                currentLine->setVisible(false);
            }

            tabLines.append(currentLine);

            tabDots.append(drawDot(p));

            lastPoint = p;

        }
    }
    makeItemGroups();
}




void Scribbler::drawLoadedFile(QList<QList<MouseEvent>> _eventsListList){
    clearScribbler();

    for (int i = 0; i < _eventsListList.length(); ++i){
        drawEventsTab(_eventsListList[i]);
    }
}


void Scribbler::changeOpacity(int tab){
    qDebug() << "Changing opacity on all exept: " << tab;
    qDebug() << itemsByTab.length();
    if (itemsByTab.length() != 0){
        itemsByTab[tab]->setOpacity(1);
        for (int i = 0; i < itemsByTab.length(); ++i){
            if (i != tab){
                itemsByTab[i]->setOpacity(0.25);
            }
        }
    }

}






