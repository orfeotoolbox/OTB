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
set(OTBObjectListTests
  otbObjectList.cxx
  otbImageList.cxx
  otbImageListToImageListApplyFilter.cxx
  otbImageListToVectorImageFilter2.cxx
  otbImageListToVectorImageFilter.cxx
  otbObjectList2.cxx
  otbVectorImageToImageListFilter.cxx
  otbObjectListTestDriver.cxx  )

add_executable(otbObjectListTestDriver ${OTBObjectListTests})
target_link_libraries(otbObjectListTestDriver ${OTBObjectList-Test_LIBRARIES})
otb_module_target_label(otbObjectListTestDriver)
otb_add_test(NAME coTvObjectList COMMAND otbObjectListTestDriver
  otbObjectList
  ${INPUTDATA}/couleurs.jpg
  ${INPUTDATA}/poupees.png
  ${INPUTDATA}/couleurs_extrait.png
  )
otb_add_test(NAME coTvImageList COMMAND otbObjectListTestDriver
  --compare-image ${NOTOL}   ${INPUTDATA}/amst.png
  ${TEMP}/amst.png
  otbImageList
  ${INPUTDATA}/amst.png
  ${TEMP}/amst.png
  )
otb_add_test(NAME bfTvImageListToImageListApplyFilter COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvImageListToImageListApplyFilterBand1.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
  ${BASELINE}/bfTvImageListToImageListApplyFilterBand2.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
  ${BASELINE}/bfTvImageListToImageListApplyFilterBand3.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
  otbImageListToImageListApplyFilter
  ${INPUTDATA}/poupees_c1.raw.hdr
  ${INPUTDATA}/poupees_c2.raw.hdr
  ${INPUTDATA}/poupees_c3.raw.hdr
  ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
  )
otb_add_test(NAME coTvImageListToVectorImageFilter2 COMMAND otbObjectListTestDriver
  otbImageListToVectorImageFilter2
  ${INPUTDATA}/poupees_c1.raw.hdr
  ${INPUTDATA}/poupees_c2.raw.hdr
  ${INPUTDATA}/poupees_c3.raw.hdr
  )
otb_add_test(NAME coTvImageListToVectorImageFilter COMMAND otbObjectListTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvImageListToVectorImageFilter.png
  ${TEMP}/coTvImageListToVectorImageFilter.png
  otbImageListToVectorImageFilter
  ${INPUTDATA}/poupees_c1.raw.hdr
  ${INPUTDATA}/poupees_c2.raw.hdr
  ${INPUTDATA}/poupees_c3.raw.hdr
  ${TEMP}/coTvImageListToVectorImageFilter.png
  )
otb_add_test(NAME coTvObjectListTestNotValid COMMAND otbObjectListTestDriver
  otbObjectList2
  )
otb_add_test(NAME bfTvVectorImageToImageListFilter COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand1.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand2.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand3.png
  otbVectorImageToImageListFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand3.png
  )
otb_add_test(NAME bfTvVectorImageToImageListFilterIt COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterItBand1.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterItBand2.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
  ${TEMP}/bfTvVectorImageToImageListFilterItBand3.png
  otbVectorImageToImageListFilterIterator
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt
  png
  )
otb_add_test(NAME bfTvVectorImageToImageListFilterIt2 COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2Band1.tif
  ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2Band2.tif
  ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2Band3.tif
  otbVectorImageToImageListFilterIterator
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2
  tif
  )
