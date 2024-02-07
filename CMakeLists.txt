cmake_minimum_required(VERSION 3.14)

project(EngagementEstimator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# To awoid warnings during TVM linking
set (LOGGING_HEADER "\"\<tvm/runtime/logging.h\>\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DDMLC_USE_LOGGING_LIBRARY=${LOGGING_HEADER} -DTVM_USE_LIBBACKTRACE=0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DDMLC_USE_LOGGING_LIBRARY=${LOGGING_HEADER} -DTVM_USE_LIBBACKTRACE=0")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets LinguistTools)

set(TS_FILES ${PROJECT_NAME}_ru_RU.ts)

file(GLOB src_files
    "src/*.cpp"
    "src/*/*.cpp"
)

FILE(GLOB_RECURSE headers "include/*.h")

set(PROJECT_SOURCES
    main.cpp
    ${src_files}
    ${headers}
    qml/qml.qrc
    ${TS_FILES}
)

find_package(OpenCV REQUIRED)

set(MTCNN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/opencv-mtcnn")
add_subdirectory("${MTCNN_PATH}")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
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

include_directories(
    "include"
    "${TVM_BUILD_DIR}/../include"
    "${TVM_BUILD_DIR}/../3rdparty/dmlc-core/include/"
    "${TVM_BUILD_DIR}/../3rdparty/dlpack/include"
    "${OpenCV_INCLUDE_DIRS}"
    "3rd_party/opencv-mtcnn/lib/include"
)

find_library(LIB_TVM NAMES tvm_runtime PATHS ${TVM_BUILD_DIR})
find_library(LIB_MTCNN NAMES mtcnn PATHS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Widgets
  ${LIB_TVM}
  ${OpenCV_LIBS}
  mtcnn
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER engagement_estimator.hse.ru
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
