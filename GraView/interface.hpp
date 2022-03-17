#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QGridLayout;
class QCheckBox;
class QSpinBox;
class QVBoxLayout;
class QHBoxLayout;
class QProgressBar;
class Blob;
QT_END_NAMESPACE


class Interface : public QWidget
{
    Q_OBJECT

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

private:
    // * All raw pointers are managed by QWidget the parent. Consequently, when the default destructor is called,
    // * first all child objects are destroyed and then application exits (no need for manual delete)
    Blob *blobObject;

    QProgressBar *slider;

    QSpinBox     *frameSpinBox;

    QPushButton  *openFileButton;
    QPushButton  *nextButton;
    QPushButton  *previousButton;
    QPushButton  *blueScaleButton;
    QPushButton  *thermalScaleButton;
    QPushButton  *grayScaleButton;
    QPushButton  *smoothButton;
    QPushButton  *sharpButton;
    QPushButton  *goToFrameButton;
    QPushButton  *exitButton;
    QPushButton  *infoButton;

    QCheckBox    *lineCheckBox;
    QCheckBox    *objCheckBox;
    QCheckBox    *heatmapCheckBox;

    QLabel       *displayBox;
    QLabel       *displayObj;
    QLabel       *displayLines;
    QLabel       *displayHeatmap;
    QLabel       *labelTitle;
    QLabel       *labelStatus;
    QLabel       *labelDescObj1;
    QLabel       *labelDescObj2;
    QLabel       *labelDescObj3;
    QLabel       *labelDescObj4;
    QLabel       *labelDescObj5;
    QLabel       *labelDescObj6;
    QLabel       *labelDescObj7;
    QLabel       *labelDescObj8;
    QLabel       *labelLegendOpenFile;
    QLabel       *labelLegendNumberFrames;
    QLabel       *labelLegendTotalObjs;
    QLabel       *labelLegendHumanObj;
    QLabel       *labelLegendHumanObjText;
    QLabel       *labelLegendTotalObjsText;
    QLabel       *labelIconFrameNr;
    QLabel       *labelLegendImg;
    QLabel       *labelLegendImgText;
    QLabel       *labelLegendSizeArray;
    QLabel       *labelLegendSizeArrayText;
    QLabel       *labelMaxValue;
    QLabel       *labelLegendMax;
    QLabel       *labelLegendMin;
    QLabel       *labelLegendMaxText;
    QLabel       *labelLegendMinText;
    QLabel       *labelMinValue;
    QLabel       *labelCountObj;
    QLabel       *labelSizeArray;
    QLabel       *labelImgCount;
    QLabel       *labelHumanObjs;
    QLabel       *labelSpace;
    QLabel       *labelSpan;

    QGridLayout  *mainLayout;
    QGridLayout  *frameDescriptionLayout;
    QVBoxLayout  *totalBytesLayout;
    QVBoxLayout  *byteIndexLayout;
    QVBoxLayout  *numberFramesLayout;
    QGridLayout  *frameImgLayout;
    QHBoxLayout  *subMenuLayout;
    QVBoxLayout  *humanObjsLayout;
    QVBoxLayout  *totalObjsLayout;
    QVBoxLayout  *welcomeMenuBarLayout;
    QVBoxLayout  *imgAndStatusLayout;
    QVBoxLayout  *minTempLayout;
    QVBoxLayout  *maxTempLayout;
    QGridLayout  *imgLayersLayout;
    QGridLayout  *rightFooterLayout;

    bool blueScaleActivated      = false;
    bool thermalActivated        = true;
    bool grayScaleActivated      = false;
    bool smoothFilterActivated   = false;

    static constexpr unsigned int correctTotalBytesA    = 130995;
    static constexpr unsigned int correctTotalBytesB    = 130994;

private slots:
    void applySharpFilter();
    void applySmoothFilter();
    void closeApp();
    void displayApp();
    void displayBlueScale();
    void displayFrameFromSpinBox();
    void displayGrayScale();
    void displayThermal();
    void infoMessage();
    void nextFrame();
    void paintEvent(QPaintEvent *);
    void previousFrame();
};

#endif // INTERFACE_H
