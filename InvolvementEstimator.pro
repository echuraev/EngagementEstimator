QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        involvementestimator.cpp \
        main.cpp \
        screencapture.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    InvolvementEstimator_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    involvementestimator.h \
    screencapture.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../OctoML/tvm/cmake-build-Debug/release/ -ltvm_runtime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../OctoML/tvm/cmake-build-Debug/debug/ -ltvm_runtime
else:unix: LIBS += -L$$PWD/../../../OctoML/tvm/cmake-build-Debug/ -ltvm_runtime

INCLUDEPATH += $$PWD/../../../OctoML/tvm/include
DEPENDPATH += $$PWD/../../../OctoML/tvm/include
INCLUDEPATH += $$PWD/../../../OctoML/tvm/3rdparty/dmlc-core/include/
DEPENDPATH += $$PWD/../../../OctoML/tvm/3rdparty/dmlc-core/include/
INCLUDEPATH += $$PWD/../../../OctoML/tvm/3rdparty/dlpack/include
DEPENDPATH += $$PWD/../../../OctoML/tvm/3rdparty/dlpack/include
