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

otb_module_test()

set(OTBIORADTests
otbIORADTestDriver.cxx
otbRADImageIOTestCanRead.cxx
)

add_executable(otbIORADTestDriver ${OTBIORADTests})
target_link_libraries(otbIORADTestDriver ${OTBIORAD-Test_LIBRARIES})
otb_module_target_label(otbIORADTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuRADImageIOCanRead_RADCR4sngl COMMAND otbIORADTestDriver
  otbRADImageIOTestCanRead
  ${INPUTDATA}/RADCR4_image.rad )

otb_add_test(NAME ioTuRADImageIOCanRead_RADCR4mult COMMAND otbIORADTestDriver
  otbRADImageIOTestCanRead
  ${INPUTDATA}/RADCR4_multimage.rad )

otb_add_test(NAME ioTuRADImageIOCanRead_RADCI2 COMMAND otbIORADTestDriver
  otbRADImageIOTestCanRead
  LARGEINPUT{RAD_CI2/3017_HH.rad})

