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
#include "otbImageSVMClassifier.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

// otb basic
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbChangeLabelImageFilter.h"

// itk
#include "itkVariableLengthVector.h"


// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

// Shift Scale Vector Image Filter
#include "otbShiftScaleVectorImageFilter.h"

// Classification filter
#include "otbSVMImageClassificationFilter.h"

#include "itkTimeProbe.h"
#include "otbStandardFilterWatcher.h"


namespace otb
{

int ImageSVMClassifier::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("ImageSVMClassifier");
  descriptor->SetDescription("Perform SVM classification based a previous computed svm model to an new input image.");
  descriptor->AddOption("InputImage", "A new image to classify",
                        "in", 1, true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("ImageStatistics", "a XML file containing mean and variance of input image.",
                        "is", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("SVMmodel", "Estimator model previously computed",
                        "svm", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("ShiftScaleImage", "Shift Scale the input image",
                        "ssi", 0, false, ApplicationDescriptor::Boolean);
  descriptor->AddOption("OutputLabeledImage", "Output labeled image",
                        "out", 1, true, ApplicationDescriptor::FileName);
  return EXIT_SUCCESS;
}

int ImageSVMClassifier::Execute(otb::ApplicationOptionsResult* parseResult)
{

  // Input Image
  typedef float                                    PixelType;
  typedef unsigned char                            LabeledPixelType;
  typedef otb::VectorImage<PixelType, 2>           VectorImageType;
  typedef otb::Image<LabeledPixelType, 2>          LabeledImageType;

  typedef otb::ImageFileReader<VectorImageType>    ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>    WriterType;

  // Statistic XML file Reader
  typedef itk::VariableLengthVector<PixelType>                          MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>                 StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<VectorImageType, VectorImageType>  RescalerType;

  /// Classification typedefs
  typedef otb::SVMImageClassificationFilter<VectorImageType, LabeledImageType>   ClassificationFilterType;
  typedef ClassificationFilterType::Pointer                                      ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType                                    ModelType;
  typedef ModelType::Pointer                                                     ModelPointerType;
  typedef otb::ChangeLabelImageFilter<LabeledImageType, VectorImageType>         ChangeLabelFilterType;
  typedef ChangeLabelFilterType::Pointer                                         ChangeLabelFilterPointerType;


  // Load input image
  ReaderType::Pointer    reader  = ReaderType::New();
  reader->SetFileName(parseResult->GetParameterString("InputImage"));
  reader->UpdateOutputInformation();

  // Load input image statistics
  StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
  statisticsReader->SetFileName(parseResult->GetParameterString("ImageStatistics"));
  MeasurementType  meanMeasurementVector     = statisticsReader->GetStatisticVectorByName("mean");
  MeasurementType  varianceMeasurementVector = statisticsReader->GetStatisticVectorByName("variance");

  std::cout << "mean: " << meanMeasurementVector << std::endl;
  std::cout << "variance: " << varianceMeasurementVector << std::endl;

  // Load svm model
  ModelPointerType modelSVM = ModelType::New();
  modelSVM->LoadModel(parseResult->GetParameterString("SVMmodel").c_str());


  // Classify
  RescalerType::Pointer rescaler = RescalerType::New();
  ClassificationFilterPointerType classificationFilter = ClassificationFilterType::New();
  classificationFilter->SetModel(modelSVM);

  // Normalize input image
  if (parseResult->IsOptionPresent("ShiftScaleImage"))
    {
    std::cout << "Shift and scale of the input image !" << std::endl;
    rescaler->SetScale(varianceMeasurementVector);
    rescaler->SetShift(meanMeasurementVector);
    rescaler->SetInput(reader->GetOutput());
    classificationFilter->SetInput(rescaler->GetOutput());
    }
  else
    {
    std::cout << "no shift and scale" << std::endl;
    classificationFilter->SetInput(reader->GetOutput());
    }


  ChangeLabelFilterPointerType changeLabelFilter = ChangeLabelFilterType::New();
  changeLabelFilter->SetInput(classificationFilter->GetOutput());
  changeLabelFilter->SetNumberOfComponentsPerPixel(3);

  /*for (ClassesMapType::iterator it = m_ClassesMap.begin(); it != m_ClassesMap.end(); ++it)
    {
    OverlayImageType::PixelType color(3);

    color[0] = static_cast<unsigned char>((*it)->GetColor()[0] * 255);
    color[1] = static_cast<unsigned char>((*it)->GetColor()[1] * 255);
    color[2] = static_cast<unsigned char>((*it)->GetColor()[2] * 255);
    changeLabelFilter->SetChange((*it)->GetId(), color);
    }*/

  // Save labeled Image
  WriterType::Pointer    writer  = WriterType::New();
  writer->SetFileName(parseResult->GetParameterString("OutputLabeledImage"));
  writer->SetInput(changeLabelFilter->GetOutput());
  writer->Update();

  std::cout<<"Classification done ... "<<std::endl;

  return EXIT_SUCCESS;
}

}
