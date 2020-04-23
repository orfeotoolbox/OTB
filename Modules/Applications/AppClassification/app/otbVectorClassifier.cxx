/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbVectorPrediction.h"

namespace otb
{
namespace Wrapper
{

using VectorClassifier = VectorPrediction<false>;

template <>
void VectorClassifier::DoInitSpecialization()
{
  SetName("VectorClassifier");
  SetDescription("Performs a classification of the input vector data according to a model file.");

  SetDocAuthors("OTB-Team");
  SetDocLongDescription(
      "This application performs a vector data classification "
      "based on a model file produced by the TrainVectorClassifier application."
      "Features of the vector data output will contain the class labels decided by the classifier "
      "(maximal class label = 65535). \n"
      "There are two modes: \n"
      "1) Update mode: add of the 'cfield' field containing the predicted class in the input file. \n"
      "2) Write mode: copies the existing fields of the input file to the output file "
      " and add the 'cfield' field containing the predicted class. \n"
      "If you have declared the output file, the write mode applies. "
      "Otherwise, the input file update mode will be applied.");

  SetDocLimitations("Shapefiles are supported, but the SQLite format is only supported in update mode.");
  SetDocSeeAlso("TrainVectorClassifier");
  AddDocTag(Tags::Learning);

  AddParameter(ParameterType_InputFilename, "in", "Name of the input vector data");
  SetParameterDescription("in", "The input vector data file to classify.");

  AddParameter(ParameterType_InputFilename, "instat", "Statistics file");
  SetParameterDescription("instat",
                          "A XML file containing mean and standard deviation to center"
                          "and reduce samples before classification, produced by ComputeImagesStatistics application.");
  MandatoryOff("instat");

  AddParameter(ParameterType_InputFilename, "model", "Model file");
  SetParameterDescription("model", "Model file produced by TrainVectorClassifier application.");

  AddParameter(ParameterType_String, "cfield", "Output field");
  SetParameterDescription("cfield",
                          "Field containing the predicted class."
                          "Only geometries with this field available will be taken into account.\n"
                          "The field is added either in the input file (if 'out' off) or in the output file.\n"
                          "Caution, the 'cfield' must not exist in the input file if you are updating the file.");
  SetParameterString("cfield", "predicted");

  AddParameter(ParameterType_ListView, "feat", "Field names to be calculated");
  SetParameterDescription("feat",
                          "List of field names in the input vector data used as features for training. "
                          "Put the same field names as the TrainVectorClassifier application.");

  AddParameter(ParameterType_Bool, "confmap", "Confidence map");
  SetParameterDescription("confmap",
                          "Confidence map of the produced classification. The confidence index depends on the model: \n\n"
                          "* LibSVM: difference between the two highest probabilities (needs a model with probability estimates, so that classes probabilities "
                          "can be computed for each sample)\n"
                          "* Boost: sum of votes\n"
                          "* DecisionTree: (not supported)\n"
                          "* KNearestNeighbors: number of neighbors with the same label\n"
                          "* NeuralNetwork: difference between the two highest responses\n"
                          "* NormalBayes: (not supported)\n"
                          "* RandomForest: Confidence (proportion of votes for the majority class). Margin (normalized difference of the votes of the 2 "
                          "majority classes) is not available for now.\n"
                          "* SVM: distance to margin (only works for 2-class models)\n");

  AddParameter(ParameterType_OutputFilename, "out", "Output vector data file");
  MandatoryOff("out");
  SetParameterDescription("out",
                          "Output vector data file storing sample values (OGR format)."
                          "If not given, the input vector data file is updated.");

  // Doc example parameter settings
  SetDocExampleParameterValue("in", "vectorData.shp");
  SetDocExampleParameterValue("instat", "meanVar.xml");
  SetDocExampleParameterValue("model", "svmModel.svm");
  SetDocExampleParameterValue("out", "vectorDataLabeledVector.shp");
  SetDocExampleParameterValue("feat", "perimeter  area  width");
  SetDocExampleParameterValue("cfield", "predicted");

  SetOfficialDocLink();
}

template <>
bool VectorClassifier::shouldComputeConfidenceMap() const
{
  bool computeConfidenceMap(GetParameterInt("confmap") && m_Model->HasConfidenceIndex());

  if (!m_Model->HasConfidenceIndex() && GetParameterInt("confmap"))
  {
    otbAppLogWARNING("Confidence map requested but the classifier doesn't support it!");
  }

  return computeConfidenceMap;
}
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorClassifier)
