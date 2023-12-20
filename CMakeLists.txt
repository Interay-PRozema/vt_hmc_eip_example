cmake_minimum_required(VERSION 3.14)

project(VT_HMC_Testing LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)

add_executable(VT_HMC_Testing
  main.cpp
  hydraulic_controller.h
  hydraulic_controller.cpp
)

include_directories("C:/Program Files (x86)/EIPScanner/include/EIPScanner")

# Link the EIPScanner library
target_link_libraries(VT_HMC_Testing
    Qt${QT_VERSION_MAJOR}::Core
    Qt6::Network
    "C:/Program Files (x86)/EIPScanner/lib/EIPScannerS.lib"
)

include(GNUInstallDirs)
install(TARGETS VT_HMC_Testing
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
