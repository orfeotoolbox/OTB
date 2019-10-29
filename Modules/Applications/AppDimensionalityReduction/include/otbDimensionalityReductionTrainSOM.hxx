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
#ifndef otbDimensionalityReductionTrainSOM_hxx
#define otbDimensionalityReductionTrainSOM_hxx
#include "otbTrainDimensionalityReductionApplicationBase.h"
#include "otbSOMModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::InitSOMParams()
{
  AddChoice("algorithm.som", "OTB SOM");
  SetParameterDescription("algorithm.som", "This group of parameters allows setting SOM parameters. ");

  AddParameter(ParameterType_StringList, "algorithm.som.s", "Map size");
  SetParameterDescription("algorithm.som.s",
                          "Sizes of the SOM map (one per "
                          "dimension). For instance, [12;15] means a 2D map of size 12x15. Support"
                          "2D to 5D maps.");
  MandatoryOff("algorithm.som.s");

  AddParameter(ParameterType_StringList, "algorithm.som.n", "Neighborhood sizes");
  SetParameterDescription("algorithm.som.n",
                          "Sizes of the initial neighborhood "
                          "in the SOM map (one per dimension). The number of sizes should be the same"
                          " as the map sizes");
  MandatoryOff("algorithm.som.n");

  AddParameter(ParameterType_Int, "algorithm.som.ni", "NumberIteration");
  SetParameterDescription("algorithm.som.ni", "Number of iterations for SOM learning");
  MandatoryOff("algorithm.som.ni");

  AddParameter(ParameterType_Float, "algorithm.som.bi", "BetaInit");
  SetParameterDescription("algorithm.som.bi", "Initial learning coefficient");
  MandatoryOff("algorithm.som.bi");

  AddParameter(ParameterType_Float, "algorithm.som.bf", "BetaFinal");
  SetParameterDescription("algorithm.som.bf", "Final learning coefficient");
  MandatoryOff("algorithm.som.bf");

  AddParameter(ParameterType_Float, "algorithm.som.iv", "InitialValue");
  SetParameterDescription("algorithm.som.iv", "Maximum initial neuron weight");
  MandatoryOff("algorithm.som.iv");

  std::vector<std::string> size(2, std::string("10"));
  std::vector<std::string> radius(2, std::string("3"));
  SetParameterStringList("algorithm.som.s", size, false);
  SetParameterStringList("algorithm.som.n", radius, false);
  DisableParameter("algorithm.som.s");
  DisableParameter("algorithm.som.n");

  SetDefaultParameterInt("algorithm.som.ni", 5);
  SetDefaultParameterFloat("algorithm.som.bi", 1.0);
  SetDefaultParameterFloat("algorithm.som.bf", 0.1);
  SetDefaultParameterFloat("algorithm.som.iv", 10.0);
}

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::BeforeTrainSOM(typename ListSampleType::Pointer trainingListSample,
                                                                                            std::string modelPath)
{
  std::vector<std::string> s      = GetParameterStringList("algorithm.som.s");
  int                      SomDim = s.size();

  if (SomDim == 2)
  {
    typedef otb::SOMModel<InputValueType, 2> SOM2DModelType;
    TrainSOM<SOM2DModelType>(trainingListSample, modelPath);
  }

  if (SomDim == 3)
  {
    typedef otb::SOMModel<InputValueType, 3> SOM3DModelType;
    TrainSOM<SOM3DModelType>(trainingListSample, modelPath);
  }

  if (SomDim == 4)
  {
    typedef otb::SOMModel<InputValueType, 4> SOM4DModelType;
    TrainSOM<SOM4DModelType>(trainingListSample, modelPath);
  }

  if (SomDim == 5)
  {
    typedef otb::SOMModel<InputValueType, 5> SOM5DModelType;
    TrainSOM<SOM5DModelType>(trainingListSample, modelPath);
  }
  if (SomDim > 5 || SomDim < 2)
  {
    otbAppLogFATAL(<< "Invalid number of dimensions : " << SomDim << ". Only support 2, 3, 4 or 5 dimensions");
  }
}

template <class TInputValue, class TOutputValue>
template <typename TSOM>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::TrainSOM(typename ListSampleType::Pointer trainingListSample,
                                                                                      std::string modelPath)
{
  typename TSOM::Pointer dimredTrainer = TSOM::New();
  dimredTrainer->SetNumberOfIterations(GetParameterInt("algorithm.som.ni"));
  dimredTrainer->SetBetaInit(GetParameterFloat("algorithm.som.bi"));
  dimredTrainer->SetWriteMap(true);
  dimredTrainer->SetBetaEnd(GetParameterFloat("algorithm.som.bf"));
  dimredTrainer->SetMaxWeight(GetParameterFloat("algorithm.som.iv"));
  typename TSOM::SizeType  size;
  std::vector<std::string> s = GetParameterStringList("algorithm.som.s");
  for (unsigned int i = 0; i < s.size(); i++)
  {
    size[i] = boost::lexical_cast<unsigned int>(s[i]);
  }

  dimredTrainer->SetMapSize(size);
  typename TSOM::SizeType  radius;
  std::vector<std::string> n = GetParameterStringList("algorithm.som.n");
  if (n.size() != s.size())
  {
    otbAppLogFATAL(<< "Wrong number of neighborhood radii : expected " << s.size() << " ; got " << n.size());
  }
  for (unsigned int i = 0; i < n.size(); i++)
  {
    radius[i] = boost::lexical_cast<unsigned int>(n[i]);
  }
  dimredTrainer->SetNeighborhoodSizeInit(radius);
  dimredTrainer->SetInputListSample(trainingListSample);
  dimredTrainer->Train();
  dimredTrainer->Save(modelPath);
}

} // end namespace wrapper
} // end namespace otb

#endif
