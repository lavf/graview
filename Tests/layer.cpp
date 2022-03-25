#include "layer.hpp"

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
