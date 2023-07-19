cmake_minimum_required(VERSION 3.14)

project(FABTemplateGenerator LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the coverage flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package( OpenCV REQUIRED )
find_package(ZLIB)
find_package(PkgConfig REQUIRED)
find_package(Catch2 3 REQUIRED)
pkg_check_modules(LIBZIP REQUIRED IMPORTED_TARGET libzip)

add_executable(FABTemplateGenerator
  main.cpp
  generation.h generation.cpp

  # containers
  containers/cardpositiondata.h containers/cardpositiondata.cpp
  containers/image.h containers/image.cpp
  containers/config.h containers/config.cpp
  containers/inputdata.h containers/inputdata.cpp
  containers/templatecontainer.h containers/templatecontainer.cpp
  containers/inputdatacontainer.h containers/inputdatacontainer.cpp

  # image-manipulation
  image-manipulation/imageoverlay.h image-manipulation/imageoverlay.cpp

  # image-manipulation::augmentation
  image-manipulation/augmentation/augmentation.h
  image-manipulation/augmentation/noiseaugmentation.h image-manipulation/augmentation/noiseaugmentation.cpp
  image-manipulation/augmentation/lensbluraugmentation.h image-manipulation/augmentation/lensbluraugmentation.cpp
  image-manipulation/augmentation/bilateralbluraugmentation.h image-manipulation/augmentation/bilateralbluraugmentation.cpp
  image-manipulation/augmentation/imageflipaugmentation.h image-manipulation/augmentation/imageflipaugmentation.cpp
  image-manipulation/augmentation/cutoutaugmentation.h image-manipulation/augmentation/cutoutaugmentation.cpp
  image-manipulation/augmentation/brightnessadjustionaugmentation.h image-manipulation/augmentation/brightnessadjustionaugmentation.cpp
  image-manipulation/augmentation/rgbshiftaugmentation.h image-manipulation/augmentation/rgbshiftaugmentation.cpp

  # json
  nlohmann-json/json.hpp

  # utils
  utils/zlibcustomimageextractor.h utils/zlibcustomimageextractor.cpp
  utils/random.h utils/random.cpp
  utils/configreader.h utils/configreader.cpp
  utils/augmentationfactory.h utils/augmentationfactory.cpp

  # utils::exceptions
  utils/exceptions/invalidvalueexception.h utils/exceptions/invalidvalueexception.cpp
  utils/exceptions/pathnotfoundexception.h utils/exceptions/pathnotfoundexception.cpp
  utils/exceptions/libraryexception.h utils/exceptions/libraryexception.cpp
  utils/exceptions/badusageexception.h utils/exceptions/badusageexception.cpp
  utils/exceptions/dividebyzero.h utils/exceptions/dividebyzero.cpp
  utils/exceptions/novalidinputsleftexception.h utils/exceptions/novalidinputsleftexception.cpp
  utils/exceptions/invalidtemplateexception.h utils/exceptions/invalidtemplateexception.cpp
)

target_link_libraries(FABTemplateGenerator
    ${OpenCV_LIBS}
    Qt${QT_VERSION_MAJOR}::Core
    ZLIB::ZLIB
    PkgConfig::LIBZIP
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

include(CTest)
enable_testing()

add_executable(FABTemplateGenerator-Test-Containers
    tests/containers/test-cardpositiondata.cpp
    tests/containers/test-config.cpp
    tests/containers/test-image.cpp
    tests/containers/test-inputdata.cpp
    tests/containers/test-inputdatacontainer.cpp
    tests/containers/test-templatecontainer.cpp
)

add_executable(FABTemplateGenerator-Test-ImageManipulations
    tests/image-manipulation/augmentation/test-augmentation.cpp
    tests/image-manipulation/test-imageoverlay.cpp
)

target_link_libraries(FABTemplateGenerator-Test-Containers
    Catch2::Catch2WithMain
    ${OpenCV_LIBS}
    Qt${QT_VERSION_MAJOR}::Core
    ZLIB::ZLIB
    PkgConfig::LIBZIP
)

target_link_libraries(FABTemplateGenerator-Test-ImageManipulations
    Catch2::Catch2WithMain
    ${OpenCV_LIBS}
    Qt${QT_VERSION_MAJOR}::Core
    ZLIB::ZLIB
    PkgConfig::LIBZIP
)

add_test(NAME FABTemplateGenerator-Test-Containers COMMAND FABTemplateGenerator-Test-Containers)
add_test(NAME FABTemplateGenerator-Test-ImageManipulations COMMAND FABTemplateGenerator-Test-ImageManipulations)

if(CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_SOURCE_DIR}/CodeCoverage.cmake)
    setup_target_for_coverage_gcovr_html(
        NAME coverage
        EXECUTABLE FABTemplateGenerator
        EXCLUDE "${CMAKE_SOURCE_DIR}/nlohmann-json"
        EXCLUDE "${CMAKE_SOURCE_DIR}/tests"
    )
endif()

install(TARGETS FABTemplateGenerator
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
