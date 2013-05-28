project(personviewer)

set(KDE_MIN_VERSION "4.6.75")
find_package (KDE4 4.6.75 REQUIRED)
find_package(Nepomuk REQUIRED)
find_package(NepomukCore REQUIRED)
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
  ${NEPOMUK_INCLUDE_DIR}
  ${LibKFbAPI_INCLUDE_DIR}
  )

add_subdirectory (pixmaps)

set(personviewer_SRCS
    abstract-person-details-widget.cpp
    email-details-widget.cpp
    im-details-widget.cpp
    persons-list-view.cpp 
    persons-proxy-model.cpp
    person-details-view.cpp
    persons-delegate.cpp
    phone-details-widget.cpp
    filter-bar.cpp
    main-window.cpp
    main.cpp
    recent-emails-details-widget.cpp
)

if(LibKFbAPI_FOUND)
    add_definitions(-DHAS_FACEBOOK)
    set(personviewer_SRCS facebook-connector.cpp ${personviewer_SRCS})
endif(LibKFbAPI_FOUND)

kde4_add_ui_files (personviewer_SRCS
                   main-window.ui
                   person-details-view.ui
)

kde4_add_executable (personviewer
                     ${personviewer_SRCS}
)

target_link_libraries (personviewer
                       kpeople
                       ${KDE4_KDEUI_LIBS}
                       ${KDE4_KIO_LIBS}
                       ${NEPOMUK_CORE_LIBRARY}
                       ${NEPOMUK_LIBRARIES}
                       ${NEPOMUK_QUERY_LIBRARIES}
                       ${SOPRANO_LIBRARIES}
                       ${LibKFbAPI_LIBRARY}
)

install (TARGETS personviewer
         DESTINATION ${BIN_INSTALL_DIR}
)

install (PROGRAMS personviewer.desktop
         DESTINATION ${XDG_APPS_INSTALL_DIR}
)
