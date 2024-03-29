TEMPLATE = lib
DEFINES += REPLACEMENTSCHEMA_LIBRARY

TARGET = replacement-schema

CONFIG(debug, debug|release) {
    mac: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}


CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    replacement.cpp \
    replacementgroup.cpp \
    replacementmodel.cpp \
    replacementpass.cpp \
    replacementreport.cpp \
    replacementreportmodel.cpp \
    replacementschema.cpp

HEADERS += \
    replacement-schema_global.h \
    replacement.h \
    replacementgroup.h \
    replacementmodel.h \
    replacementpass.h \
    replacementreport.h \
    replacementreportmodel.h \
    replacementschema.h


# Default rules for deployment.
win32:target.path = $$[QT_INSTALL_LIBS]
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
