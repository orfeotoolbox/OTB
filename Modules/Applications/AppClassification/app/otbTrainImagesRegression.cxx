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
  struct SamplingParameters
  {
    std::vector<std::string> inputVectorList;
    unsigned int numberOfSamples = 0;
    std::string filePrefix = "";
  };

  void AddRegressionField(const std::vector<std::string> & inputFileNames,
                          const std::string & filePrefix)
  {
    auto setFieldAppli = GetInternalApplication("setfield");
    auto& outputFileNames =  m_FileHandler[ filePrefix+"inputWithClassField" ]; 
    
    setFieldAppli->SetParameterString("fn", m_ClassFieldName);
    setFieldAppli->SetParameterString("fv", "0");
    
    for (unsigned int i =0; i < inputFileNames.size(); i++)
    {
      outputFileNames.push_back(GetParameterString("io.out")+"_"+filePrefix+"Withfield"+ std::to_string(i) +".shp");
      setFieldAppli->SetParameterString("in", inputFileNames[i]);
      setFieldAppli->SetParameterString("out", outputFileNames[i]);
      
      // Call ExecuteAndWriteOutput because VectorDataSetField's ExecuteInternal() does not write vector data.
      setFieldAppli->ExecuteAndWriteOutput();
    }
  }

  void ComputePolygonStatistics(const std::string & filePrefix)
  {
    auto polygonClassAppli = GetInternalApplication("polystat");
    auto& input = m_FileHandler[ filePrefix+"inputWithClassField" ];
    auto& output = m_FileHandler[ filePrefix+"statsFiles" ];
    FloatVectorImageListType* inputImageList = GetParameterImageList( "io.il" );
    
    for (unsigned int i =0; i < input.size(); i++)
    {
      output.push_back(GetParameterString("io.out")+"_"+filePrefix+"PolygonStats"+ std::to_string(i) +".xml");
      
      polygonClassAppli->SetParameterInputImage( "in", inputImageList->GetNthElement(i) );
      polygonClassAppli->SetParameterString( "vec", input[i]);
      polygonClassAppli->SetParameterString( "out", output[i]);
  
      polygonClassAppli->UpdateParameters();
      polygonClassAppli->SetParameterString( "field", m_ClassFieldName);
    
      ExecuteInternal( "polystat" );
    }
    
  }

  void ComputeSamplingRate(const std::string & filePrefix, unsigned int numberOfSamples)
  {
    auto samplingRateAppli = GetInternalApplication("rates");
    
    samplingRateAppli->SetParameterStringList( "il", m_FileHandler[ filePrefix+"statsFiles" ]);
    
    std::string outputFileName = GetParameterString("io.out")+"_"+filePrefix+"rates.csv";
    samplingRateAppli->SetParameterString("out", outputFileName);
    
    if (numberOfSamples)
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
    
    auto& rateFiles = m_FileHandler[filePrefix+"rateFiles"];
    for (unsigned int i = 0; i< m_FileHandler[filePrefix+"statsFiles"].size(); i++)
    {
      rateFiles.push_back(GetParameterString("io.out")+"_"+filePrefix+"rates_"+std::to_string(i+1)+".csv");
    }
    
  }

  void SelectSamples(const std::string & filePrefix)
  {
    auto sampleSelection = GetInternalApplication("select");
    
    FloatVectorImageListType* inputImageList = GetParameterImageList( "io.il" );
    auto& inputVectorFiles = m_FileHandler[ filePrefix+"inputWithClassField" ];
    auto& outputVectorFiles = m_FileHandler[ filePrefix+"samples" ];
    auto& rateFiles = m_FileHandler [filePrefix+"rateFiles"];
    auto& statFiles = m_FileHandler [filePrefix+"statsFiles"];
    
    for (unsigned int i =0; i < inputVectorFiles.size(); i++)
    {
      outputVectorFiles.push_back(GetParameterString("io.out")+"_"+filePrefix+"samples"+std::to_string(i)+".shp");
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
  
  void ExtractSamples(const std::string & filePrefix)
  {
    auto sampleExtraction = GetInternalApplication("extraction");
    
    FloatVectorImageListType* featureImageList = GetParameterImageList( "io.il" );
    FloatVectorImageListType* predictorImageList = GetParameterImageList("io.ip");
    auto& vectorFiles = m_FileHandler[ filePrefix+"samples" ];
 
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
    
    auto& trainSampleFileNameList = m_FileHandler["trainsamples"];
    std::vector<std::string> featureNames;
    for (unsigned int i = 0; i< GetParameterStringList("io.vd").size(); i++)
    {
      featureNames.push_back(m_FeaturePrefix+std::to_string(i));
    }
    
    trainVectorRegression->SetParameterStringList("io.vd", trainSampleFileNameList);
    trainVectorRegression->UpdateParameters();
    trainVectorRegression->SetParameterString("cfield", m_PredictionFieldName);
    trainVectorRegression->SetParameterStringList("feat", featureNames);
    
    if( IsParameterEnabled( "io.valid" ) && HasValue( "io.valid" ) )
    {
      trainVectorRegression->SetParameterStringList("valid.vd", m_FileHandler["validsamples"]);
    }
    
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
    
    AddParameter( ParameterType_InputVectorDataList, "io.valid", "Validation Vector Data List" );
    SetParameterDescription( "io.valid", "A list of vector data to select the validation samples." );
    MandatoryOff( "io.valid" );
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
    
    AddParameter( ParameterType_Int, "sample.nv", "Number of validation samples" );
    SetParameterDescription( "sample.nv", "Number of validation samples." );
    MandatoryOff( "sample.nv" );
    
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
  
  void PerformSampling(const SamplingParameters & params)
  {
    std::vector<std::string> vectorData = params.inputVectorList;
    std::string filePrefix = params.filePrefix;
    unsigned int numberOfSamples = params.numberOfSamples;
    
    AddRegressionField(vectorData, filePrefix);
    ComputePolygonStatistics(filePrefix);
    ComputeSamplingRate(filePrefix, numberOfSamples);
    SelectSamples(filePrefix);
    ExtractSamples(filePrefix);
  }
  
  
  void DoExecute() override
  {
    SamplingParameters trainParams;
    trainParams.inputVectorList = GetParameterStringList("io.vd");
    trainParams.filePrefix="train";
    if (HasValue("sample.nt"))
      trainParams.numberOfSamples = GetParameterInt("sample.nt");
    
    PerformSampling(trainParams);
    
    if( IsParameterEnabled( "io.valid" ) && HasValue( "io.valid" ) )
    {
      SamplingParameters validParams;
      validParams.inputVectorList = GetParameterStringList("io.valid");
      validParams.filePrefix="valid";
      if (HasValue("sample.nv"))
        validParams.numberOfSamples = GetParameterInt("sample.nv");
      
      PerformSampling(validParams);
    }

    
    otbAppLogINFO( "Sampling Done." );
    
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
