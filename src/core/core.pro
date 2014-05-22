TARGET = core
TEMPLATE = lib
VERSION = 1.0.1

DEFINES += WINDOWS_OS CORE_LIBRARY

QT       += core gui sql

CONFIG -= debug_and_release debug_and_release_target
# каталог, куда ложить результирующий исполняемый фа

DESTDIR = ../makler-bin/bin/lib

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

include(corewidget/corewidget.pri)
include(dialogs/dialogs.pri)
include(find/find.pri)
include(general/general.pri)
include(tablemodels/tablemodels.pri)
include(photos/photos.pri)
include(widget/widget.pri)
include(messages/messages.pri)
include(editors/editors.pri)
include(sync/sync.pri)

INCLUDEPATH += dialogs \
                widget \
                general

SOURCES +=  \
        mainwindow.cpp \
    core.cpp

HEADERS  += mainwindow.h \
    core.h

FORMS    += mainwindow.ui

RESOURCES += \
    resourses.qrc
