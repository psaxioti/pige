cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_COMPILER   /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(PiGe VERSION 0.1 LANGUAGES CXX)
add_definitions(-DVERSION=${CMAKE_PROJECT_VERSION})
add_definitions(-DPROJECTNAME=${CMAKE_PROJECT_NAME})

include_directories(../demokritoslib/inst/include)
link_directories(../demokritoslib/inst/lib)
find_package(GSL REQUIRED)

cmake_policy(SET CMP0100 NEW)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui Widgets)
#find_package(QT NAMES Qt5 REQUIRED COMPONENTS Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Widgets)
message("Found and using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

include_directories(include)
file(GLOB_RECURSE sources src/*.cc)
file(GLOB_RECURSE headers include/*.hh)

#if(${QT_VERSION_MAJOR} VERSION_LESS 6)
#    file(GLOB_RECURSE resources Resources/*.qrc)
#endif()

set(CMAKE_CXX_FLAGS -ffloat-store)

add_executable(${CMAKE_PROJECT_NAME}
    pige.cc
    ${sources}
    ${headers}
#    ${resources}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    demmasses
    demtarget
    demreaction
    demstopping
    ${GSL_LIBRARIES}
)

#if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL 6)
#    qt_add_resources(${CMAKE_PROJECT_NAME} "stoppingFiles"
#        PREFIX stopping
#        BASE Resources
#        FILES
#            Resources/SCOEF.95A
#            Resources/srim.txt
#    )

#    qt_add_resources(${CMAKE_PROJECT_NAME} "massesFile"
#        PREFIX masses
#        BASE Resources
#        FILES
#            Resources/masses.dat
#    )
#endif()

if(CMAKE_CROSSCOMPILING)
    set_target_properties(${CMAKE_PROJECT_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${WinArch}
            WIN32_EXECUTABLE TRUE
#            SUFFIX "_${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}"
    )
    file(COPY ${PROJECT_SOURCE_DIR}/R33Data DESTINATION ${CMAKE_SOURCE_DIR}/${WinArch})
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND find_dlls ${CMAKE_FIND_ROOT_PATH}
        COMMAND make_msi  ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION} "44F0A703-674F-4831-87F5-E50599899F73"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${WinArch}
    )
else()
    set_target_properties(${CMAKE_PROJECT_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
#            SUFFIX "_Qt${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}"
            MACOSX_BUNDLE TRUE
            WIN32_EXECUTABLE TRUE
    )
endif()

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY R33Data DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})

#install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

#qt_generate_deploy_app_script(
#    TARGET ${CMAKE_PROJECT_NAME}
#    OUTPUT_SCRIPT deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
#)
#install(SCRIPT ${deploy_script})
