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

#TODO add gui variable to be available from command execution, if not defined then default is no gui
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
        #Path of Ultralight directory
        if (NOT DEFINED UL_SDK_ROOT)
            message(FATAL_ERROR "Ultralight was not found!")
        endif ()
        set(ULTRALIGHT_INCLUDE_DIR "${UL_SDK_ROOT}/include")
        set(ULTRALIGHT_BINARY_DIR "${UL_SDK_ROOT}/bin")
        set(ULTRALIGHT_INSPECTOR_DIR "${UL_SDK_ROOT}/inspector")
        set(ULTRALIGHT_LIBRARY_DIR "${UL_SDK_ROOT}/bin")
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