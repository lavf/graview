/*
 * The Interface class has a relationship of Aggregation with the class "Blob", which reads and processes the binary data.
 * This means that an Interface object HAS a Blob object but there is no dependency between each other.
 *
 * Interface Class is a QWidget class from the Qt Widgets module. This QWidget class is the base of all GUI objects.
 *
 * The business logic of the programm is performed by the Blob object. Nevertheless, changes in the logic are triggered by
 * the GUI elements: Spin Box and "Go to" button. Otherwise, the algorithms work by showing the previous
 * or next frame.
 *
 * When the last frame is reached and next button is tapped, the array starts again from the first frame.
 * The same behavior is expected when the first frame is reached and previous button is tapped, although the array starts
 * from the last frame.
 *
 */

#include "interface.hpp"
#include "blob.hpp"

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QPainter>
#include <QMessageBox>
#include <QApplication>


#define NEW 1
#define OBJECT 2
#define STATICX 3
#define STATIC 4
#define MISSING 5
#define HUMAN 10
#define CRITICAL 26
#define WARNING 58
#define ALARM 90

Interface::Interface(QWidget *parent)
    : QWidget(parent),
      blobObject(new Blob)
{
    setWindowTitle(tr("GraView"));

    labelStatus                  = new QLabel;
    labelTitle                   = new QLabel;
    displayBox                   = new QLabel;
    displayObj                   = new QLabel;
    displayLines                 = new QLabel;
    labelDescObj1                = new QLabel;
    labelDescObj2                = new QLabel;
    labelDescObj3                = new QLabel;
    labelDescObj4                = new QLabel;
    labelDescObj5                = new QLabel;
    labelDescObj6                = new QLabel;
    labelDescObj7                = new QLabel;
    labelDescObj8                = new QLabel;
    labelLegendOpenFile          = new QLabel;
    labelLegendNumberFrames      = new QLabel;
    labelLegendTotalObjs         = new QLabel;
    labelLegendHumanObj          = new QLabel;
    labelLegendHumanObjText      = new QLabel;
    labelLegendTotalObjsText     = new QLabel;
    labelIconFrameNr             = new QLabel;
    labelLegendSizeArray         = new QLabel;
    labelLegendSizeArrayText     = new QLabel;
    labelLegendImg               = new QLabel;
    labelLegendImgText           = new QLabel;
    labelLegendMax               = new QLabel;
    labelLegendMin               = new QLabel;
    labelLegendMaxText           = new QLabel;
    labelLegendMinText           = new QLabel;
    labelMaxValue                = new QLabel;
    labelMinValue                = new QLabel;
    labelCountObj                = new QLabel;
    labelSizeArray               = new QLabel;
    labelImgCount                = new QLabel;
    labelHumanObjs               = new QLabel;
    labelSpace                   = new QLabel;
    labelSpan                    = new QLabel;

    frameSpinBox                 = new QSpinBox;

    slider                       = new QProgressBar;

    lineCheckBox                 = new QCheckBox;
    objCheckBox                  = new QCheckBox;

    openFileButton               = new QPushButton;
    nextButton                   = new QPushButton;
    previousButton               = new QPushButton;
    blueScaleButton              = new QPushButton;
    thermalScaleButton           = new QPushButton;
    grayScaleButton              = new QPushButton;
    smoothButton                 = new QPushButton;
    sharpButton                  = new QPushButton;
    goToFrameButton              = new QPushButton;
    exitButton                   = new QPushButton;
    infoButton                   = new QPushButton;


    // *** Creating connection for buttons ***
    connect(openFileButton,     SIGNAL(clicked()), this, SLOT(displayApp()));
    connect(blueScaleButton,    SIGNAL(clicked()), this, SLOT(displayBlueScale()));
    connect(grayScaleButton,    SIGNAL(clicked()), this, SLOT(displayGrayScale()));
    connect(nextButton,         SIGNAL(clicked()), this, SLOT(nextFrame()));
    connect(previousButton,     SIGNAL(clicked()), this, SLOT(previousFrame()));
    connect(thermalScaleButton, SIGNAL(clicked()), this, SLOT(displayThermal()));
    connect(smoothButton,       SIGNAL(clicked()), this, SLOT(applySmoothFilter()));
    connect(sharpButton,        SIGNAL(clicked()), this, SLOT(applySharpFilter()));
    connect(exitButton,         SIGNAL(clicked()), this, SLOT(closeApp()), Qt::QueuedConnection);
    connect(infoButton,         SIGNAL(clicked()), this, SLOT(infoMessage()));
    connect(goToFrameButton,    SIGNAL(clicked()), this, SLOT(displayFrameFromSpinBox()));

    displayBox->setFixedWidth(1200);
    displayBox->setFixedHeight(600);

    displayBox->setText(tr("GraView"));

    displayBox->setAlignment(Qt::AlignCenter);

    openFileButton->setText(tr("Open file"));
    exitButton->setText(tr("Exit"));
    nextButton->setText(tr(">"));
    previousButton->setText(tr("<"));
    goToFrameButton->setText(tr("Jump to"));
    objCheckBox->setText(tr("Objects"));
    lineCheckBox->setText(tr("Lines"));
    smoothButton->setText(tr("Smooth"));
    sharpButton->setText(tr("Sharp"));
    thermalScaleButton->setText(tr("Thermal"));
    grayScaleButton->setText(tr("Grayscale"));
    blueScaleButton->setText(tr("Bluescale"));
    infoButton->setText(tr("Info"));

    mainLayout           = new QGridLayout;
    welcomeMenuBarLayout = new QVBoxLayout;
    imgAndStatusLayout   = new QVBoxLayout;
    imgLayersLayout      = new QGridLayout;

    welcomeMenuBarLayout->addWidget(openFileButton);
    welcomeMenuBarLayout->addWidget(exitButton);

    imgLayersLayout->addWidget(displayBox,     0, 0);
    imgLayersLayout->addWidget(displayLines,   0, 0);
    imgLayersLayout->addWidget(displayObj,     0, 0);

    imgAndStatusLayout->addWidget(labelTitle);
    imgAndStatusLayout->addLayout(imgLayersLayout);

    mainLayout->addLayout(imgAndStatusLayout,   0, 0);
    mainLayout->addLayout(welcomeMenuBarLayout, 0, 1);
    setLayout(mainLayout);
}

