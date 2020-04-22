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

project( OTBQt )

set( OTBQt_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5OpenGL_LIBRARIES})
set( OTBQt_SYSTEM_INCLUDE_DIRS ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} )
set( OTBQt_EXPORT_CODE_BUILD
"find_package(Qt5Core REQUIRED HINTS ${Qt5Core_DIR})
find_package(Qt5Gui REQUIRED HINTS ${Qt5Gui_DIR})
find_package(Qt5Widgets REQUIRED HINTS ${Qt5Widgets_DIR})
find_package(Qt5OpenGL REQUIRED HINTS ${Qt5OpenGL_DIR})
find_package(Qt5LinguistTools HINTS ${Qt5LinguistTools_DIR})
")
set( OTBQt_EXPORT_CODE_INSTALL ${OTBQt_EXPORT_CODE_BUILD})

otb_module_impl()
