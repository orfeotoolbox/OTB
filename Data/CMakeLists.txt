if (UNIX AND NOT APPLE)

# -------------------------
# Install icons
# following freedesktop recommandations
# http://standards.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html

# The different icon sizes installed
# According to the specs, the 48x48 icon is mandatory, others are optional
set( icon_sizes 16 32 48 128 )
foreach( icon_size ${icon_sizes} )
  install(FILES Icons/monteverdi-${icon_size}x${icon_size}.png
          DESTINATION share/icons/hicolor/${icon_size}x${icon_size}/apps
          RENAME monteverdi.png
          COMPONENT Resources)
endforeach()

# Fallback on "/usr/share/pixmaps"
# See http://standards.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html#directory_layout
install(FILES Icons/monteverdi-32x32.png
        DESTINATION share/pixmaps
        RENAME monteverdi.png
        COMPONENT Resources)

install(FILES Icons/monteverdi-splash.png
        DESTINATION share/pixmaps
        RENAME monteverdi-splash.png
        COMPONENT Resources)


install(FILES Icons/monteverdi-32x32.xpm
        DESTINATION share/pixmaps
        RENAME monteverdi.xpm
        COMPONENT Resources)

# -------------------------
# Install freedesktop entry

install(FILES monteverdi.desktop
        DESTINATION share/applications
        COMPONENT Resources)

endif()
