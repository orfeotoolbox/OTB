#
# Copyright (C) 2005-2025 Centre National d'Etudes Spatiales (CNES)
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

# do not add value twice
function Add-If-Not-Exist {
  param (
    [Parameter(Mandatory)]
    [string]$var_to_check,
    [Parameter(Mandatory)]
    [string]$content
  )

  # NOTE that here we can not use the simple $env: notation as "wonderful"
  # powershell is not even capable to resole $env:$smthing
  # Thus we are force to use ununderstandable ugly verbose "Get/Set" Item
  # notation

  # check if this env var exists
  if (-Not(Test-Path Env:$var_to_check)) {
    Set-Item Env:$var_to_check $content
  }
  else { # if it exists, save its value
    $temp_var = (Get-Item Env:$var_to_check).Value
    # check that we are not re-adding it
    # use "SimpleMatch" option to not interpret $content as a regular expression
    if (-Not($temp_var | Select-String -Pattern $content -SimpleMatch)) {
      Set-Item Env:$var_to_check "$content;$temp_var"
    }
  #   else {
  #     echo "$var_to_check env var already contains $content"
  #   }
  }
}

# Setup environment for OTB package
Add-If-Not-Exist -var_to_check PATH -content "$PSScriptRoot\bin;$PSScriptRoot\lib"
Add-If-Not-Exist -var_to_check PYTHONPATH -content "$PSScriptRoot\lib\otb\python"
Add-If-Not-Exist -var_to_check OTB_APPLICATION_PATH -content "$PSScriptRoot\lib\otb\applications"

$env:GDAL_DATA="$PSScriptRoot\share\data"
$env:GDAL_DRIVER_PATH="disable"
$env:PROJ_LIB="$PSScriptRoot\share\proj"
$env:LC_NUMERIC="C"
