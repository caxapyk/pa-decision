QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

VERSION=0.1

QMAKE_TARGET_COMPANY = Alexander Sakharuk
QMAKE_TARGET_PRODUCT = Archival topographic pointer
QMAKE_TARGET_DESCRIPTION = Archival topographic pointer
QMAKE_TARGET_COPYRIGHT = (c) Alexander Sakharuk

DEFINES += APP_VERSION=$$VERSION

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    connection.cpp \
    dialogs/connectiondialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/authoritymodel.cpp \
    views/authorityview.cpp \
    views/decisionview.cpp \
    views/view.cpp

HEADERS += \
    application.h \
    connection.h \
    dialogs/connectiondialog.h \
    mainwindow.h \
    models/authoritymodel.h \
    views/authorityview.h \
    views/decisionview.h \
    views/view.h

FORMS += \
    dialogs/connectiondialog.ui \
    mainwindow.ui \
    views/authorityview.ui \
    views/decisionview.ui

TRANSLATIONS += \
    translations/pa-decision_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
