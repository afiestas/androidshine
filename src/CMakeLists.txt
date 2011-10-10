project(androidshine)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories(${KDE4_INCLUDES})

set(kded_androidshine_SRCS
    daemon.cpp
    main.cpp
)

qt4_add_dbus_interface(kded_androidshine_SRCS
    org.kde.AndroidShine.Service.xml
    androidshine_service_interface)

kde4_add_plugin(kded_androidshine
    ${kded_androidshine_SRCS}
)

target_link_libraries(kded_androidshine
    ${KDE4_KDECORE_LIBS}
)

install(TARGETS kded_androidshine DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES androidshine.desktop DESTINATION ${SERVICES_INSTALL_DIR}/kded)


