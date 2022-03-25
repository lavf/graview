QT += testlib
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  blob.cpp \
    image.cpp \
    layer.cpp \
    test.cpp

HEADERS += \
    blob.hpp \
    image.hpp \
    layer.hpp \
    test.hpp \



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
