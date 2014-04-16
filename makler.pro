#-------------------------------------------------
#
# Project created by QtCreator 2013-05-01T12:23:37
#
#-------------------------------------------------
DEFINES += WINDOWS_OS

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Makler
TEMPLATE = app
VERSION = 1.0.1

include(src/src.pri)
include(src/corewidget/corewidget.pri)
include(src/dialogs/dialogs.pri)
include(src/find/find.pri)
include(src/general/general.pri)
include(src/tablemodels/tablemodels.pri)
include(src/photos/photos.pri)
include(src/widget/widget.pri)
include(src/messages/messages.pri)
include(src/editors/editors.pri)
include(src/sync/sync.pri)


install_it.path = $$OUT_PWD
install_it.files = styles

INSTALLS += install_it

RESOURCES += \
    resourses.qrc

