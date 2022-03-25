#include "blob.hpp"

#include <QDebug>
#include <QFile>


Blob::Blob()
    : layerObject(new Layer) {
}


void Blob::calculateNextFrame(unsigned int rewFwd, QByteArray info) {
    if (rewFwd == 0)
        setBytesIndex(getHeaderBytes() + quantityImgBlckBytes);
    else if (rewFwd == getHeaderBytes())
        setBytesIndex(rewFwd + quantityImgBlckBytes);
    else if (rewFwd == (getIntArraySize() - quantityImgBytes - difference) || rewFwd == (getIntArraySize() - quantityImgBytes))
        setBytesIndex(getHeaderBytes());
    else
        setBytesIndex(rewFwd + quantityImgBlckBytes);

    calculateTemperature(info, rewFwd, getBytesIndex());
    calculateNextImageBytes(info);

    if (spinBoxActivated == false) {
        if(getImgCount() == getNumberFrames())
            setImgCount(1);
        else
            setImgCount(getImgCount() + 1);

        setResult(getImgCount());

    } else {
        if (getResult() == getNumberFrames())
            setResult(1);
        else
            setResult(getResult() + 1);

        setImgCount(getResult());
    }

    if (rewFwd == getIntArraySize() - quantityImgBytes || rewFwd == getIntArraySize() - quantityImgBytes - difference)
        setRewFwdCount(0);
    else if (rewFwd == 0)
        setRewFwdCount(getHeaderBytes() + quantityImgBlckBytes);
    else if(rewFwd == getHeaderBytes() && spinBoxActivated == false)
        setRewFwdCount(rewFwd + quantityImgBlckBytes);
    else
        setRewFwdCount(getBytesIndex());
}

void Blob::calculateNextImageBytes(QByteArray info) {
    QByteArray blockInfo;

    // ********** Calculate block of the next frame **********
    blockInfo = info.mid((getBytesIndex() - quantityBlckBytes), 1024);

    // --- Check if block has errors or not ---
    // ----- Check if the first byte is the magic number 123 -----
    if (blockInfo.at(0) == 123) {
        if (blockInfo.at(index) == blockInfo.at(0) && blockInfo.at(index + 1) == blockInfo.at(1)) {
            setBytesIndex(getBytesIndex() + index);
        }
        setSubInfoByteArray(info.mid(getBytesIndex(), quantityImgBytes));
    } else {
        // --- Special case: File "record_0", double header: first with 512 bytes ---
        if (blockInfo.at(index) == 123) {
            if ((int)blockInfo.at((index * 2) - 1) == -117) {
                setBytesIndex(getBytesIndex() + (index * 2));
            } else {
                setBytesIndex(getBytesIndex() + index);
            }
            setSubInfoByteArray(info.mid(getBytesIndex(), quantityImgBytes));
        } else {
            if ((int)blockInfo.at((index * 2) - 1) == -114) {
                setBytesIndex(getBytesIndex() + (index * 3));
            } else if (blockInfo.at((index * 2) - 1) == 110) {
                setBytesIndex(getBytesIndex() + index);
            } else {
                setBytesIndex(getBytesIndex() + quantityImgBytes);
            }
            setSubInfoByteArray(info.mid(getBytesIndex(), quantityImgBytes));
        }
    }
}

