#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

set( OTBMonteverdi_SRCS
  mvdApplication.cxx
  mvdMainWindow.cxx
  mvdPreferencesDialog.cxx
  )

#############################################################################
set( OTBMonteverdi_HEADERS_MOC
  ../include/mvdApplication.h
  ../include/mvdMainWindow.h
  ../include/mvdPreferencesDialog.h
  )

#############################################################################
# Qt designer UI files
set( OTBMonteverdi_FORMS
  mvdMainWindow.ui
  mvdPreferencesDialog.ui
  )

#############################################################################
set( OTBMonteverdi_RESOURCES
  mvdMainWindow.qrc
  )

#############################################################################
qt5_wrap_cpp( OTBMonteverdi_SRC_MOC ${OTBMonteverdi_HEADERS_MOC} OPTIONS -p "." )
qt5_wrap_ui( OTBMonteverdi_FORMS_HEADERS ${OTBMonteverdi_FORMS}  )
qt5_add_resources( OTBMonteverdi_RESOURCES_RCC ${OTBMonteverdi_RESOURCES} OPTIONS "-no-compress")

#############################################################################
add_to_qt_i18n_sources( ${OTBMonteverdi_SRCS} )
add_to_qt_i18n_headers( "../include" ${OTBMonteverdi_SRCS} )
add_to_qt_i18n_forms( ${OTBMonteverdi_FORMS} )

#############################################################################
add_library( OTBMonteverdi
  ${OTBMonteverdi_SRCS}
  ${OTBMonteverdi_FORMS_HEADERS}
  ${OTBMonteverdi_SRC_MOC}
  )
target_link_libraries( OTBMonteverdi
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBMonteverdiGUI_LIBRARIES}
  ${OTBQt_LIBRARIES}
  ${OTBQtAdapters_LIBRARIES}
  )
otb_module_target( OTBMonteverdi )

#######################[ Monteverdi executable ]#############################
if (WIN32)
  # Windows Resource file need to have the full path to icon
  # if the .ico is not in the current dir
  set( Monteverdi_App_WIN32_ICON ${OTB_SOURCE_DIR}/Utilities/Data/Icons/monteverdi.ico)
  configure_file(
    mvdWin32.rc.in
    mvdWin32.rc
    @ONLY
    )

  if (MINGW)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  endif()

  # The RC file to be added to source list
  set( Monteverdi_App_WIN32_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc )
endif()

add_executable( monteverdi
  main.cxx
  ${Monteverdi_App_WIN32_RC_FILE}
  ${OTBMonteverdi_RESOURCES_RCC}
  )

# set monterverdi stack size to 10Mbytes on windows platform because
# the default size with visual compiler it is 1Mbyte
# which is to lower for us (thanks to 6S code).
if (WIN32)
  set_linker_stack_size_flag( monteverdi 10000000 )
endif()

target_link_libraries( monteverdi
  OTBMonteverdi
  )
otb_module_target( monteverdi )
