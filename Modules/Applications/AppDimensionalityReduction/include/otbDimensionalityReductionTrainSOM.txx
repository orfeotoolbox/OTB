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
#ifndef otbDimensionalityReductionTrainSOM_txx
#define otbDimensionalityReductionTrainSOM_txx
#include "otbTrainDimensionalityReductionApplicationBase.h"
#include "otbSOMModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::InitSOMParams()
{
  AddChoice("algorithm.som", "OTB SOM");
  SetParameterDescription("algorithm.som",
                          "This group of parameters allows setting SOM parameters. "
                          );

  AddParameter(ParameterType_Int, "algorithm.som.dim","Dimension of the map");
  SetParameterDescription("algorithm.som.dim","Dimension of the SOM map.");
  
  AddParameter(ParameterType_StringList ,  "algorithm.som.s",   "Size");
    SetParameterDescription("algorithm.som.s", "Size of the SOM map");
    MandatoryOff("algorithm.som.s");
    
  AddParameter(ParameterType_StringList ,  "algorithm.som.n",   "Size Neighborhood");
    SetParameterDescription("algorithm.som.n", "Size of the initial neighborhood in the SOM map");
    MandatoryOff("algorithm.som.n");
    
    AddParameter(ParameterType_Int,  "algorithm.som.sx",   "SizeX");
    SetParameterDescription("algorithm.som.sx", "X size of the SOM map");
    MandatoryOff("algorithm.som.sx");

    AddParameter(ParameterType_Int,  "algorithm.som.sy",   "SizeY");
    SetParameterDescription("algorithm.som.sy", "Y size of the SOM map");
    MandatoryOff("algorithm.som.sy");

    AddParameter(ParameterType_Int,  "algorithm.som.nx",   "NeighborhoodX");
    SetParameterDescription("algorithm.som.nx", "X size of the initial neighborhood in the SOM map");
    MandatoryOff("algorithm.som.nx");

    AddParameter(ParameterType_Int,  "algorithm.som.ny",   "NeighborhoodY");
    SetParameterDescription("algorithm.som.ny", "Y size of the initial neighborhood in the SOM map");
    MandatoryOff("algorithm.som.nx");

    AddParameter(ParameterType_Int,  "algorithm.som.ni",   "NumberIteration");
    SetParameterDescription("algorithm.som.ni", "Number of iterations for SOM learning");
    MandatoryOff("algorithm.som.ni");

    AddParameter(ParameterType_Float,  "algorithm.som.bi",   "BetaInit");
    SetParameterDescription("algorithm.som.bi", "Initial learning coefficient");
    MandatoryOff("algorithm.som.bi");

    AddParameter(ParameterType_Float,  "algorithm.som.bf",   "BetaFinal");
    SetParameterDescription("algorithm.som.bf", "Final learning coefficient");
    MandatoryOff("algorithm.som.bf");

    AddParameter(ParameterType_Float,  "algorithm.som.iv",   "InitialValue");
    SetParameterDescription("algorithm.som.iv", "Maximum initial neuron weight");
    MandatoryOff("algorithm.som.iv");
    
    SetDefaultParameterInt("algorithm.som.sx", 32);
    SetDefaultParameterInt("algorithm.som.sy", 32);
    SetDefaultParameterInt("algorithm.som.nx", 10);
    SetDefaultParameterInt("algorithm.som.ny", 10);
    SetDefaultParameterInt("algorithm.som.ni", 5);
    SetDefaultParameterFloat("algorithm.som.bi", 1.0);
    SetDefaultParameterFloat("algorithm.som.bf", 0.1);
    SetDefaultParameterFloat("algorithm.som.iv", 10.0);

 
}

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::BeforeTrainSOM(typename ListSampleType::Pointer trainingListSample,
        std::string modelPath)
{
  int SomDim = GetParameterInt("algorithm.som.dim");
  std::cout << SomDim << std::endl;

  if(SomDim == 2)
    {
    typedef otb::SOMModel<InputValueType, 2> SOM2DModelType;
    TrainSOM<SOM2DModelType >(trainingListSample,modelPath);
    }

  if(SomDim == 3)
    {
    typedef otb::SOMModel<InputValueType, 3> SOM3DModelType;
    TrainSOM<SOM3DModelType >(trainingListSample,modelPath);
    }

  if(SomDim == 4)
    {
    typedef otb::SOMModel<InputValueType, 4> SOM4DModelType;
    TrainSOM<SOM4DModelType >(trainingListSample,modelPath);
    }

  if(SomDim == 5)
    {
    typedef otb::SOMModel<InputValueType, 5> SOM5DModelType;
    TrainSOM<SOM5DModelType >(trainingListSample,modelPath);
    }   
  if(SomDim > 5 || SomDim < 2)
    {
    std::cerr << "k : invalid dimension" << std::endl;
    }
}

template <class TInputValue, class TOutputValue>
template <typename TSOM>
void TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::TrainSOM(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
  typename TSOM::Pointer dimredTrainer = TSOM::New();
  unsigned int dim = dimredTrainer->GetDimension();
  std::cout << dim << std::endl;
  dimredTrainer->SetNumberOfIterations(GetParameterInt("algorithm.som.ni"));
  dimredTrainer->SetBetaInit(GetParameterFloat("algorithm.som.bi"));
  dimredTrainer->SetWriteMap(true);
  dimredTrainer->SetBetaEnd(GetParameterFloat("algorithm.som.bf"));
  dimredTrainer->SetMaxWeight(GetParameterFloat("algorithm.som.iv"));
  typename TSOM::SizeType size;
  std::vector<std::basic_string<char>> s= GetParameterStringList("algorithm.som.s");
  for (unsigned int i=0; i<dim; i++)
    {
    size[i]=std::stoi(s[i]);
    }

  dimredTrainer->SetMapSize(size);
  typename TSOM::SizeType radius;
  std::vector<std::basic_string<char>> n= GetParameterStringList("algorithm.som.n");
  for (unsigned int i=0; i<dim; i++)
    {
    radius[i]=std::stoi(n[i]);
    }
  dimredTrainer->SetNeighborhoodSizeInit(radius);
  dimredTrainer->SetInputListSample(trainingListSample);
  dimredTrainer->Train();
  dimredTrainer->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
