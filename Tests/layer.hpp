#ifndef LAYER_H
#define LAYER_H

#include "blob.hpp"

class Layer
{
    friend class Test;
public:

    void setNewLayerArray(unsigned int tempLayerArray[104]) {
        for ( unsigned int i = 0; i < 104; ++i ) {
            newLayerArray[i] = tempLayerArray[i];
        }
    };

    unsigned int& getNewLayerArray(unsigned int x) { return newLayerArray[x]; };

    void resetLayerArray() {
        for ( unsigned int i = 0; i < 104; ++i ) {
            newLayerArray[i] = 0;
            tempLayerArray[i] = 0;
        }
    };

private:
    static constexpr double pixelSize       = 18.95;
    static constexpr unsigned int imgHeight = 600;
    static constexpr unsigned int imgWidth  = 1200;

    Qt::GlobalColor assignObjColors(unsigned int state);

    unsigned int newLayerArray[104]      = {0};
    unsigned int tempLayerArray[104]     = {0};
};

#endif // LAYER_H

