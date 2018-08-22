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
#include "otbLibSVMMachineLearningModel.h"

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
typedef otb::ImageFunctionAdaptor<FunctionType, FunctionPrecisionType>  AdaptedFunctionType;

typedef otb::ImageFileReader<ImageType>           ImageReaderType;

typedef otb::LibSVMMachineLearningModel<PixelType,LabelType> SVMModelType;
typedef SVMModelType::Pointer SVMModelPointerType;

typedef otb::StatisticsXMLFileReader<AdaptedFunctionType::OutputType> StatisticsXMLFileReaderType;


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
  svmModel->Load(inputSVMModel);

  AdaptedFunctionType::Pointer descriptorsFunction = AdaptedFunctionType::New();
  descriptorsFunction->SetInputImage(imageReader->GetOutput());
  descriptorsFunction->GetInternalImageFunction()->SetNeighborhoodRadius(neighborhood);

  ObjectDetectionClassifierType::Pointer classifier = ObjectDetectionClassifierType::New();
  classifier->SetInputImage(imageReader->GetOutput());
  classifier->SetNeighborhoodRadius(neighborhood);
  classifier->SetModel(svmModel);
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
