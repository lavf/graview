#ifndef LAYER_H
#define LAYER_H

#include <QPixmap>


class Layer
{
public:
    // Default constructor and destructor provided by compiler implicitly
    QPixmap drawLines(QByteArray subInfoBodyLine, QByteArray subInfoFloorLine);
    QPixmap drawObjects(QByteArray subInfoObj);
    QPixmap eraseDrawings();

private:
    static constexpr double       pixelSize = 18.95;
    static constexpr unsigned int imgHeight = 600;
    static constexpr unsigned int imgWidth  = 1200;

    Qt::GlobalColor assignObjColors(unsigned int state);
};

#endif // LAYER_H
