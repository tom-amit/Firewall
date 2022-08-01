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

if (NOT DEFINED GUI_ON)
    set(GUI_ON 0)
endif ()

set(PE_NAME "Firewall")

#Table Rule Sources
set(TABLERULE_SRC_FILES
        ${PROJECT_SOURCE_DIR}/TableRule/src/RuleTable.cpp
        ${PROJECT_SOURCE_DIR}/TableRule/src/Rule.cpp
        ${PROJECT_SOURCE_DIR}/TableRule/src/BadParse.cpp)

if (TABLE_RULE_ONLY)
    add_subdirectory(TableRule)
else ()
    set(FIREWALL_SRC_FILES
            src/Firewall.cpp
            src/Controller.cpp src/NICS.cpp src/ArpTable.cpp)
    if (GUI_ON)
        include(cmake-ul/App.cmake)
        set(GUI_SOURCE
                UserInterface.cpp)
        LINK_GUI("${GUI_SOURCE};${FIREWALL_SRC_FILES};${TABLERULE_SRC_FILES}")
    else ()
        add_executable(${PE_NAME} main.cpp ${FIREWALL_SRC_FILES} ${TABLERULE_SRC_FILES})

    endif ()
    target_link_libraries(${PE_NAME} PUBLIC PcapPlusPlus::PcapPP)

    set_target_properties(${PE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/bin")
endif ()