void Interface::applySharpFilter() {
    smoothFilterActivated = false;
    if (blueScaleActivated == true) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_RGBA64)));
        displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    } else if (grayScaleActivated == true) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_Grayscale16)));
        displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));
    } else if (thermalActivated == true ) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->getSubInfoByteArray())));
        displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    }
}

void Interface::applySmoothFilter() {
    smoothFilterActivated = true;
    if (blueScaleActivated == true) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_RGBA64)));
        displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (grayScaleActivated == true) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_Grayscale16)));
        displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (thermalActivated == true ) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->getSubInfoByteArray())));
        displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void Interface::closeApp() {
    // *** qApp is a global pointer to QApplication ***
    qApp->quit();
}

void Interface::displayApp() {
    // ********** Reset values to default values: frame n°1, thermal colors, sharp filter **********
    blobObject              ->setImgCount(1);
    blobObject              ->setBytesIndex(0);
    blobObject              ->setNumberFrames(0);
    blobObject              ->setResult(1);
    blobObject              ->setRewFwdCount(blobObject->getHeaderBytes());
    blueScaleActivated      = false;
    grayScaleActivated      = false;
    thermalActivated        = true;
    smoothFilterActivated   = false;    

    // ********** Open the file **********
    QString *fileName = new QString(QFileDialog::getOpenFileName(this, tr("Select a binary file"), QDir::homePath(), "Binary file (*)"));
    QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->openFile(fileName))));

    if ((blobObject->getIntArraySize() != correctTotalBytesA) && (blobObject->getIntArraySize() != correctTotalBytesB)) {
        QMessageBox::warning(this, tr("Warning"), tr("The file does not have the standard byte length, and consequently GraView may stop working.\n\nThe device from where you obtained the file, may not be correctly calibrated."), QMessageBox::Ok);
    }

    displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));

    // ********** Label Management **********
    labelTitle ->setText("GraView");
    labelStatus->setText("File opened");

    labelLegendSizeArrayText->setText("Total Bytes:");
    QString legendSizeArray;
    legendSizeArray.setNum(blobObject->getIntArraySize());
    labelSizeArray->setText(legendSizeArray);

    labelLegendNumberFrames->setText("N° Frame");

    labelLegendTotalObjsText->setText("Total Objs.");

    labelLegendHumanObjText->setText("Human Obj.");

    labelLegendImgText->setText("Byte Index");
    QString legendImgCount;
    legendImgCount.setNum(blobObject->getHeaderBytes());
    labelImgCount    ->setText(legendImgCount);

    labelLegendSizeArrayText->setText("Total Bytes");

    labelLegendMaxText->setText("Max Temp:");

    labelLegendMinText ->setText("Min Temp:");

    // ********** Layout Management **********
    frameDescriptionLayout      = new QGridLayout;
    frameImgLayout              = new QGridLayout;
    rightFooterLayout           = new QGridLayout;
    totalBytesLayout            = new QVBoxLayout;
    byteIndexLayout             = new QVBoxLayout;
    numberFramesLayout          = new QVBoxLayout;
    minTempLayout               = new QVBoxLayout;
    maxTempLayout               = new QVBoxLayout;
    humanObjsLayout             = new QVBoxLayout;
    totalObjsLayout             = new QVBoxLayout;
    subMenuLayout               = new QHBoxLayout;

    byteIndexLayout->addWidget(labelLegendImg);
    byteIndexLayout->addWidget(labelLegendImgText);

    totalBytesLayout->addWidget(labelLegendSizeArray);
    totalBytesLayout->addWidget(labelLegendSizeArrayText);

    humanObjsLayout->addWidget(labelLegendHumanObj);
    humanObjsLayout->addWidget(labelLegendHumanObjText);

    totalObjsLayout->addWidget(labelLegendTotalObjs);
    totalObjsLayout->addWidget(labelLegendTotalObjsText);

    subMenuLayout->addWidget(exitButton);
    subMenuLayout->addWidget(infoButton);

    numberFramesLayout->addWidget(labelIconFrameNr);
    numberFramesLayout->addWidget(labelLegendNumberFrames);

    minTempLayout->addWidget(labelLegendMin);
    minTempLayout->addWidget(labelLegendMinText);

    maxTempLayout->addWidget(labelLegendMax);
    maxTempLayout->addWidget(labelLegendMaxText);

    rightFooterLayout->addLayout(byteIndexLayout,  0, 0, 1, 1);
    rightFooterLayout->addWidget(labelImgCount,    0, 1, 1, 1);
    rightFooterLayout->addWidget(labelSpan,        0, 2, 1, 1);
    rightFooterLayout->addLayout(minTempLayout,    0, 3, 1, 1);
    rightFooterLayout->addWidget(labelMinValue,    0, 4, 1, 1);
    rightFooterLayout->addWidget(labelSpan,        0, 5, 1, 1);
    rightFooterLayout->addLayout(totalObjsLayout,  0, 6, 1, 1);
    rightFooterLayout->addWidget(labelCountObj,    0, 7, 1, 1);

    rightFooterLayout->addLayout(totalBytesLayout, 1, 0, 1, 1);
    rightFooterLayout->addWidget(labelSizeArray,   1, 1, 1, 1);
    rightFooterLayout->addWidget(labelSpan,        1, 2, 1, 1);
    rightFooterLayout->addLayout(maxTempLayout,    1, 3, 1, 1);
    rightFooterLayout->addWidget(labelMaxValue,    1, 4, 1, 1);
    rightFooterLayout->addWidget(labelSpan,        1, 5, 1, 1);
    rightFooterLayout->addLayout(humanObjsLayout,  1, 6, 1, 1);
    rightFooterLayout->addWidget(labelHumanObjs,   1, 7, 1, 1);

    QGridLayout *sideMenuBar = new QGridLayout;

    sideMenuBar->addWidget(openFileButton,     0, 0, 1, 4);
    sideMenuBar->addWidget(previousButton,     0, 4, 1, 2);
    sideMenuBar->addWidget(nextButton,         0, 6, 1, 2);

    sideMenuBar->addWidget(grayScaleButton,    1, 0, 1, 2);
    sideMenuBar->addWidget(thermalScaleButton, 1, 2, 1, 2);
    sideMenuBar->addWidget(blueScaleButton,    1, 4, 1, 2);
    sideMenuBar->addWidget(labelSpace,         1, 6, 1, 2);

    sideMenuBar->addWidget(smoothButton,       2, 0, 1, 2);
    sideMenuBar->addWidget(sharpButton,        2, 2, 1, 2);
    sideMenuBar->addWidget(objCheckBox,        2, 4, 1, 2);
    sideMenuBar->addWidget(lineCheckBox,       2, 6, 1, 2);

    sideMenuBar->addWidget(labelDescObj1,      3, 0, 1, 1);
    sideMenuBar->addWidget(labelDescObj2,      3, 1, 1, 1);
    sideMenuBar->addWidget(labelDescObj3,      3, 2, 1, 1);
    sideMenuBar->addWidget(labelDescObj4,      3, 3, 1, 1);
    sideMenuBar->addWidget(labelDescObj5,      3, 4, 1, 1);
    sideMenuBar->addWidget(labelDescObj6,      3, 5, 1, 1);
    sideMenuBar->addWidget(labelDescObj7,      3, 6, 1, 1);
    sideMenuBar->addWidget(labelDescObj8,      3, 7, 1, 1);

    sideMenuBar->addWidget(labelSpace,         4, 0, 1, 1);
    sideMenuBar->addWidget(labelSpace,         4, 1, 1, 1);
    sideMenuBar->addWidget(labelSpace,         4, 2, 1, 1);
    sideMenuBar->addWidget(labelSpace,         4, 3, 1, 1);
    sideMenuBar->addWidget(labelSpace,         4, 4, 1, 1);
    sideMenuBar->addWidget(labelSpace,         4, 5, 1, 1);
    sideMenuBar->addWidget(labelSpace,         4, 6, 1, 1);
    sideMenuBar->addWidget(labelSpace,         4, 7, 1, 1);

    sideMenuBar->addLayout(numberFramesLayout, 5, 0, 1, 2);
    sideMenuBar->addWidget(frameSpinBox,       5, 2, 1, 2);
    sideMenuBar->addWidget(labelSpan,          5, 4, 1, 2);
    sideMenuBar->addWidget(goToFrameButton,    5, 6, 1, 2);

    sideMenuBar->addLayout(rightFooterLayout,  6, 0, 1, 7);

    imgAndStatusLayout->addWidget(slider);
    imgAndStatusLayout->addWidget(labelStatus);
    imgAndStatusLayout->addLayout(subMenuLayout);

    mainLayout->addLayout(sideMenuBar, 0, 1);

}

