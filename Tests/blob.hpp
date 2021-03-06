#ifndef BLOB_H
#define BLOB_H

#include "image.hpp"
#include "layer.hpp"


class Layer;

#include <QString>


class Blob
{
    friend class Test;
public:
    Blob();
    ~Blob();

    void setIntArraySize(unsigned int x) { intArraySize = x; };
    unsigned int& getIntArraySize() { return intArraySize; };

    void setSubInfoByteArray(QByteArray x) { subInfo.replace(0, x.size(), x); };
    QByteArray& getSubInfoByteArray() { return subInfo; }

    void setInfoByteArray(QByteArray x) { info.replace(0, x.size(), x); };
    QByteArray& getInfoByteArray() { return info; }

    void setSubInfoObjByteArray(QByteArray x) { subInfoObj.replace(0, x.size(), x); };
    QByteArray& getSubInfoObjByteArray() { return subInfoObj; }

    void setSubInfoBodyByteArray(QByteArray x) { subInfoBodyLine.replace(0, x.size(), x); };
    QByteArray& getSubInfoBodyByteArray() { return subInfoBodyLine; }

    void setSubInfoFloorByteArray(QByteArray x) { subInfoFloorLine.replace(0, x.size(), x); };
    QByteArray& getSubInfoFloorByteArray() { return subInfoFloorLine; }

    void setSubInfoSpinByteArray(QByteArray x) { subInfoSpin.replace(0, x.size(), x); };
    QByteArray& getSubInfoSpinByteArray() { return subInfoSpin; }

    void setRewFwdCount(unsigned int x) { rewFwdCount = x; };
    unsigned int& getRewFwdCount() { return rewFwdCount; };

    void setImgCount(unsigned int x) { imgCount = x; };
    unsigned int& getImgCount() { return imgCount; };

    void setBytesIndex(unsigned int x) { bytesIndex = x; };
    unsigned int& getBytesIndex() { return bytesIndex; };

    void setNumberFrames(unsigned int x) { numberFrames = x; };
    unsigned int& getNumberFrames() { return numberFrames; };

    void setHeaderBytes(unsigned int x) { headerBytes = x; };
    unsigned int& getHeaderBytes() { return headerBytes; };

    void setResult(unsigned int x) { result = x; };
    unsigned int& getResult() { return result; };

    void setBytesIndexSpin(unsigned int x) { bytesIndexSpin = x; };
    unsigned int& getBytesIndexSpin() { return bytesIndexSpin; };

    void setRewFwdCountSpin(unsigned int x) { rewFwdCountSpin = x; };
    unsigned int& getRewFwdCountSpin() { return rewFwdCountSpin; };

    void setMaxTempInt(unsigned int x) { maxTempInt = x; };
    int& getMaxTempInt() { return maxTempInt; };

    void setMinTempInt(unsigned int x) { minTempInt = x; };
    int& getMinTempInt() { return minTempInt; };

    void setSpinBoxActivated(unsigned int x) { spinBoxActivated = x; };
    bool& getSpinBoxActivated() { return spinBoxActivated; };

    QByteArray openFile(QString *fileName);
    QByteArray nextImage(QByteArray info);
    QByteArray previousImage(QByteArray info);
    void extractObjArray(QByteArray info);
    void extractLineArray(QByteArray info);
    void getSpinBoxValue();

    unsigned int& getNewLayerArray(unsigned int x);
    void resetLayerArray();


private:
    Layer *const layerObject;

    QByteArray info;
    QByteArray subInfo;
    QByteArray subInfoObj;
    QByteArray subInfoBodyLine;
    QByteArray subInfoFloorLine;
    QByteArray subInfoSpin;

    unsigned int rewFwdCount        = 0;
    unsigned int imgCount           = 1;
    unsigned int bytesIndex         = 0;
    unsigned int numberFrames       = 0;
    unsigned int headerBytes        = 0;
    unsigned int result             = 1;
    unsigned int bytesIndexSpin     = 0;
    unsigned int rewFwdCountSpin    = 0;
    unsigned int intArraySize       = 0;
    int maxTempInt                  = 0;
    int minTempInt                  = 0;

    bool spinBoxActivated = false;

    void calculateNextImageBytes(QByteArray info);
    void calculateNextFrame(unsigned int rewFwd, QByteArray info);
    void calculatePreviousImageBytes(QByteArray info);
    void calculatePreviousFrame(unsigned int rewFwd, QByteArray info);
    void calculateTemperature(QByteArray info, unsigned int rewFwd, unsigned int bytesIndex);


    static constexpr unsigned int quantityImgBytes      = 2048;
    static constexpr unsigned int quantityImgBlckBytes  = 2519;
    static constexpr unsigned int quantityBlckBytes     = 471;
    static constexpr unsigned int difference            = 1;
    static constexpr int          index                 = 512;

};

#endif // BLOB_H
