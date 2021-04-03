QT -= gui
QT += xml

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        replacer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../replacement-schema/release/ -lreplacement-schema
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../replacement-schema/debug/ -lreplacement-schema
else:unix: LIBS += -L$$OUT_PWD/../replacement-schema/ -lreplacement-schema

INCLUDEPATH += $$PWD/../replacement-schema
DEPENDPATH += $$PWD/../replacement-schema

HEADERS += \
    replacer.h
