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
    dialogs/authoritydialog.cpp \
    dialogs/connectiondialog.cpp \
    dialogs/decisionbasedialog.cpp \
    dialogs/decisioneditdialog.cpp \
    dialogs/decisionnewdialog.cpp \
    dialogs/detailsdialog.cpp \
    dialogs/doctypedialog.cpp \
    dialogs/protocoldetailsdialog.cpp \
    dialogs/protocoldialog.cpp \
    dialogs/recorddetailsdialog.cpp \
    dialogs/recorddialog.cpp \
    dialogs/referencedialog.cpp \
    dialogs/subjectdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/authorityflatmodel.cpp \
    models/authoritymodel.cpp \
    models/authorityproxymodel.cpp \
    models/colorpickeritemdelegate.cpp \
    models/decisionproxymodel.cpp \
    models/decisionreadmodel.cpp \
    models/documenttypemodel.cpp \
    models/protocolflatmodel.cpp \
    models/recordflatmodel.cpp \
    models/recordmodel.cpp \
    models/recordproxymodel.cpp \
    models/referencemodel.cpp \
    models/standardreferencemodel.cpp \
    models/subjectmodel.cpp \
    utils/stringsort.cpp \
    views/decisionview.cpp \
    views/explorerview.cpp \
    views/navigatorview.cpp \
    views/view.cpp \
    widgets/colorrect.cpp \
    widgets/customcontextmenu.cpp \
    widgets/decisiontab.cpp \
    widgets/dialogheader.cpp \
    widgets/paginator.cpp \
    widgets/referencebutton.cpp \
    widgets/searchpanel.cpp \
    widgets/statusbarpanel.cpp \
    widgets/tab.cpp

HEADERS += \
    application.h \
    connection.h \
    dialogs/authoritydetailsdialog.h \
    dialogs/authoritydialog.h \
    dialogs/connectiondialog.h \
    dialogs/decisionbasedialog.h \
    dialogs/decisioneditdialog.h \
    dialogs/decisionnewdialog.h \
    dialogs/detailsdialog.h \
    dialogs/doctypedialog.h \
    dialogs/protocoldetailsdialog.h \
    dialogs/protocoldialog.h \
    dialogs/recorddetailsdialog.h \
    dialogs/recorddialog.h \
    dialogs/referencedialog.h \
    dialogs/subjectdialog.h \
    mainwindow.h \
    models/authorityflatmodel.h \
    models/authoritymodel.h \
    models/authorityproxymodel.h \
    models/colorpickeritemdelegate.h \
    models/decisionproxymodel.h \
    models/decisionreadmodel.h \
    models/documenttypemodel.h \
    models/protocolflatmodel.h \
    models/recordflatmodel.h \
    models/recordmodel.h \
    models/recordproxymodel.h \
    models/referencemodel.h \
    models/standardreferencemodel.h \
    models/subjectmodel.h \
    utils/stringsort.h \
    views/decisionview.h \
    views/explorerview.h \
    views/navigatorview.h \
    views/view.h \
    widgets/colorrect.h \
    widgets/customcontextmenu.h \
    widgets/decisiontab.h \
    widgets/dialogheader.h \
    widgets/paginator.h \
    widgets/referencebutton.h \
    widgets/searchpanel.h \
    widgets/statusbarpanel.h \
    widgets/tab.h

FORMS += \
    dialogs/authoritydetailsdialog.ui \
    dialogs/connectiondialog.ui \
    dialogs/decisionbasedialog.ui \
    dialogs/protocoldetailsdialog.ui \
    dialogs/recorddetailsdialog.ui \
    dialogs/referencedialog.ui \
    mainwindow.ui \
    views/decisionview.ui \
    views/explorerview.ui \
    views/navigatorview.ui \
    widgets/dialogheader.ui \
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
