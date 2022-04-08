
project(TableRule)

add_executable(TableRule main.cpp ${TABLERULE_SRC_FILES})

target_link_libraries(TableRule PUBLIC PcapPlusPlus::PcapPP)

set_target_properties(TableRule PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/bin")
