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
  typedef TrainImagesRegression         Self;
  typedef CompositeApplication          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  /** Standard macro */
  itkTypeMacro(TrainImagesRegression, Superclass);

  /** filters typedefs*/
  typedef otb::OGRDataToSamplePositionFilter<FloatVectorImageType, UInt8ImageType, otb::PeriodicSampler> PeriodicSamplerType;
  typedef otb::SamplingRateCalculator::MapRateType MapRateType;

private:
  void DoInit() override
  {
    SetName("TrainImagesRegression");
    SetDescription(
        "Train a regression model from multiple triplets of feature images, predictor images "
        "and training vector data.");

    SetDocLongDescription(
        "Train a classifier from multiple triplets of predictor images, label images and training vector data. \n\n"

        "The training vector data must contain polygons corresponding to the input sampling positions. This data "
        "is used to extract samples using pixel values in each band of the predictor image and the corresponding "
        "ground truth extracted from the lagel image. If no training vector data is provided, the samples will be "
        "extracted on the full image extent.\n\n"

        "At the end of the application, the mean square error between groundtruth and predicted values is computed using "
        "the output model and the validation vector data. Note that if no validation data is given, the training data "
        "will be used for validation.\n\n"

        "The number of training and validation samples can be specified with parameters. If no size is given, all samples will "
        "be used. \n\n"

        "This application is based on LibSVM, OpenCV Machine Learning, and Shark ML. "
        "The output of this application is a text model file, whose format corresponds to the "
        "ML model type chosen. There is no image nor vector data output.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "TrainVectorRegression \n"
        "TrainImagesClassifier");

    AddDocTag(Tags::Learning);

    ClearApplications();

    InitIO();
    InitSampling();
    InitLearning();

    AddParameter(ParameterType_Bool, "cleanup", "Temporary files cleaning");
    SetParameterDescription("cleanup", "If activated, the application will try to clean all temporary files it created");
    SetParameterInt("cleanup", 1);

    // Doc example parameter settings
    SetDocExampleParameterValue("io.il", "inputPredictorImage.tif");
    SetDocExampleParameterValue("io.ip", "inputLabelImage.tif");
    SetDocExampleParameterValue("io.vd", "trainingData.shp");
    SetDocExampleParameterValue("io.valid", "validationData.shp");
    SetDocExampleParameterValue("sample.nt", "500");
    SetDocExampleParameterValue("sample.nv", "100");
    SetDocExampleParameterValue("io.imstat", "imageStats.xml");
    SetDocExampleParameterValue("classifier", "rf");
    SetDocExampleParameterValue("io.out", "model.txt");

    SetOfficialDocLink();
  }

  /** Holds sampling parameters specific to training or validation */
  struct SamplingParameters
  {
    std::vector<std::string> inputVectorList;
    unsigned int             numberOfSamples = 0;
    std::string              filePrefix      = "";
  };

  /** Init input/output parameters */
  void InitIO()
  {
    AddParameter(ParameterType_Group, "io", "Input and output data");
    SetParameterDescription("io", "This group of parameters allows setting input and output data.");

    AddParameter(ParameterType_InputImageList, "io.il", "Input predictor Image List");
    SetParameterDescription("io.il", "A list of input predictor images.");
    MandatoryOn("io.il");

    AddParameter(ParameterType_InputImageList, "io.ip", "Input label Image List");
    SetParameterDescription("io.ip", "A list of input label images.");
    MandatoryOn("io.ip");

    AddParameter(ParameterType_InputVectorDataList, "io.vd", "Input Vector Data List");
    SetParameterDescription("io.vd", "A list of vector data to select the training samples.");
    MandatoryOff("io.vd");

    AddParameter(ParameterType_InputVectorDataList, "io.valid", "Validation Vector Data List");
    SetParameterDescription("io.valid", "A list of vector data to select the validation samples.");
    MandatoryOff("io.valid");
  }

  /** Init sampling parameters and applications */
  void InitSampling()
  {
    if (!IsParameterEnabled("io.vd") || !HasValue("io.vd"))
      AddApplication("ImageEnvelope", "imageEnvelope", "Compute the image envelope");

    AddApplication("VectorDataSetField", "setfield", "Set additional vector field");
    AddApplication("PolygonClassStatistics", "polystat", "Polygon analysis");
    AddApplication("MultiImageSamplingRate", "rates", "Sampling rates");

    AddApplication("SampleSelection", "select", "Sample selection");
    AddApplication("SampleExtraction", "extraction", "Sample extraction");

    AddParameter(ParameterType_Group, "sample", "Sampling parameters");
    SetParameterDescription("sample", "This group of parameters allows setting sampling parameters");

    AddParameter(ParameterType_Int, "sample.nt", "Number of training samples");
    SetParameterDescription("sample.nt", "Number of training samples.");
    MandatoryOff("sample.nt");

    AddParameter(ParameterType_Int, "sample.nv", "Number of validation samples");
    SetParameterDescription("sample.nv", "Number of validation samples.");
    MandatoryOff("sample.nv");

    AddParameter(ParameterType_Float, "sample.ratio", "Training and validation sample ratio");
    SetParameterDescription("sample.ratio", "Ratio between training and validation samples.");
    SetDefaultParameterFloat("sample.ratio", 0.5);

    ShareParameter("rand", "select.rand");

    ShareParameter("ram", "polystat.ram");
    Connect("select.ram", "polystat.ram");
    Connect("extraction.ram", "polystat.ram");

    ShareParameter("sample.type", "select.sampler");

    ShareParameter("elev", "polystat.elev");
    Connect("select.elev", "polystat.elev");
    if (!IsParameterEnabled("io.vd") || !HasValue("io.vd"))
      Connect("imageEnvelope.elev", "polystat.elev");
  }

  /** Init Learning applications and share learning parameters */
  void InitLearning()
  {
    AddApplication("TrainVectorRegression", "training", "Train vector regression");

    ShareParameter("io.imstat", "training.io.stats");
    ShareParameter("io.out", "training.io.out");

    ShareParameter("classifier", "training.classifier");
    Connect("training.rand", "select.rand");

    ShareParameter("io.mse", "training.io.mse");
  }

  /** Compute the imageEnvelope of the first input predictor image, this envelope will be used as a
   * polygon to perform sampling operations */
  void ComputeImageEnvelope(const std::string& filePrefix)
  {
    auto  imageEnvelopeAppli = GetInternalApplication("imageEnvelope");
    auto& output             = m_FileHandler["imageEnvelope"];

    // For all input images, use the same vector file.
    for (unsigned int i = 0; i < GetParameterImageList("io.il")->Size(); i++)
    {
      output.push_back(GetParameterString("io.out") + "_" + filePrefix + "ImageEnvelope" + std::to_string(i) + ".shp");

      imageEnvelopeAppli->SetParameterInputImage("in", GetParameterImageList("io.il")->GetNthElement(i));
      imageEnvelopeAppli->SetParameterString("out", output[i]);

      // Call ExecuteAndWriteOutput because VectorDataSetField's ExecuteInternal() does not write vector data.
      imageEnvelopeAppli->ExecuteAndWriteOutput();
    }
  }

  /** Adds a class field to the input vectors, this is needed to perform sampling operations */
  void AddRegressionField(const std::vector<std::string>& inputFileNames, const std::string& filePrefix)
  {
    auto  setFieldAppli   = GetInternalApplication("setfield");
    auto& outputFileNames = m_FileHandler[filePrefix + "inputWithClassField"];

    setFieldAppli->SetParameterString("fn", m_ClassFieldName);
    setFieldAppli->SetParameterString("fv", "0");

    for (unsigned int i = 0; i < inputFileNames.size(); i++)
    {
      // The application is not called if the input file has already be processed (i.e. if the input list contains )
      // the same file multiple times. Instead a link is created in the file handler. We can do this
      // optimization here because the application only takes a vector Data as input but we cannot do it
      // for other sampling operations (PolygonClassStatistics and SampleSelection) because the input images
      // corresponding to one vector file may have different characteristics (resolution, origin, size)
      auto sameFileName = std::find(inputFileNames.begin(), inputFileNames.begin() + i, inputFileNames[i]);
      if (sameFileName != inputFileNames.begin() + i)
      {
        // Create a link to the corresponding output
        outputFileNames.push_back(outputFileNames[sameFileName - inputFileNames.begin()]);
      }
      else
      {
        outputFileNames.push_back(GetParameterString("io.out") + "_" + filePrefix + "Withfield" + std::to_string(i) + ".shp");
        setFieldAppli->SetParameterString("in", inputFileNames[i]);

        setFieldAppli->SetParameterString("out", outputFileNames[i]);

        // Call ExecuteAndWriteOutput because VectorDataSetField's ExecuteInternal() does not write vector data.
        setFieldAppli->ExecuteAndWriteOutput();
      }
    }
  }

  /** Prepare and execute polygonClassStatistics on each input vector data file */
  void ComputePolygonStatistics(const std::string& filePrefix)
  {
    auto                      polygonClassAppli = GetInternalApplication("polystat");
    const auto&               input             = m_FileHandler[filePrefix + "inputWithClassField"];
    auto&                     output            = m_FileHandler[filePrefix + "statsFiles"];
    FloatVectorImageListType* inputImageList    = GetParameterImageList("io.il");

    for (unsigned int i = 0; i < input.size(); i++)
    {
      output.push_back(GetParameterString("io.out") + "_" + filePrefix + "PolygonStats" + std::to_string(i) + ".xml");

      polygonClassAppli->SetParameterInputImage("in", inputImageList->GetNthElement(i));
      polygonClassAppli->SetParameterString("vec", input[i]);
      polygonClassAppli->SetParameterString("out", output[i]);

      polygonClassAppli->UpdateParameters();
      polygonClassAppli->SetParameterString("field", m_ClassFieldName);

      ExecuteInternal("polystat");
    }
  }

  /** Compute the sampling rates using the computed statistics */
  void ComputeSamplingRate(const std::string& filePrefix, unsigned int numberOfSamples)
  {
    auto samplingRateAppli = GetInternalApplication("rates");

    samplingRateAppli->SetParameterStringList("il", m_FileHandler[filePrefix + "statsFiles"]);

    std::string outputFileName = GetParameterString("io.out") + "_" + filePrefix + "rates.csv";
    samplingRateAppli->SetParameterString("out", outputFileName);

    if (numberOfSamples)
    {
      samplingRateAppli->SetParameterString("strategy", "constant");

      // Convert the integer number to string for the MultiImageSamplingRate application
      samplingRateAppli->SetParameterString("strategy.constant.nb", std::to_string(GetParameterInt("sample.nt")));
    }
    else
    {
      samplingRateAppli->SetParameterString("strategy", "all");
    }

    ExecuteInternal("rates");

    auto& rateFiles = m_FileHandler[filePrefix + "rateFiles"];
    for (unsigned int i = 0; i < m_FileHandler[filePrefix + "statsFiles"].size(); i++)
    {
      rateFiles.push_back(GetParameterString("io.out") + "_" + filePrefix + "rates_" + std::to_string(i + 1) + ".csv");
    }
  }

  /** Configure and execute Sample Selection on each vector using the computed rates and statistic files. */
  void SelectSamples(const std::string& filePrefix)
  {
    auto sampleSelection = GetInternalApplication("select");

    FloatVectorImageListType* inputImageList    = GetParameterImageList("io.il");
    const auto&               inputVectorFiles  = m_FileHandler[filePrefix + "inputWithClassField"];
    const auto&               rateFiles         = m_FileHandler[filePrefix + "rateFiles"];
    const auto&               statFiles         = m_FileHandler[filePrefix + "statsFiles"];
    auto&                     outputVectorFiles = m_FileHandler[filePrefix + "samples"];

    for (unsigned int i = 0; i < inputVectorFiles.size(); i++)
    {
      outputVectorFiles.push_back(GetParameterString("io.out") + "_" + filePrefix + "samples" + std::to_string(i) + ".shp");
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

  /** Configure and execute sampleExtraction. The application is called twice by input vector
   * first values are extracted from the predictor image and then the groundtruth is extracted
   * from the label image.*/
  void ExtractSamples(const std::string& filePrefix)
  {
    auto sampleExtraction = GetInternalApplication("extraction");

    FloatVectorImageListType* predictorImageList = GetParameterImageList("io.il");
    FloatVectorImageListType* labelImageList     = GetParameterImageList("io.ip");
    const auto&               vectorFiles        = m_FileHandler[filePrefix + "samples"];

    for (unsigned int i = 0; i < vectorFiles.size(); i++)
    {
      sampleExtraction->SetParameterString("vec", vectorFiles[i]);
      sampleExtraction->UpdateParameters();
      sampleExtraction->SetParameterString("field", m_ClassFieldName);

      // First Extraction : Values from the predictor image.
      sampleExtraction->SetParameterInputImage("in", predictorImageList->GetNthElement(i));
      sampleExtraction->SetParameterString("outfield", "prefix");
      sampleExtraction->SetParameterString("outfield.prefix.name", m_FeaturePrefix);
      ExecuteInternal("extraction");

      // Second Extraction : groundtruth from the label image.
      sampleExtraction->SetParameterInputImage("in", labelImageList->GetNthElement(i));
      sampleExtraction->SetParameterString("outfield", "list");
      sampleExtraction->SetParameterStringList("outfield.list.names", {m_PredictionFieldName});
      ExecuteInternal("extraction");
    }
  }

  void SplitTrainingAndValidationSamples(const std::string& inputSampleFilePrefix)
  {
    auto        ImageList        = GetParameterImageList("io.il");
    const auto& inputSampleFiles = m_FileHandler[inputSampleFilePrefix + "samples"];
    auto&       trainSampleFiles = m_FileHandler["trainsamples"];
    auto&       validSampleFiles = m_FileHandler["validsamples"];
    const auto& rateFiles        = m_FileHandler[inputSampleFilePrefix + "rateFiles"];

    for (unsigned int i = 0; i < ImageList->Size(); i++)
    {
      trainSampleFiles.push_back(GetParameterString("io.out") + "_trainsamples" + std::to_string(i) + ".shp");
      validSampleFiles.push_back(GetParameterString("io.out") + "_validsamples" + std::to_string(i) + ".shp");

      // Split between training and validation
      auto                     image     = ImageList->GetNthElement(i);
      ogr::DataSource::Pointer source    = ogr::DataSource::New(inputSampleFiles[i], ogr::DataSource::Modes::Read);
      ogr::DataSource::Pointer destTrain = ogr::DataSource::New(trainSampleFiles[i], ogr::DataSource::Modes::Overwrite);
      ogr::DataSource::Pointer destValid = ogr::DataSource::New(validSampleFiles[i], ogr::DataSource::Modes::Overwrite);
      // read sampling rates from ratesTrainOutputs
      SamplingRateCalculator::Pointer rateCalculator = SamplingRateCalculator::New();
      rateCalculator->Read(rateFiles[i]);
      // Compute sampling rates for train and valid
      const otb::SamplingRateCalculator::MapRateType& inputRates = rateCalculator->GetRatesByClass();
      otb::SamplingRateCalculator::MapRateType        trainRates;
      otb::SamplingRateCalculator::MapRateType        validRates;
      otb::SamplingRateCalculator::TripletType        tpt;
      for (MapRateType::const_iterator it = inputRates.begin(); it != inputRates.end(); ++it)
      {
        double        vtr         = GetParameterFloat("sample.ratio");
        unsigned long total       = std::min(it->second.Required, it->second.Tot);
        unsigned long neededValid = static_cast<unsigned long>((double)total * vtr);
        unsigned long neededTrain = total - neededValid;
        tpt.Tot                   = total;
        tpt.Required              = neededTrain;
        tpt.Rate                  = (1.0 - vtr);
        trainRates[it->first]     = tpt;
        tpt.Tot                   = neededValid;
        tpt.Required              = neededValid;
        tpt.Rate                  = 1.0;
        validRates[it->first]     = tpt;
      }

      // Use an otb::OGRDataToSamplePositionFilter with 2 outputs
      PeriodicSamplerType::SamplerParameterType param;
      param.Offset                          = 0;
      param.MaxJitter                       = 0;
      PeriodicSamplerType::Pointer splitter = PeriodicSamplerType::New();
      splitter->SetInput(image);
      splitter->SetOGRData(source);
      splitter->SetOutputPositionContainerAndRates(destTrain, trainRates, 0);
      splitter->SetOutputPositionContainerAndRates(destValid, validRates, 1);
      splitter->SetFieldName(m_ClassFieldName);
      splitter->SetLayerIndex(0);
      splitter->SetOriginFieldName(std::string(""));
      splitter->SetSamplerParameters(param);
      splitter->GetStreamer()->SetAutomaticTiledStreaming(static_cast<unsigned int>(this->GetParameterInt("ram")));
      AddProcess(splitter->GetStreamer(), "Split samples between training and validation...");
      splitter->Update();
    }
  }

  /** Configure and execute TrainVectorClassifier. Note that many parameters of TrainVectorClassifier
   * are shared with the main application during initialization. */
  void TrainModel()
  {
    auto trainVectorRegression = GetInternalApplication("training");

    const auto&              trainSampleFileNameList = m_FileHandler["trainsamples"];
    std::vector<std::string> featureNames;
    for (unsigned int i = 0; i < GetParameterImageList("io.il")->Size(); i++)
    {
      featureNames.push_back(m_FeaturePrefix + std::to_string(i));
    }

    trainVectorRegression->SetParameterStringList("io.vd", trainSampleFileNameList);
    trainVectorRegression->UpdateParameters();
    trainVectorRegression->SetParameterString("cfield", m_PredictionFieldName);
    trainVectorRegression->SetParameterStringList("feat", featureNames);

    if ((IsParameterEnabled("io.valid") && HasValue("io.valid")) || GetParameterFloat("sample.ratio") > 0)
    {
      trainVectorRegression->SetParameterStringList("valid.vd", m_FileHandler["validsamples"]);
    }

    ExecuteInternal("training");
  }

  void DoUpdateParameters() override
  {
  }

  /** remove all file in the fileHandler std::map and clear the fileHandler std::map.
   * For .shp file, this method also removes .shx, .dbf and .prj associated files.*/
  bool ClearFileHandler()
  {
    bool res = true;
    for (const auto& fileNameList : m_FileHandler)
    {
      for (const auto& filename : fileNameList.second)
      {
        res = true;
        if (itksys::SystemTools::FileExists(filename))
        {
          size_t posExt = filename.rfind('.');
          if (posExt != std::string::npos && filename.compare(posExt, std::string::npos, ".shp") == 0)
          {
            std::string shxPath = filename.substr(0, posExt) + std::string(".shx");
            std::string dbfPath = filename.substr(0, posExt) + std::string(".dbf");
            std::string prjPath = filename.substr(0, posExt) + std::string(".prj");
            itksys::SystemTools::RemoveFile(shxPath);
            itksys::SystemTools::RemoveFile(dbfPath);
            itksys::SystemTools::RemoveFile(prjPath);
          }
          res = itksys::SystemTools::RemoveFile(filename);
        }
      }
    }
    m_FileHandler.clear();
    return res;
  }

  void PerformSampling(const SamplingParameters& params)
  {
    std::vector<std::string> vectorData      = params.inputVectorList;
    std::string              filePrefix      = params.filePrefix;
    unsigned int             numberOfSamples = params.numberOfSamples;

    AddRegressionField(vectorData, filePrefix);
    ComputePolygonStatistics(filePrefix);
    ComputeSamplingRate(filePrefix, numberOfSamples);
    SelectSamples(filePrefix);
    ExtractSamples(filePrefix);
  }


  void DoExecute() override
  {
    SamplingParameters trainParams;
    trainParams.filePrefix = "vd";

    if (HasValue("sample.nt"))
      trainParams.numberOfSamples = GetParameterInt("sample.nt");

    if (IsParameterEnabled("io.vd") && HasValue("io.vd"))
    {
      trainParams.inputVectorList = GetParameterStringList("io.vd");
    }
    else
    {
      otbAppLogINFO("No input training vector data: the image envelope will be used.");
      ComputeImageEnvelope(trainParams.filePrefix);
      trainParams.inputVectorList = m_FileHandler["imageEnvelope"];
    }

    PerformSampling(trainParams);

    // User validation data
    if (IsParameterEnabled("io.valid") && HasValue("io.valid"))
    {
      m_FileHandler["trainsamples"] = m_FileHandler[trainParams.filePrefix + "samples"];

      SamplingParameters validParams;
      validParams.inputVectorList = GetParameterStringList("io.valid");
      validParams.filePrefix      = "valid";
      if (HasValue("sample.nv"))
        validParams.numberOfSamples = GetParameterInt("sample.nv");

      PerformSampling(validParams);
    }
    // Split train and validation data
    else if (GetParameterFloat("sample.ratio") > 0)
    {
      otbAppLogINFO("No input validation vector data: the input training vector data will be split.");
      SplitTrainingAndValidationSamples(trainParams.filePrefix);
    }
    else
    {
      m_FileHandler["trainsamples"] = m_FileHandler[trainParams.filePrefix + "samples"];
    }

    otbAppLogINFO("Sampling Done.");

    TrainModel();

    if (GetParameterInt("cleanup"))
    {
      otbAppLogINFO("Cleaning temporary files ...");
      ClearFileHandler();
    }
  }

  /** Field used for sampling operations. */
  std::string m_ClassFieldName = "regclass";

  /** field containg the extracted regression groundtruth */
  std::string m_PredictionFieldName = "prediction";

  /** field containing the extracted pixel values for regression */
  std::string m_FeaturePrefix = "value_";

  /** Container containing the list of temporary files created during the execution*/
  std::unordered_map<std::string, std::vector<std::string>> m_FileHandler;
};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainImagesRegression)
