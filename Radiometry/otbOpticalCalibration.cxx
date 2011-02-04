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

#include "otbOpticalCalibration.h"

#include <iostream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbImageToLuminanceImageFilter.h"
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"

#include "otbStreamingImageFileWriter.h"
#include "otbStandardWriterWatcher.h"
#include "otbPipelineMemoryPrintCalculator.h"
#include "otbMultiplyByScalarImageFilter.h"

namespace otb
{

int OpticalCalibration::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("OpticalCalibration");
  descriptor->SetDescription("Perform optical calibration TOA/TOC. Output image is in milli-reflectance.");
  descriptor->AddInputImage();
  descriptor->AddOutputImage();
  descriptor->AddOptionNParams("Level",
                           "Level of calibration toa or toc",
                           "level",true, otb::ApplicationDescriptor::String);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default","ram",1,false, otb::ApplicationDescriptor::Integer);
  return EXIT_SUCCESS;
}

int OpticalCalibration::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorImage<unsigned short int, 2>       ImageType;
  typedef otb::VectorImage<float,2>                     FloatImageType;
  typedef otb::ImageFileReader<ImageType>               ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType>      WriterType;

  typedef ImageToLuminanceImageFilter<ImageType, FloatImageType> ImageToLuminanceImageFilterType;
  typedef LuminanceToReflectanceImageFilter<FloatImageType,
      FloatImageType>                      LuminanceToReflectanceImageFilterType;
  typedef otb::MultiplyByScalarImageFilter<FloatImageType,ImageType> ScaleFilterType;
  typedef ReflectanceToSurfaceReflectanceImageFilter<FloatImageType,
      FloatImageType>             ReflectanceToSurfaceReflectanceImageFilterType;
  typedef otb::PipelineMemoryPrintCalculator        MemoryCalculatorType;

  // calibration process
  if(parseResult->IsOptionPresent("Level"))
    {
    // Read input image information
    ReaderType::Pointer reader=ReaderType::New();
    reader->SetFileName(parseResult->GetInputImage().c_str());
    reader->GenerateOutputInformation();
  
    ImageToLuminanceImageFilterType ::Pointer imageToLuminanceFilter                = ImageToLuminanceImageFilterType::New();
    LuminanceToReflectanceImageFilterType::Pointer luminanceToReflectanceFilter          = LuminanceToReflectanceImageFilterType::New();
    ReflectanceToSurfaceReflectanceImageFilterType::Pointer reflectanceToSurfaceReflectanceFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();

    imageToLuminanceFilter->SetInput(reader->GetOutput());
    luminanceToReflectanceFilter->SetInput(imageToLuminanceFilter->GetOutput());
    reflectanceToSurfaceReflectanceFilter->SetInput(luminanceToReflectanceFilter->GetOutput());
  
    ScaleFilterType::Pointer scaleFilter = ScaleFilterType::New();
    scaleFilter->SetInput(luminanceToReflectanceFilter->GetOutput());
    scaleFilter->SetCoef(1000.);

    //Instantiate the writer
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetOutputImage());
    writer->SetInput(scaleFilter->GetOutput());
    writer->SetWriteGeomFile(true);

    //Instantiate the pipeline memory print estimator
    MemoryCalculatorType::Pointer calculator = MemoryCalculatorType::New();
    const double byteToMegabyte = 1./vcl_pow(2.0, 20);

    if (parseResult->IsOptionPresent("AvailableMemory"))
      {
      long long int memory = static_cast <long long int> (parseResult->GetParameterUInt("AvailableMemory"));
      calculator->SetAvailableMemory(memory / byteToMegabyte);
      }
    else
      {
      calculator->SetAvailableMemory(256 * byteToMegabyte);
      }

    calculator->SetDataToWrite(imageToLuminanceFilter->GetOutput());
    calculator->Compute();
      
    writer->SetTilingStreamDivisions(calculator->GetOptimalNumberOfStreamDivisions());
      
    otbMsgDevMacro(<< "Guess the pipeline memory print " << calculator->GetMemoryPrint()*byteToMegabyte << " Mo");
    otbMsgDevMacro(<< "Number of stream divisions : " << calculator->GetOptimalNumberOfStreamDivisions());

    otb::StandardWriterWatcher watcher(writer,"OpticalCalibration");

    writer->Update();
    return EXIT_SUCCESS;
    }
  else
    {
    itkGenericExceptionMacro(<< "No levels of are  specified");
    }
  
}
}
