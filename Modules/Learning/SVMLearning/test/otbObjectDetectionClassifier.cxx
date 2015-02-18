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


#include <iostream>
#include <iterator>

#include "otbImage.h"
#include "otbRadiometricMomentsImageFunction.h"
#include "itkListSample.h"
#include "itkVariableLengthVector.h"
#include "otbObjectDetectionClassifier.h"
#include "otbImageFileReader.h"
#include "otbImageFunctionAdaptor.h"
#include "otbStatisticsXMLFileReader.h"
#include "itkPreOrderTreeIterator.h"

const unsigned int Dimension = 2;
typedef int        LabelType;
typedef double     PixelType;
typedef double     FunctionPrecisionType;
typedef double     CoordRepType;

typedef otb::Image<PixelType, Dimension>                              ImageType;
typedef otb::VectorData<>                                             VectorDataType;
typedef VectorDataType::PointType                                     PointType;
typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType>       TreeIteratorType;

typedef otb::ObjectDetectionClassifier
           < ImageType,
             VectorDataType,
             LabelType,
             FunctionPrecisionType,
             CoordRepType > ObjectDetectionClassifierType;

typedef otb::RadiometricMomentsImageFunction<ImageType, CoordRepType>   FunctionType;
typedef otb::ImageFunctionAdaptor<FunctionType, FunctionPrecisionType>  AdapatedFunctionType;

typedef otb::ImageFileReader<ImageType>           ImageReaderType;

typedef ObjectDetectionClassifierType::SVMModelType SVMModelType;
typedef ObjectDetectionClassifierType::SVMModelPointerType SVMModelPointerType;

typedef otb::StatisticsXMLFileReader<AdapatedFunctionType::OutputType> StatisticsXMLFileReaderType;


struct ComparePoint
{
  bool operator () (PointType p, PointType q)
  {
    // order with the y axis position
    if (p[1] < q[1])
      return true;
    if (p[1] > q[1])
      return false;

    // If one the same line,
    // order with the x axis position
    if (p[0] < q[0])
      return true;

    return false;
  }
};

std::ostream &operator<<(std::ostream &stream, PointType p)
{
  stream << p[0] << " " << p[1];
  return stream;
}

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
  classifier->GetStreamer()->SetNumberOfLinesStrippedStreaming( streaming );
  classifier->SetGridStep(neighborhood/2);
  classifier->Update();

  std::vector<ObjectDetectionClassifierType::PointType> points;
  VectorDataType::Pointer vectorData = classifier->GetOutputVectorData();

  TreeIteratorType itVector(vectorData->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPointFeature())
      {
      points.push_back(itVector.Get()->GetPoint());
      }
    ++itVector;
    }

  std::sort(points.begin(), points.end(), ComparePoint());
  std::ofstream file(outputVectorDataFileName);
  std::copy(points.begin(), points.end(), std::ostream_iterator<PointType>(file, "\n"));
  file.close();


  return EXIT_SUCCESS;
}
