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
function(install_share_dirs)

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_STAGE_BIN_DIR "${PKG_STAGE_DIR}/bin")

  #<prefix>/share for gdal data files
  set(PKG_SHARE_DEST_DIR ${PKG_STAGE_DIR}/share)

  set(PKG_SHARE_SOURCE_DIR ${SUPERBUILD_INSTALL_DIR}/share)

  ####################### install mvd i18N ############################
  set(MVD_I18N_SOURCE_DIR ${PKG_SHARE_SOURCE_DIR}/otb/i18n)
  if(NOT EXISTS "${MVD_I18N_SOURCE_DIR}")
    message(FATAL_ERROR "Error ${MVD_I18N_SOURCE_DIR} not exists")
  endif()

  ####################### install GDAL data ############################
  set(GDAL_DATA_DIR ${PKG_SHARE_SOURCE_DIR}/gdal)
  #MSVC install gdal-data in in a different directory. So we don't spoil it
  if(MSVC)
    set(GDAL_DATA_DIR ${SUPERBUILD_INSTALL_DIR}/data)
  endif()
  if(NOT EXISTS "${GDAL_DATA_DIR}/epsg.wkt")
    message(FATAL_ERROR
      "Cannot generate package without GDAL_DATA : ${GDAL_DATA_DIR}")
  endif()
  install_without_message("${GDAL_DATA_DIR}" "share" )

  ####################### install OSSIM data ##########################
  install_without_message("${PKG_SHARE_SOURCE_DIR}/ossim" "share" )

  ####################### install proj share ##########################
  if(EXISTS ${PKG_SHARE_SOURCE_DIR}/proj)
    install_without_message("${PKG_SHARE_SOURCE_DIR}/proj" "share" )
  endif()
  
  ####################### Install copyrights ##########################
  #install license for packages
  install_without_message("${PKG_SHARE_SOURCE_DIR}/copyright" "share" )
  install(FILES ${OTBPackaging_SOURCE_DIR}/LICENSE
    DESTINATION ${PKG_STAGE_DIR})

  ####################### install share/otb ###########################
  install_without_message("${PKG_SHARE_SOURCE_DIR}/otb" "share" )

endfunction()