void Blob::calculatePreviousImageBytes(QByteArray info) {
    QByteArray blockInfo;
    // *** Calculate block of the previous frame ***
    // ***** A sensor block has 1024 bytes. Every frame consists of two sensor blocks. *****
    blockInfo = info.mid((getBytesIndex() - quantityBlckBytes), 1024);

    // *** Check if block has errors or not ***
    if (blockInfo.at(0) == 123) {
        if (blockInfo.at(index) == blockInfo.at(0) && blockInfo.at(index + 1) == blockInfo.at(1))
            setBytesIndex(getBytesIndex() + index);
        setSubInfoByteArray(info.mid(getBytesIndex(), quantityImgBytes));
    } else {
        // ***** Special case: File "record_0", double header: first header with 512 bytes *****

        if (blockInfo.at((index * 2) - 1) == 112 || blockInfo.at((index * 2) - 1) == 109 || blockInfo.at((index * 2) - 1) == 96) {
            blockInfo = info.mid(((getBytesIndex() - quantityBlckBytes) - index), 1024);
            if (blockInfo.at(0) == 123)
                setBytesIndex(getBytesIndex() - index);
            setSubInfoByteArray(info.mid(getBytesIndex(), quantityImgBytes));
        } else if (blockInfo.at((index * 2) - 1) == 85 || blockInfo.at((index * 2) - 1) == 107) {
            blockInfo = info.mid(((getBytesIndex() - quantityBlckBytes) - (index * 2)), 1024);
            if (blockInfo.at(0) == 123)
                setBytesIndex(getBytesIndex() - (index * 2));
            setSubInfoByteArray(info.mid(getBytesIndex(), quantityImgBytes));
        } else
            setSubInfoByteArray(info.mid(getBytesIndex(), quantityImgBytes));
    }
}

void Blob::calculatePreviousFrame(unsigned int rewFwd, QByteArray info) {
    if (rewFwd == 0 || rewFwd == getHeaderBytes()) {
        if (getIntArraySize() % 2 == 0)
            setBytesIndex(getIntArraySize() - quantityImgBytes);
        else
            setBytesIndex(getIntArraySize() - quantityImgBytes - difference);
    } else {
        if (spinBoxActivated == true)
            setBytesIndex(rewFwd - quantityImgBlckBytes);
        else
            setBytesIndex(getBytesIndex() - quantityImgBlckBytes);
    }

    calculateTemperature(info, rewFwd, getBytesIndex());
    calculatePreviousImageBytes(info);

    if (spinBoxActivated == true) {
        if(getResult() == 1)
            setResult(getNumberFrames());
        else
            setResult(getResult() - 1);

        setImgCount(getResult());
    } else {
        if (getImgCount() == 1)
            setImgCount(getNumberFrames());
        else
            setImgCount(getImgCount() - 1);
    }

    if (rewFwd == getHeaderBytes() || rewFwd == 0) {
        if (getIntArraySize() % 2 == 0)
            setRewFwdCount(getIntArraySize() - quantityImgBytes);
        else
            setRewFwdCount(getIntArraySize() - quantityImgBytes - difference);
    } else {
        if (spinBoxActivated == true)
            setRewFwdCount(rewFwd - quantityImgBlckBytes);
        else
            setRewFwdCount(getBytesIndex());
    }
}

void Blob::calculateTemperature(QByteArray info, unsigned int rewFwd, unsigned int bytesIndex) {
    // *** Little endian: max and min temperature value ***
    unsigned char lowMin;
    unsigned char highMin;
    unsigned char lowMax;
    unsigned char highMax;

    QByteArray subInfoTem;

    if (((rewFwd == 0 && spinBoxActivated == true)|| rewFwd == getHeaderBytes() ||
         (rewFwd == getBytesIndex() && spinBoxActivated == false)))
        subInfoTem = info.mid(7, 4);
    else
        subInfoTem = info.mid((bytesIndex - quantityBlckBytes + 1), 4);

    highMin = subInfoTem.at(0);
    lowMin  = subInfoTem.at(1);
    highMax = subInfoTem.at(2);
    lowMax  = subInfoTem.at(3);
    setMaxTempInt((lowMax << 8) | highMax);
    setMinTempInt((lowMin << 8) | highMin);
}

