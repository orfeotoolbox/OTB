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
set(OTBTestKernelTests
  otbCompareAsciiTests2.cxx
  otbCompareAsciiTests3.cxx
  otbCompareAsciiTests4.cxx
  otbCompareAsciiTests5.cxx
  otbCompareAsciiTests6.cxx
  otbCompareAsciiTestScientificNotation.cxx
  otbCompareAsciiTests.cxx
  otbCompareAsciiTestsEpsilon3_WhiteSpace.cxx
  otbTestKernelTestDriver.cxx  )

add_executable(otbTestKernelTestDriver ${OTBTestKernelTests})
target_link_libraries(otbTestKernelTestDriver ${OTBTestKernel-Test_LIBRARIES})

# So the story here is that -Wl,-no-needed is a default linker flag on only on Ubuntu
# (debian or archlinux does not use this). This will exclude a lot of libraries from linking into
# executable.
# #ITK_LIBRARIES in a superbuild now contains the following libraries.
# "itkdouble-conversion itksys itkvcl itknetlib itkv3p_netlib itkvnl itkvnl_algo
# ITKVNLInstantiation ITKCommon itkNetlibSlatec ITKStatistics ITKTransform ITKLabelMap
# ITKMesh ITKMetaIO ITKSpatialObjects ITKPath ITKQuadEdgeMesh ITKIOImageBase ITKOptimizers
# ITKPolynomials ITKBiasCorrection ITKFFT ITKKLMRegionGrowing ITKOptimizersv4 ITKWatersheds"

# OTBITK module will find all of these components in Modules/ThirdParty/ITK/otb-module-init.cmake
# But due to default -Wl,-as-needed flag many of them are ignored.
# see the documentation of GNU ld -as-needed for details on this behaviour.

# Our problem is that we need these ignored ones in the some header-only modules.
# Eg: Modules/Core/LabelMap
# This module depends on ITKLabelMap but does not link with it as it doesn't create any module (lib, exe)
# All it use is a couple of headers from ITKLabelMap. So removing that dependency is out of question.

# At this point, possible solution are follows:
# 1. use -Wl,-as-no-needed to CMAKE_LINKER FLAGS and be done with it. (all executable contains many used libs)
# 2. glob all libITK*, libitk* in the package script. (ugly and I don't agree)
# 3. make a dummy executable and link that with all of OTB libs and use -Wl,-no-as-needed in that
# 4. pick one of the random test executable and only add -Wl,-no-as-needed to it. (still a hack)
# ..
# ...
# RK: I finally decided to go with 4th option.
# Take an existing test executable otbTestKernelTestDriver and force -Wl,-no-as-needed.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set_target_properties(otbTestKernelTestDriver PROPERTIES LINK_FLAGS "-Wl,-no-as-needed")
endif()

otb_module_target_label(otbTestKernelTestDriver)
otb_add_test(NAME tsTvCompareAscii2 COMMAND otbTestKernelTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef2.txt
  ${TEMP}/tsTvCompareAsciiTest2.txt
  otbCompareAsciiTests2
  ${TEMP}/tsTvCompareAsciiRef2.txt
  ${TEMP}/tsTvCompareAsciiTest2.txt
  )
set_property(TEST tsTvCompareAscii2 PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii2reverse COMMAND otbTestKernelTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef2reverse.txt
  ${TEMP}/tsTvCompareAsciiTest2reverse.txt
  otbCompareAsciiTests2
  ${TEMP}/tsTvCompareAsciiTest2reverse.txt
  ${TEMP}/tsTvCompareAsciiRef2reverse.txt
  )
set_property(TEST tsTvCompareAscii2reverse PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii3order COMMAND otbTestKernelTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef3order.txt
  ${TEMP}/tsTvCompareAsciiTest3order.txt
  otbCompareAsciiTests3
  ${TEMP}/tsTvCompareAsciiRef3order.txt
  ${TEMP}/tsTvCompareAsciiTest3order.txt
  )
