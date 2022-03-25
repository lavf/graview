# Developer Documentation

&nbsp;

GraView version 1.0 is a Qt Widgets-based App for Android 11 in C++11. Qt 5.12 used in this application under the terms of GNU Lesser General Public License for Open-Source projects with educational purposes.

The GraView OOP-structure is as follows:

- <a href="#interface-class">Interface Class</a>
    - <a href="#interface-constructor">Interface Constructor</a>
    - <a href="#interface-private-slots">Interface Private Slots</a>
- <a href="#blob-class">Blob Class</a>
    -  <a href="#blob-constructor">Blob Constructor</a>
    -  <a href="#blob-public-methods">Blob Public Methods</a>
        -  <a href="#blob-setters-and-getters">Blob Setters and Getters</a>
        -  <a href="#blob-methods">Blob Methods</a>
            -  <a href="#methods-from-the-composition-between-blob-and-image">Methods from the composition between Blob and Image</a>
            -  <a href="#methods-from-the-composition-between-blob-and-layer">Methods from the composition between Blob and Layer</a>
    -  <a href="#blob-private-methods">Blob Private Methods</a>     
- <a href="#image-class">Image Class</a>
    - <a href="#image-public-methods">Image Public Methods</a>
    - <a href="#image-private-methods">Image Private Methods</a>
- <a href="#layer-class">Layer Class</a>
    - <a href="#layer-public-methods">Layer Public Methods</a>
        - <a href="#layer-setter-and-getter">Layer Setter and Getter</a>
        - <a href="#layer-methods">Layer Methods</a>
    - <a href="#layer-private-method">Layer Private Method</a>

&nbsp;

## Interface Class

The Interface Class implements the Qt Widgets module, that is to say, it contains the QWidget class, which is the base class for all GUI objects. All raw pointers of this class are managed by the QWidget 'the parent'. Consequently, when the default destructor is called, first all child objects are destroyed and then the application exits (no need for manual delete).

```c++
class Interface : public QWidget
{
    Q_OBJECT
    ...
}
```
&nbsp;
### Interface Constructor

```c++
Interface::Interface(QWidget *parent)
    : QWidget(parent),
      blobObject(new Blob) {
}
```

&nbsp;
### Interface Private Slots

```c++
void displayApp();    
```

This slot displays the layout and all GUI objects when a file is selected.

&nbsp;
```c++
void paintEvent(QPaintEvent *);
```

This slot is in charge of exposing layers over the image. Therefore, is constantly updated even if there is nothing to draw. Most of the process could not be transferred to the Layer class since QPainter must be implemented in this method.

&nbsp;
```c++
void displayBlueScale();
```

This slot obtains the bluescale image (QPixmap) by calling first the method ```QImage getImageBlueOrGrayScale(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64)``` from the Blob Object, which then calls the method ```QImage blueOrGrayScaleImage(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64)``` from the Image Object.

&nbsp;
```c++
void displayThermal();
```

This slot obtains the thermal scale image (QPixmap) by calling first the method ```QImage getImageThermalScale(QByteArray dynamicSubInfo)``` from the Blob Object, which then calls the method ```QImage thermalImage(QByteArray dynamicSubInfo)``` from the Image Object.

&nbsp;
```c++
void displayGrayScale();
```

This slot obtains the bluescale image (QPixmap) by calling first the method ```QImage getImageBlueOrGrayScale(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64)``` from the Blob Object, which then calls the method ```QImage blueOrGrayScaleImage(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64)``` from the Image Object.

&nbsp;
```c++
void nextFrame();
```

This slot displays the next image and its data. This is made by calling methods to generate the image such as ```QImage getImageBlueOrGrayScale(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64)``` or ```QImage getImageThermalScale(QByteArray dynamicSubInfo)``` using the byte array calculated to be the next image, i. e. next 2519 bytes (image bytes + data block bytes).

&nbsp;
```c++
void previousFrame();
```

This slot displays the previous image and its data. This is made by calling methods to generate the image such as ```QImage getImageBlueOrGrayScale(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64)``` or ```QImage getImageThermalScale(QByteArray dynamicSubInfo)``` using the byte array calculated to be the previous image, i. e. previous 2519 bytes (image bytes + data block bytes).

&nbsp;
```c++
void closeApp();
```

This slot uses the global pointer ```qApp``` and calls its static slot ```quit()``` to exit the application.

&nbsp;
```c++
void applySmoothFilter();
```

This slot applies a smooth filter on the image (QPixmap) by using the direct constructor ```QPixmap(QPlatformPixmap *data)```. Consequently, the image must be created from scratch as per selected color scale and it is not possible to implement the existing image.


&nbsp;
```c++
void applySharpFilter();
```

