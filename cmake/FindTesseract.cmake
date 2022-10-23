# Take it from here: https://github.com/crow-translate/crow-translate/blob/master/cmake/FindTesseract.cmake
# Most distributions use the autotools build system for packaging Tesseract, which currently does not generate CMake Config files

find_package(PkgConfig)
pkg_check_modules(PC_Tesseract QUIET tesseract)

find_path(Tesseract_INCLUDE_DIR
    NAMES tesseract/baseapi.h
    PATHS ${PC_Tesseract_INCLUDE_DIRS}
)

include(SelectLibraryConfigurations)
select_library_configurations(Tesseract)

set(Tesseract_VERSION ${PC_Tesseract_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Tesseract
    FOUND_VAR Tesseract_FOUND
    REQUIRED_VARS
        Tesseract_LIBRARY
        Tesseract_INCLUDE_DIR
    VERSION_VAR Tesseract_VERSION
)

if(Tesseract_FOUND)
    set(Tesseract_LIBRARIES ${Tesseract_LIBRARY})
    set(Tesseract_INCLUDE_DIRS ${Tesseract_INCLUDE_DIR})
    set(Tesseract_DEFINITIONS ${PC_Tesseract_CFLAGS_OTHER})
endif()

mark_as_advanced(
    Tesseract_INCLUDE_DIR
    Tesseract_LIBRARY
)
