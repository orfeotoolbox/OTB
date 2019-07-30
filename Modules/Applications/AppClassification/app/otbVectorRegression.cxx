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

#include "otbVectorPrediction.h"

namespace otb
{
namespace Wrapper
{

using VectorRegression = VectorPrediction<true, float, float>;

template<>
void
VectorRegression
::DoInitSpecialization()
{
  SetName("VectorRegression");
  SetDescription("Performs regression on the input vector data according to a model file.");

  SetDocAuthors("OTB-Team");
  SetDocLongDescription("This application performs a vector data regression "
    "based on a model file produced by the TrainVectorRegression application."
    "Features of the vector data output will contain the values predicted by the classifier. \n"
    "There are two modes: \n"
      "1) Update mode: add of the 'cfield' field containing the predicted value in the input file. \n"
      "2) Write mode: copies the existing fields of the input file to the output file "
         " and add the 'cfield' field containing the predicted value. \n"
    "If you have declared the output file, the write mode applies. "
    "Otherwise, the input file update mode will be applied.");

  SetDocLimitations("Shapefiles are supported, but the SQLite format is only supported in update mode.");
  SetDocSeeAlso("TrainVectorRegression");
  AddDocTag(Tags::Learning);

  AddParameter(ParameterType_InputVectorData, "in", "Name of the input vector data");
  SetParameterDescription("in","The input vector data file to classify.");

  AddParameter(ParameterType_InputFilename, "instat", "Statistics file");
  SetParameterDescription("instat", "A XML file containing mean and standard deviation to center"
    "and reduce samples before classification, produced by ComputeImagesStatistics application.");
  MandatoryOff("instat");

  AddParameter(ParameterType_InputFilename, "model", "Model file");
  SetParameterDescription("model", "Model file produced by TrainVectorRegression application.");

  AddParameter(ParameterType_String,"cfield","Output field");
  SetParameterDescription("cfield","Field containing the predicted value."
    "Only geometries with this field available will be taken into account.\n"
    "The field is added either in the input file (if 'out' off) or in the output file.\n"
    "Caution, the 'cfield' must not exist in the input file if you are updating the file.");
  SetParameterString("cfield","predicted");
  
  AddParameter(ParameterType_ListView, "feat", "Field names to be calculated");
  SetParameterDescription("feat","List of field names in the input vector data used as features for training. "
    "Put the same field names as the TrainVectorRegression application.");

  AddParameter(ParameterType_OutputFilename, "out", "Output vector data file");
  MandatoryOff("out");

  SetParameterDescription("out","Output vector data file storing sample values (OGR format)."
    "If not given, the input vector data file is updated.");
  MandatoryOff("out");

  // Doc example parameter settings
  SetDocExampleParameterValue("in", "vectorData.shp");
  SetDocExampleParameterValue("instat", "meanVar.xml");
  SetDocExampleParameterValue("model", "rfModel.rf");
  SetDocExampleParameterValue("out", "vectorDataLabeledVector.shp");
  SetDocExampleParameterValue("feat", "perimeter  area  width");
  SetDocExampleParameterValue("cfield", "predicted");

  SetOfficialDocLink();
}

template<>
void
VectorRegression
::CreatePredictionField(OGRFeatureDefn & layerDefn, otb::ogr::Layer & outLayer)
{
  int idx = layerDefn.GetFieldIndex(GetParameterString("cfield").c_str());
  if (idx >= 0)
  {
    if (layerDefn.GetFieldDefn(idx)->GetType() != OFTReal)
      itkExceptionMacro("Field name "<< GetParameterString("cfield") << " already exists with a different type!");
  }
  else
  {
    OGRFieldDefn predictedField(GetParameterString("cfield").c_str(), OFTReal);
    ogr::FieldDefn predictedFieldDef(predictedField);
    outLayer.CreateField(predictedFieldDef);
  }
}

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorRegression)