void Interface::displayBlueScale() {
    QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_RGBA64)));
    if (smoothFilterActivated == true)
        displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(),Qt::KeepAspectRatio));

    blueScaleActivated = true;
    thermalActivated   = false;
    grayScaleActivated = false;
}

void Interface::displayFrameFromSpinBox() {
}

void Interface::displayGrayScale() {
    QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_Grayscale16)));
    if (smoothFilterActivated == true)
        displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));

    blueScaleActivated = false;
    thermalActivated   = false;
    grayScaleActivated = true;
}

void Interface::displayThermal() {
    QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->getSubInfoByteArray())));
    if (smoothFilterActivated == true)
        displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));

    blueScaleActivated = false;
    thermalActivated   = true;
    grayScaleActivated = false;
}

void Interface::infoMessage() {
    QMessageBox::information(this, tr("About"), tr("GraView for Android 11\nQWidget-App using Qt 5.15.2\nVersion 1.0\n\nFor further info, please visit:\nhttps://github.com/lavf/graview"), QMessageBox::Ok);
}

void Interface::nextFrame() {
    if (blueScaleActivated == true) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->nextImage(blobObject->getInfoByteArray()), QImage::Format_RGBA64)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    } else if (thermalActivated == true) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->nextImage(blobObject->getInfoByteArray()))));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    } else {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->nextImage(blobObject->getInfoByteArray()), QImage::Format_Grayscale16)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    }

    labelMaxValue->setNum(blobObject->getMaxTempInt());
    labelMinValue->setNum(blobObject->getMinTempInt());

    QString legendImgCount;
    legendImgCount.setNum(blobObject->getBytesIndex());
    labelImgCount->setText(legendImgCount);

}