This slot applies a sharp filter on the image (QPixmap) by using the direct constructor ```QPixmap(QPlatformPixmap *data)```. Consequently, the image must be created from scratch as per selected color scale and it is not possible to implement the existing image.

&nbsp;
```c++
void infoMessage();
```

This slot calls the info message (QMessageBox), which displays general info about the application. On the desktop version of GraView, the info icon is displayed but in the mobile version, the warning icon (possible QtWidgets bug).

&nbsp;
```c++
void displayFrameFromSpinBox();
```

This slot obtains the value of the Spin Box, then generates the image by cross-referencing the value and calculating the corresponding byte array. In this manner, all related data to this byte array is displayed and GUI is adapted accordingly.

<a href="#developer-documentation">^ Go to top</a>

&nbsp;

## Blob Class

There is an aggregation between the Blob class and the class "Interface", which contains GUI objects. This means that an Interface object HAS a Blob object but there is no dependency between each other. A Blob object can exist without instantiating the Interface class and can be used by another GUI class if needed.

```c++
class Blob {
}
```

&nbsp;
### Blob Constructor

```c++
Blob::Blob()
    : imageObject(new Image),
      layerObject(new Layer) {
}
```

&nbsp;
### Blob Public Methods

#### Blob Setters and Getters

The following methods are setters and getters since this Blob class acts as a Controller and at the same time Model (see reference MVC-Architecture).

```c++
void setIntArraySize(unsigned int x) {
    intArraySize = x;
};

unsigned int& getIntArraySize() {
    return intArraySize;
};
```

```c++
void setSubInfoByteArray(QByteArray x) {
    subInfo.replace(0, x.size(), x);
};

QByteArray& getSubInfoByteArray() {
    return subInfo;
}
```

```c++
void setInfoByteArray(QByteArray x) { 
    info.replace(0, x.size(), x);
};

QByteArray& getInfoByteArray() { 
    return info; 
};
```

```c++
void setSubInfoObjByteArray(QByteArray x) { 
    subInfoObj.replace(0, x.size(), x); 
};

QByteArray& getSubInfoObjByteArray() { 
    return subInfoObj; 
};
```

```c++
void setSubInfoBodyByteArray(QByteArray x) { 
    subInfoBodyLine.replace(0, x.size(), x); 
};

QByteArray& getSubInfoBodyByteArray() { 
    return subInfoBodyLine;
};
```

```c++
void setSubInfoFloorByteArray(QByteArray x) { 
    subInfoFloorLine.replace(0, x.size(), x); 
};

QByteArray& getSubInfoFloorByteArray() { 
    return subInfoFloorLine;
};
```

```c++
void setSubInfoSpinByteArray(QByteArray x) { 
    subInfoSpin.replace(0, x.size(), x); 
};

QByteArray& getSubInfoSpinByteArray() { 
    return subInfoSpin; 
};
```

```c++
void setRewFwdCount(unsigned int x) { 
    rewFwdCount = x; 
};

unsigned int& getRewFwdCount() { 
    return rewFwdCount; 
};
```

```c++
void setImgCount(unsigned int x) { 
    imgCount = x; 
};

unsigned int& getImgCount() { 
    return imgCount; 
};
```

```c++
void setBytesIndex(unsigned int x) { 
    bytesIndex = x; 
};

unsigned int& getBytesIndex() { 
    return bytesIndex; 
};
```

```c++
void setNumberFrames(unsigned int x) { 
    numberFrames = x; 
};

unsigned int& getNumberFrames() { 
    return numberFrames; 
};
```

```c++
void setHeaderBytes(unsigned int x) { 
    headerBytes = x; 
};

unsigned int& getHeaderBytes() { 
    return headerBytes; 
};
```

```c++
void setResult(unsigned int x) { 
    result = x; 
};

unsigned int& getResult() { 
    return result; 
};
```

```c++
void setBytesIndexSpin(unsigned int x) { 
    bytesIndexSpin = x; 
};

unsigned int& getBytesIndexSpin() { 
    return bytesIndexSpin; 
};
```

```c++
void setRewFwdCountSpin(unsigned int x) { 
    rewFwdCountSpin = x; 
};

unsigned int& getRewFwdCountSpin() { 
    return rewFwdCountSpin; 
};
```

```c++
void setMaxTempInt(unsigned int x) { 
    maxTempInt = x; 
};

int& getMaxTempInt() { 
    return maxTempInt; 
};
```

```c++
void setMinTempInt(unsigned int x) { 
    minTempInt = x; 
};

int& getMinTempInt() { 
    return minTempInt; 
};
```

