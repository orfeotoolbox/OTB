if (UNIX AND NOT APPLE)

install(FILES Icons/monteverdi2.xpm Icons/monteverdi2.png
        DESTINATION share/pixmaps
        COMPONENT Resources)

install(FILES monteverdi2.desktop
        DESTINATION share/applications
        COMPONENT Resources)

endif()