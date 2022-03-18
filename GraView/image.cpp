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
    unsigned int i = 0;

    for (unsigned int z = 0; z < imgHeight; z++) {
        for (unsigned int j = 0; j < imgWidth; j++) {
            QRgb value;
            if (subInfo.at(i) == 0) {
                value = colors[0];
            } else if (subInfo.at(i) >= 1 && subInfo.at(i) <= 2) {
                value = colors[1];
            } else if (subInfo.at(i) >= 3 && subInfo.at(i) <= 4) {
                value = colors[2];
            } else if (subInfo.at(i) >= 5 && subInfo.at(i) <= 6) {
                value = colors[3];
            } else if (subInfo.at(i) >= 7 && subInfo.at(i) <= 8) {
                value = colors[4];
            } else if (subInfo.at(i) >= 9 && subInfo.at(i) <= 10) {
                value = colors[5];
            } else if (subInfo.at(i) >= 11 && subInfo.at(i) <= 12) {
                value = colors[6];
            } else if (subInfo.at(i) >= 13 && subInfo.at(i) <= 14) {
                value = colors[7];
            } else if (subInfo.at(i) >= 15 && subInfo.at(i) <= 16) {
                value = colors[8];
            } else if (subInfo.at(i) >= 17 && subInfo.at(i) <= 18) {
                value = colors[9];
            } else if (subInfo.at(i) >= 19 && subInfo.at(i) <= 20) {
                value = colors[10];
            } else if (subInfo.at(i) >= 21 && subInfo.at(i) <= 22) {
                value = colors[11];
            } else if (subInfo.at(i) >= 23 && subInfo.at(i) <= 24) {
                value = colors[12];
            } else if (subInfo.at(i) >= 25 && subInfo.at(i) <= 26) {
                value = colors[13];
            } else if (subInfo.at(i) >= 27 && subInfo.at(i) <= 28) {
                value = colors[14];
            } else if (subInfo.at(i) >= 29 && subInfo.at(i) <= 30) {
                value = colors[15];
            } else if (subInfo.at(i) >= 31 && subInfo.at(i) <= 32) {
                value = colors[16];
            } else if (subInfo.at(i) >= 33 && subInfo.at(i) <= 34) {
                value = colors[17];
            } else if (subInfo.at(i) >= 35 && subInfo.at(i) <= 36) {
                value = colors[18];
            } else if (subInfo.at(i) >= 37 && subInfo.at(i) <= 38) {
                value = colors[19];
            } else if (subInfo.at(i) >= 39 && subInfo.at(i) <= 40) {
                value = colors[20];
            } else if (subInfo.at(i) >= 41 && subInfo.at(i) <= 42) {
                value = colors[21];
            } else if (subInfo.at(i) >= 43 && subInfo.at(i) <= 44) {
                value = colors[22];
            } else if (subInfo.at(i) >= 45 && subInfo.at(i) <= 46) {
                value = colors[23];
            } else if (subInfo.at(i) >= 47 && subInfo.at(i) <= 48) {
                value = colors[24];
            } else if (subInfo.at(i) >= 49 && subInfo.at(i) <= 50) {
                value = colors[25];
            } else if (subInfo.at(i) >= 51 && subInfo.at(i) <= 52) {
                value = colors[26];
            } else if (subInfo.at(i) >= 53 && subInfo.at(i) <= 54) {
                value = colors[27];
            } else if (subInfo.at(i) >= 55 && subInfo.at(i) <= 56) {
                value = colors[28];
            } else if (subInfo.at(i) >= 57 && subInfo.at(i) <= 58) {
                value = colors[29];
            } else if (subInfo.at(i) >= 59 && subInfo.at(i) <= 60) {
                value = colors[30];
            } else if (subInfo.at(i) >= 61 && subInfo.at(i) <= 62) {
                value = colors[31];
            } else if (subInfo.at(i) >= 63 && subInfo.at(i) <= 64) {
                value = colors[32];
            } else if (subInfo.at(i) >= 65 && subInfo.at(i) <= 66) {
                value = colors[33];
            } else if (subInfo.at(i) >= 67 && subInfo.at(i) <= 68) {
                value = colors[34];
            } else if (subInfo.at(i) >= 69 && subInfo.at(i) <= 70) {
                value = colors[35];
            } else if (subInfo.at(i) >= 71 && subInfo.at(i) <= 72) {
                value = colors[36];
            } else if (subInfo.at(i) >= 73 && subInfo.at(i) <= 74) {
                value = colors[37];
            } else if (subInfo.at(i) >= 75 && subInfo.at(i) <= 76) {
                value = colors[38];
            } else if (subInfo.at(i) >= 77 && subInfo.at(i) <= 78) {
                value = colors[39];
            } else if (subInfo.at(i) >= 79 && subInfo.at(i) <= 80) {
                value = colors[40];
            } else if (subInfo.at(i) >= 81 && subInfo.at(i) <= 82) {
                value = colors[41];
            } else if (subInfo.at(i) >= 83 && subInfo.at(i) <= 84) {
                value = colors[42];
            } else if (subInfo.at(i) >= 85 && subInfo.at(i) <= 86) {
                value = colors[43];
            } else if (subInfo.at(i) >= 87 && subInfo.at(i) <= 88) {
                value = colors[44];
            } else if (subInfo.at(i) >= 89 && subInfo.at(i) <= 90) {
                value = colors[45];
            } else if (subInfo.at(i) >= 91 && subInfo.at(i) <= 92) {
                value = colors[46];
            } else if (subInfo.at(i) >= 93 && subInfo.at(i) <= 94) {
                value = colors[47];
            } else if (subInfo.at(i) >= 95 && subInfo.at(i) <= 96) {
                value = colors[48];
            } else if (subInfo.at(i) >= 97 && subInfo.at(i) <= 98) {
                value = colors[49];
            } else if (subInfo.at(i) >= 99 && subInfo.at(i) <= 100) {
                value = colors[50];
            } else if (subInfo.at(i) >= 101 && subInfo.at(i) <= 102) {
                value = colors[51];
            } else if (subInfo.at(i) >= 103 && subInfo.at(i) <= 104) {
                value = colors[52];
            } else if (subInfo.at(i) >= 105 && subInfo.at(i) <= 106) {
                value = colors[53];
            } else if (subInfo.at(i) >= 107 && subInfo.at(i) <= 108) {
                value = colors[54];
            } else if (subInfo.at(i) >= 109 && subInfo.at(i) <= 110) {
                value = colors[55];
            } else if (subInfo.at(i) >= 111 && subInfo.at(i) <= 112) {
                value = colors[56];
            } else if (subInfo.at(i) >= 113 && subInfo.at(i) <= 114) {
                value = colors[57];
            } else if (subInfo.at(i) >= 115 && subInfo.at(i) <= 116) {
                value = colors[58];
            } else if (subInfo.at(i) >= 117 && subInfo.at(i) <= 118) {
                value = colors[59];
            } else if (subInfo.at(i) >= 119 && subInfo.at(i) <= 120) {
                value = colors[60];
            } else if (subInfo.at(i) >= 121 && subInfo.at(i) <= 122) {
                value = colors[61];
            } else if (subInfo.at(i) >= 123 && subInfo.at(i) <= 124) {
                value = colors[62];
            } else if (subInfo.at(i) >= 125 && subInfo.at(i) <= 126) {
                value = colors[63];
            } else {
            value = colors[64];
            }
            ++i;
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
