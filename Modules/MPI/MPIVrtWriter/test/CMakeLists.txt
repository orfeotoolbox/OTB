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

#${otb-module} will be the name of this module and will not need to be #changed when this module is renamed.

set(${otb-module}Tests
  otbMPIVrtWriterTestDriver.cxx
   otbMPIReadWriteTest.cxx
)

add_executable(otbMPIVrtWriterTestDriver ${${otb-module}Tests}) 
target_link_libraries(otbMPIVrtWriterTestDriver ${${otb-module}-Test_LIBRARIES})
otb_module_target_label(otbMPIVrtWriterTestDriver)


# MPI Hello test
otb_add_test_mpi(NAME otbMPIVrtWriterTest
   NBPROCS 2
   COMMAND otbMPIVrtWriterTestDriver
   --compare-image ${NOTOL}
   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
   ${TEMP}/otbMPIVrtWriterTestOutput.vrt
   otbMPIReadWriteTest
   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
   ${TEMP}/otbMPIVrtWriterTestOutput.vrt
   )
