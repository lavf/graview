#ifndef IMAGE_H
#define IMAGE_H

#include <QByteArray>
#include <QImage>


class Image
{
public:
    // Default constructor and destructor provided by compiler implicitly
    QImage blueOrGrayScaleImage(QByteArray subInfo, QImage::Format grayscaleOrRgba64);
    QImage thermalImage(QByteArray subInfo);    

private:
    static constexpr unsigned int imgHeight             = 32;
    static constexpr unsigned int imgWidth              = 64;
    static constexpr unsigned int quantityImgBytes      = 2048;
    static constexpr unsigned int quantityImgBlckBytes  = 2519;
    static constexpr unsigned int quantityBlckBytes     = 471;
    static constexpr unsigned int difference            = 1;
    static constexpr unsigned int objBytes              = 12;

    QImage colorAssignation(QByteArray subInfo, QImage displayImage, QRgb colors [33]);
    QImage grayOrBlueScaleAssignation(QByteArray subInfo, QImage displayImage);
};


#endif // IMAGE_H
