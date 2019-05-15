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

#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataToSamplePositionFilter.h"

namespace otb
{
namespace Wrapper
{

class TrainImagesRegression : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef TrainImagesRegression Self;
  typedef CompositeApplication Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  /** Standard macro */
  itkTypeMacro( TrainImagesRegression, Superclass );
  
protected:

  void AddRegressionField()
  {
    auto setFieldAppli = GetInternalApplication("setfield");
    auto inputFileNames = GetParameterStringList( "io.vd" );
    auto& outputFileNames =  m_FileHandler[ "inputWithClassField" ]; 
    
    setFieldAppli->SetParameterString("fn", m_ClassFieldName);
    setFieldAppli->SetParameterString("fv", "0");
    
    for (unsigned int i =0; i < inputFileNames.size(); i++)
    {
      outputFileNames.push_back(GetParameterString("io.out")+"_setfield"+ std::to_string(i) +".shp");
      setFieldAppli->SetParameterString("in", inputFileNames[i]);
      setFieldAppli->SetParameterString("out", outputFileNames[i]);
      
      // Call ExecuteAndWriteOutput because VectorDataSetField's ExecuteInternal() does not write vector data.
      setFieldAppli->ExecuteAndWriteOutput();
    }
  }

  void ComputePolygonStatistics()
  {
    auto polygonClassAppli = GetInternalApplication("polystat");
    auto& input = m_FileHandler[ "inputWithClassField" ];
    auto& output = m_FileHandler[ "statsFiles" ];
    FloatVectorImageListType* inputImageList = GetParameterImageList( "io.il" );
    
    
    for (unsigned int i =0; i < input.size(); i++)
    {
      output.push_back(GetParameterString("io.out")+"_polygonstat"+ std::to_string(i) +".xml");
      
      polygonClassAppli->SetParameterInputImage( "in", inputImageList->GetNthElement(i) );
      polygonClassAppli->SetParameterString( "vec", input[i]);
      polygonClassAppli->SetParameterString( "out", output[i]);
  
      polygonClassAppli->UpdateParameters();
      polygonClassAppli->SetParameterString( "field", m_ClassFieldName);
    
      ExecuteInternal( "polystat" );
    }
    
  }

  void ComputeSamplingRate()
  {
    auto samplingRateAppli = GetInternalApplication("rates");
    
    samplingRateAppli->SetParameterStringList( "il", m_FileHandler[ "statsFiles" ]);
    
    std::string outputFileName = GetParameterString("io.out")+"_rates.csv";
    samplingRateAppli->SetParameterString("out", outputFileName);
    
    if (HasValue("sample.nt"))
    {
      samplingRateAppli->SetParameterString("strategy", "constant");
      
      // TODO why nb is a string in MultiImageSamplingRate + MultiImageSamplingRate seems to return nb-1 samples ...
      samplingRateAppli->SetParameterString("strategy.constant.nb", std::to_string(GetParameterInt("sample.nt")));
    }
    else
    {
      samplingRateAppli->SetParameterString("strategy", "all");
    }
    
    ExecuteInternal( "rates");
    
    auto& rateFiles = m_FileHandler["rateFiles"];
    for (unsigned int i = 0; i< m_FileHandler["statsFiles"].size(); i++)
    {
      rateFiles.push_back(GetParameterString("io.out")+"_rates_"+std::to_string(i+1)+".csv");
    }
    
  }

  void SelectSamples()
  {
    auto sampleSelection = GetInternalApplication("select");
    
    FloatVectorImageListType* inputImageList = GetParameterImageList( "io.il" );
    auto& inputVectorFiles = m_FileHandler[ "inputWithClassField" ];
    auto& outputVectorFiles = m_FileHandler[ "samples" ];
    auto& rateFiles = m_FileHandler ["rateFiles"];
    auto& statFiles = m_FileHandler ["statsFiles"];
    
    for (unsigned int i =0; i < inputVectorFiles.size(); i++)
    {
      outputVectorFiles.push_back(GetParameterString("io.out")+"_samples"+std::to_string(i)+".shp");
      sampleSelection->SetParameterInputImage("in", inputImageList->GetNthElement(i));
      sampleSelection->SetParameterString("vec", inputVectorFiles[i]);
      sampleSelection->SetParameterString("instats", statFiles[i]);
      sampleSelection->SetParameterString("strategy", "byclass");
      sampleSelection->SetParameterString("strategy.byclass.in", rateFiles[i]);
      sampleSelection->SetParameterString("out", outputVectorFiles[i]);
      
      sampleSelection->UpdateParameters();
      sampleSelection->SetParameterString("field", m_ClassFieldName);
      
      ExecuteInternal("select");
    }
  }
  
  void ExtractSamples()
  {
    auto sampleExtraction = GetInternalApplication("extraction");
    
    FloatVectorImageListType* featureImageList = GetParameterImageList( "io.il" );
    FloatVectorImageListType* predictorImageList = GetParameterImageList("io.ip");
    auto& vectorFiles = m_FileHandler[ "samples" ];
 
    for (unsigned int i =0; i < vectorFiles.size(); i++)
    {
      sampleExtraction->SetParameterString("vec", vectorFiles[i]);
      sampleExtraction->UpdateParameters();
      sampleExtraction->SetParameterString("field", m_ClassFieldName);
      
      //First Extraction
      sampleExtraction->SetParameterInputImage("in", featureImageList->GetNthElement(i));
      sampleExtraction->SetParameterString("outfield", "prefix");
      sampleExtraction->SetParameterString("outfield.prefix.name", m_FeaturePrefix);
      ExecuteInternal("extraction");
      
      //Second Extraction
      sampleExtraction->SetParameterInputImage("in", predictorImageList->GetNthElement(i));
      sampleExtraction->SetParameterString("outfield", "list");
      sampleExtraction->SetParameterStringList("outfield.list.names", {m_PredictionFieldName});
      ExecuteInternal("extraction");
    }
  }
  