QByteArray Blob::openFile(QString *fileName) {
    QFile file(*fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open bin file";
    } else {
        qDebug() << "Open";

        info = file.readAll();
        file.close();
        setInfoByteArray(info);

        intArraySize = (unsigned int)info.size();
        setIntArraySize(intArraySize);

        calculateTemperature(info, getRewFwdCount(), getBytesIndex());

        QByteArray framesArray = info.mid(1, 1);

        unsigned int countFrames = (unsigned int)((unsigned char)framesArray.at(0));
        numberFrames = countFrames;
        setNumberFrames(numberFrames);

        // ********** Viewer reads files with different sizes (for example: file with errors) **********
        if (intArraySize % 2 == 0) {
            headerBytes = (intArraySize - ((numberFrames * quantityImgBytes) + ((numberFrames - difference) * quantityBlckBytes)));
            setHeaderBytes(headerBytes);
        } else {
            unsigned int fileWithExtraByte = ((quantityBlckBytes + 6) + quantityImgBytes) + (quantityImgBlckBytes * (numberFrames - difference)) + difference;
            if (intArraySize < fileWithExtraByte) {
                headerBytes = (intArraySize - ((numberFrames * quantityImgBytes) + ((numberFrames - difference) * quantityBlckBytes)) - difference);
                setHeaderBytes(headerBytes);
            } else {
                headerBytes = quantityBlckBytes + 6;
                setHeaderBytes(headerBytes);
            }
            info.remove(intArraySize, 1);
            setInfoByteArray(info);
        }

        subInfo = info.mid(headerBytes, quantityImgBytes);
        setSubInfoByteArray(subInfo);

    }
    return subInfo;
}

QByteArray Blob::nextImage(QByteArray info) {
    if (spinBoxActivated == false) {
        // ********** Calculate array of next frame after clicking next button *********
        // --- Calculate from which byte the frame should be shown (first byte of frame array) ---
        calculateNextFrame(getRewFwdCount(), info);

    } else {
        // ********** Calculate array of next frame after using SpinBox *********
        calculateNextFrame(getRewFwdCountSpin(), info);
        spinBoxActivated = false;
        setRewFwdCountSpin(getRewFwdCount());
    }
    return subInfo;
}

QByteArray Blob::previousImage(QByteArray info) {
    if (spinBoxActivated == false) {
        // ********** Calculate array of previous frame after clicking previous button *********
        // --- Calculate from which byte the frame should be shown (first byte of frame array) ---
        calculatePreviousFrame(getRewFwdCount(), info);
    } else {
        // ********** Calculate array of previous frame after using SpinBox *********
        calculatePreviousFrame(getRewFwdCountSpin(), info);
        spinBoxActivated = false;
        setRewFwdCountSpin(getRewFwdCount());
    }
    return subInfo;
}

void Blob::extractObjArray(QByteArray info) {
    // ********** Extract Array with Object info **********
    if (getRewFwdCount() == 0 || getRewFwdCount() == 477) {
       setSubInfoObjByteArray(info.mid(285, 192));
    } else {
        if (spinBoxActivated == false) {
            setSubInfoObjByteArray(info.mid((getBytesIndex() - quantityBlckBytes + 279), 192));
        } else
            setSubInfoObjByteArray(info.mid((getBytesIndexSpin() - quantityBlckBytes + 279), 192));
    }
}

void Blob::extractLineArray(QByteArray info) {
    // ********** Extract Array with Floor and Bodylines info **********
    if (getRewFwdCount() == 0 || getRewFwdCount() == 477) {
       setSubInfoFloorByteArray(info.mid(29, 128));
       setSubInfoBodyByteArray(info.mid(157, 128));
    } else {
        if (spinBoxActivated == false) {
            setSubInfoFloorByteArray(info.mid((getBytesIndex() - quantityBlckBytes + 23), 128));
            setSubInfoBodyByteArray(info.mid((getBytesIndex() - quantityBlckBytes + 151), 128));
        } else {
            setSubInfoFloorByteArray(info.mid((getBytesIndexSpin() - quantityBlckBytes + 23), 128));
            setSubInfoBodyByteArray(info.mid((getBytesIndexSpin() - quantityBlckBytes + 151), 128));
        }
    }
}

