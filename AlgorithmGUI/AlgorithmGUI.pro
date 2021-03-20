QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VERSION = 1.1
QMAKE_TARGET_COMPANY = Demetrey Co
QMAKE_TARGET_PRODUCT = Ford-Fulkerson Algorithm
QMAKE_TARGET_DESCRIPTION = https://github.com/Demetrey
QMAKE_TARGET_COPYRIGHT = Demetrey

SOURCES += \
    algorithm.cpp \
    interlayer.cpp \
    logview.cpp \
    main.cpp \
    mainwindow.cpp \
    scaledpixmap.cpp \
    vizualizer.cpp \
    vizualizerform.cpp

HEADERS += \
    algorithm.h \
    interlayer.h \
    logview.h \
    mainwindow.h \
    scaledpixmap.h \
    vizualizer.h \
    vizualizerform.h

FORMS += \
    logview.ui \
    mainwindow.ui \
    vizualizerform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ResourceFile.qrc

win32:RC_ICONS += $$PWD/Resources/icon.ico
