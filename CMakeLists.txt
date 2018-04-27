#
#/*=========================================================================
# *  Program:   ORFEO Toolbox
# *  Language:  C++
# *  Date:      $Date$
# * Version:   $Revision$
# *
# * Copyright (C) Damia Belazouz - 2017
# *
# *  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
# * See OTBCopyright.txt for details.
# *
# *
# *     This software is distributed WITHOUT ANY WARRANTY; without even
# *     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# *    PURPOSE.  See the above copyright notices for more information.
# *
# *
# * Copyright (C) CS SI
# *
# * This file is part of Orfeo Toolbox
# *
# *     https://www.orfeo-toolbox.org/
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *     http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# 
#=========================================================================*/



cmake_minimum_required(VERSION 2.8.9)
project(OTBStereoFilters)

if(NOT OTB_SOURCE_DIR)
  find_package(OTB REQUIRED)
  list(APPEND CMAKE_MODULE_PATH ${OTB_CMAKE_DIR})
  include(OTBModuleExternal)
else()
  otb_module_impl()
endif()


