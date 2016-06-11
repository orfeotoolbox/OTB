/*=========================================================================
 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#ifndef __otbTrainSharkRandomForests_txx
#define __otbTrainSharkRandomForests_txx
#include "otbLearningApplicationBase.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::InitSharkRandomForestsParams()
{


  AddChoice("classifier.sharkrf", "Shark Random forests classifier");
  SetParameterDescription("classifier.sharkrf",
                          "This group of parameters allows setting Shark Random Forests classifier parameters. "
                          "See complete documentation here \\url{http://image.diku.dk/shark/doxygen_pages/html/classshark_1_1_r_f_trainer.html}.");
  //MaxNumberOfTrees
  AddParameter(ParameterType_Int, "classifier.sharkrf.nbtrees",
               "Maximum number of trees in the forest");
  SetParameterInt("classifier.sharkrf.nbtrees", 100);
  SetParameterDescription(
    "classifier.sharkrf.nbtrees",
    "The maximum number of trees in the forest. Typically, the more trees you have, the better the accuracy. "
    "However, the improvement in accuracy generally diminishes and reaches an asymptote for a certain number of trees. "
    "Also to keep in mind, increasing the number of trees increases the prediction time linearly.");
  
  
  //NodeSize
  AddParameter(ParameterType_Int, "classifier.sharkrf.nodesize", "Min size of the node for a split");
  SetParameterInt("classifier.sharkrf.nodesize", 25);
  SetParameterDescription(
    "classifier.sharkrf.nodesize",
    "If the number of samples in a node is smaller than this parameter, "
    "then the node will not be split. A reasonable value is a small percentage of the total data e.g. 1 percent.");

  //MTry
  AddParameter(ParameterType_Int, "classifier.sharkrf.mtry", "Number of features tested at each node");
  SetParameterInt("classifier.sharkrf.mtry", 0);
  SetParameterDescription(
    "classifier.sharkrf.mtry",
    "The number of features (variables) which will be tested at each node in "
    "order to compute the split. If set to zero, the square root of the number of "
    "features is used.");


  //OOB Ratio
  AddParameter(ParameterType_Float, "classifier.sharkrf.oobr", "Out of bound ratio");
  SetParameterFloat("classifier.sharkrf.oobr", 0.66);
  SetParameterDescription("classifier.sharkrf.oobr", 
                          "Set the fraction of the original training dataset to use as the out of bag sample." 
                          "A good default value is 0.66. ");
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::TrainSharkRandomForests(typename ListSampleType::Pointer trainingListSample,
                          typename TargetListSampleType::Pointer trainingLabeledListSample,
                          std::string modelPath)
{
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

} //end namespace wrapper
} //end namespace otb

#endif
