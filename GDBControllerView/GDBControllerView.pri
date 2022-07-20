QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    $$PWD/buildanddebugtools.h \
    $$PWD/debuger.h \
    $$PWD/debugtool.h \
#    $$PWD/gdbcontrollerview.h \
    $$PWD/framevariablebrowser.h \
    $$PWD/lldbdebuger.h \
    $$PWD/registerbrowser.h \
    $$PWD/runtool.h

SOURCES += \
    $$PWD/buildanddebugtools.cpp \
    $$PWD/debuger.cpp \
    $$PWD/debugtool.cpp \
#    $$PWD/gdbcontrollerview.cpp \
    $$PWD/framevariablebrowser.cpp \
    $$PWD/lldbdebuger.cpp \
    $$PWD/registerbrowser.cpp \
    $$PWD/runtool.cpp

RESOURCES += \
    $$PWD/GDBResources.qrc
