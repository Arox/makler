INCLUDEPATH += src/corewidget \
                src/models \
                src/find \
                src/editors \
                src/sync

SOURCES += \
    $$PWD/apartmentwidget.cpp \
    $$PWD/viewobjectwidget.cpp \
    $$PWD/rentwidget.cpp \
    $$PWD/generalwidget.cpp \
    src/widget/homewidget.cpp \
    src/widget/widgetforcontrol.cpp \
    src/widget/classicmenuwidget.cpp \
    src/widget/findobjectwidget.cpp \
    src/widget/clientswidget.cpp \
    src/widget/adminwidget.cpp \
    src/widget/messagewidget.cpp \
    src/widget/generalwindow.cpp \
    src/widget/topmenu.cpp

HEADERS  += \
    $$PWD/apartmentwidget.h \
    $$PWD/viewobjectwidget.h \
    $$PWD/rentwidget.h \
    $$PWD/generalwidget.h \
    src/widget/homewidget.h \
    src/widget/widgetforcontrol.h \
    src/widget/classicmenuwidget.h \
    src/widget/findobjectwidget.h \
    src/widget/clientswidget.h \
    src/widget/adminwidget.h \
    src/widget/messagewidget.h \
    src/widget/generalwindow.h \
    src/widget/topmenu.h

FORMS    +=  \
    $$PWD/apartmentwidget.ui \
    $$PWD/viewobjectwidget.ui \
    $$PWD/rentwidget.ui \
    src/widget/homewidget.ui \
    src/widget/classicmenuwidget.ui \
    src/widget/findobjectwidget.ui \
    src/widget/clientswidget.ui \
    src/widget/adminwidget.ui \
    src/widget/messagewidget.ui \
    src/widget/generalwindow.ui \
    src/widget/topmenu.ui
