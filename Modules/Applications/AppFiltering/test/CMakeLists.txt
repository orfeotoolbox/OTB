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
#----------- Smoothing TESTS ----------------

otb_test_application(NAME  apTvUtSmoothingTest_OutXML
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest_OutXML.tif
                             -type mean
                             -outxml ${TEMP}/apTvUtSmoothingTest_OutXML.xml
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest_OutXML.tif)

otb_test_application(NAME  apTvUtSmoothingTest_InXML
                     APP  Smoothing
                     OPTIONS -inxml ${TEMP}/apTvUtSmoothingTest_OutXML.xml
               	             -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest_InXML.tif
                             -type mean
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest_InXML.tif)

set_tests_properties( apTvUtSmoothingTest_InXML
    PROPERTIES DEPENDS apTvUtSmoothingTest_OutXML)

otb_test_application(NAME  apTvUtSmoothingTestGaussian
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTestGaussian.tif
                             -type gaussian
                             -type.gaussian.stdev 2.0
                             -type.gaussian.maxerror 0.1
                             -type.gaussian.maxwidth 64
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTestGaussian.tif
                             ${TEMP}/apTvUtSmoothingTestGaussian.tif)

otb_test_application(NAME  apTvUtSmoothingTestAnisotropic
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTestAnisotropic.tif
                             -type anidif
                             -type.anidif.timestep 0.125
                             -type.anidif.nbiter 10
                             -type.anidif.conductance 1.
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/apTvUtSmoothingTestAnisotropic.tif
                             ${TEMP}/apTvUtSmoothingTestAnisotropic.tif)

#----------- Contrast TESTS ----------------

otb_test_application(NAME  apTvUtContrastTest_base
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/QB_Suburb.png
                             -out ${TEMP}/apTvUtContrastTest_base.tif int16
                             -bins 256
                             -spatial.local.h 51
                             -spatial.local.w 67
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_base.tif
                             ${TEMP}/apTvUtContrastTest_base.tif)

otb_test_application(NAME  apTvUtContrastTest_base_glob
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/QB_Suburb.png
                             -out ${TEMP}/apTvUtContrastTest_base_glob.tif int16
                             -bins 256
                             -spatial global
                             -minmax manual
                             -minmax.manual.min 0 
                             -minmax.manual.max 255
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_base_glob.tif
                             ${TEMP}/apTvUtContrastTest_base_glob.tif)

otb_test_application(NAME  apTvUtContrastTest_lum_glob
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/anaglyphInput1.tif
                             -out ${TEMP}/apTvUtContrastTest_lum_glob.tif int16
                             -bins 256
                             -spatial global
                             -hfact 2.7
                             -nodata 0
                             -mode lum
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_lum_glob.tif
                             ${TEMP}/apTvUtContrastTest_lum_glob.tif)

otb_test_application(NAME  apTvUtContrastTest_lum
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/anaglyphInput1.tif
                             -out ${TEMP}/apTvUtContrastTest_lum.tif int16
                             -bins 256
                             -spatial.local.h 33
                             -spatial.local.w 47
                             -hfact 2.1
                             -nodata 0
                             -mode lum
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_lum.tif
                             ${TEMP}/apTvUtContrastTest_lum.tif)
                             
#----------- NL Means TESTS ----------------

otb_test_application(NAME  nlMeansTest_base
                     APP  FastNLMeans
                     OPTIONS -in ${INPUTDATA}/GomaAvant.tif
                             -out ${TEMP}/GomaAvant_NLMeans.tif
                             -patchradius 2
                             -searchradius 11
                             -thresh 30
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/GomaAvant_NLMeans.tif
                             ${TEMP}/GomaAvant_NLMeans.tif)

