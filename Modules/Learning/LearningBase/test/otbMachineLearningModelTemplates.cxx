/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <otbMachineLearningModel.h>

typedef otb::MachineLearningModel<float,short>          MachineLearningModelType1;
typedef MachineLearningModelType1::InputValueType       InputValueType1;
typedef MachineLearningModelType1::InputSampleType      InputSampleType1;
typedef MachineLearningModelType1::InputListSampleType  InputListSampleType1;
typedef MachineLearningModelType1::TargetValueType      TargetValueType1;
typedef MachineLearningModelType1::TargetSampleType     TargetSampleType1;
typedef MachineLearningModelType1::TargetListSampleType TargetListSampleType1;

typedef otb::MachineLearningModel<float,itk::VariableLengthVector<double> > MachineLearningModelType2;
typedef MachineLearningModelType2::InputValueType       InputValueType2;
typedef MachineLearningModelType2::InputSampleType      InputSampleType2;
typedef MachineLearningModelType2::InputListSampleType  InputListSampleType2;
typedef MachineLearningModelType2::TargetValueType      TargetValueType2;
typedef MachineLearningModelType2::TargetSampleType     TargetSampleType2;
typedef MachineLearningModelType2::TargetListSampleType TargetListSampleType2;



