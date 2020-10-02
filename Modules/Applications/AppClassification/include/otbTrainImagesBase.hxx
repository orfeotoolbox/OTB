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

#ifndef otbTrainImagesBase_hxx
#define otbTrainImagesBase_hxx

#include "otbTrainImagesBase.h"

namespace otb
{
namespace Wrapper
{
void TrainImagesBase::InitIO()
{
  // Group IO
  AddParameter(ParameterType_Group, "io", "Input and output data");
  SetParameterDescription("io", "This group of parameters allows setting input and output data.");

  AddParameter(ParameterType_InputImageList, "io.il", "Input Image List");
  SetParameterDescription("io.il", "A list of input images.");
  AddParameter(ParameterType_InputVectorDataList, "io.vd", "Input Vector Data List");
  SetParameterDescription("io.vd", "A list of vector data to select the training samples.");
  MandatoryOn("io.vd");

  AddParameter(ParameterType_Bool, "cleanup", "Temporary files cleaning");
  SetParameterDescription("cleanup", "If activated, the application will try to clean all temporary files it created");
  SetParameterInt("cleanup", 1);
}

void TrainImagesBase::InitSampling()
{
  AddApplication("PolygonClassStatistics", "polystat", "Polygon analysis");
  AddApplication("MultiImageSamplingRate", "rates", "Sampling rates");
  AddApplication("SampleSelection", "select", "Sample selection");
  AddApplication("SampleExtraction", "extraction", "Sample extraction");

  // Sampling settings
  AddParameter(ParameterType_Group, "sample", "Training and validation samples parameters");
  SetParameterDescription("sample", "This group of parameters allows you to set training and validation sample lists parameters.");
  AddParameter(ParameterType_Int, "sample.mt", "Maximum training sample size per class");
  SetDefaultParameterInt("sample.mt", 1000);
  SetParameterDescription("sample.mt",
                          "Maximum size per class (in pixels) of "
                          "the training sample list (default = 1000) (no limit = -1). If equal to -1,"
                          " then the maximal size of the available training sample list per class "
                          "will be equal to the surface area of the smallest class multiplied by the"
                          " training sample ratio.");
  AddParameter(ParameterType_Int, "sample.mv", "Maximum validation sample size per class");
  SetDefaultParameterInt("sample.mv", 1000);
  SetParameterDescription("sample.mv",
                          "Maximum size per class (in pixels) of "
                          "the validation sample list (default = 1000) (no limit = -1). If equal to -1,"
                          " then the maximal size of the available validation sample list per class "
                          "will be equal to the surface area of the smallest class multiplied by the "
                          "validation sample ratio.");
  AddParameter(ParameterType_Int, "sample.bm", "Bound sample number by minimum");
  SetDefaultParameterInt("sample.bm", 1);
  SetParameterDescription("sample.bm",
                          "Bound the number of samples for each "
                          "class by the number of available samples by the smaller class. Proportions "
                          "between training and validation are respected. Default is true (=1).");
  AddParameter(ParameterType_Float, "sample.vtr", "Training and validation sample ratio");
  SetParameterDescription("sample.vtr",
                          "Ratio between training and validation samples (0.0 = all training, 1.0 = "
                          "all validation) (default = 0.5).");
  SetParameterFloat("sample.vtr", 0.5);
  SetMaximumParameterFloatValue("sample.vtr", 1.0);
  SetMinimumParameterFloatValue("sample.vtr", 0.0);

  //  AddParameter( ParameterType_Float, "sample.percent", "Percentage of sample extract from images" );
  //  SetParameterDescription( "sample.percent", "Percentage of sample extract from images for "
  //          "training and validation when only images are provided." );
  //  SetDefaultParameterFloat( "sample.percent", 1.0 );
  //  SetMinimumParameterFloatValue( "sample.percent", 0.0 );
  //  SetMaximumParameterFloatValue( "sample.percent", 1.0 );

  ShareSamplingParameters();
  ConnectSamplingParameters();
}

void TrainImagesBase::ShareSamplingParameters()
{
  // hide sampling parameters
  // ShareParameter("sample.strategy","rates.strategy");
  // ShareParameter("sample.mim","rates.mim");
  ShareParameter("ram", "polystat.ram");
  ShareParameter("elev", "polystat.elev");
  ShareParameter("sample.vfn", "polystat.field", "Field containing the class integer label for supervision",
                 "Field containing the class id for supervision. "
                 "The values in this field shall be cast into integers.");
}

void TrainImagesBase::ConnectSamplingParameters()
{
  Connect("extraction.field", "polystat.field");
  Connect("extraction.layer", "polystat.layer");

  Connect("select.ram", "polystat.ram");
  Connect("extraction.ram", "polystat.ram");

  Connect("select.field", "polystat.field");
  Connect("select.layer", "polystat.layer");
  Connect("select.elev", "polystat.elev");

  Connect("extraction.in", "select.in");
  Connect("extraction.vec", "select.out");
}

void TrainImagesBase::InitClassification()
{
  AddApplication("TrainVectorClassifier", "training", "Model training");

  AddParameter(ParameterType_InputVectorDataList, "io.valid", "Validation Vector Data List");
  SetParameterDescription("io.valid", "A list of vector data to select the validation samples.");
  MandatoryOff("io.valid");

  ShareClassificationParams();
  ConnectClassificationParams();
};

void TrainImagesBase::ShareClassificationParams()
{
  ShareParameter("io.imstat", "training.io.stats");
  ShareParameter("io.out", "training.io.out");

  ShareParameter("classifier", "training.classifier");
  ShareParameter("rand", "training.rand");

  ShareParameter("io.confmatout", "training.io.confmatout");
}

void TrainImagesBase::ConnectClassificationParams()
{
  Connect("select.rand", "training.rand");
}

void TrainImagesBase::ComputePolygonStatistics(FloatVectorImageListType* imageList, const std::vector<std::string>& vectorFileNames,
                                               const std::vector<std::string>& statisticsFileNames)
{
  unsigned int nbImages = static_cast<unsigned int>(imageList->Size());
  for (unsigned int i = 0; i < nbImages; i++)
  {
    GetInternalApplication("polystat")->SetParameterInputImage("in", imageList->GetNthElement(i));
    GetInternalApplication("polystat")->SetParameterString("vec", vectorFileNames[i]);
    GetInternalApplication("polystat")->SetParameterString("out", statisticsFileNames[i]);
    ExecuteInternal("polystat");
  }
}


TrainImagesBase::SamplingRates TrainImagesBase::ComputeFinalMaximumSamplingRates(bool dedicatedValidation)
{
  SamplingRates rates;
  GetInternalApplication("rates")->SetParameterString("mim", "proportional");
  double vtr = GetParameterFloat("sample.vtr");
  long   mt  = GetParameterInt("sample.mt");
  long   mv  = GetParameterInt("sample.mv");
  // compute final maximum training and final maximum validation
  // By default take all samples (-1 means all samples)
  rates.fmt = -1;
  rates.fmv = -1;
  if (GetParameterInt("sample.bm") == 0)
  {
    if (dedicatedValidation)
    {
      // fmt and fmv will be used separately
      rates.fmt = mt;
      rates.fmv = mv;
      if (mt > -1 && mv <= -1 && vtr < 0.99999)
      {
        rates.fmv = static_cast<long>((double)mt * vtr / (1.0 - vtr));
      }
      if (mt <= -1 && mv > -1 && vtr > 0.00001)
      {
        rates.fmt = static_cast<long>((double)mv * (1.0 - vtr) / vtr);
      }
    }
    else
    {
      // only fmt will be used for both training and validation samples
      // So we try to compute the total number of samples given input
      // parameters mt, mv and vtr.
      if (mt > -1 && vtr < 0.99999)
      {
        rates.fmt = static_cast<long>((double)mt / (1.0 - vtr));
      }
      if (mv > -1 && vtr > 0.00001)
      {
        if (rates.fmt > -1)
        {
          rates.fmt = std::min(rates.fmt, static_cast<long>((double)mv / vtr));
        }
        else
        {
          rates.fmt = static_cast<long>((double)mv / vtr);
        }
      }
    }
  }
  return rates;
}


void TrainImagesBase::ComputeSamplingRate(const std::vector<std::string>& statisticsFileNames, const std::string& ratesFileName, long maximum)
{
  // Sampling rates
  GetInternalApplication("rates")->SetParameterStringList("il", statisticsFileNames);
  GetInternalApplication("rates")->SetParameterString("out", ratesFileName);
  if (GetParameterInt("sample.bm") != 0)
  {
    GetInternalApplication("rates")->SetParameterString("strategy", "smallest");
  }
  else
  {
    if (maximum > -1)
    {
      std::ostringstream oss;
      oss << maximum;
      GetInternalApplication("rates")->SetParameterString("strategy", "constant");
      GetInternalApplication("rates")->SetParameterString("strategy.constant.nb", oss.str());
    }
    else
    {
      GetInternalApplication("rates")->SetParameterString("strategy", "all");
    }
  }
  ExecuteInternal("rates");
}

void TrainImagesBase::TrainModel(FloatVectorImageListType* imageList, const std::vector<std::string>& sampleTrainFileNames,
                                 const std::vector<std::string>& sampleValidationFileNames)
{
  GetInternalApplication("training")->SetParameterStringList("io.vd", sampleTrainFileNames);
  if (!sampleValidationFileNames.empty())
    GetInternalApplication("training")->SetParameterStringList("valid.vd", sampleValidationFileNames);

  UpdateInternalParameters("training");
  // set field names
  FloatVectorImageType::Pointer image   = imageList->GetNthElement(0);
  unsigned int                  nbBands = image->GetNumberOfComponentsPerPixel();
  std::vector<std::string>      selectedNames;
  for (unsigned int i = 0; i < nbBands; i++)
  {
    std::ostringstream oss;
    oss << i;
    selectedNames.push_back("value_" + oss.str());
  }
  GetInternalApplication("training")->SetParameterStringList("feat", selectedNames);
  
  // cfield is not directly connected to sample.vfn (ListView parameter) because the associated vector
  // data is not the same (the fields are not the same).
  GetInternalApplication("training")->SetParameterStringList("cfield", 
      {GetChoiceNames("sample.vfn")[GetSelectedItems("sample.vfn").front()]});
  
  ExecuteInternal("training");
}

void TrainImagesBase::SelectAndExtractSamples(FloatVectorImageType* image, std::string vectorFileName, std::string sampleFileName,
                                              std::string statisticsFileName, std::string ratesFileName, SamplingStrategy strategy, std::string selectedField)
{
  GetInternalApplication("select")->SetParameterInputImage("in", image);
  GetInternalApplication("select")->SetParameterString("out", sampleFileName);

  // Change the selection strategy based on selected sampling strategy
  switch (strategy)
  {
  //    case GEOMETRIC:
  //      GetInternalApplication( "select" )->SetParameterString( "sampler", "random");
  //      GetInternalApplication( "select" )->SetParameterString( "strategy", "percent");
  //      GetInternalApplication( "select" )->SetParameterFloat( "strategy.percent.p",
  //                                                             GetParameterFloat( "sample.percent" ));
  //      break;
  case CLASS:
  default:
    GetInternalApplication("select")->SetParameterString("vec", vectorFileName);
    GetInternalApplication("select")->SetParameterString("instats", statisticsFileName);
    GetInternalApplication("select")->SetParameterString("sampler", "periodic");
    GetInternalApplication("select")->SetParameterInt("sampler.periodic.jitter", 50);
    GetInternalApplication("select")->SetParameterString("strategy", "byclass");
    GetInternalApplication("select")->SetParameterString("strategy.byclass.in", ratesFileName);
    break;
  }

  // select sample positions
  ExecuteInternal("select");

  GetInternalApplication("extraction")->SetParameterString("vec", sampleFileName);
  UpdateInternalParameters("extraction");
  if (!selectedField.empty())
    GetInternalApplication("extraction")->SetParameterString("field", selectedField);

  GetInternalApplication("extraction")->SetParameterString("outfield", "prefix");
  GetInternalApplication("extraction")->SetParameterString("outfield.prefix.name", "value_");

  // extract sample descriptors
  ExecuteInternal("extraction");
}


void TrainImagesBase::SelectAndExtractTrainSamples(const TrainFileNamesHandler& fileNames, FloatVectorImageListType* imageList,
                                                   std::vector<std::string> vectorFileNames, SamplingStrategy strategy, std::string selectedFieldName)
{

  for (unsigned int i = 0; i < imageList->Size(); ++i)
  {
    std::string vectorFileName = vectorFileNames.empty() ? "" : vectorFileNames[i];
    SelectAndExtractSamples(imageList->GetNthElement(i), vectorFileName, fileNames.sampleOutputs[i], fileNames.polyStatTrainOutputs[i],
                            fileNames.ratesTrainOutputs[i], strategy, selectedFieldName);
  }
}


void TrainImagesBase::SelectAndExtractValidationSamples(const TrainFileNamesHandler& fileNames, FloatVectorImageListType* imageList,
                                                        const std::vector<std::string>& validationVectorFileList)
{
  for (unsigned int i = 0; i < imageList->Size(); ++i)
  {
    SelectAndExtractSamples(imageList->GetNthElement(i), validationVectorFileList[i], fileNames.sampleValidOutputs[i], fileNames.polyStatValidOutputs[i],
                            fileNames.ratesValidOutputs[i], Self::CLASS);
  }
}

void TrainImagesBase::SplitTrainingToValidationSamples(const TrainFileNamesHandler& fileNames, FloatVectorImageListType* imageList)
{
  for (unsigned int i = 0; i < imageList->Size(); ++i)
  {
    SplitTrainingAndValidationSamples(imageList->GetNthElement(i), fileNames.sampleOutputs[i], fileNames.sampleTrainOutputs[i], fileNames.sampleValidOutputs[i],
                                      fileNames.ratesTrainOutputs[i]);
  }
}

void TrainImagesBase::SplitTrainingAndValidationSamples(FloatVectorImageType* image, std::string sampleFileName, std::string sampleTrainFileName,
                                                        std::string sampleValidFileName, std::string ratesTrainFileName)

{
  // Split between training and validation
  ogr::DataSource::Pointer source    = ogr::DataSource::New(sampleFileName, ogr::DataSource::Modes::Read);
  ogr::DataSource::Pointer destTrain = ogr::DataSource::New(sampleTrainFileName, ogr::DataSource::Modes::Overwrite);
  ogr::DataSource::Pointer destValid = ogr::DataSource::New(sampleValidFileName, ogr::DataSource::Modes::Overwrite);
  // read sampling rates from ratesTrainOutputs
  SamplingRateCalculator::Pointer rateCalculator = SamplingRateCalculator::New();
  rateCalculator->Read(ratesTrainFileName);
  // Compute sampling rates for train and valid
  const MapRateType&                       inputRates = rateCalculator->GetRatesByClass();
  MapRateType                              trainRates;
  MapRateType                              validRates;
  otb::SamplingRateCalculator::TripletType tpt;
  for (MapRateType::const_iterator it = inputRates.begin(); it != inputRates.end(); ++it)
  {
    double        vtr         = GetParameterFloat("sample.vtr");
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
  splitter->SetFieldName(this->GetParameterStringList("sample.vfn")[0]);
  splitter->SetLayerIndex(0);
  splitter->SetOriginFieldName(std::string(""));
  splitter->SetSamplerParameters(param);
  splitter->GetStreamer()->SetAutomaticTiledStreaming(static_cast<unsigned int>(this->GetParameterInt("ram")));
  AddProcess(splitter->GetStreamer(), "Split samples between training and validation...");
  splitter->Update();
}
}
}

#endif
