#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
function(search_library input_file pkg_searchdirs result)
  set(${result} "" PARENT_SCOPE)
  foreach(pkg_searchdir ${${pkg_searchdirs}})

    if(EXISTS ${pkg_searchdir}/${input_file} )
      if(PKG_DEBUG)
        message("Found '${pkg_searchdir}/${input_file}' (return)")
      endif()
      set(${result} "${pkg_searchdir}/${input_file}" PARENT_SCOPE)
      return()
    endif()

    #check for file with lowercase
    string(TOLOWER "${input_file}" input_file_lower )
    if(EXISTS ${pkg_searchdir}/${input_file_lower})
      set(${result} "${pkg_searchdir}/${input_file_lower}"  PARENT_SCOPE)
      return()
    endif()
  endforeach()
endfunction()