  void TrainModel()
  {
    auto trainVectorRegression = GetInternalApplication("training");
    
    auto& sampleFileNameList = m_FileHandler["samples"];
    std::vector<std::string> featureNames;
    for (unsigned int i = 0; i<sampleFileNameList.size(); i++)
    {
      featureNames.push_back(m_FeaturePrefix+std::to_string(i));
    }
    
    trainVectorRegression->SetParameterStringList("io.vd", sampleFileNameList);
    trainVectorRegression->UpdateParameters();
    trainVectorRegression->SetParameterString("cfield", m_PredictionFieldName);
    trainVectorRegression->SetParameterStringList("feat", featureNames);
    ExecuteInternal("training");
  }
  
  void InitIO()
  {
    AddParameter( ParameterType_Group, "io", "Input and output data" );
    SetParameterDescription( "io", "This group of parameters allows setting input and output data." );

    AddParameter( ParameterType_InputImageList, "io.il", "Input Image List" );
    SetParameterDescription( "io.il", "A list of input images." );
    MandatoryOn( "io.il" );
    
    AddParameter( ParameterType_InputImageList, "io.ip", "Input Predictor Image List" );
    SetParameterDescription( "io.ip", "A list of input images." );
    MandatoryOn( "io.ip" );
    
    AddParameter( ParameterType_InputVectorDataList, "io.vd", "Input Vector Data List" );
    SetParameterDescription( "io.vd", "A list of vector data to select the training samples." );
    MandatoryOn( "io.vd" );
  }
  
  void InitSampling()
  {  
    AddApplication( "VectorDataSetField", "setfield", "Set additional vector field");
    AddApplication( "PolygonClassStatistics", "polystat", "Polygon analysis" );
    AddApplication( "MultiImageSamplingRate", "rates", "Sampling rates" );
    
    AddParameter( ParameterType_Group, "sample", "Sampling parameters" );
    SetParameterDescription( "sample", "This group of parameters allows setting sampling parameters" );

    AddParameter( ParameterType_Int, "sample.nt", "Number of training samples" );
    SetParameterDescription( "sample.nt", "Number of training samples." );
    MandatoryOff( "sample.nt" );
    
    AddApplication( "SampleSelection", "select", "Sample selection" );
    AddApplication( "SampleExtraction", "extraction", "Sample extraction" );
    
  }
  
  void InitLearning()
  {
    AddApplication( "TrainVectorRegression", "training", "Train vector regression"); 
    
    ShareParameter( "io.imstat", "training.io.stats" );
    ShareParameter( "io.out", "training.io.out" );

    ShareParameter( "classifier", "training.classifier" );
    ShareParameter( "rand", "training.rand" );

    ShareParameter( "io.mse", "training.io.mse" );
  }

private:

  void DoInit() override
  {
    SetName("TrainImagesRegression");
    SetDescription(" ");

    SetDocLongDescription(" ");
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    
    AddDocTag(Tags::Learning);
    
    SetOfficialDocLink();
    
    ClearApplications();
    
    InitIO();
    InitSampling();
    InitLearning();
    
    AddParameter( ParameterType_Bool, "cleanup", "Temporary files cleaning" );
    SetParameterDescription( "cleanup",
                           "If activated, the application will try to clean all temporary files it created" );
    SetParameterInt( "cleanup", 1);
  }
  
  void DoUpdateParameters() override
  {
  }

  bool ClearFileHandler()
  {
    bool res = true;
    for (const auto & fileNameList : m_FileHandler)
    {
      for (const auto & filename : fileNameList.second)
      {
        res = true;
        if( itksys::SystemTools::FileExists( filename ) )
        {
          size_t posExt = filename.rfind( '.' );
          if( posExt != std::string::npos && filename.compare( posExt, std::string::npos, ".shp" ) == 0 )
          {
            std::string shxPath = filename.substr( 0, posExt ) + std::string( ".shx" );
            std::string dbfPath = filename.substr( 0, posExt ) + std::string( ".dbf" );
            std::string prjPath = filename.substr( 0, posExt ) + std::string( ".prj" );
            itksys::SystemTools::RemoveFile( shxPath );
            itksys::SystemTools::RemoveFile( dbfPath );
            itksys::SystemTools::RemoveFile( prjPath );
          }
        res = itksys::SystemTools::RemoveFile( filename );
        }
      }
    }
    m_FileHandler.clear();
    return res;
  }
  
  void DoExecute() override
  {
    //TODO validation set ??
    
    AddRegressionField();
    
    std::cout << "Regression field added" << std::endl;
    
    ComputePolygonStatistics();
      
    std::cout << "Polygon class statistic done" << std::endl;
    
    ComputeSamplingRate();
  
    std::cout << "Sampling rate computation done" << std::endl;
    
    SelectSamples();
    
    ExtractSamples();
    
    TrainModel();
   
    if (GetParameterInt("cleanup"))
    {
      otbAppLogINFO( "Cleaning temporary files ..." );
      ClearFileHandler();
    }
  }
  
  std::string m_ClassFieldName = "regclass";
  
  std::string m_PredictionFieldName = "prediction";
  
  std::string m_FeaturePrefix = "value_";
  
  std::map< std::string, std::vector<std::string>> m_FileHandler;
  
};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainImagesRegression)