```c++
void setSpinBoxActivated(unsigned int x) { 
    spinBoxActivated = x; 
};

bool& getSpinBoxActivated() { 
    return spinBoxActivated; 
};
```
&nbsp;
```c++
unsigned int& getNewLayerArray(unsigned int x);

void resetLayerArray();
```
This getter ```getNewLayerArray``` calls the getter ```unsigned int& getNewLayerArray(unsigned int x)``` from the Layer class (for further information see <a href="#layer-setter-and-getter">Layer Setter and Getter</a>). The resetter calls the method ```void resetArrayLayer()``` from the Layer class (for further information see <a href="#layer-methods">Layer Methods</a>)

<a href="#developer-documentation">^ Go to top</a>
&nbsp;

#### Blob Methods


```c++
QByteArray openFile(QString *fileName);
```

This method uses the interface QFile to open the file and read it. Here the flag is set to "read only" when opening so the no changes in the file are made. All the content is read and returned as  a byte array. This method delivers only the array for the first image (default image when opening a file). Nevertheless, array with all content is saved in the memory as "info" array.

&nbsp;
```c++
QByteArray nextImage(QByteArray info);
```

This method provides the byte array for the next image depending on if spin box is used or not, and is called by the method ```void nextFrame()``` from the Interface class.

&nbsp;
```c++
QByteArray previousImage(QByteArray info);
```

This method provides the byte array for the previous image depending on if spin box is used or not, and is called by the method ```void previousFrame()``` from the Interface class.

&nbsp;
```c++
void extractObjArray(QByteArray info);
```

This method extracts the object byte array, which contains data about the objects, and set the byte array ```subInfoObj```.


&nbsp;
```c++
void extractLineArray(QByteArray info);
```

This method extracts the floor and body lines byte arrays, which contains data about the floor and body lines, and sets both byte arrays ```subInfoFloor``` and ```subInfoBody``` accordingly.

&nbsp;
```c++
void getSpinBoxValue();
```

This method obtains the value from the spin box, then calculates and sets the byte array for the corresponding image. In this manner, the boolean ```spinBoxActivated``` is set to *true* and the only different calculation is when the value of the spin box is 1. This is due to the first image has a header, which includes 477 bytes.


&nbsp;
#### Methods from the composition between Blob and Image

```c++
QImage getImageThermalScale(QByteArray dynamicSubInfo);
```   

This method calls the method ```QImage thermalImage(QByteArray dynamicSubInfo)``` from the Image class.

&nbsp;
```c++
QImage getImageBlueOrGrayScale(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64);
```

This method calls the method ```QImage blueOrGrayScaleImage(QByteArray dynamicSubInfo, QImage::Format grayscaleOrRgba64)``` from the Image class.


&nbsp;
#### Methods from the composition between Blob and Layer

```c++
QPixmap getLayerLines();
```

This method calls the method ```QPixmap drawLines(QByteArray subInfoBodyLine, QByteArray subInfoFloorLine)``` from the Layer class.


&nbsp;
```c++
QPixmap getLayerObjects();
```

This method calls the method ```QPixmap drawObjects(QByteArray subInfoObj)``` from the Layer class.

&nbsp;
```c++
QPixmap getLayerHeatmap();
```

This method calls the method ```QPixmap drawHeatmap(QByteArray subInfoObj, unsigned int imgCount)``` from the Layer class.


&nbsp;
```c++
QPixmap eraseLayer();
```

This method calls the method ```QPixmap eraseDrawings()``` from the Layer class.


&nbsp;
### Blob Private Methods
```c++
void calculateNextImageBytes(QByteArray info);
```

This method calculates the byte array containing next image data and sets the arguments such as: ```byteIndex``` and ```subInfo```.

&nbsp;
```c++
void calculateNextFrame(unsigned int rewFwd, QByteArray info);
```

This method cross-references the values from the GUI (number of frame, etc.) and calculates ```byteIndex``` of the next image. Then it calls ```void calculateNextImageBytes(QByteArray info)``` and ```void calculateTemperature(QByteArray info, unsigned int rewFwd, unsigned int bytesIndex)``` to display image and temperatures in the GUI.


&nbsp;
```c++
void calculatePreviousImageBytes(QByteArray info);
```

This method calculates the byte array containing previous image data and sets the arguments such as: ```byteIndex``` and ```subInfo```.

&nbsp;
```c++
void calculatePreviousFrame(unsigned int rewFwd, QByteArray info);
```

This method cross-references the values from the GUI (number of frame, etc.) and calculates ```byteIndex``` of the previous image. Then it calls ```void calculatePreviousImageBytes(QByteArray info)``` and ```void calculateTemperature(QByteArray info, unsigned int rewFwd, unsigned int bytesIndex)``` to display image and temperatures in the GUI.

&nbsp;
```c++
void calculateTemperature(QByteArray info, unsigned int rewFwd, unsigned int bytesIndex);
```

This method calculates the minimal and maximal temperature displayed in the image by calculating the relevant bytes in the little endian byte order.

