#include "test.hpp"

Test::Test()
    : testBlob(new Blob),
      testLayer(new Layer)
{

}

void Test::init()
{
    testBlob->setHeaderBytes(headerB);
    qDebug("Setting header bytes.");
}

void Test::initTestCase()
{
    qDebug("Starting all tests...");
}

// ***** Test Case 1: Open the binary file *****
void Test::testBlobClassOpenBin() {

    testBlob->openFile(&fileName1);

    QVERIFY2(testBlob->getIntArraySize() >= 130994, "First file has not 130994 bytes.");
    QVERIFY2(testBlob->getIntArraySize() <= 130995, "First file has not 130995 bytes.");


    testBlob->openFile(&fileName2);

    QVERIFY2(testBlob->getIntArraySize() >= 130994, "Second file has not 130994 bytes.");
    QVERIFY2(testBlob->getIntArraySize() <= 130995, "Second file has not 130995 bytes.");

    testBlob->openFile(&fileName3);

    QVERIFY2(testBlob->getIntArraySize() >= 130994, "First file has not 130994 bytes.");
    QVERIFY2(testBlob->getIntArraySize() <= 130995, "Second file has not 130995 bytes.");


    testBlob->openFile(&errorFile);

    QVERIFY2(testBlob->getIntArraySize() != 130995, "Error file has 130995 bytes.");
    QVERIFY2(testBlob->getIntArraySize() != 130994, "Error file has 130994 bytes.");
    QVERIFY2(testBlob->getIntArraySize() == 134579, "Error file has not 134579 bytes.");
}

void Test::testToFail1BlobClassOpenBin() {
    testBlob->openFile(&errorFile);
    // *** Test-to-Fail Case: File with errors (more bytes than a standard file) ***
    QVERIFY2(testBlob->getIntArraySize() < 130994, "Error file has more than 130994 bytes.");  // 134579
}

void Test::testToFail2BlobClassOpenBin() {
    testBlob->openFile(&errorFile);
    // *** Test-to-Fail Case: File with errors (more bytes than a standard file) ***
    QVERIFY2(testBlob->getIntArraySize() == 130994, "Error file has not 130995 bytes."); // 134579
}

void Test::testToFail3BlobClassOpenBin() {
    testBlob->openFile(&errorFile);
    // *** Test-to-Fail Case: File with errors (more bytes than a standard file) ***
    QVERIFY2(testBlob->getIntArraySize() == 130995, "Error file has not 130994 bytes."); // 134579
}
// *****  End of Test Case 1 *****


// ***** Test Case 2: Bytes in the header block *****
void Test::testBlobClassHeaderBytes(){
    // *** Test-to-Pass Cases : valid arguments
    QCOMPARE(testBlob->getHeaderBytes(), testBlob->headerBytes);  // 477

    testBlob->setHeaderBytes(headerZero);

    QCOMPARE(testBlob->getHeaderBytes(), headerZero);
}
// ***** End of Test Case 2 *****


// ***** Test Case 3: Bytes of the next frame *****
void Test::testBlobClassCalculateNextArray() {

    testBlob->openFile(&fileName1);

    // *** Test-to-Pass Cases : invalid arguments
    testBlob->calculateNextFrame(2, testBlob->getInfoByteArray());

    QVERIFY2(testBlob->getBytesIndex() == 4569, "The index byte of the nex frame is not 4569.");

    // *** Test-to-Pass Cases : valid arguments
    testBlob->calculateNextFrame(0, testBlob->getInfoByteArray());

    QVERIFY2(testBlob->getBytesIndex() == 2996, "The index byte of the nex frame is not 2996.");

    testBlob->calculateNextFrame(477 + testBlob->quantityImgBlckBytes, testBlob->getInfoByteArray());

    QVERIFY2(testBlob->getBytesIndex() == 5515, "The index byte of the nex frame is not 5515.");    
}

void Test::testToFailBlobClassCalculateNextArray() {
    // *** Test-to-Fail Cases : invalid arguments
    testBlob->calculateNextFrame(-2, testBlob->getInfoByteArray());
    QVERIFY2(testBlob->getBytesIndex() == 4564, "The index byte of the nex frame is not 4565 and it should be.");
}
// ***** End of Test Case 3 *****


