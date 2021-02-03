QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

VERSION=1.0

QMAKE_TARGET_COMPANY = Alexander Sakharuk
QMAKE_TARGET_PRODUCT = Public authority decisions
QMAKE_TARGET_DESCRIPTION = Public authority decisions
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
    dialogs/referencedialog.cpp \
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
    utils/htmltemplate.cpp \
    utils/itemcounter.cpp \
    utils/stringsort.cpp \
    views/afview.cpp \
    views/authorityview.cpp \
    views/documenttabview.cpp \
    views/documenttypeview.cpp \
    views/documentview.cpp \
    views/protocolview.cpp \
    views/referenceview.cpp \
    views/subjectdelegate.cpp \
    views/subjecttypeview.cpp \
    views/subjectview.cpp \
    views/tab.cpp \
    views/tablewidgetview.cpp \
    views/treeview.cpp \
    views/view.cpp \
    widgets/basecontextmenu.cpp \
    widgets/colorrect.cpp \
    widgets/explorer.cpp \
    widgets/navpanel.cpp \
    widgets/paginator.cpp \
    widgets/referencebutton.cpp \
    widgets/searchpanel.cpp \
    widgets/statusbarpanel.cpp \

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
    dialogs/referencedialog.h \
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
    utils/htmltemplate.h \
    utils/itemcounter.h \
    utils/stringsort.h \
    views/afview.h \
    views/authorityview.h \
    views/documenttabview.h \
    views/documenttypeview.h \
    views/documentview.h \
    views/protocolview.h \
    views/referenceview.h \
    views/subjectdelegate.h \
    views/subjecttypeview.h \
    views/subjectview.h \
    views/tab.h \
    views/tablewidgetview.h \
    views/treeview.h \
    views/view.h \
    widgets/basecontextmenu.h \
    widgets/colorrect.h \
    widgets/explorer.h \
    widgets/navpanel.h \
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
    widgets/navpanel.ui \
    widgets/paginator.ui \
    widgets/referencebutton.ui \
    widgets/searchpanel.ui \
    widgets/statusbarpanel.ui

TRANSLATIONS += \
    translations/pa-decision_ru_RU.ts

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
#    assets/padecision.sh \
#    assets/icon.ico \
#    assets/padecision.desktop \
#    assets/padecision.svg \
    assets/tmp/documentform.css \
    assets/tmp/documentform.html

unix {
    DEPLOY_DIR = $$OUT_PWD/padecision_v"$$VERSION"_"$$QMAKE_HOST.os"_"$$QMAKE_HOST.arch"

    target.path += $$DEPLOY_DIR

    shortcutfiles.files += assets/padecision.sh \
                        assets/padecision.desktop
    shortcutfiles.path = $$DEPLOY_DIR

    icons.files += assets/padecision.svg
    icons.path = $$DEPLOY_DIR

    licence.files += LICENSE
    licence.path = $$DEPLOY_DIR

    templates.files += $$files(assets/tmp/*.*)
    templates.path = $$DEPLOY_DIR/templates

    libs.files += $$[QT_INSTALL_PREFIX]/lib/libicudata.so.5* \
                $$[QT_INSTALL_PREFIX]/lib/libicui18n.so.5* \
                $$[QT_INSTALL_PREFIX]/lib/libicuuc.so.5* \
                $$[QT_INSTALL_PREFIX]/lib/libQt5Core.so.5 \
                $$[QT_INSTALL_PREFIX]/lib/libQt5Sql.so.5 \
                $$[QT_INSTALL_PREFIX]/lib/libQt5DBus.so.5 \
                $$[QT_INSTALL_PREFIX]/lib/libQt5Gui.so.5 \
                $$[QT_INSTALL_PREFIX]/lib/libQt5Widgets.so.5 \
                $$[QT_INSTALL_PREFIX]/lib/libQt5PrintSupport.so.5 \
                $$[QT_INSTALL_PREFIX]/lib/libQt5XcbQpa.so.5
    libs.path = $$DEPLOY_DIR

    platforms.files += $$[QT_INSTALL_PREFIX]/plugins/platforms/libqxcb.so
    platforms.path = $$DEPLOY_DIR/platforms

    printsupport.files += $$[QT_INSTALL_PREFIX]/plugins/printsupport/libcupsprintersupport.so
    printsupport.path = $$DEPLOY_DIR/printsupport

    sqldrivers.files += $$[QT_INSTALL_PREFIX]/plugins/sqldrivers/libqsqlmysql.so
    sqldrivers.path = $$DEPLOY_DIR/sqldrivers

    INSTALLS += target \
                shortcutfiles \
                icons \
                templates \
                licence \
                libs \
                platforms \
                printsupport \
                sqldrivers
}

win32 {
    DEPLOY_DIR = $$OUT_PWD/padecision_v"$$VERSION"_"$$QMAKE_HOST.os"_"$$QMAKE_HOST.arch"

    target.path += $$DEPLOY_DIR

    icons.files += assets/icon.ico
    icons.path = $$DEPLOY_DIR

    templates.files += $$files(assets/tmp/*.*)
    templates.path = $$DEPLOY_DIR/templates

    licence.files += LICENSE
    licence.path = $$DEPLOY_DIR

    libs.files += $$files(assets/libs/*.dll)
    libs.path = $$DEPLOY_DIR

    QMAKE_EXTRA_TARGETS += deploy
    deploy.commands = $$(QTDIR)/bin/windeployqt --no-quick-import --no-system-d3d-compiler --no-virtualkeyboard --no-webkit2 --no-angle --no-opengl-sw $$DEPLOY_DIR
    deploy.depends += install

    INSTALLS += target \
                icons \
                templates \
                licence \
                libs
}

