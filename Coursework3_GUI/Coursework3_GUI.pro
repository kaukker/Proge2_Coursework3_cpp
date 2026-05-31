QT += widgets

CONFIG += c++17

win32 {
    DLL_DIR = $$PWD/../Coursework2dll/x64/Debug

    LIBS += -L$$DLL_DIR -lCoursework2dll

    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$DLL_DIR/Coursework2dll.dll) $$quote($$OUT_PWD/) $$escape_expand(\n\t)
}

#LIBS += -L$$OUT_PWD/debug -lCoursework2DLL

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    readerthread.cpp

HEADERS += \
    coursework2dll.h \
    mainwindow.h \
    readerthread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
