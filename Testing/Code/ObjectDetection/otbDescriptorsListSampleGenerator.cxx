/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iterator>

#include "otbImage.h"
#include "otbVectorData.h"
#include "otbRadiometricMomentsImageFunction.h"
#include "itkListSample.h"
#include "itkFixedArray.h"
#include "otbDescriptorsListSampleGenerator.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"

const unsigned int Dimension = 2;
typedef int        LabelType;
typedef double     PixelType;
typedef double     FunctionPrecisionType;
typedef double     CoordRepType;

typedef otb::Image<PixelType, Dimension>                              ImageType;
typedef otb::VectorData<>                                             VectorDataType;
typedef otb::RadiometricMomentsImageFunction<ImageType, CoordRepType> FunctionType;

typedef FunctionType::OutputType                        SampleType;
typedef itk::Statistics::ListSample<SampleType>         ListSampleType;
typedef itk::FixedArray<LabelType, 1>                   LabelSampleType;
typedef itk::Statistics::ListSample<LabelSampleType>    LabelListSampleType;

typedef otb::DescriptorsListSampleGenerator
           < ImageType,
             VectorDataType,
             ListSampleType,
             LabelListSampleType,
             FunctionPrecisionType,
             CoordRepType > DescriptorsListSampleGeneratorType;

typedef otb::ImageFileReader<ImageType>           ImageReaderType;
typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;

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

ostream &operator<<(ostream &stream, SampleEntry entry)
{
  stream << "---" << std::endl
         << "Label        : " << entry.label << std::endl
         << "Position     : " << entry.position << std::endl
         << "Measurements : " << entry.measurement;
  return stream;
}

int otbDescriptorsListSampleGeneratorNew(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  // instantiation
  DescriptorsListSampleGeneratorType::Pointer generator = DescriptorsListSampleGeneratorType::New();

  std::cout << generator << std::endl;

  return EXIT_SUCCESS;
}

int otbDescriptorsListSampleGenerator(int argc, char* argv[])
{
  if (argc != 5)
    {
    std::cerr << "Wrong number of arguments" << std::endl;
    return EXIT_FAILURE;
    }

  const char* inputImageFileName = argv[1];
  const char* inputSamplesLocation = argv[2];
  const char* outputFileName = argv[3];
  int streaming = atoi(argv[4]);

  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(inputImageFileName);

  VectorDataReaderType::Pointer vectorDataReader = VectorDataReaderType::New();
  vectorDataReader->SetFileName(inputSamplesLocation);

  //imageReader->Update();
  //vectorDataReader->Update();

  FunctionType::Pointer descriptorsFunction = FunctionType::New();
  descriptorsFunction->SetInputImage(imageReader->GetOutput());
  descriptorsFunction->SetNeighborhoodRadius(5);

  DescriptorsListSampleGeneratorType::Pointer descriptorsGenerator = DescriptorsListSampleGeneratorType::New();
  descriptorsGenerator->SetInputImage(imageReader->GetOutput());
  descriptorsGenerator->SetSamplesLocations(vectorDataReader->GetOutput());
  descriptorsGenerator->SetDescriptorsFunction(descriptorsFunction.GetPointer());

  if (streaming == 0)
    {
    descriptorsGenerator->GetStreamer()->SetNumberOfStreamDivisions(1);
    }
  else
    {
    descriptorsGenerator->GetStreamer()->SetNumberOfStreamDivisions(streaming);
    }

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
