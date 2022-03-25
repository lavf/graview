# Developer Documentation - GraView v1.0

&nbsp;

GraView version 1.0 is a Qt Widgets-based App for Android 11 in C++11. Qt 5.12 used in this application under the terms of GNU Lesser General Public License for Open-Source projects with educational purposes.

The GraView OOP-structure is as follows:

- <a href="#interface-class">Interface Class</a>
- <a href="#blob-class">Blob Class</a>
- <a href="#image-class">Image Class</a>
- <a href="#layer-class">Layer Class</a>

&nbsp;

## Interface Class

The Interface Class implements the Qt Widgets module, that is to say, it contains the QWidget class, which is the base class for all GUI objects. All raw pointers of this class are managed by the QWidget 'the parent'. Consequently, when the default destructor is called, first all child objects are destroyed and then the application exits (no need for manual delete).

```markdown
class Interface : public QWidget
{
    Q_OBJECT
    ...
}
```
&nbsp;
### Constructor

```markdown
Interface::Interface(QWidget *parent)
    : QWidget(parent),
      blobObject(new Blob) {
}
```

&nbsp;
### Private Slots

```markdown
void displayApp();    
```

This method displays the layout and all GUI objects when a file is selected.

```markdown
void paintEvent(QPaintEvent *);
```

This method is in charge of exposing layers over the image. Therefore, is constantly updated even if there is nothing to draw.

```markdown
void displayBlueScale();
```

This method calls the blue scale filter from the Blob Object, which calls the method ```colorAssignation(QByteArray subInfo, QImage displayImage, QRgb colors [33])``` from the Image Object.
  
```markdown
void displayThermal();
```
This method calls the thermal filter from the Blob Object, which calls the method ```grayOrBlueScaleAssignation(QByteArray subInfo, QImage displayImage)``` from the Image Object.

```markdown
void displayGrayScale();
```

```markdown
void nextFrame();
```

```markdown
void previousFrame();
```

```markdown
void closeApp();
```

```markdown
void applySmoothFilter();
```

```markdown
void applySharpFilter();
```

```markdown
void infoMessage();
```

```markdown
void displayFrameFromSpinBox();
```

<a href="#developer-documentation---graview-v10">^ Go to top</a>

&nbsp;

## Blob Class

There is an aggregation between the Blob class and the class "Interface", which contains GUI objects. This means that an Interface object HAS a Blob object but there is no dependency between each other. A Blob object can exist without instantiating the Interface class and can be used by another GUI class if needed.

```markdown
class Blob {
}
```

&nbsp;
### Constructor

```markdown
Blob::Blob()
    : imageObject(new Image),
      layerObject(new Layer) {
}
```

&nbsp;
### Public Methods

&nbsp;
### Private Methods

<a href="#developer-documentation---graview-v10">^ Go to top</a>

&nbsp;

## Image Class

```markdown
class Image {
}
```

&nbsp;
### Constructor

&nbsp;
### Methods

<a href="#developer-documentation---graview-v10">^ Go to top</a>

## Layer Class

```markdown
class Layer {
}
```

&nbsp;
### Constructor

&nbsp;
### Methods

<a href="#developer-documentation---graview-v10">^ Go to top</a>

&nbsp;


<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />This documentation is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.

