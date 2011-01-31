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
#include "itkVariableLengthVector.h"
#include "otbObjectDetectionClassifier.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbImageFunctionAdaptor.h"
#include "otbStatisticsXMLFileReader.h"

const unsigned int Dimension = 2;
typedef int        LabelType;
typedef double     PixelType;
typedef double     FunctionPrecisionType;
typedef double     CoordRepType;

typedef otb::Image<PixelType, Dimension>                              ImageType;
typedef otb::VectorData<>                                             VectorDataType;

typedef otb::ObjectDetectionClassifier
           < ImageType,
             VectorDataType,
             LabelType,
             FunctionPrecisionType,
             CoordRepType > ObjectDetectionClassifierType;

typedef otb::RadiometricMomentsImageFunction<ImageType, CoordRepType>   FunctionType;
typedef otb::ImageFunctionAdaptor<FunctionType, FunctionPrecisionType>  AdapatedFunctionType;

typedef otb::ImageFileReader<ImageType>           ImageReaderType;
typedef otb::VectorDataFileWriter<VectorDataType> VectorDataWriterType;

typedef ObjectDetectionClassifierType::SVMModelType SVMModelType;
typedef ObjectDetectionClassifierType::SVMModelPointerType SVMModelPointerType;

typedef otb::StatisticsXMLFileReader<AdapatedFunctionType::OutputType> StatisticsXMLFileReaderType;

int otbObjectDetectionClassifierNew(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  ObjectDetectionClassifierType::Pointer object = ObjectDetectionClassifierType::New();
  std::cout << object << std::endl;
  return EXIT_SUCCESS;
}


int otbObjectDetectionClassifier(int argc, char* argv[])
{
  if (argc != 7)
    {
    std::cerr << "Wrong number of arguments" << std::endl;
    return EXIT_FAILURE;
    }

  const char* inputImageFileName = argv[1];
  const char* inputFeatureStats = argv[2];
  const char* inputSVMModel = argv[3];
  const char* outputVectorDataFileName = argv[4];
  int streaming = atoi(argv[5]);
  int neighborhood = atoi(argv[6]);

  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(inputImageFileName);

  StatisticsXMLFileReaderType::Pointer statisticsReader = StatisticsXMLFileReaderType::New();
  statisticsReader->SetFileName(inputFeatureStats);


  SVMModelPointerType svmModel = SVMModelType::New();
  svmModel->LoadModel(inputSVMModel);

  AdapatedFunctionType::Pointer descriptorsFunction = AdapatedFunctionType::New();
  descriptorsFunction->SetInputImage(imageReader->GetOutput());
  descriptorsFunction->GetInternalImageFunction()->SetNeighborhoodRadius(neighborhood);

  ObjectDetectionClassifierType::Pointer classifier = ObjectDetectionClassifierType::New();
  classifier->SetInputImage(imageReader->GetOutput());
  classifier->SetNeighborhoodRadius(neighborhood);
  classifier->SetSVMModel(svmModel);
  classifier->SetDescriptorsFunction(descriptorsFunction);
  classifier->SetNoClassLabel(0);
  classifier->SetClassKey("Class");
  classifier->SetShifts(statisticsReader->GetStatisticVectorByName("mean"));
  classifier->SetScales(statisticsReader->GetStatisticVectorByName("stddev"));

  if (streaming == 0)
    {
    classifier->GetStreamer()->SetNumberOfStreamDivisions(1);
    }
  else
    {
    classifier->GetStreamer()->SetNumberOfStreamDivisions(streaming);
    }

  classifier->Update();

  VectorDataWriterType::Pointer writer = VectorDataWriterType::New();
  writer->SetFileName(outputVectorDataFileName);
  writer->SetInput(classifier->GetOutputVectorData());
  writer->Update();

  return EXIT_SUCCESS;
}
