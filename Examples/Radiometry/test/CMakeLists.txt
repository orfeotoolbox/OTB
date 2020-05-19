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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

# -------            ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

otb_add_test(NAME raTeARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
    ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
  Execute $<TARGET_FILE:ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter>
    ${INPUTDATA}/poupees_sub.png
    ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
    ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex3.tif
    ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex2.tif
    1
    2
    3
)

# -------            AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest   ------------------------------

otb_add_test(NAME raTeAVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  #--compare-image ${NOTOL}
  #  ${BASELINE}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
  #  ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
  Execute $<TARGET_FILE:AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter>
    ${INPUTDATA}/qb_toulouse_sub.tif
    ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
    ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest2.png
    ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest3.png
    3
    2
    4 # indices of the channels
)

if(OTBOpticalCalibration_LOADED)
otb_add_test(NAME raTeAtmosphericCorrectionSequencementTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/Example_RomaniaAtmosphericCorrectionSequencement.tif
    ${TEMP}/Example_RomaniaAtmosphericCorrectionSequencement.tif
  Execute $<TARGET_FILE:AtmosphericCorrectionSequencement>
    ${INPUTDATA}/Romania_Extract.tif
    ${TEMP}/Example_RomaniaAtmosphericCorrectionSequencement.tif
    ${INPUTDATA}/atmosphericCorrectionSequencement_alpha_beta.txt
    ${INPUTDATA}/atmosphericCorrectionSequencement_solar_illumination.txt
    ${INPUTDATA}/atmosphericCorrectionSequencement_wavelength_spectral_bands_spot4_1.txt

    # ImageToRadiance parameters
    # RadianceToReflectance parameters
    27.3    #solar azimuthal elevation
    4       #day
    12      #month
    # AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms parameters
    152.7
    2.5
    -77.0
    1013.
    2.48134
    0.34400
    1
    0.199854
    # ReflectanceToSurfaceReflectance parameters
    # SurfaceAdjacencyEffect6SCorrectionSchemeFilter parameters
    2       # Radius;
    0.020   # pixel spacing in kilometers
)
endif()
