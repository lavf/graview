#ifndef IMAGE_H
#define IMAGE_H

#include <QByteArray>
#include <QImage>


class Image
{
public:
    // Default constructor and destructor provided by compiler implicitly
    QImage thermalImage(QByteArray subInfo);    

private:
    static constexpr unsigned int imgHeight             = 32;
    static constexpr unsigned int imgWidth              = 64;

    QImage colorAssignation(QByteArray subInfo, QImage displayImage, QRgb colors [33]);
};


#endif // IMAGE_H