void Blob::getSpinBoxValue() {
    // ********** Obtain the SpinBox value and create the frame **********
    spinBoxActivated = true;

    // *** Standard files can have 130994 or 130995 bytes in total ***
    // ***** Files with an extra byte had the following structure:
    // ((header + 6 bytes) * image) + ((total nr of frames - 1) * (image + header)) + extra byte
    // First frame has always 6 bytes extra *****
    unsigned int fileWithExtraByte = ((quantityBlckBytes + 6) + quantityImgBytes) + (quantityImgBlckBytes * (getNumberFrames() - difference)) + difference;

    // --- Depending on total bytes: 130994 or 130995 bytes, values are set accordingly ---
    if(intArraySize <= fileWithExtraByte) {
        if (getResult() == 1) {
            setBytesIndexSpin(getHeaderBytes());
            setRewFwdCountSpin(getHeaderBytes());
            setResult(1);
            setImgCount(getResult());
        } else {
            setBytesIndexSpin(getHeaderBytes() + (quantityImgBlckBytes * (getResult() - difference)));
            setRewFwdCountSpin(getHeaderBytes() + (quantityImgBlckBytes * (getResult() - difference)));
            setImgCount(getResult());
        }
    } else {
        int index = 512;
        if (getResult() == 1) {
            bytesIndexSpin = headerBytes;
            rewFwdCountSpin = headerBytes;
            result = 1;
            imgCount = getResult();
        } else {
            // --- Special case: File "record_0", this algorithm only works for this file with errors ---

            if (getResult() >= 22 && getResult() <= 23) {
                bytesIndexSpin = index + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
                rewFwdCountSpin = index + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
            } else if (getResult() == 24) {
                bytesIndexSpin = quantityImgBytes - index + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
                rewFwdCountSpin = quantityImgBytes - index + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
            } else if (getResult() >= 25 && result <= 26) {
                bytesIndexSpin = quantityImgBytes + headerBytes  + (quantityImgBlckBytes * (getResult() - difference));
                rewFwdCountSpin = quantityImgBytes + headerBytes  + (quantityImgBlckBytes * (getResult() - difference));
            } else if (getResult() >= 27 && result <= 41) {
                bytesIndexSpin = (index * 2) + quantityImgBytes + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
                rewFwdCountSpin = (index * 2) + quantityImgBytes  + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
            } else if (getResult() >= 42 && getResult() <= 52) {
                bytesIndexSpin = (index * 3) + quantityImgBytes + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
                rewFwdCountSpin = (index * 3) + quantityImgBytes  + headerBytes + (quantityImgBlckBytes * (getResult() - difference));
            } else {
                bytesIndexSpin = headerBytes + (quantityImgBlckBytes * (getResult() - difference));
                rewFwdCountSpin = headerBytes + (quantityImgBlckBytes * (getResult() - difference));
            }
            imgCount = result;
        }
    }

    setSubInfoSpinByteArray(getInfoByteArray().mid(getBytesIndexSpin(), quantityImgBytes));

    calculateTemperature(getInfoByteArray(), getRewFwdCountSpin(), getBytesIndexSpin());
}


/*
// *** Composition - Methods from Image Class ***

QImage Blob::getImageThermalScale(QByteArray dynamicSubInfo) {
    return imageObject->thermalImage(dynamicSubInfo);
}

QImage Blob::getImageBlueOrGrayScale(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64) {
    return imageObject->blueOrGrayScaleImage(dynamicSubInfo, grayscaleOrRgba64);
}



// *** Composition - Methods from Layer Class ***

QPixmap Blob::getLayerLines() {
    return layerObject->drawLines(getSubInfoBodyByteArray(), getSubInfoFloorByteArray());
}

QPixmap Blob::getLayerObjects() {
    return layerObject->drawObjects(getSubInfoObjByteArray());
}

QPixmap Blob::eraseLayer() {
    return layerObject->eraseObjects();
}

QPixmap Blob::getLayerHeatmap() {
    return layerObject->drawHeatmap(getSubInfoObjByteArray(), getImgCount());
}

*/
unsigned int& Blob::getNewLayerArray(unsigned int x) {
    return layerObject->getNewLayerArray(x);
}

void Blob::resetLayerArray() {
    return layerObject->resetLayerArray();
}

// *** Destructor ***

Blob::~Blob(){
}
