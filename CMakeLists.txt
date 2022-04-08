cmake_minimum_required(VERSION 3.12)

project(Firewall)

find_package(PcapPlusPlus CONFIG REQUIRED)
message(STATUS "Using Pcap++ ${PcapPlusPlus_VERSION}")
message(STATUS "Include dir: ${PcapPlusPlus_INCLUDE_DIR}")

# Set C++17
set(CMAKE_CXX_STANDARD 17)
# popen()/pclose() are not C++ standards
set(CMAKE_CXX_EXTENSIONS ON)


# Project Debug Variables
set(TABLE_RULE_ONLY 0)
set(GUI_ON 1)

set(PE_NAME "Firewall")
#Table Rule Sources
set(TABLERULE_SRC_FILES
        ${PROJECT_SOURCE_DIR}/TableRule/src/RuleTable.cpp
        ${PROJECT_SOURCE_DIR}/TableRule/src/Rule.cpp
        ${PROJECT_SOURCE_DIR}/TableRule/src/BadParse.cpp)

if (TABLE_RULE_ONLY)
    add_subdirectory(TableRule)
else ()
    if (GUI_ON)
        message(STATUS "GUI FLAG ON")
        set(SDK_ROOT "${CMAKE_BINARY_DIR}/SDK/")
        set(PORT UltralightLinux)
        set(IN_SOURCE_BUILD 0)
        set(SDK_ROOT "${CMAKE_CURRENT_LIST_DIR}/Ultralight")
        set(ULTRALIGHT_INCLUDE_DIR "${SDK_ROOT}/include")
        set(ULTRALIGHT_BINARY_DIR "${SDK_ROOT}/bin")
        set(ULTRALIGHT_INSPECTOR_DIR "${SDK_ROOT}/inspector")
        set(ULTRALIGHT_LIBRARY_DIR "${SDK_ROOT}/bin")
        SET(CMAKE_SKIP_BUILD_RPATH FALSE)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        SET(CMAKE_INSTALL_RPATH $ORIGIN)

        add_subdirectory("GUI")
    else ()
        set(FIREWALL_SRC_FILES
                src/Firewall.cpp
                src/Controller.cpp)

        add_executable(${PE_NAME} main.cpp ${FIREWALL_SRC_FILES} ${TABLERULE_SRC_FILES})

        target_link_libraries(${PE_NAME} PUBLIC PcapPlusPlus::PcapPP)

        set_target_properties(${PE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                "${CMAKE_BINARY_DIR}/bin")
    endif ()
endif ()