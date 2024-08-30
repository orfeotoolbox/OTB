/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbTrainImagesBase_h
#define otbTrainImagesBase_h

#include "otbVectorDataFileWriter.h"
#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStatisticsXMLFileWriter.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbSamplingRateCalculator.h"
#include "otbOGRDataToSamplePositionFilter.h"
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class TrainImagesBase
 * \brief Base class for the TrainImagesClassifier
 *
 * This class intends to hold common input/output parameters and
 * composite application connection for both supervised and unsupervised
 * model training.
 *
 * \ingroup OTBAppLearning
 */
class TrainImagesBase : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef TrainImagesBase               Self;
  typedef CompositeApplication          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(TrainImagesBase, Superclass);

  /** filters typedefs*/
  typedef otb::OGRDataToSamplePositionFilter<FloatVectorImageType, UInt8ImageType, otb::PeriodicSampler> PeriodicSamplerType;

  typedef otb::SamplingRateCalculator::MapRateType MapRateType;

protected:
  typedef enum { CLASS, GEOMETRIC } SamplingStrategy;
  struct SamplingRates;
  class TrainFileNamesHandler;

  /**
   * Initialize all the input and output parameter used for the train images
   */
  void InitIO();

  /**
   * Initialize sampling related application and parameters
   */
  void InitSampling();

  void ShareSamplingParameters();
  void ConnectSamplingParameters();
  void InitClassification();
  void ShareClassificationParams();
  void ConnectClassificationParams();

  /**
   * Compute polygon statistics given provided strategy with PolygonClassStatistics class
   * \param imageList list of input images
   * \param vectorFileNames list of input vector file names
   * \param statisticsFileNames list of out
   */
  void ComputePolygonStatistics(FloatVectorImageListType* imageList, const std::vector<std::string>& vectorFileNames,
                                const std::vector<std::string>& statisticsFileNames);

  /**
   * Compute final maximum training and validation
   * \param dedicatedValidation
   * \return SamplingRates final maximum training and final maximum validation
   */
  SamplingRates ComputeFinalMaximumSamplingRates(bool dedicatedValidation);


  /**
   * Compute rates using MultiImageSamplingRate application
   * \param statisticsFileNames
   * \param ratesFileName
   * \param maximum final maximum value computed by ComputeFinalMaximumSamplingRates
   * \sa ComputeFinalMaximumSamplingRates
   */
  void ComputeSamplingRate(const std::vector<std::string>& statisticsFileNames, const std::string& ratesFileName, long maximum);
  /**
   * Train the model with training and optional validation data samples
   * \param imageList list of input images
   * \param sampleTrainFileNames files names of the training samples
   * \param sampleValidationFileNames file names of the validation sample
   */
  void TrainModel(FloatVectorImageListType* imageList, const std::vector<std::string>& sampleTrainFileNames,
                  const std::vector<std::string>& sampleValidationFileNames);

  /**
   * Select samples by class or by geographic strategy
   * \param image
   * \param vectorFileName
   * \param sampleFileName
   * \param statisticsFileName
   * \param ratesFileName
   * \param strategy
   */
  void SelectAndExtractSamples(FloatVectorImageType* image, std::string vectorFileName, std::string sampleFileName, std::string statisticsFileName,
                               std::string ratesFileName, SamplingStrategy strategy, std::string selectedField = "");
  /**
   * Select and extract samples with the SampleSelection and SampleExtraction application.
   * \param fileNames
   * \param imageList
   * \param vectorFileNames
   * \param strategy the strategy used for selection (by class or with geometry)
   * \param selectedFieldName
   */
  void SelectAndExtractTrainSamples(const TrainFileNamesHandler& fileNames, FloatVectorImageListType* imageList, std::vector<std::string> vectorFileNames,
                                    SamplingStrategy strategy, std::string selectedFieldName = "");


  /**
   * Function used to select validation samples based on a defined strategy (geometric in unsupervised mode)
   * and extract them. With dedicated validation the 'by class' sampling strategy and statistics are used.
   * Otherwise this function split training to validation samples corresponding to sample.vtr percentage.
   * or do nothing if this percentage is == 0
   * \param fileNames
   * \param imageList
   * \param validationVectorFileList optional validation vector file for each images
   */
  void SelectAndExtractValidationSamples(const TrainFileNamesHandler& fileNames, FloatVectorImageListType* imageList,
                                         const std::vector<std::string>& validationVectorFileList = std::vector<std::string>());

  /**
   * Function used to split all training samples from all images in a set of training and validation.
   * \param fileNames
   * \param imageList
   * \sa SplitTrainingAndValidationSamples
   */
  void SplitTrainingToValidationSamples(const TrainFileNamesHandler& fileNames, FloatVectorImageListType* imageList);

