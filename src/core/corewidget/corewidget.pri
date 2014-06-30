INCLUDEPATH += $$PWD/../general \
               $$PWD/../tablemodels

SUBDIRS = address

#ADDRESS
SOURCES += \
    $$PWD/address/baseaddresswidget.cpp \
    $$PWD/address/address.cpp \
    $$PWD/mytableview.cpp \
    $$PWD/connecttodatabase.cpp \
    $$PWD/editorsettingsdatabase.cpp \
    $$PWD/address/citywidget.cpp \
    $$PWD/address/citymodel.cpp \
    $$PWD/address/locationselect.cpp \
    $$PWD/address/locationmodel.cpp \
    $$PWD/address/verticalcontanerwidget.cpp \
    $$PWD/address/microdistrictmodel.cpp \
    $$PWD/address/streetmodel.cpp \
    $$PWD/address/microdistrictwidget.cpp \
    $$PWD/address/streetwidget.cpp \
    corewidget/address/addresscity.cpp \
    corewidget/address/addresshome.cpp

HEADERS  +=  \
    $$PWD/address/baseaddresswidget.h \
    $$PWD/address/address.h \
    $$PWD/mytableview.h \
    $$PWD/connecttodatabase.h \
    $$PWD/editorsettingsdatabase.h \
    $$PWD/address/citywidget.h \
    $$PWD/address/citymodel.h \
    $$PWD/address/locationselect.h \
    $$PWD/address/locationmodel.h \
    $$PWD/address/generalsettingslocation.h \
    $$PWD/address/verticalcontanerwidget.h \
    $$PWD/address/microdistrictmodel.h \
    $$PWD/address/streetmodel.h \
    $$PWD/address/microdistrictwidget.h \
    $$PWD/address/streetwidget.h \
    corewidget/address/addresscity.h \
    corewidget/address/addresshome.h

FORMS    +=  \
    $$PWD/address/baseaddresswidget.ui \
    $$PWD/address/address.ui \
    $$PWD/connecttodatabase.ui \
    $$PWD/editorsettingsdatabase.ui \
    $$PWD/address/verticalcontanerwidget.ui

#ALL OTHER
SOURCES += \
    $$PWD/pricewidget.cpp \
    $$PWD/informationarea.cpp \
    $$PWD/apartmentinformation.cpp \
    $$PWD/clientheaderwidget.cpp \
    $$PWD/typeapartment.cpp \
    $$PWD/commentwidget.cpp \
    $$PWD/buttonswidget.cpp \
    $$PWD/typehomewidget.cpp \
    $$PWD/mainwidget.cpp \
    $$PWD/findobjectswidget.cpp \
    $$PWD/clientheaderalternativewidget.cpp \
    $$PWD/passportwidget.cpp \
    $$PWD/viewobjects.cpp \
    $$PWD/agentwidget.cpp \
    $$PWD/helpwidget.cpp \
    $$PWD/aboutproducerwidget.cpp \
    $$PWD/addinfoaboutdatabase.cpp \
    $$PWD/buttontopmenuwidget.cpp \
    $$PWD/mylineedit.cpp \
    $$PWD/clientsphonewidget.cpp


HEADERS  +=  \
    $$PWD/pricewidget.h \
    $$PWD/informationarea.h \
    $$PWD/apartmentinformation.h \
    $$PWD/clientheaderwidget.h \
    $$PWD/typeapartment.h \
    $$PWD/commentwidget.h \
    $$PWD/buttonswidget.h \
    $$PWD/typehomewidget.h \
    $$PWD/mainwidget.h \
    $$PWD/findobjectswidget.h \
    $$PWD/clientheaderalternativewidget.h \
    $$PWD/passportwidget.h \
    $$PWD/viewobjects.h \
    $$PWD/agentwidget.h \
    $$PWD/helpwidget.h \
    $$PWD/aboutproducerwidget.h \
    $$PWD/addinfoaboutdatabase.h \
    $$PWD/buttontopmenuwidget.h \
    $$PWD/mylineedit.h \
    $$PWD/clientsphonewidget.h

FORMS    +=  \
    $$PWD/pricewidget.ui \
    $$PWD/informationarea.ui \
    $$PWD/apartmentinformation.ui \
    $$PWD/clientheaderwidget.ui \
    $$PWD/typeapartment.ui \
    $$PWD/commentwidget.ui \
    $$PWD/buttonswidget.ui \
    $$PWD/typehomewidget.ui \
    $$PWD/findobjectswidget.ui \
    $$PWD/clientheaderalternativewidget.ui \
    $$PWD/passportwidget.ui \
    $$PWD/viewobjects.ui \
    $$PWD/agentwidget.ui \
    $$PWD/helpwidget.ui \
    $$PWD/aboutproducerwidget.ui \
    $$PWD/addinfoaboutdatabase.ui \
    $$PWD/buttontopmenuwidget.ui \
    $$PWD/clientsphonewidget.ui
