project(PersonViewer)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.3.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(KF5 REQUIRED COMPONENTS WidgetsAddons KDELibs4Support)
find_package(KF5People 0.5 REQUIRED)
find_package(LibKFbAPI 1.0)
set_package_properties(LibKFbAPI PROPERTIES DESCRIPTION "A library to access Facebook services"
                                            URL "http://projects.kde.org/libkfbapi"
                                            TYPE OPTIONAL
                                            PURPOSE "LibKFbAPI is required to build Akonadi resources to access Facebook's contacts, events, notes and posts" )

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

set(personviewer_SRCS
    persons-list-view.cpp 
    persons-proxy-model.cpp
    persons-delegate.cpp
    filter-bar.cpp
    main-window.cpp
    main.cpp
)

qt5_wrap_ui (personviewer_SRCS
                   main-window.ui
)

add_executable (personviewer
                     ${personviewer_SRCS}
)

target_link_libraries (personviewer
                       KF5::PeopleWidgets
                       KF5::KDELibs4Support
)

install (TARGETS personviewer
         ${INSTALL_TARGETS_DEFAULT_ARGS}
)

install (PROGRAMS personviewer.desktop
         DESTINATION ${XDG_APPS_INSTALL_DIR}
)
