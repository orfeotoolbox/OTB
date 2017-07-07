/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbClassKMeansBase.h"

namespace otb
{
namespace Wrapper
{

class KMeansClassification: public ClassKMeansBase
{
public:
  /** Standard class typedefs. */
  typedef KMeansClassification Self;
  typedef ClassKMeansBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, Superclass);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("KMeansClassification");
    SetDescription("Unsupervised KMeans image classification");

    SetDocName("Unsupervised KMeans image classification");
    SetDocLongDescription("Performs unsupervised KMeans image classification.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Segmentation);

    // Perform initialization
    ClearApplications();

    // initialisations parameters and synchronizes parameters
    initKMParams();

    if ( HasValue("vm") ) ConnectKMClassificationMask();

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image containing the class indexes.");
    SetDefaultOutputPixelType("out",ImagePixelType_uint8);

    AddRAMParameter(); // TODO verifier si les RAMParameter sont bien tous cablés

    AddRANDParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("ts", "1000");
    SetDocExampleParameterValue("nc", "5");
    SetDocExampleParameterValue("maxit", "1000");
    SetDocExampleParameterValue("ct", "0.0001");
    SetDocExampleParameterValue("out", "ClassificationFilterOutput.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
  }

  void DoExecute() ITK_OVERRIDE
  {
    KMeansFileNamesHandler fileNames;

    std::string fieldName = "field";

    fileNames.CreateTemporaryFileNames(GetParameterString( "out" ));
    otbAppLogINFO(" init filename : " << fileNames.tmpVectorFile);      //RM

    // Create an image envelope
    ComputeImageEnvelope(fileNames.tmpVectorFile);
    // Add a new field at the ImageEnvelope output file
    ComputeAddField(fileNames.tmpVectorFile, fieldName);

    // Compute PolygonStatistics app
    UpdateKMPolygonClassStatisticsParameters(fileNames.tmpVectorFile);
    ComputePolygonStatistics(fileNames.polyStatOutput, fieldName);

    // Compute number of sample max for KMeans
    const int theoricNBSamplesForKMeans = GetParameterInt("ts");
    const int upperThresholdNBSamplesForKMeans = 1000 * 1000;
    const int actualNBSamplesForKMeans = std::min(theoricNBSamplesForKMeans,
                                                  upperThresholdNBSamplesForKMeans);
    otbAppLogINFO(<< actualNBSamplesForKMeans << " is the maximum sample size that will be used." \
                  << std::endl);

    // Compute SampleSelection and SampleExtraction app
    SelectAndExtractSamples(fileNames.sampleSelectOutput, fileNames.polyStatOutput, 
                            fieldName, fileNames.sampleExtractOutput,
                            actualNBSamplesForKMeans);

    // Compute a train model with TrainVectorClassifier app
    TrainKMModel(GetParameterImage("in"), fileNames.sampleExtractOutput,
                 fileNames.modelFile);

    // Compute a classification of the input image according to a model file
    KMeansClassif();

    // remove all tempory files
    if( IsParameterEnabled( "cleanup" ) )
      {
      otbAppLogINFO( <<"Final clean-up ..." );
      fileNames.clear();
      }

  }

private :

  void UpdateKMPolygonClassStatisticsParameters(const std::string &vectorFileName)
  {
    GetInternalApplication( "polystats" )->SetParameterString( "vec", vectorFileName, false );
    UpdateInternalParameters( "polystats" );
  }

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::KMeansClassification)


