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

#include "otbMultiChannelExtractROI.h"

namespace otb
{

int ImageSVMClassifier::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("ImageSVMClassifier");
  descriptor->SetDescription("Perform SVM classification based a previous computed svm model to an new input image.");
  descriptor->AddOption("InputImage", "A new image to classify",
                        "in", 1, true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("ImageStatistics", "a XML file containing mean and variance of input images used to train svm model.",
                        "is", 1, false, ApplicationDescriptor::FileName);
  descriptor->AddOption("SVMmodel", "Estimated model previously computed",
                        "svm", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputLabeledImage", "Output labeled image",
                        "out", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("ROIStartX", "Start X of the ROI",
                        "x", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("ROIStartY", "Start Y of the ROI",
                        "y", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("ROISizeX", "Size X of the ROI",
                        "sx", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("ROISizeY", "Size Y of the ROI",
                        "sy", 1, false, ApplicationDescriptor::Integer);
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

  //typedef otb::ExtractROIBase<VectorImageType, VectorImageType>          ExtractROIType;
  typedef otb::MultiChannelExtractROI<PixelType, PixelType>  ExtractROIType;

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


  //--------------------------
  // Load input image
  ReaderType::Pointer    reader  = ReaderType::New();
  reader->SetFileName(parseResult->GetParameterString("InputImage"));
  reader->UpdateOutputInformation();

  // Load svm model
  ModelPointerType modelSVM = ModelType::New();
  modelSVM->LoadModel(parseResult->GetParameterString("SVMmodel").c_str());

  //--------------------------
  // Normalize input image (optional)
  StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
  MeasurementType  meanMeasurementVector;
  MeasurementType  varianceMeasurementVector;
  RescalerType::Pointer rescaler = RescalerType::New();

  //--------------------------
  // Classify
  ClassificationFilterPointerType classificationFilter = ClassificationFilterType::New();
  classificationFilter->SetModel(modelSVM);

  // Normalize input image
  if (parseResult->IsOptionPresent("ImageStatistics"))
    {
    // Load input image statistics
    statisticsReader->SetFileName(parseResult->GetParameterString("ImageStatistics"));
    meanMeasurementVector     = statisticsReader->GetStatisticVectorByName("mean");
    varianceMeasurementVector = statisticsReader->GetStatisticVectorByName("variance");
    std::cout << "mean: " << meanMeasurementVector << std::endl;
    std::cout << "variance: " << varianceMeasurementVector << std::endl;
    std::cout << "Shift and scale of the input image !" << std::endl;
    // Rescale vector image
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

  //--------------------------
  // Save labeled Image
  WriterType::Pointer    writer  = WriterType::New();
  writer->SetFileName(parseResult->GetParameterString("OutputLabeledImage"));

  // Extract ROI if needed
  if ( (parseResult->IsOptionPresent("ROIStartX")) &&
       (parseResult->IsOptionPresent("ROIStartY")) &&
       (parseResult->IsOptionPresent("ROISizeX")) &&
       (parseResult->IsOptionPresent("ROISizeY")) )
    {
    ExtractROIType::Pointer extract =  ExtractROIType::New();
    extract->SetInput(changeLabelFilter->GetOutput());

    std::cout << parseResult->GetParameterUInt("ROIStartX") << ", "
              << parseResult->GetParameterUInt("ROIStartY") << ", "
              << parseResult->GetParameterUInt("ROISizeX") << ", "
              << parseResult->GetParameterUInt("ROISizeY") << std::endl;

    extract->SetStartX((unsigned long) parseResult->GetParameterUInt("ROIStartX"));
    extract->SetStartY((unsigned long) parseResult->GetParameterUInt("ROIStartY"));
    extract->SetSizeX((unsigned long) parseResult->GetParameterUInt("ROISizeX"));
    extract->SetSizeY((unsigned long) parseResult->GetParameterUInt("ROISizeY"));

    extract->Update();

    writer->SetInput(extract->GetOutput());
    }
  else
    {
    writer->SetInput(changeLabelFilter->GetOutput());
    }

  writer->Update();

  std::cout<<"Classification done ... "<<std::endl;

  return EXIT_SUCCESS;
}

}
