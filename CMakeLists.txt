project(personviewer)

set(KDE_MIN_VERSION "4.6.75")
find_package (KDE4 4.6.75 REQUIRED)
find_package(Nepomuk REQUIRED)

include (KDE4Defaults)
include (MacroLibrary)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  )

set(personviewer_SRCS photo-overlay-buttons.cpp person-details-view.cpp persons-delegate.cpp photo-overlay-event-filter.cpp
    filter-bar.cpp
    main-window.cpp
    main.cpp)

kde4_add_ui_files (personviewer_SRCS
                   main-window.ui
                   person-details-view.ui
)

kde4_add_executable (person-viewer
                     ${personviewer_SRCS}
)

target_link_libraries (person-viewer
                       person
                       ${KDE4_KDEUI_LIBS}
                       ${KDE4_KIO_LIBS}
                       ${NEPOMUK_LIBRARIES}
                       ${NEPOMUK_QUERY_LIBRARIES}
                       ${SOPRANO_LIBRARIES}
                       nepomukdatamanagement
)

install (TARGETS person-viewer
         DESTINATION ${BIN_INSTALL_DIR}
)