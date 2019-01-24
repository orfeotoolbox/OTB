#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#----------- LineSegmentDetection TESTS ----------------
otb_test_application(NAME   apTvFeLineSegmentDetectionNoRescale
                     APP  LineSegmentDetection
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF}
                             -out ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp
                             -norescale true
                     # VALID   --compare-ogr ${EPSILON_9}
                     #         ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI_NoRescale.shp
                     #         ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp)

otb_test_application(NAME   apTvFeLineSegmentDetection
                     APP  LineSegmentDetection
#                     OPTIONS -in ${INPUTDATA}/ctest/ROI_QB_TOULOUSE_ortho_200x200.tif
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF}
                             -out ${TEMP}/feTvLineSegmentDetection_LI.shp
                     # VALID   --compare-ogr ${EPSILON_9}
                     #         ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI.shp
                     #         ${TEMP}/feTvLineSegmentDetection_LI.shp)


#----------- EdgeExtraction TESTS ----------------
otb_test_application(NAME  apTvFEEdgeExtraction
                     APP  EdgeExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -filter sobel
                             -out ${TEMP}/apTvFEEdgeExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEEdgeExtraction.tif
                 		     ${TEMP}/apTvFEEdgeExtraction.tif)
