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
#include <QLineEdit>
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
    setWindowIcon(QIcon(":/icons/hdpiGraView.ico"));
    setWindowTitle(tr("GraView"));

    labelStatus                  = new QLabel;
    labelTitle                   = new QLabel;
    displayBox                   = new QLabel;
    displayObj                   = new QLabel;
    displayLines                 = new QLabel;
    displayHeatmap               = new QLabel;
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
    heatmapCheckBox              = new QCheckBox;

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


    // *** First icons to be displayed in Welcome Window ***
    QIcon openIcon(QPixmap(":/icons/resources/folder.ico"));
    openFileButton      ->setIcon(openIcon);
    openFileButton      ->setIconSize(QSize(240, 240));
    openFileButton      ->setStyleSheet("QPushButton { border: 10px solid transparent; border-radius: 15px;}");

    QIcon exitIcon(QPixmap(":/icons/resources/close.ico"));
    exitButton          ->setIcon(exitIcon);
    exitButton          ->setIconSize(QSize(120, 120));
    exitButton          ->setStyleSheet("QPushButton { border: 10px solid transparent; border-radius: 15px;}");

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

    // ***
    displayBox->setPixmap(QPixmap(":/logo/resources/logo.png").scaledToHeight(600));

    displayBox->setAlignment(Qt::AlignCenter);


    mainLayout           = new QGridLayout;
    welcomeMenuBarLayout = new QVBoxLayout;
    imgAndStatusLayout   = new QVBoxLayout;
    imgLayersLayout      = new QGridLayout;

    welcomeMenuBarLayout->addWidget(openFileButton);
    welcomeMenuBarLayout->addWidget(exitButton);

    imgLayersLayout->addWidget(displayBox,     0, 0);
    imgLayersLayout->addWidget(displayHeatmap, 0, 0);
    imgLayersLayout->addWidget(displayLines,   0, 0);
    imgLayersLayout->addWidget(displayObj,     0, 0);

    imgAndStatusLayout->addWidget(labelTitle);
    imgAndStatusLayout->addLayout(imgLayersLayout);

    mainLayout->addLayout(imgAndStatusLayout,   0, 0);
    mainLayout->addLayout(welcomeMenuBarLayout, 0, 1);
    setLayout(mainLayout);
}

void Interface::applySharpFilter() {
    // --- UX selected button ---
    sharpButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #7570ff; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    smoothButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B5B3ff; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    smoothFilterActivated = false;
    if (blobObject->getSpinBoxActivated() == false) {
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
    } else {
        if (blueScaleActivated == true) {
            QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoSpinByteArray(), QImage::Format_RGBA64)));
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
        } else if (grayScaleActivated == true) {
            QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoSpinByteArray(), QImage::Format_Grayscale16)));
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));
        } else if (thermalActivated == true ) {
            QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->getSubInfoSpinByteArray())));
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio));
        }
    }
}

void Interface::applySmoothFilter() {
    // --- UX selected button ---
    smoothButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #7570ff; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    sharpButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B5B3ff; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    smoothFilterActivated = true;
    if (blobObject->getSpinBoxActivated() == false) {
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
    } else {
        if (blueScaleActivated == true) {
            QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoSpinByteArray(), QImage::Format_RGBA64)));
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else if (grayScaleActivated == true) {
            QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoSpinByteArray(), QImage::Format_Grayscale16)));
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else if (thermalActivated == true ) {
            QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->getSubInfoSpinByteArray())));
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void Interface::closeApp() {
    // --- UX pressed button ---
    exitButton->setStyleSheet("QPushButton { background-color : #cccccc; border: none }");
    // *** qApp is a global pointer to QApplication ***
    qApp->quit();
}

