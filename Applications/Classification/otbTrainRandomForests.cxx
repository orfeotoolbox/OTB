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

#include "otbTrainMachineLearningImagesClassifier.h"

namespace otb
{
namespace Wrapper
{
void TrainMachineLearningImagesClassifier::InitRandomForestsParams()
{
  AddChoice("classifier.rf", "Random forests classifier");
  SetParameterDescription("classifier.rf",
                          "This group of parameters allows to set Random Forests classifier parameters."
                          "See complete documentation here http://docs.opencv.org/modules/ml/doc/random_trees.html");
  //MaxDepth
  AddParameter(ParameterType_Int, "classifier.rf.max", "Maximum depth of the tree");
  SetParameterInt("classifier.rf.max", 25);
  SetParameterDescription(
      "classifier.rf.max",
      "The depth of the tree. A low value will likely underfit and conversely a high value will likely overfit. "
      "The optimal value can be obtained using cross validation or other suitable methods.");

  //MinSampleCount
  AddParameter(ParameterType_Int, "classifier.rf.min", "Minimum number of samples in each node");
  SetParameterInt("classifier.rf.min", 5);
  SetParameterDescription(
      "classifier.rf.min", "If the number of samples in a node is less than this parameter "
      "then the node will not be split. A reasonable value is a small percentage of the total data e.g. 1%");

  //RegressionAccuracy
  AddParameter(ParameterType_Float, "classifier.rf.ra", "Termination Criteria for regression tree.");
  SetParameterFloat("classifier.rf.ra", 0.);
  SetParameterDescription("classifier.rf.min", "If all absolute differences between an estimated value in a node"
                          "and values of train samples in this node are less than this parameter"
                          "then the node will not be split.");

  //UseSurrogates : don't need to be exposed !
  //AddParameter(ParameterType_Empty, "classifier.rf.sur", "Surrogate splits will be built");
  //SetParameterDescription("classifier.rf.sur","These splits allow to work with missing data and compute variable importance correctly.");

  //MaxNumberOfCategories
  AddParameter(ParameterType_Int, "classifier.rf.cat",
               "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split");
  SetParameterInt("classifier.rf.cat", 15);
  SetParameterDescription(
      "classifier.rf.cat",
      "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split");

  //Priors are not exposed.

  //CalculateVariableImportance not exposed

  //MaxNumberOfVariables
  AddParameter(ParameterType_Int, "classifier.rf.var",
               "Size of the randomly selected subset of features at each tree node");
  SetParameterInt("classifier.rf.var", 4);
  SetParameterDescription(
      "classifier.rf.var",
      "The size of the randomly selected subset of features at each tree node and that are used to find the best split(s)."
      "If you set it to 0 then the size will be set to the square root of the total number of features.");

  //MaxNumberOfTrees
  AddParameter(ParameterType_Int, "classifier.rf.nbtrees",
               "Maximum number of trees in the forest");
  SetParameterInt("classifier.rf.nbtrees", 100);
  SetParameterDescription(
      "classifier.rf.nbtrees",
      "The maximum number of trees in the forest (surprise, surprise). Typically the more trees you have the better the accuracy."
      "However, the improvement in accuracy generally diminishes and asymptotes pass a certain number of trees. "
      "Also to keep in mind, the number of tree increases the prediction time linearly.");

  //ForestAccuracy
  AddParameter(ParameterType_Float, "classifier.rf.acc",
               "Sufficient accuracy (OOB error)");
  SetParameterFloat("classifier.rf.acc", 0.01);
  SetParameterDescription("classifier.rf.acc","Sufficient accuracy (OOB error)");


  //TerminationCriteria not exposed
}

void TrainMachineLearningImagesClassifier::TrainRandomForests(ListSampleType::Pointer trainingListSample,
                                                              LabelListSampleType::Pointer trainingLabeledListSample)
{
  RandomForestType::Pointer classifier = RandomForestType::New();
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->SetMaxDepth(GetParameterInt("classifier.rf.max"));
  classifier->SetMinSampleCount(GetParameterInt("classifier.rf.min"));
  classifier->SetRegressionAccuracy(GetParameterFloat("classifier.rf.ra"));
  classifier->SetMaxNumberOfCategories(GetParameterInt("classifier.rf.cat"));
  classifier->SetMaxNumberOfVariables(GetParameterInt("classifier.rf.var"));
  classifier->SetMaxNumberOfTrees(GetParameterInt("classifier.rf.nbtrees"));
  classifier->SetForestAccuracy(GetParameterFloat("classifier.rf.acc"));

  classifier->Train();
  classifier->Save(GetParameterString("io.out"));
}

} //end namespace wrapper
} //end namespace otb
