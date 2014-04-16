INCLUDEPATH += src/photos
SOURCES += \
    $$PWD/dialogauthentification.cpp \
    $$PWD/dialogphotoalbum.cpp \
    $$PWD/dialoguniversal.cpp \
    $$PWD/dialogaside.cpp \
    $$PWD/dialogshoot.cpp \
    src/dialogs/dialogasideup.cpp \
    src/dialogs/phonefinddialog.cpp

HEADERS  +=  \
    $$PWD/dialogauthentification.h \
    $$PWD/dialogphotoalbum.h \
    $$PWD/dialoguniversal.h \
    $$PWD/dialogaside.h \
    $$PWD/dialogshoot.h \
    src/dialogs/dialogasideup.h \
    src/dialogs/phonefinddialog.h

FORMS    += $$PWD/dialogauthentification.ui \
    $$PWD/dialogphotoalbum.ui \
    $$PWD/dialoguniversal.ui \
    $$PWD/dialogaside.ui \
    $$PWD/dialogshoot.ui \
    src/dialogs/dialogasideup.ui \
    src/dialogs/phonefinddialog.ui