void Interface::displayApp() {
    // --- UX pressed button ---
    openFileButton->setStyleSheet("QPushButton { background-color : #cccccc; border: 10px solid transparent; border-radius: 15px;}");

    // ********** Reset values to default values: frame n°1, thermal colors, sharp filter **********
    heatmapCheckBox         ->setCheckState(Qt::Unchecked);
    blobObject              ->resetLayerArray();
    blobObject              ->setSpinBoxActivated(false);
    blobObject              ->setImgCount(1);
    blobObject              ->setBytesIndex(0);
    blobObject              ->setNumberFrames(0);
    blobObject              ->setResult(1);
    blobObject              ->setRewFwdCount(blobObject->getHeaderBytes());
    frameSpinBox            ->setValue(blobObject->getImgCount());
    slider                  ->setValue(blobObject->getImgCount());
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

    // ********** Button / Icon Management **********
    openFileButton->setIconSize(QSize(90, 90));
    openFileButton->setStyleSheet("QPushButton { background-color: #fdfdfd; border: 6px solid transparent; border-radius: 15px;}");

    exitButton->setIconSize(QSize(70, 70));
    exitButton->setStyleSheet("QPushButton { background-color: #fdfdfd; padding-top: 10px ; padding-bottom: 10px; border: 6px solid transparent; border-radius: 15px;}");

    QIcon previousIcon(QPixmap(":/icons/left.ico"));
    previousButton->setIcon(previousIcon);
    previousButton->setIconSize(QSize(70, 70));
    previousButton->setStyleSheet("QPushButton { background-color: #fdfdfd; padding-top: 10px ; padding-bottom: 10px; border: 6px solid transparent; border-radius: 15px;}");

    QIcon nextIcon(QPixmap(":/icons/right.ico"));
    nextButton->setIcon(nextIcon);
    nextButton->setIconSize(QSize(70, 70));
    nextButton->setStyleSheet("QPushButton { background-color: #fdfdfd; padding-top: 10px ; padding-bottom: 10px; border: 6px solid transparent; border-radius: 15px;}");

    QIcon refreshIcon(QPixmap(":/icons/goto.ico"));
    goToFrameButton->setIcon(refreshIcon);
    goToFrameButton->setIconSize(QSize(90, 90));
    goToFrameButton->setStyleSheet("QPushButton { background-color: #fdfdfd; padding-top: 10px ; padding-bottom: 10px; border: 6px solid transparent; border-radius: 15px;}");

    QIcon grayIcon(QPixmap(":/icons/grayscale.ico"));
    grayScaleButton->setIcon(grayIcon);
    grayScaleButton->setIconSize(QSize(90, 90));
    grayScaleButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    QIcon thermalIcon(QPixmap(":/icons/thermal.ico"));
    thermalScaleButton->setIcon(thermalIcon);
    thermalScaleButton->setIconSize(QSize(90, 90));
    thermalScaleButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #85FF85; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    QIcon smoothIcon(QPixmap(":/icons/smooth.ico"));
    smoothButton->setIcon(smoothIcon);
    smoothButton->setIconSize(QSize(90, 90));
    smoothButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B5B3ff; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    QIcon sharpIcon(QPixmap(":/icons/sharpness.ico"));
    sharpButton->setIcon(sharpIcon);
    sharpButton->setIconSize(QSize(90, 90));
    sharpButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #7570ff; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    QIcon blueIcon(QPixmap(":/icons/bluescale.ico"));
    blueScaleButton->setIcon(blueIcon);
    blueScaleButton->setIconSize(QSize(75, 75));
    blueScaleButton->setStyleSheet("QPushButton { padding-top: 17px ; padding-bottom: 17px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    QIcon objIcon(QPixmap(":/icons/object.ico"));
    objCheckBox->setIcon(objIcon);
    objCheckBox->setIconSize(QSize(90, 90));

    QIcon lineIcon(QPixmap(":/icons/lines.ico"));
    lineCheckBox->setIcon(lineIcon);
    lineCheckBox->setIconSize(QSize(90, 90));

    QIcon heatmapIcon(QPixmap(":/icons/heatmap.ico"));
    heatmapCheckBox->setIcon(heatmapIcon);
    heatmapCheckBox->setIconSize(QSize(90, 90));

    QIcon infoIcon(QPixmap(":/icons/info.ico"));
    infoButton->setIcon(infoIcon);
    infoButton->setIconSize(QSize(80, 80));
    infoButton->setStyleSheet("QPushButton { background-color: #fdfdfd; padding-top: 5px ; padding-bottom: 5px; border: 6px solid transparent; border-radius: 15px;}");

    // ********** Label Management **********
    labelTitle ->setPixmap(QPixmap(":/logo/logoGraViewTitle.png").scaledToWidth(1200));
    labelStatus->setText("File opened");
    labelStatus->setAlignment(Qt::AlignCenter);
    labelStatus->setStyleSheet("QLabel { background-color : #e7eded; color : #696868; }");

    labelLegendSizeArrayText->setText("Total Bytes:");
    QString legendSizeArray;
    legendSizeArray.setNum(blobObject->getIntArraySize());
    labelSizeArray->setText(legendSizeArray);

    labelLegendNumberFrames->setText("N° Frame");
    labelLegendNumberFrames->setStyleSheet("QLabel { font-size: 18px; color: #868686; }");
    labelLegendNumberFrames->setAlignment(Qt::AlignCenter);

    labelLegendTotalObjsText->setText("Total Objs.");
    labelLegendTotalObjsText->setStyleSheet("QLabel { font-size: 18px; color: #868686; }");
    labelLegendTotalObjsText->setAlignment(Qt::AlignCenter);
    labelCountObj           ->setStyleSheet("color: #868686;");

    labelLegendHumanObjText->setText("Human Obj.");
    labelLegendHumanObjText->setStyleSheet("QLabel { font-size: 18px; color: #868686; }");
    labelLegendHumanObjText->setAlignment(Qt::AlignCenter);

    labelLegendImgText->setText("Byte Index");
    labelLegendImgText->setStyleSheet("font-size: 18px; color: #868686;");
    labelLegendImgText->setAlignment(Qt::AlignCenter);
    labelImgCount     ->setStyleSheet("color: #868686;");
    QString legendImgCount;
    legendImgCount.setNum(blobObject->getHeaderBytes());
    labelImgCount    ->setText(legendImgCount);

    labelLegendSizeArrayText->setText("Total Bytes");
    labelLegendSizeArrayText->setStyleSheet("font-size: 18px; color: #868686;");
    labelLegendSizeArrayText->setAlignment(Qt::AlignCenter);
    labelSizeArray          ->setStyleSheet("color: #868686;");

    labelLegendMaxText->setText("Max Temp:");
    labelLegendMaxText->setStyleSheet("font-size: 18px; color: #868686;");
    labelLegendMaxText->setAlignment(Qt::AlignCenter);
    labelMaxValue     ->setStyleSheet("color: #868686;");
    labelMaxValue     ->setNum(blobObject->getMaxTempInt());

    labelLegendMinText ->setText("Min Temp:");
    labelLegendMinText ->setStyleSheet("font-size: 18px; color: #868686;");
    labelLegendMinText ->setAlignment(Qt::AlignCenter);
    labelMinValue      ->setStyleSheet("color: #868686;");
    labelMinValue      ->setNum(blobObject->getMinTempInt());

    // *** Label / Icon Management ***
    labelLegendImg      ->setPixmap(QPixmap(":/icons/booleangrey.ico"));
    labelLegendImg      ->setAlignment(Qt::AlignCenter);
    labelLegendSizeArray->setPixmap(QPixmap(":/icons/chipgrey.ico"));
    labelLegendSizeArray->setAlignment(Qt::AlignCenter);
    labelLegendTotalObjs->setPixmap(QPixmap(":/icons/totalobjsgrey.ico"));
    labelLegendTotalObjs->setAlignment(Qt::AlignCenter);
    labelLegendHumanObj ->setPixmap(QPixmap(":/icons/humangrey.ico"));
    labelLegendHumanObj ->setAlignment(Qt::AlignCenter);
    labelIconFrameNr    ->setPixmap(QPixmap(":/icons/frame.ico"));
    labelIconFrameNr    ->setFixedHeight(80);
    labelIconFrameNr    ->setAlignment(Qt::AlignCenter);
    labelLegendMin      ->setPixmap(QPixmap(":/icons/mintempgrey.ico"));
    labelLegendMin      ->setAlignment(Qt::AlignCenter);
    labelLegendMax      ->setPixmap(QPixmap(":/icons/maxtempgrey.ico"));
    labelLegendMax      ->setAlignment(Qt::AlignCenter);

    // *** SpinBox features ***
    frameSpinBox->setValue(blobObject->getImgCount());
    frameSpinBox->setMaximum(blobObject->getNumberFrames());
    frameSpinBox->setMinimum(1);
    frameSpinBox->setStyleSheet("QSpinBox { font-size: 85px; }");

    QLineEdit *frameLineEdit(frameSpinBox->findChild<QLineEdit *>());
    frameLineEdit->setReadOnly(true);
    frameLineEdit->setFocusPolicy(Qt::NoFocus);


    // *** Slider features ***
    slider->setMaximumWidth(displayBox->width());
    slider->setMaximum(blobObject->getNumberFrames());

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
    sideMenuBar->addWidget(heatmapCheckBox,    1, 6, 1, 2);

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
    // --- UX selected button ---
    blueScaleButton     ->setStyleSheet("QPushButton { padding-top: 17px ; padding-bottom: 17px; background-color: #85FF85; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    grayScaleButton     ->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    thermalScaleButton  ->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    if (blobObject->getSpinBoxActivated() == false) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_RGBA64)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(),Qt::KeepAspectRatio));
    } else {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoSpinByteArray(), QImage::Format_RGBA64)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(),Qt::KeepAspectRatio));
    }
    blueScaleActivated = true;
    thermalActivated   = false;
    grayScaleActivated = false;
}

