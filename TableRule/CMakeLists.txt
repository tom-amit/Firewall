cmake_minimum_required(VERSION 3.10)

project(TableRule)
if (NOT DEFINED PE_NAME)
    # Set C++17
    set(CMAKE_CXX_STANDARD 17)
    # popen()/pclose() are not C++ standards
    set(CMAKE_CXX_EXTENSIONS ON)

    find_package(PcapPlusPlus CONFIG REQUIRED)
    message(STATUS "Using Pcap++ ${PcapPlusPlus_VERSION}")
    message(STATUS "Include dir: ${PcapPlusPlus_INCLUDE_DIR}")

    set(TABLERULE_SRC_FILES
            src/RuleTable.cpp
            src/Rule.cpp
            src/BadParse.cpp)
    set(PE_NAME "TableRule")
endif ()
message(STATUS "Lmao ${PE_NAME}")
add_executable(${PE_NAME} main.cpp ${TABLERULE_SRC_FILES})

target_link_libraries(${PE_NAME} PUBLIC PcapPlusPlus::PcapPP)

set_target_properties(${PE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/bin")