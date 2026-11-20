#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
set(CTEST_PROJECT_NAME OTB)

if(CMAKE_VERSION VERSION_GREATER 3.14)
  set(CTEST_SUBMIT_URL https://cdash.orfeo-toolbox.org/submit.php?project=OTB)
else()
  set(CTEST_DROP_METHOD "https")
  set(CTEST_DROP_SITE "cdash.orfeo-toolbox.org")
  set(CTEST_DROP_LOCATION "/submit.php?project=OTB")
endif()

set(CTEST_DROP_SITE_CDASH TRUE)
