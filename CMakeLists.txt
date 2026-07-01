cmake_minimum_required(VERSION 3.10)
project(Walper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_executable(walper
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/filedialog.h
    src/filedialog.cpp
)

target_link_libraries(walper Qt5::Widgets)

# --- Terjemahan embedded (dua file) ---
find_program(LRELEASE_EXECUTABLE NAMES lrelease lrelease5 lrelease-qt5)
if(LRELEASE_EXECUTABLE)
    set(TS_FILES
        ${CMAKE_SOURCE_DIR}/translations/walper_en.ts
        ${CMAKE_SOURCE_DIR}/translations/walper_id.ts)
    set(QM_FILES
        ${CMAKE_SOURCE_DIR}/translations/walper_en.qm
        ${CMAKE_SOURCE_DIR}/translations/walper_id.qm)

    add_custom_target(translations_qm DEPENDS ${QM_FILES})

    list(LENGTH TS_FILES num_ts)
    math(EXPR num_last "${num_ts} - 1")
    foreach(i RANGE 0 ${num_last})
        list(GET TS_FILES ${i} ts_file)
        list(GET QM_FILES ${i} qm_file)
        add_custom_command(
            OUTPUT ${qm_file}
            COMMAND ${LRELEASE_EXECUTABLE} ${ts_file} -qm ${qm_file}
            DEPENDS ${ts_file}
            COMMENT "Generating translation: ${qm_file}"
        )
    endforeach()

    set(RESOURCE_FILE ${CMAKE_SOURCE_DIR}/resources/translations.qrc)
    qt5_add_resources(QRC_SOURCES ${RESOURCE_FILE})
    target_sources(walper PRIVATE ${QRC_SOURCES})
    add_dependencies(walper translations_qm)
    message(STATUS "Embedded translation enabled for en + id")
else()
    message(WARNING "lrelease not found. Translation will not be embedded.")
endif()

# --- Instalasi ---
install(TARGETS walper RUNTIME DESTINATION bin)
install(FILES data/walper.desktop DESTINATION share/applications)

# Salin hy.sh ke ~/.config/walper/ saat instalasi
install(CODE "
    set(HY_SRC \"${CMAKE_SOURCE_DIR}/data/hy.sh\")
    set(CONFIG_DIR \"\$ENV{HOME}/.config/walper\")
    file(MAKE_DIRECTORY \${CONFIG_DIR})
    file(COPY \${HY_SRC} DESTINATION \${CONFIG_DIR}
         FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                         GROUP_READ GROUP_EXECUTE
                         WORLD_READ WORLD_EXECUTE)
    message(\"hy.sh installed to \${CONFIG_DIR}\")
")

# --- Uninstall target (pesan aman tanpa tanda kurung) ---
if(NOT TARGET uninstall)
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -E echo "=== Uninstalling Walper ==="
        COMMAND ${CMAKE_COMMAND} -E remove -f
            "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/walper"
            "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/applications/walper.desktop"
        COMMAND ${CMAKE_COMMAND} -E echo "Removing binary and desktop file"
        COMMAND ${CMAKE_COMMAND} -E remove -f
            "$ENV{HOME}/.config/walper/hy.sh"
        COMMAND ${CMAKE_COMMAND} -E echo "Removing hy.sh from ~/.config/walper"
        COMMAND ${CMAKE_COMMAND} -E echo "Note: ~/.config/walper/history not removed if exists."
        COMMAND ${CMAKE_COMMAND} -E echo "To fully remove config, run: rm -rf ~/.config/walper"
    )
endif()
