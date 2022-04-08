cmake_minimum_required(VERSION 3.12)

set(TABLE_RULE_ONLY 0)

project(Firewall)

find_package(PcapPlusPlus CONFIG REQUIRED)
message(STATUS "Using Pcap++ ${PcapPlusPlus_VERSION}")
message(STATUS "Include dir: ${PcapPlusPlus_INCLUDE_DIR}")

# Set C++17
set(CMAKE_CXX_STANDARD 17)
# popen()/pclose() are not C++ standards
set(CMAKE_CXX_EXTENSIONS ON)

set(TABLERULE_SRC_FILES
        ${PROJECT_SOURCE_DIR}/TableRule/src/RuleTable.cpp
        ${PROJECT_SOURCE_DIR}/TableRule/src/Rule.cpp
        ${PROJECT_SOURCE_DIR}/TableRule/src/BadParse.cpp)

if (TABLE_RULE_ONLY)
    add_subdirectory(TableRule)
else ()
    set(FIREWALL_SRC_FILES
            src/Firewall.cpp
            src/Controller.cpp)

    add_executable(Firewall main.cpp ${FIREWALL_SRC_FILES} ${TABLERULE_SRC_FILES})

    target_link_libraries(Firewall PUBLIC PcapPlusPlus::PcapPP)

    set_target_properties(Firewall PROPERTIES RUNTIME_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/bin")
endif ()