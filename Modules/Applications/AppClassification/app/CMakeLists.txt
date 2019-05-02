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

otb_create_application(
  NAME           ComputeOGRLayersFeaturesStatistics
  SOURCES        otbComputeOGRLayersFeaturesStatistics.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SOMClassification
  SOURCES        otbSOMClassification.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ImageClassifier
  SOURCES        otbImageClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           TrainVectorClassifier
  SOURCES        otbTrainVectorClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ComputeConfusionMatrix
  SOURCES        otbComputeConfusionMatrix.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           FusionOfClassifications
  SOURCES        otbFusionOfClassifications.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

if(OTB_USE_SHARK)
  otb_create_application(
    NAME           KMeansClassification
    SOURCES        otbKMeansClassification.cxx
    LINK_LIBRARIES ${${otb-module}_LIBRARIES})
endif()

otb_create_application(
  NAME           TrainImagesClassifier
  SOURCES        otbTrainImagesClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           TrainRegression
  SOURCES        otbTrainRegression.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           TrainVectorRegression
  SOURCES        otbTrainVectorRegression.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           PredictRegression
  SOURCES        otbPredictRegression.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ClassificationMapRegularization
  SOURCES        otbClassificationMapRegularization.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           OGRLayerClassifier
  SOURCES        otbOGRLayerClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ComputeImagesStatistics
  SOURCES        otbComputeImagesStatistics.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           PolygonClassStatistics
  SOURCES        otbPolygonClassStatistics.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SampleSelection
  SOURCES        otbSampleSelection.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SampleExtraction
  SOURCES        otbSampleExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           MultiImageSamplingRate
  SOURCES        otbMultiImageSamplingRate.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VectorClassifier
  SOURCES        otbVectorClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SampleAugmentation
  SOURCES        otbSampleAugmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ZonalStatistics
  SOURCES        otbZonalStatistics.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

# Mantis-1427 : temporary fix
add_dependencies(${otb-module}-all otbapp_ImageEnvelope)
