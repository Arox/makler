TARGET = Makler
TEMPLATE = app
VERSION = 0.0.1

DEFINES += WINDOWS_OS

QT       += core gui sql

INCLUDEPATH += ../core

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

LIBS += -L$$DESTDIR/lib -lcore1

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#install_it.path = DESTDIR
#install_it.files = styles

#INSTALLS += install_it

#RESOURCES += \
#    resourses.qrc

SOURCES += \
    main.cpp

RESOURCES += \
    resource.qrc

