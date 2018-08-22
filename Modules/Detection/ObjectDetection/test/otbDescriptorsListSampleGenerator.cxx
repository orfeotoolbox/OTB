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
#include <iterator>

#include "otbImage.h"
#include "otbRadiometricMomentsImageFunction.h"
#include "otbDescriptorsListSampleGenerator.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"
#include "otbImageFunctionAdaptor.h"

#include "otbStatisticsXMLFileReader.h"
#include "otbShiftScaleSampleListFilter.h"
#include "otbLibSVMMachineLearningModel.h"

const unsigned int Dimension = 2;
typedef int        LabelType;
typedef double     PixelType;
typedef double     FunctionPrecisionType;
typedef double     CoordRepType;

typedef otb::Image<PixelType, Dimension>                               ImageType;
typedef otb::VectorData<>                                              VectorDataType;
typedef otb::RadiometricMomentsImageFunction<ImageType, CoordRepType>  FunctionType;
typedef otb::ImageFunctionAdaptor<FunctionType, FunctionPrecisionType> AdaptedFunctionType;

typedef itk::VariableLengthVector<FunctionPrecisionType> SampleType;
typedef itk::Statistics::ListSample<SampleType>          ListSampleType;
typedef itk::FixedArray<LabelType, 1>                    LabelSampleType;
typedef itk::Statistics::ListSample<LabelSampleType>     LabelListSampleType;

typedef otb::DescriptorsListSampleGenerator
           < ImageType,
             VectorDataType,
             ListSampleType,
             LabelType,
             FunctionPrecisionType,
             CoordRepType > DescriptorsListSampleGeneratorType;

typedef otb::ImageFileReader<ImageType>           ImageReaderType;
typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;

typedef otb::StatisticsXMLFileReader<SampleType>   StatisticsReader;
typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType> ShiftScaleListSampleFilterType;

typedef otb::LibSVMMachineLearningModel<FunctionPrecisionType, LabelType> SVMType;

typedef FunctionType::PointType PointType;
typedef DescriptorsListSampleGeneratorType::SamplesPositionType SamplesPositionType;

struct SampleEntry
{
  PointType  position;
  LabelType  label;
  SampleType measurement;
};

struct CompareSampleEntry
{
  bool operator () (SampleEntry p, SampleEntry q)
  {
    // order with the y axis position
    if (p.position[1] < q.position[1])
      return true;
    if (p.position[1] > q.position[1])
      return false;

    // If one the same line,
    // order with the x axis position
    if (p.position[0] < q.position[0])
      return true;

    return false;
  }
};

std::ostream &operator<<(std::ostream &stream, SampleEntry entry)
{
  stream << "---" << std::endl
         << "Label        : " << entry.label << std::endl
         << "Position     : " << entry.position << std::endl
         << "Measurements : " << entry.measurement;
  return stream;
}


