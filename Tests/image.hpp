#ifndef IMAGE_H
#define IMAGE_H

#include <QByteArray>


class Image
{
private:
    static constexpr unsigned int imgHeight             = 32;
    static constexpr unsigned int imgWidth              = 64;
    static constexpr unsigned int quantityImgBytes      = 2048;
    static constexpr unsigned int quantityImgBlckBytes  = 2519;
    static constexpr unsigned int quantityBlckBytes     = 471;
    static constexpr unsigned int difference            = 1;
    static constexpr unsigned int objBytes              = 12;

};


#endif // IMAGE_H
