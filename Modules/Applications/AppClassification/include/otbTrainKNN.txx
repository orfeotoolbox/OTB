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
    SetParameterDescription("classifier.knn", "This group of parameters allows to set KNN classifier parameters. "
        "See complete documentation here \\url{http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html}.");

    //K parameter
    AddParameter(ParameterType_Int, "classifier.knn.k", "Number of Neighbors");
    SetParameterInt("classifier.knn.k", 32);
    SetParameterDescription("classifier.knn.k","The number of neighbors to use.");

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

    knnClassifier->Train();
    knnClassifier->Save(modelPath);
  }

} //end namespace wrapper
} //end namespace otb

#endif
