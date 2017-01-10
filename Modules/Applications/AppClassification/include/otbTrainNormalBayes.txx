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
#ifndef otbTrainNormalBayes_txx
#define otbTrainNormalBayes_txx
#include "otbLearningApplicationBase.h"

namespace otb
{
namespace Wrapper
{

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::InitNormalBayesParams()
  {
    AddChoice("classifier.bayes", "Normal Bayes classifier");
    SetParameterDescription("classifier.bayes", "Use a Normal Bayes Classifier. "
        "See complete documentation here \\url{http://docs.opencv.org/modules/ml/doc/normal_bayes_classifier.html}.");

  }

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::TrainNormalBayes(typename ListSampleType::Pointer trainingListSample,
                     typename TargetListSampleType::Pointer trainingLabeledListSample,
                     std::string modelPath)
  {
    typename NormalBayesType::Pointer classifier = NormalBayesType::New();
    classifier->SetRegressionMode(this->m_RegressionFlag);
    classifier->SetInputListSample(trainingListSample);
    classifier->SetTargetListSample(trainingLabeledListSample);
    classifier->Train();
    classifier->Save(modelPath);
  }

} //end namespace wrapper
} //end namespace otb

#endif
