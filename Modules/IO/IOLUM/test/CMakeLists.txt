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

otb_module_test()

set(OTBIOLUMTests
otbIOLUMTestDriver.cxx
otbLUMImageIOTestCanWrite.cxx
otbLUMImageIOTestCanRead.cxx
)

add_executable(otbIOLUMTestDriver ${OTBIOLUMTests})
target_link_libraries(otbIOLUMTestDriver ${OTBIOLUM-Test_LIBRARIES})
otb_module_target_label(otbIOLUMTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuLUMImageIOCanWrite COMMAND otbIOLUMTestDriver otbLUMImageIOTestCanWrite
  ${TEMP}/poupees.lum)

otb_add_test(NAME ioTuLUMImageIOCanRead COMMAND otbIOLUMTestDriver otbLUMImageIOTestCanRead
  ${INPUTDATA}/poupees.lum)

