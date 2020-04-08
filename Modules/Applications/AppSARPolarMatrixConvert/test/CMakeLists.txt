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
#----------- SARPolarMatrixConvert TESTS ----------------

#1
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCoherency
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocoherency 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCoherency.tif
							 
                     )
            
#2                            
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocovariance 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovariance.tif
							 
                     )

#3
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCirCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocircovariance 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCirCovariance.tif
							 
                     )

#4
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCohToMueller
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcoherencytomueller
							 -outf ${TEMP}/apTvSARPolarMatrixConvertRecCohToMueller.tif
							 
                     )
                             
                             
#5
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovToCohDeg
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherencydegree
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovToCohDeg.tif
							 
                     )
                             

#6 
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovToRecCoh
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherency
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovToRecCoh.tif
							 
                     )

#7 
otb_test_application(NAME  apTvSARPolarMatrixConvertRecLinCovToRecCirCov
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mlinearcovariancetocircularcovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecLinCovToRecCirCov.tif
							 
                     )
                             
                             
#8 
otb_test_application(NAME  apTvSARPolarMatrixConvertMuellerToRecCov
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertomcovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertMuellerToRecCov.tif
							 
                     )


#9
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCoherency
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocoherency
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCoherency.tif
							 
                     )
                             
                             
#10
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCovariance.tif
							 
                     )
                             
#11
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCirCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocircovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCirCovariance.tif
							 
                     )
   
                             
#12 
otb_test_application(NAME  apTvSARPolarMatrixConvertSincToMueller
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_HV.tif #monostatic
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv sinclairtomueller
							 -outf ${TEMP}/apTvSARPolarMatrixConvertSincToMueller.tif
							 
                     )
                             
                             
#13 
otb_test_application(NAME  apTvSARPolarMatrixConvertMuellerToPolDeGPow
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertopoldegandpower
							 -outf ${TEMP}/apTvSARPolarMatrixConvertMuellerToPolDeGPow.tif
							 
                     )
