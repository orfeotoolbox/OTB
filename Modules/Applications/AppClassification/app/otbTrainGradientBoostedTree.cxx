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
void TrainImagesClassifier::InitGradientBoostedTreeParams()
{
  AddChoice("classifier.gbt", "Gradient Boosted Tree classifier");
  SetParameterDescription(
      "classifier.gbt",
      "This group of parameters allows to set Gradient Boosted Tree classifier parameters. "
      "See complete documentation here \\url{http://docs.opencv.org/modules/ml/doc/gradient_boosted_trees.html}.");
  //LossFunctionType : not exposed, as only one type is used for Classification,
  // the other three are used for regression.

  //WeakCount
  AddParameter(ParameterType_Int, "classifier.gbt.w", "Number of boosting algorithm iterations");
  SetParameterInt("classifier.gbt.w", 200);
  SetParameterDescription(
      "classifier.gbt.w",
      "Number \"w\" of boosting algorithm iterations, with w*K being the total number of trees in "
      "the GBT model, where K is the output number of classes.");

  //Shrinkage
  AddParameter(ParameterType_Float, "classifier.gbt.s", "Regularization parameter");
  SetParameterFloat("classifier.gbt.s", 0.01);
  SetParameterDescription("classifier.gbt.s", "Regularization parameter.");

  //SubSamplePortion
  AddParameter(ParameterType_Float, "classifier.gbt.p",
               "Portion of the whole training set used for each algorithm iteration");
  SetParameterFloat("classifier.gbt.p", 0.8);
  SetParameterDescription(
      "classifier.gbt.p",
      "Portion of the whole training set used for each algorithm iteration. The subset is generated randomly.");

  //MaxDepth
  AddParameter(ParameterType_Int, "classifier.gbt.max", "Maximum depth of the tree");
  SetParameterInt("classifier.gbt.max", 3);
  SetParameterDescription(
        "classifier.gbt.max", "The training algorithm attempts to split each node while its depth is smaller than the maximum "
        "possible depth of the tree. The actual depth may be smaller if the other termination criteria are met, and/or "
        "if the tree is pruned.");

  //UseSurrogates : don't need to be exposed !
  //AddParameter(ParameterType_Empty, "classifier.gbt.sur", "Surrogate splits will be built");
  //SetParameterDescription("classifier.gbt.sur","These splits allow to work with missing data and compute variable importance correctly.");

}

void TrainImagesClassifier::TrainGradientBoostedTree(
    ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
{
  GradientBoostedTreeType::Pointer classifier = GradientBoostedTreeType::New();
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->SetWeakCount(GetParameterInt("classifier.gbt.w"));
  classifier->SetShrinkage(GetParameterFloat("classifier.gbt.s"));
  classifier->SetSubSamplePortion(GetParameterFloat("classifier.gbt.p"));
  classifier->SetMaxDepth(GetParameterInt("classifier.gbt.max"));

  classifier->Train();
  classifier->Save(GetParameterString("io.out"));
}

} //end namespace wrapper
} //end namespace otb
