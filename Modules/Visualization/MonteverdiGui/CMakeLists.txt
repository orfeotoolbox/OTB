#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

project( OTBMonteverdiGUI )

set( OTBMonteverdiGUI_LIBRARIES OTBMonteverdiGUI )

# folder where ui headers are generated
set( OTBMonteverdiGUI_INCLUDE_DIRS
  ${OTBMonteverdiGUI_BINARY_DIR}/src
  )


set(OTBMonteverdiGUI_DATA_DIR "${CMAKE_SOURCE_DIR}/Utilities/Data")

if (UNIX AND NOT APPLE)
  # -------------------------
  # Install icons
  # following freedesktop recommandations
  # http://standards.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html

  # The different icon sizes installed
  # According to the specs, the 48x48 icon is mandatory, others are optional
  set( icon_sizes 16 32 48 128 )
  foreach( icon_size ${icon_sizes} )
    install(FILES ${OTBMonteverdiGUI_DATA_DIR}/Icons/monteverdi-${icon_size}x${icon_size}.png
      DESTINATION share/icons/hicolor/${icon_size}x${icon_size}/apps
      RENAME monteverdi.png)
  endforeach()

  # Fallback on "/usr/share/pixmaps"
  # See http://standards.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html#directory_layout
  install(FILES ${OTBMonteverdiGUI_DATA_DIR}/Icons/monteverdi-32x32.png
    DESTINATION share/pixmaps
    RENAME monteverdi.png)

  install(FILES ${OTBMonteverdiGUI_DATA_DIR}/Icons/monteverdi-splash.png
    DESTINATION share/pixmaps
    RENAME monteverdi-splash.png)

  install(FILES ${OTBMonteverdiGUI_DATA_DIR}/Icons/monteverdi-32x32.xpm
    DESTINATION share/pixmaps
    RENAME monteverdi.xpm)

  # -------------------------
  # Install freedesktop entry

  configure_file(${OTBMonteverdiGUI_DATA_DIR}/monteverdi.desktop.in ${CMAKE_BINARY_DIR}/monteverdi.desktop)
  install(FILES ${CMAKE_BINARY_DIR}/monteverdi.desktop
    DESTINATION share/applications)

endif()

otb_module_impl()
