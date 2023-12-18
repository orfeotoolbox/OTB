/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

    AddParameter(ParameterType_Group, "out", "Optional outputs");

    AddParameter(ParameterType_OutputFilename, "out.xml", "Output XML file");
    SetParameterDescription("out.xml", "XML filename where the statistics are saved for future reuse.");
    MandatoryOff("out.xml");

    AddParameter(ParameterType_String, "out.mean", "Mean pixel Value");
    SetParameterDescription("out.mean", "Mean pixel value.");
    SetParameterRole("out.mean", Role_Output);
    MandatoryOff("out.mean");

    AddParameter(ParameterType_String, "out.min", "Min pixel Value");
    SetParameterDescription("out.min", "Minimum pixel value.");
    SetParameterRole("out.min", Role_Output);
    MandatoryOff("out.min");

    AddParameter(ParameterType_String, "out.max", "Max pixel Value");
    SetParameterDescription("out.max", "Maximum pixel value.");
    SetParameterRole("out.max", Role_Output);
    MandatoryOff("out.max");

    AddParameter(ParameterType_String, "out.std", "Standard deviation of pixel Value");
    SetParameterDescription("out.std", "Standard deviation of pixel value.");
    SetParameterRole("out.std", Role_Output);
    MandatoryOff("out.std");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "QB_1_ortho.tif");
    SetDocExampleParameterValue("out.xml", "EstimateImageStatisticsQB1.xml");

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

    // Build a Measurement Vector of min
    MatrixValueType min(nbBands, static_cast<unsigned int>(nbImages));
    min.Fill(itk::NumericTraits<MatrixValueType::ValueType>::max());

    // Build a Measurement Vector of max
    MatrixValueType max(nbBands, static_cast<unsigned int>(nbImages));
    max.Fill(itk::NumericTraits<MatrixValueType::ValueType>::min());

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
      MeasurementType minPerBand       = statsEstimator->GetMinimum();
      MeasurementType maxPerBand       = statsEstimator->GetMaximum();

      for (unsigned int itBand = 0; itBand < nbBands; itBand++)
      {
        mean(itBand, imageId)      = meanPerBand[itBand];
        min(itBand, imageId)       = minPerBand[itBand];
        max(itBand, imageId)       = maxPerBand[itBand];
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

    MeasurementType totalMinPerBand;
    totalMinPerBand.SetSize(nbBands);
    totalMinPerBand.Fill(itk::NumericTraits<MeasurementType::ValueType>::max());

    MeasurementType totalMaxPerBand;
    totalMaxPerBand.SetSize(nbBands);
    totalMaxPerBand.Fill(itk::NumericTraits<MeasurementType::ValueType>::min());

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
        totalMinPerBand[itBand] = std::min(totalMinPerBand[itBand], min(itBand, imageId));
        totalMaxPerBand[itBand] = std::max(totalMaxPerBand[itBand], max(itBand, imageId));
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
        totalMinPerBand[itBand] = itk::NumericTraits<ValueType>::Zero;
        totalMaxPerBand[itBand] = itk::NumericTraits<ValueType>::Zero;
      }
    }

    MeasurementType stddev;
    stddev.SetSize(nbBands);
    stddev.Fill(itk::NumericTraits<MeasurementType::ValueType>::Zero);
    for (unsigned int i = 0; i < totalVariancePerBand.GetSize(); ++i)
    {
      stddev[i] = std::sqrt(totalVariancePerBand[i]);
    }

    // Display the pixel value
    std::ostringstream oss_mean, oss_min, oss_max, oss_std;
    oss_mean << totalMeanPerBand;
    oss_min << totalMinPerBand;
    oss_max << totalMaxPerBand;
    oss_std << stddev;
    SetParameterString("out.mean", oss_mean.str());
    SetParameterString("out.min", oss_min.str());
    SetParameterString("out.max", oss_max.str());
    SetParameterString("out.std", oss_std.str());

    if (HasValue("out.xml"))
    {
      // Write the Statistics via the statistic writer
      typedef otb::StatisticsXMLFileWriter<MeasurementType> StatisticsWriter;
      StatisticsWriter::Pointer                             writer = StatisticsWriter::New();
      writer->SetFileName(GetParameterString("out.xml"));
      writer->AddInput("mean", totalMeanPerBand);
      writer->AddInput("min", totalMinPerBand);
      writer->AddInput("max", totalMaxPerBand);
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
