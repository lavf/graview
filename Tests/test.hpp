#ifndef TEST_H
#define TEST_H

#include "layer.hpp"
#include "blob.hpp"

#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT
public:
    Test();
private slots:
    // *** Called before the all the test function ***
    void initTestCase();
    // called before every test function
    void init();
    // called after every test function
    void cleanup();
    // *** Called after the last test function ***
    void cleanupTestCase();

    //-- tests --
    void testLayerClassColors();
    void testToFail1LayerClassColors();
    void testToFail2LayerClassColors();

    void testBlobClassHeaderBytes();

    void testBlobClassOpenBin();
    void testToFail1BlobClassOpenBin();
    void testToFail2BlobClassOpenBin();
    void testToFail3BlobClassOpenBin();

    void testBlobClassTemperature();
    void testToFail1BlobClassTemperature();
    void testToFail2BlobClassTemperature();

    void testBlobClassCalculateNextArray();
    void testToFailBlobClassCalculateNextArray();

private:
    std::unique_ptr<Blob>  testBlob;
    std::unique_ptr<Layer> testLayer;

    unsigned int headerB = 477;
    unsigned int headerZero = 0;
    QString fileName1 = "record_arbeitsplatz-herumlaufen";
    QString fileName2 = "record_0";
    QString fileName3 = "record2";
    QString errorFile = "record1";
};

#endif // TEST_H