void Interface::displayFrameFromSpinBox() {
    // --- UX pressed button ---
    goToFrameButton->setStyleSheet("QPushButton { background-color : #cccccc; padding-top: 10px ; padding-bottom: 10px; border: 6px solid transparent; border-radius: 15px;}");
    blobObject->setResult(frameSpinBox->value());
    blobObject->getSpinBoxValue();

    if (blueScaleActivated == true)
        displayBlueScale();
    else if (thermalActivated == true)
        displayThermal();
    else if (grayScaleActivated == true)
        displayGrayScale();

    // *** Setting max and min temperature ***
    labelMaxValue->setNum(blobObject->getMaxTempInt());
    labelMinValue->setNum(blobObject->getMinTempInt());
    // *** Setting progress bar ***
    slider->setValue(blobObject->getImgCount());
    // *** Setting byte index ***
    QString legendImgCount;
    legendImgCount.setNum(blobObject->getBytesIndexSpin());
    labelImgCount->setText(legendImgCount);

    // --- UX released button ---
    goToFrameButton->setStyleSheet("QPushButton { background-color : #fdfdfd; padding-top: 10px ; padding-bottom: 10px; border: 6px solid transparent; border-radius: 15px;}");
}

void Interface::displayGrayScale() {
    // --- UX selected button ---
    grayScaleButton    ->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #85FF85; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    thermalScaleButton ->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    blueScaleButton    ->setStyleSheet("QPushButton { padding-top: 17px ; padding-bottom: 17px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    if (blobObject->getSpinBoxActivated() == false) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoByteArray(), QImage::Format_Grayscale16)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));
    } else {
        QPixmap p(QPixmap::fromImage(blobObject->getImageBlueOrGrayScale(blobObject->getSubInfoSpinByteArray(), QImage::Format_Grayscale16)));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(), displayBox->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));
    }
    blueScaleActivated = false;
    thermalActivated   = false;
    grayScaleActivated = true;
}

