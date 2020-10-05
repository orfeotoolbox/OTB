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

#Contact: arnaud.durand@sertit.u-strasbg.fr

#SERTIT - University of Strasbourg http://sertit.u-strasbg.fr
#Known issue with MSVC - https://github.com/sertit/SertitObject/issues/2
otb_fetch_module(SertitObject
  "This module provides 2 applications dedicated to object-oriented image analysis.

   Aggregate application:

The aim of this application is to merge the result of a segmentation with a
pixel-based image classification in order to produce an object-oriented image
classification. The input segmentation is a labeled image, typically the result
provided by the OTB application LSMSSegmentation. The output is a vector dataset
containing objects and the corresponding class in the attribute table. Connected
regions belonging to the same class are merged. This application could be used
at the last step of the LSMS pipeline in replacement of the application
LSMSVectorization.

   ObjectsRadiometricStatistics application:

This application computes radiometric and shape attributes on a vector dataset,
using an image. The results are stored in the attribute table. Shape attributes
are : number of pixels, flatness, roundness, elongation, perimeter. Radiometric
attributes are for each band of the input image : mean, standard-deviation,
median, variance, kurtosis, skewness. The result could be use to perform further
object-oriented image analysis.
"
  GIT_REPOSITORY https://github.com/gpo-geo/SertitObject
  GIT_TAG b13961b775724c8de4fa45bc5c5bec14b6d2afef
)

# use a temporary fork because pull request #8 not merged yet
