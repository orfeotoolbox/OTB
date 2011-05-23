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
#include "otbStreamingImageFileWriter.h"
#include "otbChangeLabelImageFilter.h"
#include "otbStandardWriterWatcher.h"

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
  descriptor->AddOption("InputImageMask", "A mask associated with the new image to classify",
                        "inm", 1, false, ApplicationDescriptor::InputImage);
  descriptor->AddOption("ImageStatistics", "a XML file containing mean and standard deviation of input images used to train svm model.",
                        "is", 1, false, ApplicationDescriptor::FileName);
  descriptor->AddOption("SVMmodel", "Estimated model previously computed",
                        "svm", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputLabeledImage", "Output labeled image",
                        "out", 1, true, ApplicationDescriptor::OutputImage);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram", 1, false, otb::ApplicationDescriptor::Integer);
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
  typedef otb::ImageFileReader<LabeledImageType>   LabeledReaderType;
  typedef otb::StreamingImageFileWriter<LabeledImageType>   WriterType;

  typedef otb::PipelineMemoryPrintCalculator        MemoryCalculatorType;

  // Statistic XML file Reader
  typedef itk::VariableLengthVector<PixelType>                          MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>                 StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<VectorImageType, VectorImageType>  RescalerType;

  /// Classification typedefs
  typedef otb::SVMImageClassificationFilter<VectorImageType, LabeledImageType>   ClassificationFilterType;
  typedef ClassificationFilterType::Pointer                                      ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType                                    ModelType;
  typedef ModelType::Pointer                                                     ModelPointerType;
//  typedef otb::ChangeLabelImageFilter<LabeledImageType, VectorImageType>         ChangeLabelFilterType;
//  typedef ChangeLabelFilterType::Pointer                                         ChangeLabelFilterPointerType;


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
  MeasurementType  stddevMeasurementVector;
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
    stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
    std::cout << "mean used: " << meanMeasurementVector << std::endl;
    std::cout << "standard deviation used: " << stddevMeasurementVector << std::endl;
    std::cout << "Shift and scale of the input image !" << std::endl;
    // Rescale vector image
    rescaler->SetScale(stddevMeasurementVector);
    rescaler->SetShift(meanMeasurementVector);
    rescaler->SetInput(reader->GetOutput());

    classificationFilter->SetInput(rescaler->GetOutput());
    }
  else
    {
    std::cout << "no shift and scale" << std::endl;
    classificationFilter->SetInput(reader->GetOutput());
    }

  LabeledReaderType::Pointer readerMask = LabeledReaderType::New();
  //--------------------------
  // Set an input mask to exclude some areas (optional)
  if (parseResult->IsOptionPresent("InputImageMask"))
    {
    readerMask->SetFileName(parseResult->GetParameterString("InputImageMask"));
    readerMask->UpdateOutputInformation();
    classificationFilter->SetInputMask(readerMask->GetOutput());
    std::cout << "Set an input image mask!" << std::endl;
    }

  //ChangeLabelFilterPointerType changeLabelFilter = ChangeLabelFilterType::New();
  //changeLabelFilter->SetInput(classificationFilter->GetOutput());
  //changeLabelFilter->SetNumberOfComponentsPerPixel(3);

  //--------------------------
  // Save labeled Image
  WriterType::Pointer    writer  = WriterType::New();
  writer->SetInput(classificationFilter->GetOutput());
  writer->SetFileName(parseResult->GetParameterString("OutputLabeledImage"));
  unsigned int ram = 256;
  if (parseResult->IsOptionPresent("AvailableMemory"))
    {
    ram = parseResult->GetParameterUInt("AvailableMemory");
    }
  writer->SetAutomaticTiledStreaming(ram);
    
  otb::StandardWriterWatcher watcher(writer,"Classification");
  writer->Update();
  return EXIT_SUCCESS;
}

}
