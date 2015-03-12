#-------------------------------------------------
#
# Project created by QtCreator 2011-06-18T19:05:42
#
#-------------------------------------------------

QT       += core gui sql

TARGET = Interface
TEMPLATE = app


SOURCES +=\
        login.cpp \
    window.cpp \
    setting.cpp \
    main.cpp \
    manageWidget.cpp \
    statistics.cpp \
    showscore.cpp \
    change.cpp \
    about.cpp \
    add.cpp

HEADERS  += login.h \
    window.h \
    setting.h \
    manageWidget.h \
    sqlite.h \
    statistics.h \
    showscore.h \
    change.h \
    about.h \
    add.h


FORMS    += login.ui \
    setting.ui \
    statistics.ui \
    showscore.ui \
    change.ui \
    about.ui \
    add.ui \
    window.ui \
    window.ui

RESOURCES += \
    image.qrc

RC_FILE = icon.rc

OTHER_FILES += \
    myapp.rc \
    icon.rc
