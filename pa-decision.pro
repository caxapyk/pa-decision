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
    dialogs/authoritydetaildialog.cpp \
    dialogs/connectiondialog.cpp \
    dialogs/doctypedialog.cpp \
    dialogs/recorddialog.cpp \
    dialogs/referencedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/authorityproxymodel.cpp \
    models/authoritytreemodel.cpp \
    models/colorpickeritemdelegate.cpp \
    models/decisionmodel.cpp \
    models/doctypemodel.cpp \
    models/recordproxymodel.cpp \
    models/recordtreemodel.cpp \
    utils/stringsort.cpp \
    views/authorityview.cpp \
    views/collectionview.cpp \
    views/decisionview.cpp \
    views/navigatorview.cpp \
    views/view.cpp \
    widgets/colorrect.cpp \
    widgets/customcontextmenu.cpp \
    widgets/searchpanel.cpp \
    widgets/statusbarpanel.cpp

HEADERS += \
    application.h \
    connection.h \
    dialogs/authoritydetaildialog.h \
    dialogs/connectiondialog.h \
    dialogs/doctypedialog.h \
    dialogs/recorddialog.h \
    dialogs/referencedialog.h \
    mainwindow.h \
    models/authorityproxymodel.h \
    models/authoritytreemodel.h \
    models/colorpickeritemdelegate.h \
    models/decisionmodel.h \
    models/doctypemodel.h \
    models/recordproxymodel.h \
    models/recordtreemodel.h \
    utils/stringsort.h \
    views/authorityview.h \
    views/collectionview.h \
    views/decisionview.h \
    views/navigatorview.h \
    views/view.h \
    widgets/colorrect.h \
    widgets/customcontextmenu.h \
    widgets/searchpanel.h \
    widgets/statusbarpanel.h

FORMS += \
    dialogs/authoritydetaildialog.ui \
    dialogs/connectiondialog.ui \
    dialogs/doctypedialog.ui \
    dialogs/recorddialog.ui \
    dialogs/referencedialog.ui \
    mainwindow.ui \
    views/authorityview.ui \
    views/collectionview.ui \
    views/decisionview.ui \
    views/navigatorview.ui \
    widgets/searchpanel.ui \
    widgets/statusbarpanel.ui

TRANSLATIONS += \
    translations/pa-decision_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
