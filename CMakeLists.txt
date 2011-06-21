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

set(personviewer_SRCS
    persons-model.cpp
    main-window.cpp
    main.cpp)

kde4_add_ui_files (personviewer_SRCS
                   main-window.ui
)

kde4_add_executable (person-viewer
                     ${personviewer_SRCS}
)

target_link_libraries (person-viewer
                       ${KDE4_KDEUI_LIBS}
                       ${NEPOMUK_LIBRARIES}
                       ${NEPOMUK_QUERY_LIBRARIES}
                       ${SOPRANO_LIBRARIES}
)

install (TARGETS person-viewer
         DESTINATION ${BIN_INSTALL_DIR}
)