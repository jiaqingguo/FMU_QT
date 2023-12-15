QT       += core gui gui-private xml charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
DEFINES += NOMINMAX

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addValueDialog.cpp \
    curveShowDialog.cpp \
    main.cpp \
    operstionWidget.cpp \
    widget.cpp

HEADERS += \
    addValueDialog.h \
    curveShowDialog.h \
    operstionWidget.h \
    widget.h

FORMS += \
    addValueDialog.ui \
    curveShowDialog.ui \
    operstionWidget.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/fmu/lib/ -lfmi4cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/fmu/lib/ -lfmi4cppd
else:unix: LIBS += -L$$PWD/fmu/lib/ -lfmi4cpp

INCLUDEPATH += $$PWD/fmu/include
DEPENDPATH += $$PWD/fmu/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/fmu/lib/libfmi4cpp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/fmu/lib/libfmi4cppd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/fmu/lib/fmi4cpp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/fmu/lib/fmi4cppd.lib
else:unix: PRE_TARGETDEPS += $$PWD/fmu/lib/libfmi4cpp.a