private:
  /**
   * Function used to split training samples in set of training and validation.
   * \param image input image
   * \param sampleFileName the input sample file name
   * \param sampleTrainFileName the input training file name
   * \param sampleValidFileName the input validation file name
   * \param ratesTrainFileName the rates file name
   */
  void SplitTrainingAndValidationSamples(FloatVectorImageType* image, std::string sampleFileName, std::string sampleTrainFileName,
                                         std::string sampleValidFileName, std::string ratesTrainFileName);


protected:
  struct SamplingRates
  {
    long int fmt;
    long int fmv;
  };

  /**
   * \class TrainFileNamesHandler
   * This class is used to store file names requires for the application's input and output.
   * And to clear temporary files generated by the applications
   * \ingroup OTBAppLearning
   */
  class TrainFileNamesHandler
  {
  public:
    void CreateTemporaryFileNames(std::string outModel, size_t nbInputs, bool dedicatedValidation)
    {

      if (dedicatedValidation)
      {
        rateTrainOut = outModel + "_ratesTrain.csv";
      }
      else
      {
        rateTrainOut = outModel + "_rates.csv";
      }

      rateValidOut = outModel + "_ratesValid.csv";
      for (unsigned int i = 0; i < nbInputs; i++)
      {
        std::ostringstream oss;
        oss << i + 1;
        std::string strIndex(oss.str());
        if (dedicatedValidation)
        {
          polyStatTrainOutputs.push_back(outModel + "_statsTrain_" + strIndex + ".xml");
          polyStatValidOutputs.push_back(outModel + "_statsValid_" + strIndex + ".xml");
          ratesTrainOutputs.push_back(outModel + "_ratesTrain_" + strIndex + ".csv");
          ratesValidOutputs.push_back(outModel + "_ratesValid_" + strIndex + ".csv");
          sampleOutputs.push_back(outModel + "_samplesTrain_" + strIndex + ".shp");
        }
        else
        {
          polyStatTrainOutputs.push_back(outModel + "_stats_" + strIndex + ".xml");
          ratesTrainOutputs.push_back(outModel + "_rates_" + strIndex + ".csv");
          sampleOutputs.push_back(outModel + "_samples_" + strIndex + ".shp");
        }
        sampleTrainOutputs.push_back(outModel + "_samplesTrain_" + strIndex + ".shp");
        sampleValidOutputs.push_back(outModel + "_samplesValid_" + strIndex + ".shp");
      }
    }

    void clear()
    {
      for (unsigned int i = 0; i < polyStatTrainOutputs.size(); i++)
        RemoveFile(polyStatTrainOutputs[i]);
      for (unsigned int i = 0; i < polyStatValidOutputs.size(); i++)
        RemoveFile(polyStatValidOutputs[i]);
      for (unsigned int i = 0; i < ratesTrainOutputs.size(); i++)
        RemoveFile(ratesTrainOutputs[i]);
      for (unsigned int i = 0; i < ratesValidOutputs.size(); i++)
        RemoveFile(ratesValidOutputs[i]);
      for (unsigned int i = 0; i < sampleOutputs.size(); i++)
        RemoveFile(sampleOutputs[i]);
      for (unsigned int i = 0; i < sampleTrainOutputs.size(); i++)
        RemoveFile(sampleTrainOutputs[i]);
      for (unsigned int i = 0; i < sampleValidOutputs.size(); i++)
        RemoveFile(sampleValidOutputs[i]);
      for (unsigned int i = 0; i < tmpVectorFileList.size(); i++)
        RemoveFile(tmpVectorFileList[i]);
    }

  public:
    std::vector<std::string> polyStatTrainOutputs;
    std::vector<std::string> polyStatValidOutputs;
    std::vector<std::string> ratesTrainOutputs;
    std::vector<std::string> ratesValidOutputs;
    std::vector<std::string> sampleOutputs;
    std::vector<std::string> sampleTrainOutputs;
    std::vector<std::string> sampleValidOutputs;
    std::vector<std::string> tmpVectorFileList;
    std::string              rateValidOut;
    std::string              rateTrainOut;

  private:
    bool RemoveFile(std::string& filePath)
    {
      bool res = true;
      if (itksys::SystemTools::FileExists(filePath))
      {
        size_t posExt = filePath.rfind('.');
        if (posExt != std::string::npos && filePath.compare(posExt, std::string::npos, ".shp") == 0)
        {
          std::string shxPath = filePath.substr(0, posExt) + std::string(".shx");
          std::string dbfPath = filePath.substr(0, posExt) + std::string(".dbf");
          std::string prjPath = filePath.substr(0, posExt) + std::string(".prj");
          RemoveFile(shxPath);
          RemoveFile(dbfPath);
          RemoveFile(prjPath);
        }
        res = itksys::SystemTools::RemoveFile(filePath);
        if (!res)
        {
          // otbAppLogINFO( <<"Unable to remove file  "<<filePath );
        }
      }
      return res;
    }
  };
};

} // end namespace Wrapper
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTrainImagesBase.hxx"
#endif

#endif // otbTrainImagesBase_h
