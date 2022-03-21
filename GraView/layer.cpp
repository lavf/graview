/*
 * The Layer class has a relationship of Composition with the "Blob" class. Every Blob object has a
 * Layer object and a Layer object cannot be instantiated without instantiating first the Blob class.
 *
 * A Layer object offers three layers which work independently. The first one displays the objects if they exist,
 * the second one displays the floor and body lines and the third one a heatmap, which is a beta version.
 *
 */

#include "layer.hpp"

#include <QPainter>

// *** Assign colors to object squares as per state recognized by the sensors
Qt::GlobalColor Layer::assignObjColors(unsigned int state) {
    switch (state) {
    case 1:
        return Qt::yellow;
    case 2:
        return Qt::yellow;
    case 3:
        return Qt::blue;
    case 4:
        return Qt::magenta;
    case 5:
        return Qt::white;
    case 10:
        return Qt::green;
    case 26:
        return Qt::darkGreen;
    case 58:
        return Qt::darkRed;
    case 90:
        return Qt::red;
    default:
        return Qt::black;
    }
}

// *** Draw the body and floor lines on the image ***
QPixmap Layer::drawLines(QByteArray subInfoBodyLine, QByteArray subInfoFloorLine) {
    static unsigned int constant = 235;

    QPixmap pixLines(imgWidth, imgHeight);
    pixLines.fill(Qt::transparent);
    QPainter painterLines(&pixLines);

    QColor cobaltBlue(78, 168, 222);

    QPen penLine(cobaltBlue, 3, Qt::SolidLine);
    painterLines.setPen(penLine);

    // ********** Assign points to draw a body line **********
    QPointF pointsBody[64];

    int x = 0;
    int y = 0;

    // Added static cast since the value is not converted by the compiler
    for (int i = 0; i < 64; ++i) {
        if (((unsigned int)(subInfoBodyLine.at(i * 2)) * 3) > constant) {
            x = (unsigned int)(subInfoBodyLine.at(i * 2)) * 3 - constant;
        } else {
            x = ((imgHeight - 75) + ((unsigned int)(subInfoBodyLine.at(i * 2)) * 3));
        }
        pointsBody[i] = QPointF ((pixelSize * i), x);
    }

    painterLines.drawPolyline(pointsBody, 64);

    // ********** Assign points to draw a floor line **********
    QPointF pointsFloor[64];

    for (int i = 0; i < 64; ++i) {
        if (((unsigned int)(subInfoFloorLine.at(i * 2))*3) >= constant) {
            y = ((unsigned int)(subInfoFloorLine.at(i * 2) * 3)  - constant);
        } else {
            y = ((imgHeight - 75) + ((int)(subInfoFloorLine.at(i * 2) * 3)));
        }
        pointsFloor[i] = QPointF ((pixelSize * i), y);
    }

    painterLines.drawPolyline(pointsFloor, 64);
    return pixLines;
}

// *** Draw existing objects recognized by the sensors on the image
QPixmap Layer::drawObjects(QByteArray subInfoObj) {
    static unsigned int objBytes = 12;

    QPixmap pixObject(imgWidth, imgHeight);
    pixObject.fill(Qt::transparent);
    QPainter painterObject(&pixObject);

    unsigned int widthLeft = 0;
    unsigned int widthRight = 0;
    double width = 0;
    unsigned int heightTop = 0;
    unsigned int heightBottom = 0;
    unsigned int state = 0;
    double height = 0;

    QPen pen(Qt::black, 3, Qt::SolidLine);
    pen.setJoinStyle(Qt::MiterJoin);

    for (int j = 0; j < 16; ++j) {
        if (subInfoObj.at(objBytes * j) == 1) {
            widthLeft = subInfoObj.at((objBytes * j) + 4);
            widthRight = subInfoObj.at((objBytes * j) + 5);
            width = widthRight - widthLeft;
            heightTop = subInfoObj.at((objBytes * j) + 6);
            heightBottom = subInfoObj.at((objBytes * j) + 7);
            height = heightBottom - heightTop;

            state = subInfoObj.at((objBytes * j) + 8);

            pen.setColor(assignObjColors(state));
            painterObject.setPen(pen);
            painterObject.drawRect((widthLeft * pixelSize), (heightTop * pixelSize), (width * pixelSize), (height * pixelSize));
        }
    }
    return pixObject;
}

// *** Clean all drawings from the layer ***
QPixmap Layer::eraseDrawings() {
    QPixmap pixClear(imgWidth, imgHeight);
    pixClear.fill(Qt::transparent);
    return pixClear;
}
