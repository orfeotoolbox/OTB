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
#----------- RadiometricIndices TESTS ----------------
otb_test_application(NAME  apTvFERadiometricIndices_veg
                     APP  RadiometricIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                     		 -channels.red 3
                     		 -channels.green 2
                     		 -channels.nir 4
                             -list Vegetation:NDVI Vegetation:RVI Vegetation:IPVI
                             -out ${TEMP}/apTvFERadiometricIndices_veg.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
                 		     ${TEMP}/apTvFERadiometricIndices_veg.tif)

otb_test_application(NAME  apTvFERadiometricIndices_wat
                     APP  RadiometricIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                             -list Water:NDWI2
                             -channels.red 3
                     		 -channels.green 2
                     		 -channels.nir 4
                             -out ${TEMP}/apTvFERadiometricIndices_wat.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricWaterIndices_.tif
                 		     ${TEMP}/apTvFERadiometricIndices_wat.tif)

