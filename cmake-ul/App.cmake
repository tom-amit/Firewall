
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)



set(SDK_ROOT "${CMAKE_BINARY_DIR}/SDK")
set(ULTRALIGHT_INCLUDE_DIR "${SDK_ROOT}/include")
set(ULTRALIGHT_BINARY_DIR "${SDK_ROOT}/bin")
set(ULTRALIGHT_RESOURCES_DIR "${SDK_ROOT}/resources")
set(ULTRALIGHT_INSPECTOR_DIR "${SDK_ROOT}/inspector")

set(PORT UltralightLinux)
set(PLATFORM "linux")
set(ULTRALIGHT_LIBRARY_DIR "${SDK_ROOT}/bin")

if (NOT DEFINED ULTRALIGHT_VERSION)
    set(ULTRALIGHT_VERSION 1.2.1)
endif ()
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else ()
    set(ARCHITECTURE "x86")
endif ()

ExternalProject_Add(UltralightSDK
        URL https://github.com/ultralight-ux/Ultralight/releases/download/v${ULTRALIGHT_VERSION}/ultralight-sdk-${ULTRALIGHT_VERSION}-${PLATFORM}-${ARCHITECTURE}.7z
        SOURCE_DIR "${SDK_ROOT}"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        )

MACRO(LINK_GUI source_list)
    set(APP_NAME ${PE_NAME})

    include_directories("${ULTRALIGHT_INCLUDE_DIR}")
    link_directories("${ULTRALIGHT_LIBRARY_DIR}")
    link_libraries(UltralightCore AppCore Ultralight WebCore)

    SET(CMAKE_SKIP_BUILD_RPATH FALSE)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    SET(CMAKE_INSTALL_RPATH $ORIGIN)

    add_executable(${APP_NAME} ${source_list})

    # Copy all binaries to target directory
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_BINARY_DIR}" $<TARGET_FILE_DIR:${APP_NAME}>)

    # Set the assets path to "/assets"
    set(ASSETS_PATH "$<TARGET_FILE_DIR:${APP_NAME}>/assets")
    # set(RULES_PATH "$<TARGET_FILE_DIR:${APP_NAME}>/rules")
    # Copy assets to assets path
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "../assets" "${ASSETS_PATH}")

    if (${ENABLE_INSPECTOR})
        # Copy inspector to assets directory
        add_custom_command(TARGET ${APP_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_INSPECTOR_DIR}" "${ASSETS_PATH}/inspector")
    endif ()

    # Copy resources to assets directory
    #add_custom_command(TARGET ${APP_NAME} POST_BUILD
    #  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_RESOURCES_DIR}" "${ASSETS_PATH}/resources")

    add_dependencies(${APP_NAME} UltralightSDK)

ENDMACRO()