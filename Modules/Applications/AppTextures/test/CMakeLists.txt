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
#----------- HaralickTextureExtraction TESTS ----------------
otb_test_application(NAME  apTvFEHaralickTextureExtraction
                     APP  HaralickTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -texture simple
                             -out ${TEMP}/apTvFEHaralickTextureExtraction.tif
                             -parameters.min 127
                             -parameters.max 1578
                     VALID   --compare-image ${EPSILON_15}
                   			 ${BASELINE}/apTvFEHaralickTextureExtraction.tif
                 		     ${TEMP}/apTvFEHaralickTextureExtraction.tif)


#----------- SFSTextureExtraction TESTS ----------------
otb_test_application(NAME  apTvFESFSTextureExtraction
                     APP  SFSTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -out ${TEMP}/apTvFESFSTextureExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFESFSTextureExtraction.tif
                 		     ${TEMP}/apTvFESFSTextureExtraction.tif)

