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

otb_module_test()

set(OTBStereoFiltersTests
  otbStereoFiltersTestDriver.cxx
  CostVolumeFilter.cpp
  PatchMatch.cpp
)

add_executable(otbStereoFiltersTestDriver ${OTBStereoFiltersTests})
target_link_libraries(otbStereoFiltersTestDriver ${OTBStereoFilters-Test_LIBRARIES})
otb_module_target_label(otbStereoFiltersTestDriver)


otb_add_test(NAME dmTvPatchMatch COMMAND otbStereoFiltersTestDriver testPatchMatch 9 -15 0 
${INPUTDATA}/sensor_stereo_left_gridbasedresampling.tif 
${INPUTDATA}/sensor_stereo_right_gridbasedresampling.tif 
${TEMP}/dmTvPatchMatch/)


#----------- StereoFilters TESTS ----------------
otb_test_application(NAME CostVolumeFilterTest
                     APP  CostVolumeFilter
                     OPTIONS -io.inleft ${INPUTDATA}/sensor_stereo_left_gridbasedresampling.tif
                             -io.inright ${INPUTDATA}/sensor_stereo_right_gridbasedresampling.tif
                             -io.out ${TEMP}/apTvCostVolumeFilterTest.tif
                             -hdispmin -15
                             -hdispmax 0                                          
                             -radius 9
                             -rmf 19
                             
                     VALID   --compare-image ${EPSILON_10}
                         ${BASELINE}/apTvCostVolumeFilterTest.tif
                         ${TEMP}/apTvCostVolumeFilterTest.tif
                     )


otb_test_application(NAME PatchMatchTest
                     APP  PatchMatch
                     OPTIONS -io.inleft ${INPUTDATA}/sensor_stereo_left_gridbasedresampling.tif
                             -io.inright ${INPUTDATA}/sensor_stereo_right_gridbasedresampling.tif
                             -io.out ${TEMP}/apTvPatchMatchTest.tif
                             -hdispmin -15
                             -hdispmax 0                                          
                             -radius 9
                             -rmf 19
                             
                     VALID   --compare-image ${EPSILON_10}
                         ${BASELINE}/apTvPatchMatchTest.tif
                         ${TEMP}/apTvPatchMatchTest.tif
                     )
