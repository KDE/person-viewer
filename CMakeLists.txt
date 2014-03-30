project(personviewer)

set(KDE_MIN_VERSION "4.6.75")
find_package(KDE4 4.6.75 REQUIRED)
find_package(KPeople REQUIRED)
find_package(LibKFbAPI 1.0)
set_package_properties(LibKFbAPI PROPERTIES DESCRIPTION "A library to access Facebook services"
                                            URL "http://projects.kde.org/libkfbapi"
                                            TYPE OPTIONAL
                                            PURPOSE "LibKFbAPI is required to build Akonadi resources to access Facebook's contacts, events, notes and posts" )

include (KDE4Defaults)
include (MacroLibrary)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${LibKFbAPI_INCLUDE_DIR}
  ${KPEOPLE_INCLUDES}
)

set(personviewer_SRCS
    persons-list-view.cpp 
    persons-proxy-model.cpp
    persons-delegate.cpp
    filter-bar.cpp
    main-window.cpp
    main.cpp
)

kde4_add_ui_files (personviewer_SRCS
                   main-window.ui
)

kde4_add_executable (personviewer
                     ${personviewer_SRCS}
)

target_link_libraries (personviewer
                       ${KPEOPLE_LIBRARY}
                       ${KPEOPLEWIDGETS_LIBRARY}
                       ${KDE4_KDEUI_LIBS}
                       ${KDE4_KIO_LIBS}
                       ${SOPRANO_LIBRARIES}
                       ${LibKFbAPI_LIBRARY}
)

install (TARGETS personviewer
         DESTINATION ${BIN_INSTALL_DIR}
)

install (PROGRAMS personviewer.desktop
         DESTINATION ${XDG_APPS_INSTALL_DIR}
)