void Interface::paintEvent(QPaintEvent *) {
    blobObject  ->extractObjArray(blobObject->getInfoByteArray());
    blobObject  ->extractLineArray(blobObject->getInfoByteArray());

    // ********** Object Recognition **********
    if (objCheckBox->isChecked() == true) {
        labelSpace->setVisible(false);

        labelSpace->setVisible(false);

        labelDescObj1->setVisible(true);
        labelDescObj2->setVisible(true);
        labelDescObj3->setVisible(true);
        labelDescObj4->setVisible(true);
        labelDescObj5->setVisible(true);
        labelDescObj6->setVisible(true);
        labelDescObj7->setVisible(true);
        labelDescObj8->setVisible(true);
        labelHumanObjs->setVisible(true);

        // * unsigned is not accepted by QLabel::setNum() ---
        int countObj   = 0;
        int countHuman = 0;
        unsigned int countNew      = 0;
        unsigned int countObject   = 0;
        unsigned int countStaticX  = 0;
        unsigned int countStatic   = 0;
        unsigned int countMissing  = 0;
        unsigned int countCritical = 0;
        unsigned int countWarning  = 0;
        unsigned int countAlarm    = 0;

        unsigned int state = 0;
        static constexpr unsigned int objBytes = 12;

        for (int j = 0; j < 16; ++j) {
            if (blobObject->getSubInfoObjByteArray().at(objBytes * j) == 1) {

                state = blobObject->getSubInfoObjByteArray().at((objBytes * j) + 8);

                ++countObj;

                switch (state) {
                case NEW:
                    ++countNew;
                    break;
                case OBJECT:
                    ++countObject;
                    break;
                case STATICX:
                    ++countStaticX;
                    break;
                case STATIC:
                    ++countStatic;
                    break;
                case MISSING:
                    ++countMissing;
                    break;
                case HUMAN:
                    ++countHuman;
                    break;
                case CRITICAL:
                    ++countCritical;
                    break;
                case WARNING:
                    ++countWarning;
                    break;
                case ALARM:
                    ++countAlarm;
                    break;
                }

                QString labelCountNew;
                labelCountNew.setNum(countNew);
                QString labelCountObject;
                labelCountObject.setNum(countObject);
                QString labelCountStaticX;
                labelCountStaticX.setNum(countStaticX);
                QString labelCountStatic;
                labelCountStatic.setNum(countStatic);
                QString labelCountMissing;
                labelCountMissing.setNum(countMissing);
                QString labelCountCritical;
                labelCountCritical.setNum(countCritical);
                QString labelCountWarning;
                labelCountWarning.setNum(countWarning);
                QString labelCountAlarm;
                labelCountAlarm.setNum(countAlarm);

                labelDescObj1   ->setText("Ne\n" + labelCountNew);
                labelDescObj1   ->setAlignment(Qt::AlignCenter);
                labelDescObj2   ->setText("Ob\n" + labelCountObject);
                labelDescObj2   ->setAlignment(Qt::AlignCenter);
                labelDescObj3   ->setText("St\n" + labelCountStatic);
                labelDescObj3   ->setAlignment(Qt::AlignCenter);
                labelDescObj4   ->setText("Sx\n" + labelCountStaticX);
                labelDescObj4   ->setAlignment(Qt::AlignCenter);
                labelDescObj5   ->setText("Mi\n" + labelCountMissing);
                labelDescObj5   ->setAlignment(Qt::AlignCenter);
                labelDescObj6   ->setText("Cr\n" + labelCountCritical);
                labelDescObj6   ->setAlignment(Qt::AlignCenter);
                labelDescObj7   ->setText("Wa\n" + labelCountWarning);
                labelDescObj7   ->setAlignment(Qt::AlignCenter);
                labelDescObj8   ->setText("Al\n" + labelCountAlarm);
                labelDescObj8   ->setAlignment(Qt::AlignCenter);
                labelHumanObjs  ->setNum(countHuman);

                // *** Label Status ***
                if (countCritical != 0) {
                    labelStatus->setText("Alarm State: CRITICAL");
                    labelStatus->setStyleSheet("QLabel { color: #f5f5f5; background-color: green;}");
                } else {
                    labelStatus->setText("Alarm State: OFF");
                    labelStatus->setStyleSheet("QLabel { color: #666666; background-color: #d6d6d6;}");
                }
            }
        }
        labelCountObj->setNum(countObj);

        displayObj->setPixmap(blobObject->getLayerObjects());
    } else {
        labelSpace->setVisible(true);

        labelDescObj1   ->setVisible(false);
        labelDescObj2   ->setVisible(false);
        labelDescObj3   ->setVisible(false);
        labelDescObj4   ->setVisible(false);
        labelDescObj5   ->setVisible(false);
        labelDescObj6   ->setVisible(false);
        labelDescObj7   ->setVisible(false);
        labelDescObj8   ->setVisible(false);
        labelHumanObjs  ->setVisible(false);

        labelStatus->setText("File opened");
        labelStatus->setStyleSheet("QLabel { background-color : #e7eded; color : #696868; }");

        labelCountObj->setText("");

        displayObj->setPixmap(blobObject->eraseLayer());
    }

    // ********** Floor and Body Lines **********
    if (lineCheckBox->isChecked() == true) {
        displayLines->setPixmap(blobObject->getLayerLines());
    } else {
        displayLines->setPixmap(blobObject->eraseLayer());
    }

}

void Interface::previousFrame() {
    if (blueScaleActivated == true) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->previousImage(blobObject->getInfoByteArray()), QImage::Format_RGBA64)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    } else if (thermalActivated == true){
        QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->previousImage(blobObject->getInfoByteArray()))));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    } else {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->previousImage(blobObject->getInfoByteArray()), QImage::Format_Grayscale16)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
    }

    labelMaxValue->setNum(blobObject->getMaxTempInt());
    labelMinValue->setNum(blobObject->getMinTempInt());

    QString legendImgCount;
    legendImgCount.setNum(blobObject->getBytesIndex());
    labelImgCount->setText(legendImgCount);
}


Interface::~Interface() {
}