<a href="#developer-documentation">^ Go to top</a>

&nbsp;

## Image Class

The Image class has a relationship of Composition with the "Blob" class. Every Blob object has an Image object and an Image object cannot be instantiated without instantiating the Blob class.
In this class, the pixels are created and colors are assigned repectively. Thermal colors were customized and every color is unique, therefore the scale does not follow a logic.
With regards to gray scale and blue scale, the gray scale uses the same color scale from the blue scale but applies a different format provided by QImage "GrayScale". 
The default constructor and destructor provided by compiler are used implicitly. 

```c++
class Image {
}
```

&nbsp;
### Image Public Methods
```c++
QImage blueOrGrayScaleImage(QByteArray subInfo, QImage::Format grayscaleOrRgba64);
```
This method basically generates a QImage and then calls the method ```QImage grayOrBlueScaleAssignation(QByteArray subInfo, QImage displayImage)``` and returns a QImage.


&nbsp;
```c++
QImage thermalImage(QByteArray subInfo);
```

This method generates a QImage and contains an array with RGB values (33 thermal colors). Then it calls the method ```QImage colorAssignation(QByteArray subInfo, QImage displayImage, QRgb colors [33])``` and returns a QImage.

&nbsp;
### Image Private Methods
```c++
QImage colorAssignation(QByteArray subInfo, QImage displayImage, QRgb colors [33]);
```

This method uses a main for-loop and a nested for-loop to assign Rgb values to every pixel. The main for-loop counts till the image height is reached (32) and the nested for-loop till the image width (64). Every color belongs to a range of temperature. If the pixel temperature is within this range, color is assigned.

&nbsp;
```c++
QImage grayOrBlueScaleAssignation(QByteArray subInfo, QImage displayImage);
```

This method generates a blue scale color array with 64 colors by incrementing the RGB value following certain pattern. The first 51 colors have a pattern, the next 12 another (darker colors) and the last color has no pattern (the darkest color). These pattern were created to give a better pixel contrast. Then it uses a main for-loop and a nested for-loop to assign Rgb values to every pixel. The main for-loop counts till the image height is reached (32) and the nested for-loop till the image width (64). In these loops, another for-loop assigns the values according the pixel temperature. With x as an iterator, the ranges of temperature are more or equal to x-1 and less or equal to x, and x increments every time by 2.


<a href="#developer-documentation">^ Go to top</a>

&nbsp;

## Layer Class

The Layer class has a relationship of Composition with the "Blob" class. Every Blob object has a Layer object and a Layer object cannot be instantiated without instantiating first the Blob class.
A Layer object offers three layers which work independently. The first one displays the objects if they exist, the second one displays the floor and body lines and the third one a heatmap, which is a beta version.
The default constructor and destructor provided by compiler are used implicitly. 

```c++
class Layer {
}
```

&nbsp;
### Layer Public Methods

#### Layer Setter and Getter

The following setter, resetter and getter are implemented for the heatmap.

```c++
void setNewLayerArray(unsigned int tempLayerArray[104]) {
    for ( unsigned int i = 0; i < 104; ++i ) {
       newLayerArray[i] = tempLayerArray[i];
    }
};
```

&nbsp;
```c++
unsigned int& getNewLayerArray(unsigned int x) { return newLayerArray[x]; };
```

&nbsp;
```c++
void resetLayerArray() {
    for ( unsigned int i = 0; i < 104; ++i ) {
        newLayerArray[i] = 0;
        tempLayerArray[i] = 0;
    }
};
```

&nbsp;
#### Layer Methods

```c++
QPixmap drawHeatmap(QByteArray subInfoObj, unsigned int imgCount);
```

This method generates a heatmap. This is generated with a black background and every bottom center point of the human objects are drawn in white with opacity. This is generated from the image when the heatmap was checked. The result can be seen in the last image.

&nbsp;
```c++
QPixmap drawLines(QByteArray subInfoBodyLine, QByteArray subInfoFloorLine);
```

This method draws two lines: a body line and a floor line. These are drawn by using the coordinates contained in every QByteArray.

&nbsp;
```c++
QPixmap drawObjects(QByteArray subInfoObj);
```

This method draws every object with coordinates from  the ```subInfoObj``` array and assigns the color by calling the method ```Qt::GlobalColor assignObjColors(unsigned int state)```

&nbsp;
```c++
QPixmap eraseDrawings();
```
This method replace a certain layer with an empty one. Therefore, all objects are deleted.


&nbsp;
### Layer Private Method

```c++
Qt::GlobalColor assignObjColors(unsigned int state);
```

This method assigns colors as per object status. There are nine status, which are represented by integers, and these integeres are switched by Qt::GlobalColor.

<a href="#developer-documentation">^ Go to top</a>

&nbsp;


<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />This documentation is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.

