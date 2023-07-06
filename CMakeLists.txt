cmake_minimum_required(VERSION 3.14)

project(FABTemplateGenerator LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package( OpenCV REQUIRED )
find_package(ZLIB)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED IMPORTED_TARGET libzip)

add_executable(FABTemplateGenerator
  main.cpp

  # containers
  containers/cardpositiondata.h containers/cardpositiondata.cpp
  containers/image.h containers/image.cpp
  containers/config.h containers/config.cpp
  containers/inputdata.h containers/inputdata.cpp

  # image manipulation
  image-manipulation/imageoverlay.h image-manipulation/imageoverlay.cpp
  image-manipulation/augmentation.h image-manipulation/augmentation.cpp

  # json
  nlohmann-json/json.hpp

  # utils
  utils/zlibcustomimageextractor.h utils/zlibcustomimageextractor.cpp
  utils/random.h utils/random.cpp
  utils/configreader.h utils/configreader.cpp

  # utils::exceptions
  utils/exceptions/invalidvalueexception.h utils/exceptions/invalidvalueexception.cpp
)
target_link_libraries(FABTemplateGenerator
    ${OpenCV_LIBS}
    Qt${QT_VERSION_MAJOR}::Core
    ZLIB::ZLIB
    PkgConfig::LIBZIP
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

install(TARGETS FABTemplateGenerator
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
