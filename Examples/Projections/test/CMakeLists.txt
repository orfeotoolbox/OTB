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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Projections)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

otb_add_test(NAME prTeOrthoRectificationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_2}
    ${BASELINE}/panOrthoToul.tif
    ${TEMP}/panOrthoToul.tif
  Execute $<TARGET_FILE:OrthoRectificationExample>
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
    ${TEMP}/panOrthoToul.tif
    31
    N
    375000
    4828100
    500
    500
    0.6
    -0.6
)

otb_add_test(NAME prTeOrthoRectificationExampleXSTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_2}
    ${BASELINE}/xsOrthoToul.tif
    ${TEMP}/xsOrthoToul.tif
  Execute $<TARGET_FILE:OrthoRectificationExample>
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
    ${TEMP}/xsOrthoToul.tif
    31
    N
    375000
    4828100
    500
    500
    0.6
    -0.6
)

otb_add_test(NAME prTeVectorDataProjectionExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ogr ${NOTOL}
    ${BASELINE}/vectorDataProjectionExample.shp
    ${TEMP}/vectorDataProjectionExample.shp
  Execute $<TARGET_FILE:VectorDataProjectionExample>
    ${OTB_DATA_ROOT}/Input/Capitole-Shadows.kml
    ${OTB_DATA_ROOT}/Input/QB_PAN_ROI_1000_100.tif
    ${TEMP}/vectorDataProjectionExample.shp
)

otb_add_test(NAME prTeGeometriesProjectionTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ogr ${EPSILON_7}
    ${BASELINE}/geometriesProjectionExample.shp
    ${TEMP}/geometriesProjectionExample.shp
  Execute $<TARGET_FILE:GeometriesProjectionExample>
    ${OTB_DATA_ROOT}/Input/Capitole-Shadows.shp
    ${OTB_DATA_ROOT}/Input/QB_PAN_ROI_1000_100.tif
    ${TEMP}/geometriesProjectionExample.shp
)

#TODO change the example by using a SPOT4 roamnia
otb_add_test(NAME prTeEstimateRPCSensorModelExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_4}
    ${BASELINE}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
    ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
  --ignore-lines-with 6 PipelineMTime ImportImageContaine Source: Image Time: Pointer:
  Execute $<TARGET_FILE:EstimateRPCSensorModelExample>
    ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
    ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
  2000 2000 1.41809 43.6143 0
  2000 2050 1.4181 43.6131 0
  2000 2100 1.4181 43.6119 0
  2000 2150 1.41811 43.6107 0
  2000 2200 1.41812 43.6095 0
  2000 2250 1.41812 43.6083 0
  2000 2300 1.41813 43.6071 0
  2000 2350 1.41814 43.6058 0
  2000 2400 1.41814 43.6046 0
  2000 2450 1.41815 43.6034 0
  2050 2000 1.41967 43.6143 0
  2050 2050 1.41968 43.6131 0
  2050 2100 1.41969 43.6119 0
  2050 2150 1.41969 43.6107 0
  2050 2200 1.4197 43.6095 0
  2050 2250 1.41971 43.6083 0
  2050 2300 1.41971 43.6071 0
  2050 2350 1.41972 43.6059 0
  2050 2400 1.41973 43.6046 0
  2050 2450 1.41973 43.6034 0
  2100 2000 1.42126 43.6143 0
  2100 2050 1.42126 43.6131 0
  2100 2100 1.42127 43.6119 0
  2100 2150 1.42128 43.6107 0
  2100 2200 1.42128 43.6095 0
  2100 2250 1.42129 43.6083 0
  2100 2300 1.4213 43.6071 0
  2100 2350 1.4213 43.6059 0
  2100 2400 1.42131 43.6046 0
  2100 2450 1.42132 43.6034 0
  2150 2000 1.42284 43.6143 0
  2150 2050 1.42285 43.6131 0
  2150 2100 1.42285 43.6119 0
  2150 2150 1.42286 43.6107 0
  2150 2200 1.42286 43.6095 0
  2150 2250 1.42287 43.6083 0
  2150 2300 1.42288 43.6071 0
  2150 2350 1.42288 43.6059 0
  2150 2400 1.42289 43.6046 0
  2150 2450 1.4229 43.6034 0
  2200 2000 1.42442 43.6144 0
  2200 2050 1.42443 43.6131 0
  2200 2100 1.42443 43.6119 0
  2200 2150 1.42444 43.6107 0
  2200 2200 1.42445 43.6095 0
  2200 2250 1.42445 43.6083 0
  2200 2300 1.42446 43.6071 0
  2200 2350 1.42447 43.6059 0
  2200 2400 1.42447 43.6046 0
  2200 2450 1.42448 43.6034 0
  2250 2000 1.42601 43.6144 0
  2250 2050 1.42601 43.6131 0
  2250 2100 1.42602 43.6119 0
  2250 2150 1.42602 43.6107 0
  2250 2200 1.42603 43.6095 0
  2250 2250 1.42604 43.6083 0
  2250 2300 1.42604 43.6071 0
  2250 2350 1.42605 43.6059 0
  2250 2400 1.42605 43.6047 0
  2250 2450 1.42606 43.6034 0
  2300 2000 1.42759 43.6144 0
  2300 2050 1.42759 43.6131 0
  2300 2100 1.4276 43.6119 0
  2300 2150 1.42761 43.6107 0
  2300 2200 1.42761 43.6095 0
  2300 2250 1.42762 43.6083 0
  2300 2300 1.42762 43.6071 0
  2300 2350 1.42763 43.6059 0
  2300 2400 1.42764 43.6047 0
  2300 2450 1.42764 43.6034 0
  2350 2000 1.42917 43.6144 0
  2350 2050 1.42918 43.6131 0
  2350 2100 1.42918 43.6119 0
  2350 2150 1.42919 43.6107 0
  2350 2200 1.42919 43.6095 0
  2350 2250 1.4292 43.6083 0
  2350 2300 1.42921 43.6071 0
  2350 2350 1.42921 43.6059 0
  2350 2400 1.42922 43.6047 0
  2350 2450 1.42922 43.6034 0
  2400 2000 1.43075 43.6144 0
  2400 2050 1.43076 43.6131 0
  2400 2100 1.43076 43.6119 0
  2400 2150 1.43077 43.6107 0
  2400 2200 1.43078 43.6095 0
  2400 2250 1.43078 43.6083 0
  2400 2300 1.43079 43.6071 0
  2400 2350 1.43079 43.6059 0
  2400 2400 1.4308 43.6047 0
  2400 2450 1.43081 43.6034 0
  2450 2000 1.43234 43.6144 0
  2450 2050 1.43234 43.6132 0
  2450 2100 1.43235 43.6119 0
  2450 2150 1.43235 43.6107 0
  2450 2200 1.43236 43.6095 0
  2450 2250 1.43236 43.6083 0
  2450 2300 1.43237 43.6071 0
  2450 2350 1.43238 43.6059 0
  2450 2400 1.43238 43.6047 0
  2450 2450 1.43239 43.6034 0
)

otb_add_test(NAME prTePlaceNameToLonLatExampleTest COMMAND ${OTB_TEST_DRIVER}
  Execute $<TARGET_FILE:PlaceNameToLonLatExample>
    Toulouse
)
