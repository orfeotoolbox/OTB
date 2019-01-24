/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbDimensionalityReductionTrainPCA_hxx
#define otbDimensionalityReductionTrainPCA_hxx

#include "otbTrainDimensionalityReductionApplicationBase.h"
#include "otbPCAModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::InitPCAParams()
{
  AddChoice("algorithm.pca", "Shark PCA");
  SetParameterDescription("algorithm.pca",
                          "This group of parameters allows setting Shark PCA parameters. "
                          );

   //Output Dimension
  AddParameter(ParameterType_Int, "algorithm.pca.dim",
               "Dimension of the output of the pca transformation");
  SetParameterInt("algorithm.pca.dim",10, false);
  SetParameterDescription(
    "algorithm.pca.dim",
    "Dimension of the output of the pca transformation.");
}

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::TrainPCA(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
    typedef otb::PCAModel<InputValueType> PCAModelType;
    typename PCAModelType::Pointer dimredTrainer = PCAModelType::New();
    dimredTrainer->SetDimension(GetParameterInt("algorithm.pca.dim"));
    dimredTrainer->SetInputListSample(trainingListSample);
    dimredTrainer->SetWriteEigenvectors(true);
    dimredTrainer->Train();
    dimredTrainer->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
