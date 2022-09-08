cmake_minimum_required(VERSION 3.14)

project(InvolvementEstimator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick LinguistTools)

set(TS_FILES InvolvementEstimator_ru_RU.ts)

set(PROJECT_SOURCES
        enetb08bestafewwrapper.cpp
        facedetectionwrapper.cpp
        involvementestimator.cpp
        main.cpp
        modelexecutor.cpp
        screencapture.cpp
        qml.qrc
        ${TS_FILES}
)

find_package(OpenCV REQUIRED)

set(MTCNN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/opencv-mtcnn/lib")
add_subdirectory("${MTCNN_PATH}")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(InvolvementEstimator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET InvolvementEstimator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(InvolvementEstimator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(InvolvementEstimator
          ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

set(TVM_BUILD_DIR $ENV{TVM_BUILD_DIR})
if(NOT DEFINED TVM_BUILD_DIR)
    message(FATAL_ERROR "Cannot find path to TVM build, please set TVM_BUILD_DIR environment variable")
else()
    message("Found TVM build at ${TVM_BUILD_DIR}")
endif()

target_include_directories(InvolvementEstimator PRIVATE
    "${TVM_BUILD_DIR}/../include"
    "${TVM_BUILD_DIR}/../3rdparty/dmlc-core/include/"
    "${TVM_BUILD_DIR}/../3rdparty/dlpack/include"
    "${MTCNN_LIB_INCLUDE_PATH}"
    ${OpenCV_INCLUDE_DIRS}
)

find_library(LIB_TVM NAMES tvm_runtime PATHS ${TVM_BUILD_DIR})

target_compile_definitions(InvolvementEstimator
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(InvolvementEstimator
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  ${LIB_TVM}
  ${OpenCV_LIBS}
  mtcnn
)

set_target_properties(InvolvementEstimator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(InvolvementEstimator)
    qt_finalize_executable(InvolvementEstimator)
endif()