void Interface::displayThermal() {
    // --- UX selected button ---
    thermalScaleButton ->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #85FF85; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    grayScaleButton    ->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");
    blueScaleButton    ->setStyleSheet("QPushButton { padding-top: 17px ; padding-bottom: 17px; background-color: #B3FFB3; border-style: solid; border-color: transparent; border-width: 4px; border-radius: 15px; }");

    if (blobObject->getSpinBoxActivated() == false) {
        QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->getSubInfoByteArray())));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));
    } else {
        QPixmap p(QPixmap::fromImage(blobObject->getImageThermalScale(blobObject->getSubInfoSpinByteArray())));
        if (smoothFilterActivated == true)
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            displayBox->setPixmap(p.scaled(displayBox->width(),displayBox->height(),Qt::KeepAspectRatio));
    }
    blueScaleActivated = false;
    thermalActivated   = true;
    grayScaleActivated = false;
}

void Interface::infoMessage() {
    // --- UX pressed button ---
    infoButton->setStyleSheet("QPushButton { padding-top: 5px ; padding-bottom: 5px; background-color : #cccccc; border: 6px solid transparent; border-radius: 15px;}");
    QMessageBox::information(this, tr("About"), tr("GraView for Android 11\nQWidget-App using Qt 5.15.2\nVersion 1.0\n\nFor further info, please visit:\nhttps://github.com/lavf/graview"), QMessageBox::Ok);
    // --- UX released button ---
    infoButton->setStyleSheet("QPushButton { padding-top: 5px ; padding-bottom: 5px; background-color : #fdfdfd; border: 6px solid transparent; border-radius: 15px;}");
}