int otbDescriptorsListSampleGenerator(int argc, char* argv[])
{
  if (argc != 6)
    {
    std::cerr << "Wrong number of arguments" << std::endl;
    return EXIT_FAILURE;
    }

  const char* inputImageFileName = argv[1];
  const char* inputSamplesLocation = argv[2];
  const char* outputFileName = argv[3];
  int streaming = atoi(argv[4]);
  int neighborhood = atoi(argv[5]);

  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(inputImageFileName);

  VectorDataReaderType::Pointer vectorDataReader = VectorDataReaderType::New();
  vectorDataReader->SetFileName(inputSamplesLocation);

  //imageReader->Update();
  //vectorDataReader->Update();

  AdaptedFunctionType::Pointer descriptorsFunction = AdaptedFunctionType::New();
  descriptorsFunction->SetInputImage(imageReader->GetOutput());
  descriptorsFunction->GetInternalImageFunction()->SetNeighborhoodRadius(5);

  DescriptorsListSampleGeneratorType::Pointer descriptorsGenerator = DescriptorsListSampleGeneratorType::New();
  descriptorsGenerator->SetInputImage(imageReader->GetOutput());
  descriptorsGenerator->SetSamplesLocations(vectorDataReader->GetOutput());
  descriptorsGenerator->SetDescriptorsFunction(descriptorsFunction.GetPointer());
  descriptorsGenerator->SetNeighborhoodRadius(neighborhood);
  descriptorsGenerator->GetStreamer()->SetNumberOfLinesStrippedStreaming( streaming );
  descriptorsGenerator->Update();

  ListSampleType::Pointer      samples = descriptorsGenerator->GetListSample();
  LabelListSampleType::Pointer labels  = descriptorsGenerator->GetLabelListSample();
  SamplesPositionType&         positions = descriptorsGenerator->GetSamplesPositions();

  ListSampleType::Iterator sampleIt = samples->Begin();
  LabelListSampleType::Iterator labelIt = labels->Begin();
  SamplesPositionType::const_iterator posIt = positions.begin();

  ListSampleType::Iterator sampleEnd = samples->End();
  LabelListSampleType::Iterator labelEnd = labels->End();
  SamplesPositionType::const_iterator posEnd = positions.end();

  std::vector<SampleEntry> entries;

  while (sampleIt != sampleEnd && labelIt != labelEnd && posIt != posEnd)
    {
    SampleEntry entry;
    entry.position = *posIt;
    entry.label = labelIt.GetMeasurementVector()[0];
    entry.measurement = sampleIt.GetMeasurementVector();

    entries.push_back(entry);

    ++sampleIt;
    ++labelIt;
    ++posIt;
    }

  std::sort(entries.begin(), entries.end(), CompareSampleEntry());
  std::ofstream file(outputFileName);
  std::copy(entries.begin(), entries.end(), std::ostream_iterator<SampleEntry>(file, "\n"));
  file.close();

  return EXIT_SUCCESS;
}



int otbDescriptorsSVMModelCreation(int argc, char* argv[])
{
  if (argc != 7)
    {
    std::cerr << "Wrong number of arguments" << std::endl;
    return EXIT_FAILURE;
    }

  const char* inputImageFileName = argv[1];
  const char* inputSamplesLocation = argv[2];
  const char* featureStatisticsFileName = argv[3];
  const char* outputFileName = argv[4];
  int streaming = atoi(argv[5]);
  int neighborhood = atoi(argv[6]);

  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(inputImageFileName);

  VectorDataReaderType::Pointer vectorDataReader = VectorDataReaderType::New();
  vectorDataReader->SetFileName(inputSamplesLocation);

  //imageReader->Update();
  //vectorDataReader->Update();

  AdaptedFunctionType::Pointer descriptorsFunction = AdaptedFunctionType::New();
  descriptorsFunction->SetInputImage(imageReader->GetOutput());
  descriptorsFunction->GetInternalImageFunction()->SetNeighborhoodRadius(neighborhood);

  DescriptorsListSampleGeneratorType::Pointer descriptorsGenerator = DescriptorsListSampleGeneratorType::New();
  descriptorsGenerator->SetInputImage(imageReader->GetOutput());
  descriptorsGenerator->SetSamplesLocations(vectorDataReader->GetOutput());
  descriptorsGenerator->SetDescriptorsFunction(descriptorsFunction.GetPointer());
  descriptorsGenerator->SetNeighborhoodRadius(5);
  descriptorsGenerator->GetStreamer()->SetNumberOfLinesStrippedStreaming( streaming );
  descriptorsGenerator->Update();

  // Normalize the samples
  // Read the mean and variance form the XML file
  StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
  statisticsReader->SetFileName(featureStatisticsFileName);
  SampleType meanMeasurentVector     = statisticsReader->GetStatisticVectorByName("mean");
  SampleType varianceMeasurentVector = statisticsReader->GetStatisticVectorByName("stddev");

  // Shift scale the samples
  ShiftScaleListSampleFilterType::Pointer shiftscaleFilter = ShiftScaleListSampleFilterType::New();
  shiftscaleFilter->SetInput(descriptorsGenerator->GetListSample());
  shiftscaleFilter->SetShifts(meanMeasurentVector);
  shiftscaleFilter->SetScales(varianceMeasurentVector);
  shiftscaleFilter->Update();

  SVMType::Pointer svmEstimator = SVMType::New();
  svmEstimator->SetInputListSample(shiftscaleFilter->GetOutput());
  svmEstimator->SetTargetListSample(descriptorsGenerator->GetLabelListSample());
  svmEstimator->Train();
  svmEstimator->Save(outputFileName);

  return EXIT_SUCCESS;
}
