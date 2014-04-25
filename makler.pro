#-------------------------------------------------
#
# Project created by QtCreator 2013-05-01T12:23:37
#
#-------------------------------------------------
TARGET = Makler
TEMPLATE = app
VERSION = 1.0.1

DEFINES += WINDOWS_OS FORFILL

QT       += core gui sql

CONFIG -= debug_and_release debug_and_release_target
# каталог, куда ложить результирующий исполняемый фа

DESTDIR = ../makler-bin/bin

# промежуточные файлы, созданные moc
MOC_DIR = ../makler-bin/moc

# объектные файлы
OBJECTS_DIR = ../makler-bin/obj

# промежуточные файлы, созданные компилятором
# ресурсов Qt (rcc)
RCC_DIR = ../makler-bin/rcc

# промежуточные файлы, созданные компилятором
# файлов форм (.ui) Qt (uic)
UI_DIR = ../makler-bin/uic

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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


install_it.path = DESTDIR
install_it.files = styles

INSTALLS += install_it

RESOURCES += \
    resourses.qrc

