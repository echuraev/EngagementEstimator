QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        involvementestimator.cpp \
        main.cpp \
        modelexecutor.cpp \
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
    modelexecutor.h \
    screencapture.h

TVM_BUILD_DIR = $$(TVM_BUILD_DIR)
!isEmpty(TVM_BUILD_DIR) {
    #win32:CONFIG(release, debug|release): LIBS += -L$${TVM_BUILD_DIR}/release/ -ltvm_runtime
    #else:win32:CONFIG(debug, debug|release): LIBS += -L$${TVM_BUILD_DIR}/debug/ -ltvm_runtime
    #else:unix: LIBS += -L$${TVM_BUILD_DIR}/ -ltvm_runtime
    LIBS += -L$${TVM_BUILD_DIR}/ -ltvm_runtime

    INCLUDEPATH += $${TVM_BUILD_DIR}/../include
    DEPENDPATH += $${TVM_BUILD_DIR}/../include
    INCLUDEPATH += $${TVM_BUILD_DIR}/../3rdparty/dmlc-core/include/
    DEPENDPATH += $${TVM_BUILD_DIR}/../3rdparty/dmlc-core/include/
    INCLUDEPATH += $${TVM_BUILD_DIR}/../3rdparty/dlpack/include
    DEPENDPATH += $${TVM_BUILD_DIR}/../3rdparty/dlpack/include
}
else {
    error("Error! Environment variable TVM_HOME should be specified!")
}
