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

#include "otbTrainImagesClassifier.h"


namespace otb
{
namespace Wrapper
{
  void TrainImagesClassifier::InitNormalBayesParams()
  {
    AddChoice("classifier.bayes", "Normal Bayes classifier");
    SetParameterDescription("classifier.bayes", "Use a Normal Bayes Classifier."
        "See complete documentation here http://docs.opencv.org/modules/ml/doc/normal_bayes_classifier.html");

  }


  void TrainImagesClassifier::TrainNormalBayes(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
  {
    NormalBayesType::Pointer classifier = NormalBayesType::New();
    classifier->SetInputListSample(trainingListSample);
    classifier->SetTargetListSample(trainingLabeledListSample);
    classifier->Train();
    classifier->Save(GetParameterString("io.out"));
  }

} //end namespace wrapper
} //end namespace otb
