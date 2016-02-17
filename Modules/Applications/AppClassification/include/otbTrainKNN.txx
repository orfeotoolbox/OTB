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
#ifndef __otbTrainKNN_txx
#define __otbTrainKNN_txx
#include "otbLearningApplicationBase.h"

namespace otb
{
namespace Wrapper
{

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::InitKNNParams()
  {
    AddChoice("classifier.knn", "KNN classifier");
    SetParameterDescription("classifier.knn", "This group of parameters allows setting KNN classifier parameters. "
        "See complete documentation here \\url{http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html}.");

    //K parameter
    AddParameter(ParameterType_Int, "classifier.knn.k", "Number of Neighbors");
    SetParameterInt("classifier.knn.k", 32);
    SetParameterDescription("classifier.knn.k","The number of neighbors to use.");

    if (this->m_RegressionFlag)
      {
      // Decision rule : mean / median
      AddParameter(ParameterType_Choice, "classifier.knn.rule", "Decision rule");
      SetParameterDescription("classifier.knn.rule", "Decision rule for regression output");

      AddChoice("classifier.knn.rule.mean", "Mean of neighbors values");
      SetParameterDescription("classifier.knn.rule.mean","Returns the mean of neighbors values");

      AddChoice("classifier.knn.rule.median", "Median of neighbors values");
      SetParameterDescription("classifier.knn.rule.median","Returns the median of neighbors values");
      }
  }

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::TrainKNN(typename ListSampleType::Pointer trainingListSample,
             typename TargetListSampleType::Pointer trainingLabeledListSample,
             std::string modelPath)
  {
    typename KNNType::Pointer knnClassifier = KNNType::New();
    knnClassifier->SetRegressionMode(this->m_RegressionFlag);
    knnClassifier->SetInputListSample(trainingListSample);
    knnClassifier->SetTargetListSample(trainingLabeledListSample);
    knnClassifier->SetK(GetParameterInt("classifier.knn.k"));
    if (this->m_RegressionFlag)
      {
      std::string decision = this->GetParameterString("classifier.knn.rule");
      if (decision == "mean")
        {
        knnClassifier->SetDecisionRule(KNNType::KNN_MEAN);
        }
      else if (decision == "median")
        {
        knnClassifier->SetDecisionRule(KNNType::KNN_MEDIAN);
        }
      }

    knnClassifier->Train();
    knnClassifier->Save(modelPath);
  }

} //end namespace wrapper
} //end namespace otb

#endif
