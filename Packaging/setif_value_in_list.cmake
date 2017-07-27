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
function(setif_value_in_list matched VALUE list)
  set(${matched} FALSE  PARENT_SCOPE)
  string(TOLOWER ${VALUE} value)
  foreach (PATTERN ${${list}})
    string(TOLOWER ${PATTERN} pattern)
    if("${value}" MATCHES "${pattern}")
      set(${matched} TRUE PARENT_SCOPE)
      #we found a match. return from loop.
      return()
    endif()
  endforeach()
  #message("'${value}' not found in ${list}")
endfunction()
