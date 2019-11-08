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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStatisticsXMLFileWriter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include <sstream>

namespace otb
{
namespace Wrapper
{

class ComputeImagesStatistics : public Application
{
public:
  /** Standard class typedefs. */
  typedef ComputeImagesStatistics       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputeImagesStatistics, otb::Application);

private:
  void DoInit() override
  {
    SetName("ComputeImagesStatistics");
    SetDescription(
        "Computes global mean and standard deviation for each band "
        "from a set of images and optionally saves the results in an XML file.");
    SetDocLongDescription(
        "This application computes a global mean and standard deviation "
        "for each band of a set of images and optionally saves the results in an XML file."
        " The output XML is intended to be used as an input "
        "for the TrainImagesClassifier application to normalize samples before learning. "
        "You can also normalize the image with the XML file in the ImageClassifier application.");

    SetDocLimitations(
        "Each image of the set must contain the same bands as the others"
        " (i.e. same types, in the same order).");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Documentation of the TrainImagesClassifier and ImageClassifier application.");

    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);

    AddParameter(ParameterType_InputImageList, "il", "Input images");
    SetParameterDescription("il", "List of input image filenames.");

    AddParameter(ParameterType_Float, "bv", "Background Value");
    SetParameterDescription("bv", "Background value to ignore in computation of statistics.");
    MandatoryOff("bv");

    AddParameter(ParameterType_OutputFilename, "out", "Output XML file");
    SetParameterDescription("out", "XML filename where the statistics are saved for future reuse.");
    MandatoryOff("out");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "QB_1_ortho.tif");
    SetDocExampleParameterValue("out", "EstimateImageStatisticsQB1.xml");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    // Statistics estimator
    typedef otb::StreamingStatisticsVectorImageFilter<FloatVectorImageType> StreamingStatisticsVImageFilterType;

    // Samples
    typedef double                               ValueType;
    typedef itk::VariableLengthVector<ValueType> MeasurementType;
    typedef itk::VariableSizeMatrix<ValueType>   MatrixValueType;

    unsigned int nbBands = 0;

    FloatVectorImageListType*                           imageList = GetParameterImageList("il");
    FloatVectorImageListType::InternalContainerSizeType nbImages  = imageList->Size();

    // Initialization, all image have same size and number of band/component
    FloatVectorImageType* firstImage = imageList->GetNthElement(0);
    nbBands                          = firstImage->GetNumberOfComponentsPerPixel();

    // Build a Measurement Vector of mean
    MatrixValueType mean(nbBands, static_cast<unsigned int>(nbImages));
    mean.Fill(itk::NumericTraits<MatrixValueType::ValueType>::Zero);

    // Build a Measurement Matrix of variance
    MatrixValueType variance(nbBands, static_cast<unsigned int>(nbImages));
    variance.Fill(itk::NumericTraits<MatrixValueType::ValueType>::Zero);

    // Build a Measurement Matrix of nbSamples
    MatrixValueType nbSamples(nbBands, static_cast<unsigned int>(nbImages));
    nbSamples.Fill(itk::NumericTraits<MatrixValueType::ValueType>::Zero);

    // Iterate over all input images
    for (unsigned int imageId = 0; imageId < nbImages; ++imageId)
    {
      FloatVectorImageType* image = imageList->GetNthElement(imageId);
      if (nbBands != image->GetNumberOfComponentsPerPixel())
      {
        itkExceptionMacro(<< "The image #" << imageId + 1 << " has " << image->GetNumberOfComponentsPerPixel() << " bands, while the image #1 has " << nbBands);
      }

      // Compute Statistics of each VectorImage
      StreamingStatisticsVImageFilterType::Pointer statsEstimator = StreamingStatisticsVImageFilterType::New();
      std::ostringstream                           processName;
      processName << "Processing Image (" << imageId + 1 << "/" << imageList->Size() << ")";
      AddProcess(statsEstimator->GetStreamer(), processName.str());
      statsEstimator->SetInput(image);
      statsEstimator->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));

      if (HasValue("bv"))
      {
        statsEstimator->SetIgnoreUserDefinedValue(true);
        statsEstimator->SetUserIgnoredValue(GetParameterFloat("bv"));
      }
      statsEstimator->Update();

      MeasurementType nbRelevantPixels = statsEstimator->GetNbRelevantPixels();
      MeasurementType meanPerBand      = statsEstimator->GetMean();

      for (unsigned int itBand = 0; itBand < nbBands; itBand++)
      {
        mean(itBand, imageId)      = meanPerBand[itBand];
        variance(itBand, imageId)  = (statsEstimator->GetCovariance())(itBand, itBand);
        nbSamples(itBand, imageId) = nbRelevantPixels[itBand];
      }
    }

    // Compute total mean and pooled variation for each band of the image list
    MeasurementType totalSamplesPerBand;
    totalSamplesPerBand.SetSize(nbBands);
    totalSamplesPerBand.Fill(itk::NumericTraits<MeasurementType::ValueType>::Zero);

    MeasurementType totalMeanPerBand;
    totalMeanPerBand.SetSize(nbBands);
    totalMeanPerBand.Fill(itk::NumericTraits<MeasurementType::ValueType>::Zero);

    MeasurementType totalVariancePerBand;
    totalVariancePerBand.SetSize(nbBands);
    totalVariancePerBand.Fill(itk::NumericTraits<MeasurementType::ValueType>::Zero);

    for (unsigned int imageId = 0; imageId < nbImages; ++imageId)
    {
      for (unsigned int itBand = 0; itBand < nbBands; itBand++)
      {
        MeasurementType::ValueType nbSample = nbSamples(itBand, imageId);
        totalSamplesPerBand[itBand] += nbSample;
        totalMeanPerBand[itBand] += mean(itBand, imageId) * nbSample;
        totalVariancePerBand[itBand] += variance(itBand, imageId) * (nbSample - 1);
      }
    }

    // Check 0 division
    for (unsigned int itBand = 0; itBand < nbBands; itBand++)
    {
      MeasurementType::ValueType nbSample = totalSamplesPerBand[itBand];

      if (nbSample > nbImages)
      {
        totalVariancePerBand[itBand] /= (nbSample - nbImages);
      }
      else
      {
        totalVariancePerBand[itBand] = itk::NumericTraits<ValueType>::Zero;
      }

      if (nbSample != 0)
      {
        totalMeanPerBand[itBand] /= nbSample;
      }
      else
      {
        totalMeanPerBand[itBand] = itk::NumericTraits<ValueType>::Zero;
      }
    }

    MeasurementType stddev;
    stddev.SetSize(nbBands);
    stddev.Fill(itk::NumericTraits<MeasurementType::ValueType>::Zero);
    for (unsigned int i = 0; i < totalVariancePerBand.GetSize(); ++i)
    {
      stddev[i] = std::sqrt(totalVariancePerBand[i]);
    }

    if (HasValue("out"))
    {
      // Write the Statistics via the statistic writer
      typedef otb::StatisticsXMLFileWriter<MeasurementType> StatisticsWriter;
      StatisticsWriter::Pointer                             writer = StatisticsWriter::New();
      writer->SetFileName(GetParameterString("out"));
      writer->AddInput("mean", totalMeanPerBand);
      writer->AddInput("stddev", stddev);
      writer->Update();
    }
    else
    {
      otbAppLogINFO("Mean: " << mean << std::endl);
      otbAppLogINFO("Standard Deviation: " << stddev << std::endl);
    }
  }

  itk::LightObject::Pointer m_FilterRef;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeImagesStatistics)
