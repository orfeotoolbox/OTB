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

# --------------------------------------------------------------------------------
# Manage PHR data input
if(OTB_DATA_USE_LARGEINPUT)
  set(GenericTestPHR_DATA_INPUTS
    ${GenericTestPHR_DATA_INPUTS}
    ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES-PRE/TLSE_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits/IMG_PHR1Z_PMS_N_001/IMG_PHR1A_PMS-N_201006181052297_ORT_IPU_20111011_0619-001_R1C1.JP2
    ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES-PRE/TLSE_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20111109_7807-004_R1C1.TIF
    ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF
    ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2
    ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R1C1.JP2
    ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES/PARIS_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits/IMGPHR_PHR1A_ORTHO_20120502_IPU_20120430_6347-003_V1/IMG_R1C1.JP2
    )

  set(GenericTestPHR_DATA_TYPES
    ${GenericTestPHR_DATA_TYPES}
    SIMUL_PHR_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits
    SIMUL_PHR_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits
    SIMUL_PHR_TIFF_PRIMARY_DIMAPv2_MS_lossy_12bits
    PHR_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits
    PHR_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits
    PHR_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits
    )

  set(GenericTestSPOT6_DATA_INPUTS
    ${GenericTestSPOT6_DATA_INPUTS}
    ${OTB_DATA_LARGEINPUT_ROOT}/SPOT6/SPOT6_Ortho_Bundle_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201306031242000_ORT_816005101_R1C1.JP2
    ${OTB_DATA_LARGEINPUT_ROOT}/SPOT6/SPOT6_Ortho_Pan-sharpened_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_PMS_001_A/IMG_SPOT6_PMS_201305251604372_ORT_816009101_R1C1.JP2
    ${OTB_DATA_LARGEINPUT_ROOT}/SPOT6/SPOT6_Primary_Bundle_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201211100138085_SEN_816007101_R1C1.JP2
    )

  set(GenericTestSPOT6_DATA_TYPES
    ${GenericTestSPOT6_DATA_TYPES}
    SPOT6_Ortho_Bundle_12_bits
    SPOT6_Ortho_Pan-sharpened_12_bits
    SPOT6_Primary_Bundle_12_bits
    )
endif()
