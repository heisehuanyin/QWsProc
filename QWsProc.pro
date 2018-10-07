#-------------------------------------------------
#
# Project created by QtCreator 2018-09-23T08:12:28
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QWsProc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    defaultconfigport.cpp \
    defaultlogport.cpp \
    main.cpp \
    singalview.cpp \
    wscore.cpp \
    defaulttextmodel.cpp

HEADERS += \
    def_configport.h \
    def_contentview.h \
    def_logport.h \
    def_menubar.h \
    def_plugin_base.h \
    def_window.h \
    defaultconfigport.h \
    defaultlogport.h \
    singalview.h \
    wscore.h \
    def_channelpreface.h \
    def_textmodel.h \
    defaulttextmodel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CPP编程规范.md \
    README.md

FORMS +=
