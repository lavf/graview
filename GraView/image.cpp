/*
 * The Image class has a relationship of Composition with the "Blob" class. Every Blob object has an
 * Image object and an Image object cannot be instantiated without instantiating the Blob class.
 *
 * In this class, the pixels are created and colors are assigned repectively. Thermal colors were
 * customized and every color is unique, therefore the scale does not follow a logic.
 * With regards to gray scale and blue scale, the gray scale uses the same color scale from the blue
 * scale but applies a different format provided by QImage "GrayScale".
 *
 */


#include "image.hpp"

QImage Image::blueOrGrayScaleImage(QByteArray subInfo, QImage::Format grayscaleOrRgba64) {
    QImage displayImage(imgWidth, imgHeight, grayscaleOrRgba64);
    // *** Calling function to assign blue scale colors. Format makes the difference if gray or blue format is applied ***
    return grayOrBlueScaleAssignation(subInfo, displayImage);
}

QImage Image::colorAssignation(QByteArray subInfo, QImage displayImage, QRgb colors [33]) {
    // *** Iterator indicates pixel position in byte array ***
    unsigned int i = 0;

    // *** Assigning thermal colors to every pixel according to temperature ***
    for (unsigned int z = 0; z < imgHeight; z++) {
       for (unsigned int j = 0; j < imgWidth; j++) {
           QRgb value;
           if (subInfo.at(i) >= 0 && subInfo.at(i) <= 5) {
               value = colors[0];
           } else if (subInfo.at(i) >= 6 && subInfo.at(i) <= 9) {
               value = colors[1];
           } else if (subInfo.at(i) >= 10 && subInfo.at(i) <= 15) {
               value = colors[2];
           } else if (subInfo.at(i) >= 16 && subInfo.at(i) <= 19) {
               value = colors[3];
           } else if (subInfo.at(i) >= 20 && subInfo.at(i) <= 25) {
               value = colors[4];
           } else if (subInfo.at(i) >= 26 && subInfo.at(i) <= 29) {
               value = colors[5];
           } else if (subInfo.at(i) >= 30 && subInfo.at(i) <= 35) {
               value = colors[6];
           } else if (subInfo.at(i) >= 36 && subInfo.at(i) <= 39) {
               value = colors[7];
           } else if (subInfo.at(i) >= 40 && subInfo.at(i) <= 45) {
               value = colors[8];
           } else if (subInfo.at(i) >= 46 && subInfo.at(i) <= 49) {
               value = colors[9];
           } else if (subInfo.at(i) >= 50 && subInfo.at(i) <= 55) {
               value = colors[10];
           } else if (subInfo.at(i) >= 56 && subInfo.at(i) <= 59) {
               value = colors[11];
           } else if (subInfo.at(i) == 60) {
               value = colors[12];
           } else if (subInfo.at(i) == 61) {
               value = colors[13];
           } else if (subInfo.at(i) == 62) {
               value = colors[14];
           } else if (subInfo.at(i) == 63) {
               value = colors[15];
           } else if (subInfo.at(i) == 64) {
               value = colors[16];
           } else if (subInfo.at(i) == 65) {
               value = colors[17];
           } else if (subInfo.at(i) >= 66 && subInfo.at(i) <= 69) {
               value = colors[18];
           } else if (subInfo.at(i) >= 70 && subInfo.at(i) <= 75) {
               value = colors[19];
           } else if (subInfo.at(i) >= 76 && subInfo.at(i) <= 79) {
               value = colors[20];
           } else if (subInfo.at(i) >= 80 && subInfo.at(i) <= 85) {
               value = colors[21];
           } else if (subInfo.at(i) >= 86 && subInfo.at(i) <= 89) {
               value = colors[22];
           } else if (subInfo.at(i) >= 90 && subInfo.at(i) <= 95) {
               value = colors[23];
           } else if (subInfo.at(i) >= 96 && subInfo.at(i) <= 99) {
               value = colors[24];
           } else if (subInfo.at(i) >= 100 && subInfo.at(i) <= 105) {
               value = colors[25];
           } else if (subInfo.at(i) >= 106 && subInfo.at(i) <= 109) {
               value = colors[26];
           } else if (subInfo.at(i) >= 110 && subInfo.at(i) <= 115) {
               value = colors[27];
           } else if (subInfo.at(i) >= 116 && subInfo.at(i) <= 119) {
               value = colors[28];
           } else if (subInfo.at(i) >= 120 && subInfo.at(i) <= 126) {
               value = colors[29];
           } else if (subInfo.at(i) == 127) {
               value = colors[30];
           } else if (subInfo.at(i) < 0) {
               value = colors[31];
           } else {
               value = colors[32];
           }
           ++i;
           displayImage.setPixel(j, z, value);
       }
    }
    return displayImage;
}

