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
    dialogs/afdialog.cpp \
    dialogs/authoritydetailsdialog.cpp \
    dialogs/choicedialog.cpp \
    dialogs/connectiondialog.cpp \
    dialogs/documentformdialog.cpp \
    dialogs/documenttypedialog.cpp \
    dialogs/funddetailsdialog.cpp \
    dialogs/inventorydetailsdialog.cpp \
    dialogs/protocoldetailsdialog.cpp \
    dialogs/protocoldialog.cpp \
    dialogs/recorddetailsdialog.cpp \
    dialogs/subjecttypedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/afsortmodel.cpp \
    models/aftreemodel.cpp \
    models/authoritymodel.cpp \
    models/authoritysortmodel.cpp \
    models/colorpickeritemdelegate.cpp \
    models/decisionmodel.cpp \
    models/documenttypemodel.cpp \
    models/protocolmodel.cpp \
    models/subjecttypemodel.cpp \
    utils/itemcounter.cpp \
    utils/stringsort.cpp \
    views/afview.cpp \
    views/authorityview.cpp \
    views/documenttypeview.cpp \
    views/documentview.cpp \
    views/protocolview.cpp \
    views/referenceview.cpp \
    views/subjecttypeview.cpp \
    views/subjectview.cpp \
    views/tablewidgetview.cpp \
    views/treeview.cpp \
    widgets/basecontextmenu.cpp \
    widgets/colorrect.cpp \
    widgets/documenttab.cpp \
    widgets/explorer.cpp \
    widgets/paginator.cpp \
    widgets/referencebutton.cpp \
    widgets/searchpanel.cpp \
    widgets/statusbarpanel.cpp \
    widgets/tab.cpp

HEADERS += \
    application.h \
    connection.h \
    dialogs/afdialog.h \
    dialogs/authoritydetailsdialog.h \
    dialogs/choicedialog.h \
    dialogs/connectiondialog.h \
    dialogs/documentformdialog.h \
    dialogs/documenttypedialog.h \
    dialogs/funddetailsdialog.h \
    dialogs/inventorydetailsdialog.h \
    dialogs/protocoldetailsdialog.h \
    dialogs/protocoldialog.h \
    dialogs/recorddetailsdialog.h \
    dialogs/subjecttypedialog.h \
    mainwindow.h \
    models/afsortmodel.h \
    models/aftreemodel.h \
    models/authoritymodel.h \
    models/authoritysortmodel.h \
    models/colorpickeritemdelegate.h \
    models/decisionmodel.h \
    models/documenttypemodel.h \
    models/protocolmodel.h \
    models/subjecttypemodel.h \
    utils/itemcounter.h \
    utils/stringsort.h \
    views/afview.h \
    views/authorityview.h \
    views/documenttypeview.h \
    views/documentview.h \
    views/protocolview.h \
    views/referenceview.h \
    views/subjecttypeview.h \
    views/subjectview.h \
    views/tablewidgetview.h \
    views/treeview.h \
    widgets/basecontextmenu.h \
    widgets/colorrect.h \
    widgets/documenttab.h \
    widgets/explorer.h \
    widgets/paginator.h \
    widgets/referencebutton.h \
    widgets/searchpanel.h \
    widgets/statusbarpanel.h \
    widgets/tab.h

FORMS += \
    dialogs/authoritydetailsdialog.ui \
    dialogs/choicedialog.ui \
    dialogs/connectiondialog.ui \
    dialogs/documentformdialog.ui \
    dialogs/funddetailsdialog.ui \
    dialogs/inventorydetailsdialog.ui \
    dialogs/protocoldetailsdialog.ui \
    dialogs/recorddetailsdialog.ui \
    mainwindow.ui \
    widgets/paginator.ui \
    widgets/referencebutton.ui \
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

DISTFILES +=