// ***** Test Case 4: Temperature conversion *****
void Test::testBlobClassTemperature() {
    testBlob->openFile(&fileName1);

    // *** Test-to-Pass Cases : valid arguments
    QVERIFY2(testBlob->getMaxTempInt() == 3043, "Temp is not 3043 dK°"); // max 3043
    QVERIFY2(testBlob->getMinTempInt() == 2935, "Temp is not 2935 dK°"); // min 2935
}

void Test::testToFail1BlobClassTemperature() {
    // *** Test-to-Fail Cases : invalid arguments
    testBlob->openFile(&fileName2);
    QVERIFY2(testBlob->getMaxTempInt() == 3043, "Temp is not 3043 dK°, it should be 3011 dK°"); // max 3011
}

void Test::testToFail2BlobClassTemperature() {
    // *** Test-to-Fail Cases : invalid arguments
    testBlob->openFile(&fileName2);
    QVERIFY2(testBlob->getMinTempInt() == 2935, "Temp is not 2935 dK°, it should be 2815 dK°"); // min 2815
}
// ***** End of Test Case 4 *****


// ***** Test Case 5: Colors of the objects *****
void Test::testLayerClassColors() {

    // *** Test-to-Pass Cases : valid input
    unsigned int newStatus             = 1;
    unsigned int objectStatus          = 2;
    unsigned int staticExtensionStatus = 3;
    unsigned int staticStatus          = 4;
    unsigned int missingStatus         = 5;
    unsigned int humanStatus           = 10;
    unsigned int criticalStatus        = 26;
    unsigned int warningStatus         = 58;
    unsigned int alarmStatus           = 90;

    QVERIFY(testLayer->assignObjColors(newStatus)               == Qt::yellow);
    QVERIFY(testLayer->assignObjColors(objectStatus)            == Qt::yellow);
    QVERIFY(testLayer->assignObjColors(staticExtensionStatus)   == Qt::blue);
    QVERIFY(testLayer->assignObjColors(staticStatus)            == Qt::magenta);
    QVERIFY(testLayer->assignObjColors(missingStatus)           == Qt::white);
    QVERIFY(testLayer->assignObjColors(humanStatus)             == Qt::green);
    QVERIFY(testLayer->assignObjColors(criticalStatus)          == Qt::darkGreen);
    QVERIFY(testLayer->assignObjColors(warningStatus)           == Qt::darkRed);
    QVERIFY(testLayer->assignObjColors(alarmStatus)             == Qt::red);

    // *** Test-to-Pass Cases : invalid input
    unsigned int firstRandomNr         = 0;
    unsigned int secondRandomNr        = 101;
    unsigned int thirdRandomNr         = 02;
    double fourthRandomNr              = 3.5;
    char fifthRandomCase               = 'x';

    QVERIFY2(testLayer->assignObjColors(firstRandomNr)     == Qt::black,  "First false case with 0 doesn't match");
    QVERIFY2(testLayer->assignObjColors(secondRandomNr)    == Qt::black,  "Second false case with 101 doesn't match");
    QVERIFY2(testLayer->assignObjColors(thirdRandomNr)     == Qt::yellow, "Third false case with 02 doesn't match");
    QVERIFY2(testLayer->assignObjColors(fourthRandomNr)    == Qt::blue,  "Fourth false case with 3.5 doesn't match");
    QVERIFY2(testLayer->assignObjColors(fifthRandomCase)   == Qt::black, "Fifth false case with 'x' doesn't match");
}

void Test::testToFail1LayerClassColors() {
    // *** Test-to-Fail Cases : invalid input
    double fourthRandomNr              = 3.5;
    QVERIFY2(testLayer->assignObjColors(fourthRandomNr)    == Qt::yellow,  "Fourth false case with 3.5 doesn't match, it should be Qt::blue");
}

void Test::testToFail2LayerClassColors() {
    // *** Test-to-Fail Cases : invalid input
    char fifthRandomCase               = 'x';
    QVERIFY2(testLayer->assignObjColors(fifthRandomCase)   == Qt::darkRed, "Fifth false case with 'x' doesn't match, it should be Qt::black");
}
// ***** End of Test Case 5 *****


void Test::cleanup()
{
    qDebug("One test was carried out.");
}

void Test::cleanupTestCase()
{
    qDebug("All tests were carried out.");
}



// Testing C++ Classes not Qt Application
QTEST_APPLESS_MAIN(Test)
