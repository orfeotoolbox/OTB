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

#ifndef otbTrainSharkRandomForests_hxx
#define otbTrainSharkRandomForests_hxx

#include "otbLearningApplicationBase.h"
#include "otbSharkRandomForestsMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::InitSharkRandomForestsParams()
{


  AddChoice("classifier.sharkrf", "Shark Random forests classifier");
  SetParameterDescription("classifier.sharkrf",
                          "http://image.diku.dk/shark/doxygen_pages/html/classshark_1_1_r_f_trainer.html.\n It is noteworthy that training is parallel.");
  // MaxNumberOfTrees
  AddParameter(ParameterType_Int, "classifier.sharkrf.nbtrees", "Maximum number of trees in the forest");
  SetParameterInt("classifier.sharkrf.nbtrees", 100);
  SetParameterDescription("classifier.sharkrf.nbtrees",
                          "The maximum number of trees in the forest. Typically, the more trees you have, the better the accuracy. "
                          "However, the improvement in accuracy generally diminishes and reaches an asymptote for a certain number of trees. "
                          "Also to keep in mind, increasing the number of trees increases the prediction time linearly.");


  // NodeSize
  AddParameter(ParameterType_Int, "classifier.sharkrf.nodesize", "Min size of the node for a split");
  SetParameterInt("classifier.sharkrf.nodesize", 25);
  SetParameterDescription("classifier.sharkrf.nodesize",
                          "If the number of samples in a node is smaller than this parameter, "
                          "then the node will not be split. A reasonable value is a small percentage of the total data e.g. 1 percent.");

  // MTry
  AddParameter(ParameterType_Int, "classifier.sharkrf.mtry", "Number of features tested at each node");
  SetParameterInt("classifier.sharkrf.mtry", 0);
  SetParameterDescription("classifier.sharkrf.mtry",
                          "The number of features (variables) which will be tested at each node in "
                          "order to compute the split. If set to zero, the square root of the number of "
                          "features is used.");


  // OOB Ratio
  AddParameter(ParameterType_Float, "classifier.sharkrf.oobr", "Out of bound ratio");
  SetParameterFloat("classifier.sharkrf.oobr", 0.66);
  SetParameterDescription("classifier.sharkrf.oobr",
                          "Set the fraction of the original training dataset to use as the out of bag sample."
                          "A good default value is 0.66. ");
}

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::TrainSharkRandomForests(typename ListSampleType::Pointer trainingListSample,
                                                                                 typename TargetListSampleType::Pointer trainingLabeledListSample,
                                                                                 std::string                            modelPath)
{
  typedef otb::SharkRandomForestsMachineLearningModel<InputValueType, OutputValueType> SharkRandomForestType;
  typename SharkRandomForestType::Pointer classifier = SharkRandomForestType::New();
  classifier->SetRegressionMode(this->m_RegressionFlag);
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->SetNodeSize(GetParameterInt("classifier.sharkrf.nodesize"));
  classifier->SetOobRatio(GetParameterFloat("classifier.sharkrf.oobr"));
  classifier->SetNumberOfTrees(GetParameterInt("classifier.sharkrf.nbtrees"));
  classifier->SetMTry(GetParameterInt("classifier.sharkrf.mtry"));

  classifier->Train();
  classifier->Save(modelPath);
}

} // end namespace wrapper
} // end namespace otb

#endif