void Interface::nextFrame() {
    // --- UX pressed button ---
    nextButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color : #cccccc; border: 6px solid transparent; border-radius: 15px; }");

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

    slider       ->setValue(blobObject->getImgCount());
    frameSpinBox ->setValue(blobObject->getImgCount());

    // --- UX released button ---
    nextButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color : #fdfdfd; border: 6px solid transparent; border-radius: 15px;}");
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
                labelDescObj1   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid yellow; color: gray;}");
                labelDescObj2   ->setText("Ob\n" + labelCountObject);
                labelDescObj2   ->setAlignment(Qt::AlignCenter);
                labelDescObj2   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid yellow; color: gray;}");
                labelDescObj3   ->setText("St\n" + labelCountStatic);
                labelDescObj3   ->setAlignment(Qt::AlignCenter);
                labelDescObj3   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid magenta; color: gray;}");
                labelDescObj4   ->setText("Sx\n" + labelCountStaticX);
                labelDescObj4   ->setAlignment(Qt::AlignCenter);
                labelDescObj4   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid blue; color: gray;}");
                labelDescObj5   ->setText("Mi\n" + labelCountMissing);
                labelDescObj5   ->setAlignment(Qt::AlignCenter);
                labelDescObj5   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid white; color: gray;}");
                labelDescObj6   ->setText("Cr\n" + labelCountCritical);
                labelDescObj6   ->setAlignment(Qt::AlignCenter);
                labelDescObj6   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid darkgreen; color: gray;}");
                labelDescObj7   ->setText("Wa\n" + labelCountWarning);
                labelDescObj7   ->setAlignment(Qt::AlignCenter);
                labelDescObj7   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid darkred; color: gray;}");
                labelDescObj8   ->setText("Al\n" + labelCountAlarm);
                labelDescObj8   ->setAlignment(Qt::AlignCenter);
                labelDescObj8   ->setStyleSheet("QLabel { font-size: 50px; border: 3px solid red; color: gray;}");
                labelHumanObjs  ->setNum(countHuman);
                labelHumanObjs  ->setStyleSheet("QLabel { color: #868686;}");

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

    // ********** Heatmap **********
    if (heatmapCheckBox->isChecked() == true) {
        if (labelHumanObjs->isVisible() == false)
            labelHumanObjs->setVisible(true);

        QPixmap  pixmapFromLayer = blobObject->getLayerHeatmap();
        QPainter painterHeatmap(&pixmapFromLayer);

        int                             countHuman = 0;
        unsigned int                    state      = 0;
        static constexpr unsigned int   objBytes   = 12;
        double                          pixelSize  = 17;

        for (int j = 0; j < 16; ++j) {
            if (blobObject->getSubInfoObjByteArray().at(objBytes * j) == 1) {
                state = blobObject->getSubInfoObjByteArray().at((objBytes * j) + 8);
                if (state == 10) {
                    ++countHuman;
                }
                labelHumanObjs->setNum(countHuman);
                labelHumanObjs->setStyleSheet("QLabel { color: #868686;}");
            }
        }

        QPen penNew(Qt::white, pixelSize, Qt::SolidLine);
        painterHeatmap.setPen(penNew);
        painterHeatmap.setOpacity(0.4);

        for (unsigned int i = 0; i < 104; ++i) {
            if ( blobObject->getNewLayerArray(i * 2) != 0 &&
                 blobObject->getNewLayerArray((i * 2) + 1) != 0 ) {
                painterHeatmap.drawPoint(blobObject->getNewLayerArray(i * 2), blobObject->getNewLayerArray((i * 2) + 1));
            }
        }
        displayHeatmap->setPixmap(pixmapFromLayer);
    } else {
        if (objCheckBox->isChecked() == true)
            labelHumanObjs->setVisible(true);
        else
            labelHumanObjs->setVisible(false);
        displayHeatmap->setPixmap(blobObject->eraseLayer());
    }
}

void Interface::previousFrame() {
    // --- UX pressed button ---
    previousButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color : #cccccc; border: 6px solid transparent; border-radius: 15px;}");

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

    slider      ->setValue(blobObject->getImgCount());
    frameSpinBox->setValue(blobObject->getImgCount());

    // --- UX released button ---
    previousButton->setStyleSheet("QPushButton { padding-top: 10px ; padding-bottom: 10px; background-color : #fdfdfd; border: 6px solid transparent; border-radius: 15px; }");
}


Interface::~Interface() {
}
