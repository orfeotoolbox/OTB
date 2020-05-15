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

set(OTBPathTests
otbPathTestDriver.cxx
otbCompacityPathRectangle.cxx
otbImageToEdgePathFilter.cxx
otbCompacityPathCircle.cxx
otbDrawPathListFilterWithValue.cxx
otbDrawPath.cxx
otbCompacityPathSquare.cxx
otbPathLengthFunctor.cxx
otbPathListToHistogramGenerator.cxx
otbImageFittingPolygonListFilter.cxx
otbDrawPathListFilter.cxx
otbDrawPathFilter.cxx
otbOrientationPath.cxx
otbPolyLineImageConstIterator.cxx
otbRegionImageToRectangularPathListFilter.cxx
otbClosePathFunctor.cxx
otbPolyLineImageIterator.cxx
)

add_executable(otbPathTestDriver ${OTBPathTests})
target_link_libraries(otbPathTestDriver ${OTBPath-Test_LIBRARIES})
otb_module_target_label(otbPathTestDriver)

# Tests Declaration

otb_add_test(NAME feTuCompacityPathRectangle COMMAND otbPathTestDriver
  otbCompacityPathRectangle 10.0  20.0)

otb_add_test(NAME feTvImageToEdgePathFilterBis COMMAND otbPathTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feImageToEdgePathFilter_maskrom.png
  ${TEMP}/feImageToEdgePathFilter_maskrom.png
  otbImageToEdgePathFilter
  ${INPUTDATA}/maskrom.png
  ${TEMP}/feImageToEdgePathFilter_maskrom.png
  0
  )

otb_add_test(NAME feTvImageToEdgePathFilter COMMAND otbPathTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feImageToEdgePathFilterOutput.tif
  ${TEMP}/feImageToEdgePathFilterOutput.tif
  otbImageToEdgePathFilter
  ${INPUTDATA}/labelImage_UnsignedChar.tif
  ${TEMP}/feImageToEdgePathFilterOutput.tif
  3
  )

otb_add_test(NAME feTuCompacityPathCircle COMMAND otbPathTestDriver
  otbCompacityPathCircle 1000)

otb_add_test(NAME coTvDrawPathListFilterWithValue COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvDrawPathListFilterWithValueOutput.png
  ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
  otbDrawPathListFilterWithValue
  512 512 20
  ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
  )

otb_add_test(NAME feTvDrawPathTestCarre COMMAND otbPathTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathDessinCarre.png
  ${TEMP}/feDrawPathDessinCarre.png
  otbDrawPathDessinCarre
  ${INPUTDATA}/DeuxTraits.png
  ${TEMP}/feDrawPathDessinCarre.png)

otb_add_test(NAME feTuCompacityPathSquare COMMAND otbPathTestDriver
  otbCompacityPathSquare 10.0)

otb_add_test(NAME bfTvPathLengthFunctor COMMAND otbPathTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfPathLengthFunctorTest.txt
  ${TEMP}/bfPathLengthFunctorTest.txt
  otbPathLengthFunctor
  ${INPUTDATA}/polygon.png
  ${TEMP}/bfPathLengthFunctorTest.txt
  )

otb_add_test(NAME coTuPathListToHistogramGenerator COMMAND otbPathTestDriver
  otbPathListToHistogramGenerator 100 2)

otb_add_test(NAME feTvImageFittingPolygonListFilter COMMAND otbPathTestDriver
  --compare-ogr ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageFittingPolygonListFilter_Output.shp
  ${TEMP}/feTvImageFittingPolygonListFilter_Output.shp
  otbImageFittingPolygonListFilter
  ${INPUTDATA}/polygon.png
  ${INPUTDATA}/polygon-start.shp
  ${TEMP}/feTvImageFittingPolygonListFilter_Output.shp
  5 10
  )

otb_add_test(NAME coTvDrawPathListFilter COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvDrawPathListFilterOutput.png
  ${TEMP}/coTvDrawPathListFilterOutput.png
  otbDrawPathListFilter
  512 512 20
  ${TEMP}/coTvDrawPathListFilterOutput.png
  )

otb_add_test(NAME coTvDrawPatFilter COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvDrawPathFilterOutput.png
  ${TEMP}/coTvDrawPathFilterOutput.png
  otbDrawPathFilter
  512 512 20
  ${TEMP}/coTvDrawPathFilterOutput.png
  )


otb_add_test(NAME feTuOrientationPath_090 COMMAND otbPathTestDriver
  otbOrientationPath 90.0)

otb_add_test(NAME feTuOrientationPath_045 COMMAND otbPathTestDriver
  otbOrientationPath 45.0)

otb_add_test(NAME feTuOrientationPath_000 COMMAND otbPathTestDriver
  otbOrientationPath 0.0)

otb_add_test(NAME feTuOrientationPath1_80 COMMAND otbPathTestDriver
  otbOrientationPath 180.0)


otb_add_test(NAME coTvPolyLineImageConstIterator COMMAND otbPathTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvPolyLineImageConstIterator.txt
  ${TEMP}/coTvPolyLineImageConstIterator.txt
  otbPolyLineImageConstIterator
  512 512 20
  ${TEMP}/coTvPolyLineImageConstIterator.txt
  )

otb_add_test(NAME feTvRegionImageToRectangularPathListFilter COMMAND otbPathTestDriver
  --compare-ascii ${EPSILON_3}     ${BASELINE_FILES}/feRectangle.txt
  ${TEMP}/feRectangle.txt
  otbRegionImageToRectangularPathListFilter
  ${INPUTDATA}/rectangle.png
  ${TEMP}/feRectangle.txt
  0.9 #fit score
  10)  #minimum size

otb_add_test(NAME bfTvClosePathFunctor COMMAND otbPathTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfClosePathFunctorTest.txt
  ${TEMP}/bfClosePathFunctorTest.txt
  otbClosePathFunctor
  ${INPUTDATA}/polygon.png
  ${TEMP}/bfClosePathFunctorTest.txt
  )

otb_add_test(NAME coTvPolyLineImageIterator COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvPolyLineImageIterator.png
  ${TEMP}/coTvPolyLineImageIterator.png
  otbPolyLineImageIterator
  512 512 20
  ${TEMP}/coTvPolyLineImageIterator.png
  )
