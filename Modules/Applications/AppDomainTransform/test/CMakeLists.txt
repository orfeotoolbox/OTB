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
otb_test_application(NAME apTvDomainTransform_wav_db20_fwd
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode wavelet
  -mode.wavelet.form db20
  -mode.wavelet.nlevels 2
  -direction forward
  -out ${TEMP}/apTvDomainTransform_wav_db20_fwd.tif
  )

otb_test_application(NAME apTvDomainTransform_wav_haar_inv
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode wavelet
  -mode.wavelet.form haar
  -direction inverse
  -out ${TEMP}/apTvDomainTransform_wav_haar_inv.tif
  )

otb_test_application(NAME apTvDomainTransform_fft_shift_fwd
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -mode.fft.shift true
  -direction forward
  -out ${TEMP}/apTvDomainTransform_fft_shift_fwd.tif
  )


otb_test_application(NAME apTvDomainTransform_fft_inv
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -direction inverse
  -out ${TEMP}/apTvDomainTransform_fft_shift_inv.tif
  )

