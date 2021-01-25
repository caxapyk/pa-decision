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
    dialogs/authoritydetailsdialog.cpp \
    dialogs/connectiondialog.cpp \
    dialogs/decisionformdialog.cpp \
    dialogs/doctypedialog.cpp \
    dialogs/funddetailsdialog.cpp \
    dialogs/inventorydetailsdialog.cpp \
    dialogs/protocoldetailsdialog.cpp \
    dialogs/protocoldialog.cpp \
    dialogs/recorddetailsdialog.cpp \
    dialogs/recorddialog.cpp \
    dialogs/subjtypedialog.cpp \
    dialogs/treedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/authoritymodel.cpp \
    models/authorityproxymodel.cpp \
    models/colorpickeritemdelegate.cpp \
    models/decisionmodel.cpp \
    models/decisionproxymodel.cpp \
    models/documenttypemodel.cpp \
    models/protocolmodel.cpp \
    models/recordmodel.cpp \
    models/recordproxymodel.cpp \
    models/subjtypemodel.cpp \
    utils/customcontextmenu.cpp \
    utils/itemcounter.cpp \
    utils/stringsort.cpp \
    views/authorityview.cpp \
    views/decisionview.cpp \
    views/explorerview.cpp \
    views/view.cpp \
    widgets/authoritytree.cpp \
    widgets/colorrect.cpp \
    widgets/customtreeview.cpp \
    widgets/decisiontab.cpp \
    widgets/decisiontable.cpp \
    widgets/paginator.cpp \
    widgets/referencebutton.cpp \
    widgets/searchpanel.cpp \
    widgets/statusbarpanel.cpp \
    widgets/tab.cpp \
    widgets/table.cpp

HEADERS += \
    application.h \
    connection.h \
    dialogs/authoritydetailsdialog.h \
    dialogs/connectiondialog.h \
    dialogs/decisionformdialog.h \
    dialogs/doctypedialog.h \
    dialogs/funddetailsdialog.h \
    dialogs/inventorydetailsdialog.h \
    dialogs/protocoldetailsdialog.h \
    dialogs/protocoldialog.h \
    dialogs/recorddetailsdialog.h \
    dialogs/recorddialog.h \
    dialogs/subjtypedialog.h \
    dialogs/treedialog.h \
    mainwindow.h \
    models/authoritymodel.h \
    models/authorityproxymodel.h \
    models/colorpickeritemdelegate.h \
    models/decisionmodel.h \
    models/decisionproxymodel.h \
    models/documenttypemodel.h \
    models/protocolmodel.h \
    models/recordmodel.h \
    models/recordproxymodel.h \
    models/subjtypemodel.h \
    utils/customcontextmenu.h \
    utils/itemcounter.h \
    utils/stringsort.h \
    views/authorityview.h \
    views/decisionview.h \
    views/explorerview.h \
    views/view.h \
    widgets/authoritytree.h \
    widgets/colorrect.h \
    widgets/customtreeview.h \
    widgets/decisiontab.h \
    widgets/decisiontable.h \
    widgets/paginator.h \
    widgets/referencebutton.h \
    widgets/searchpanel.h \
    widgets/statusbarpanel.h \
    widgets/tab.h \
    widgets/table.h

FORMS += \
    dialogs/authoritydetailsdialog.ui \
    dialogs/connectiondialog.ui \
    dialogs/decisionformdialog.ui \
    dialogs/funddetailsdialog.ui \
    dialogs/inventorydetailsdialog.ui \
    dialogs/protocoldetailsdialog.ui \
    dialogs/recorddetailsdialog.ui \
    dialogs/treedialog.ui \
    mainwindow.ui \
    views/authorityview.ui \
    views/decisionview.ui \
    views/explorerview.ui \
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
