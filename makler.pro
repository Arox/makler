#-------------------------------------------------
#
# Project created by QtCreator 2013-05-01T12:23:37
#
#-------------------------------------------------
TEMPLATE = subdirs
SUBDIRS +=  src/core \
            src/application

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
