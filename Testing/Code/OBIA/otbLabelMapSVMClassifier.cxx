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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include <fstream>
#include <iostream>

#include "otbVectorImage.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "otbLabelMapWithClassLabelToLabeledSampleListFilter.h"
#include "otbSVMSampleListModelEstimator.h"
#include "otbLabelMapSVMClassifier.h"
#include "otbLabelMapWithClassLabelToClassLabelImageFilter.h"

const unsigned int Dimension = 2;
typedef unsigned short LabelType;
typedef double         PixelType;

typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, Dimension, double, LabelType> LabelObjectType;
typedef itk::LabelMap<LabelObjectType>                                          LabelMapType;
typedef otb::VectorImage<PixelType, Dimension>                                  VectorImageType;
typedef otb::Image<unsigned int, 2>                                              LabeledImageType;

typedef otb::ImageFileReader<VectorImageType>                                   ReaderType;
typedef otb::ImageFileReader<LabeledImageType>                                  LabeledReaderType;
typedef otb::ImageFileWriter<VectorImageType>                                   WriterType;
typedef otb::ImageFileWriter<LabeledImageType>                                  LabeledWriterType;

typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>          LabelMapFilterType;
typedef otb::ShapeAttributesLabelMapFilter<LabelMapType>                        ShapeFilterType;
typedef otb::BandsStatisticsAttributesLabelMapFilter<LabelMapType, VectorImageType>  BandsStatisticsFilterType;

// SVM model estimation
typedef itk::VariableLengthVector<double>                      VectorType;
typedef itk::FixedArray<LabelType, 1>                           TrainingVectorType;
typedef itk::Statistics::ListSample<VectorType>                ListSampleType;
typedef itk::Statistics::ListSample<TrainingVectorType>        TrainingListSampleType;
typedef otb::LabelMapWithClassLabelToLabeledSampleListFilter<LabelMapType, ListSampleType, TrainingListSampleType>
                                                               ListSampleFilterType;

typedef otb::Functor::VariableLengthVectorToMeasurementVectorFunctor<VectorType> MeasurementVectorFunctorType;
typedef otb::SVMSampleListModelEstimator<ListSampleType, TrainingListSampleType,
  MeasurementVectorFunctorType>                                                  SVMEstimatorType;

typedef otb::LabelMapSVMClassifier<LabelMapType>                                ClassifierType;
typedef otb::LabelMapWithClassLabelToClassLabelImageFilter
          <LabelMapType, LabeledImageType>                                       ClassifImageGeneratorType;


LabelObjectType::Pointer makeTrainingSample(LabelMapType* labelMap, LabelType labelObjectId, LabelType classLabel)
{
  LabelObjectType::Pointer newLabelObject = LabelObjectType::New();
  newLabelObject->CopyAllFrom( labelMap->GetLabelObject(labelObjectId) );
  newLabelObject->SetClassLabel(classLabel);
  return newLabelObject;
}

int otbLabelMapSVMClassifierNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  ClassifierType::Pointer classifier = ClassifierType::New();
  return EXIT_SUCCESS;
}

int otbLabelMapSVMClassifier(int itkNotUsed(argc), char * argv[])
{
  const char * infname  = argv[1];
  const char * lfname   = argv[2];
  const char * outfname = argv[3];

  // Filters instanciation
  ReaderType::Pointer                reader              = ReaderType::New();
  LabeledReaderType::Pointer         labeledReader       = LabeledReaderType::New();
  LabelMapFilterType::Pointer        filter              = LabelMapFilterType::New();
  ShapeFilterType::Pointer           shapeFilter         = ShapeFilterType::New();
  BandsStatisticsFilterType::Pointer radiometricFilter   = BandsStatisticsFilterType::New();
  LabelMapType::Pointer              trainingLabelMap    = LabelMapType::New();
  ListSampleFilterType::Pointer      labelMap2SampleList = ListSampleFilterType::New();
  SVMEstimatorType::Pointer          svmEstim            = SVMEstimatorType::New();
  ClassifierType::Pointer            classifier          = ClassifierType::New();
  ClassifImageGeneratorType::Pointer imGenerator         = ClassifImageGeneratorType::New();
  LabeledWriterType::Pointer         writer              = LabeledWriterType::New();

  // Read inputs
  reader->SetFileName(infname);
  labeledReader->SetFileName(lfname);

  // Make a LabelMap out of it
  filter->SetInput(labeledReader->GetOutput());
  filter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());

  //Compute shape and radimometric attributes
  shapeFilter->SetInput(filter->GetOutput());
  radiometricFilter->SetInput(shapeFilter->GetOutput());
  radiometricFilter->SetFeatureImage(reader->GetOutput());
  radiometricFilter->Update();

  // Build a sub-LabelMap with class-labeled LabelObject
  LabelMapType::Pointer labelMap = radiometricFilter->GetOutput();

  // The following is very specific to the input specified in CMakeLists
  // water
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 13, 0));
  // road
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 88, 1));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 114, 1));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 144, 1));
  // boat
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 52, 2));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 31, 2));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 11, 2));
  // roof
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 58, 3));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 60, 3));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 27, 3));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 81, 3));
  // vegetation
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 161, 4));
  trainingLabelMap->PushLabelObject(makeTrainingSample(labelMap, 46, 4));

  // Make a ListSample out of trainingLabelMap
  labelMap2SampleList->SetInputLabelMap(trainingLabelMap);

  std::vector<std::string> attributes = labelMap->GetLabelObject(0)->GetAvailableAttributes();
  std::vector<std::string>::const_iterator attrIt;
  for (attrIt = attributes.begin(); attrIt != attributes.end(); ++attrIt)
    {
    labelMap2SampleList->GetMeasurementFunctor().AddAttribute((*attrIt).c_str());
    }

  labelMap2SampleList->Update();

  // Estimate SVM model
  svmEstim->SetInputSampleList(labelMap2SampleList->GetOutputSampleList());
  svmEstim->SetTrainingSampleList(labelMap2SampleList->GetOutputTrainingSampleList());
  svmEstim->Modified();
  svmEstim->Update();

  // Classify using the whole LabelMap with estimated model
  classifier->SetInput(labelMap);
  classifier->SetModel(svmEstim->GetModel());

  for (attrIt = attributes.begin(); attrIt != attributes.end(); ++attrIt)
    {
    classifier->GetMeasurementFunctor().AddAttribute((*attrIt).c_str());
    }

  classifier->Update();

  // Make a labeled image with the classification result
  imGenerator->SetInput(classifier->GetOutput());

  writer->SetInput(imGenerator->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