set_property(TEST tsTvCompareAscii3order PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii3 COMMAND otbTestKernelTestDriver
  --ignore-order --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef3.txt
  ${TEMP}/tsTvCompareAsciiTest3.txt
  otbCompareAsciiTests3
  ${TEMP}/tsTvCompareAsciiRef3.txt
  ${TEMP}/tsTvCompareAsciiTest3.txt
  )
otb_add_test(NAME tsTvCompareAscii4 COMMAND otbTestKernelTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef4.txt
  ${TEMP}/tsTvCompareAsciiTest4.txt
  otbCompareAsciiTests4
  ${TEMP}/tsTvCompareAsciiRef4.txt
  ${TEMP}/tsTvCompareAsciiTest4.txt
  )
set_property(TEST tsTvCompareAscii4 PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii4epsilon2 COMMAND otbTestKernelTestDriver
  --compare-ascii ${EPSILON_2}
  ${TEMP}/tsTvCompareAsciiRef4epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon2.txt
  otbCompareAsciiTests4
  ${TEMP}/tsTvCompareAsciiRef4epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon2.txt
  )
otb_add_test(NAME tsTvCompareAscii4epsilon3 COMMAND otbTestKernelTestDriver
  --compare-ascii ${EPSILON_3}
  ${TEMP}/tsTvCompareAsciiRef4epsilon3.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon3.txt
  otbCompareAsciiTests4
  ${TEMP}/tsTvCompareAsciiRef4epsilon3.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon3.txt
  )
set_property(TEST tsTvCompareAscii4epsilon3 PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii5epsilon2 COMMAND otbTestKernelTestDriver
  --ignore-order  --compare-ascii ${EPSILON_2}
  ${TEMP}/tsTvCompareAsciiRef5epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest5epsilon2.txt
  otbCompareAsciiTests5
  ${TEMP}/tsTvCompareAsciiRef5epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest5epsilon2.txt
  )
otb_add_test(NAME tsTvCompareAsciiScientificNotation COMMAND otbTestKernelTestDriver
  --compare-ascii ${EPSILON_2}
  ${TEMP}/tsTvCompareAsciiRef_SN.txt
  ${TEMP}/tsTvCompareAsciiTest_SN.txt
  otbCompareAsciiTestScientificNotation
  ${TEMP}/tsTvCompareAsciiRef_SN.txt
  ${TEMP}/tsTvCompareAsciiTest_SN.txt
  )
otb_add_test(NAME tsTvCompareAscii COMMAND otbTestKernelTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef.txt
  ${TEMP}/tsTvCompareAsciiTest.txt
  otbCompareAsciiTests
  ${TEMP}/tsTvCompareAsciiRef.txt
  ${TEMP}/tsTvCompareAsciiTest.txt
  )
otb_add_test(NAME tsTvCompareAsciiepsilon3_WhiteSpaceOK COMMAND otbTestKernelTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/tsTvCompareAsciiEpsilon3_Base.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestOK.txt
  otbCompareAsciiTestsEpsilon3_WhiteSpace
  ${INPUTDATA}/tsTvCompareAsciiEpsilon3_TestOK.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestOK.txt
  )
otb_add_test(NAME tsTvCompareAsciiepsilon3_WhiteSpaceKO COMMAND otbTestKernelTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/tsTvCompareAsciiEpsilon3_Base.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestKO.txt
  otbCompareAsciiTestsEpsilon3_WhiteSpace
  ${INPUTDATA}/tsTvCompareAsciiEpsilon3_TestKO.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestKO.txt
  )
set_property(TEST tsTvCompareAsciiepsilon3_WhiteSpaceKO PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii6 COMMAND otbTestKernelTestDriver
  --compare-ascii ${EPSILON_3}
  ${TEMP}/tsTvCompareAscii6_ref.txt
  ${TEMP}/tsTvCompareAscii6_test.txt
  otbCompareAsciiTests6
  ${TEMP}/tsTvCompareAscii6_ref.txt
  ${TEMP}/tsTvCompareAscii6_test.txt
  )