QImage Image::grayOrBlueScaleAssignation(QByteArray subInfo, QImage displayImage) {
    // ********** Generate blue scale colors array **********
    QRgb colors [65] = {0};
    int a = 250;
    int b = 3;

    for (unsigned int i = 0; i < 52; i++) {
        colors[i] = qRgb((a - (b * (15 + i))), (a - (b * (15 + i))), 255);
    }
    for (unsigned int i = 52, j = 0; i < 64; i++, j++) {
        colors[i] = qRgb((b * (11 - j)), (b * (11 - j)), 255);
    }
    colors[64] = qRgb(5, 9, 240);


    // ********** Assign blue scale colors to a range of pixels **********
    // *** Iterator indicates pixel position in byte array ***
    unsigned int iterA = 0;

    for (unsigned int z = 0; z < imgHeight; z++) {
        for (unsigned int j = 0; j < imgWidth; j++) {
            QRgb value;


            for (unsigned int iterB = 0; iterB < 127; iterB++) {
                if (iterB == 0) {
                    if(subInfo.at(iterA) == char(iterB)) {
                        value = colors[iterB];
                    }
                } else if (iterB % 2 == 0) {
                    if (subInfo.at(iterA) >= char(iterB) - 1 && subInfo.at(iterA) <= char(iterB)) {
                        value = colors[iterB/2];
                    } else if (subInfo.at(iterA) < 0 || subInfo.at(iterA) > char(iterB))
                        value = colors[64];
                }
            }
            ++iterA;
            displayImage.setPixel(j, z, value);
        }
    }
    return displayImage;
}

QImage Image::thermalImage(QByteArray subInfo) {
    QImage displayImage(imgWidth, imgHeight, QImage::Format_RGBA64);

    QRgb thermalColors [33] = {
        qRgb(179,   5,   5),
        qRgb(201,   5,   5),
        qRgb(214,   5,   5),
        qRgb(219,   6,   6),
        qRgb(223,   6,   6),
        qRgb(232,   6,   6),
        qRgb(223,  37,   9),
        qRgb(218,  53,  10),
        qRgb(213,  68,  11),
        qRgb(233,  75,  12),
        qRgb(228, 129,  17),
        qRgb(233, 162,  24),
        qRgb(250, 243,  43),
        qRgb(251, 241,  96),
        qRgb(252, 244, 136),
        qRgb(171, 119, 136),
        qRgb(163, 107, 145),
        qRgb(155,  94, 154),
        qRgb(147,  82, 164),
        qRgb(139,  69, 173),
        qRgb(131,  57, 182),
        qRgb(123,  44, 191),
        qRgb(109,  40, 191),
        qRgb( 94,  35, 191),
        qRgb( 87,  33, 191),
        qRgb( 79,  31, 191),
        qRgb( 64,  26, 190),
        qRgb( 34,  17, 190),
        qRgb(  4,   7, 189),
        qRgb(  4,   7, 175),
        qRgb(  4,   7, 168),
        qRgb(  3,   6, 160),
        qRgb(  0,   0,   0)
    };

    // *** Calling function to assign colors to pixels ***
    return colorAssignation(subInfo, displayImage, thermalColors);
}

