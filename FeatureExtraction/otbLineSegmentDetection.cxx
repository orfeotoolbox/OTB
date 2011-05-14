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
#include "otbLineSegmentDetection.h"
#include "otbStreamingLineSegmentDetector.h"

#include "otbImage.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStandardFilterWatcher.h"

namespace otb
{

int LineSegmentDetection::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Line Segment Detection");
  descriptor->SetDescription(
                             "Detects line segment in an image");
  descriptor->AddInputImage();
  descriptor->AddOption("OutputVectorData", "Output Shape file name", "outshape", 1, true,
                        ApplicationDescriptor::FileName);
  return EXIT_SUCCESS;
}


int LineSegmentDetection::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension>       ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;

  typedef otb::VectorData<double, Dimension>          VectorDataType;
  typedef VectorDataType::Pointer                     VectorDataPointerType;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataFileWriterType;
  typedef VectorDataFileWriterType::Pointer           VectorDataFileWriterPointerType;

  typedef otb::StreamingLineSegmentDetector
    < ImageType >::FilterType LSDFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());
  reader->UpdateOutputInformation();

  LSDFilterType::Pointer connected
    = LSDFilterType::New();
  connected->GetFilter()->SetInput(reader->GetOutput());

  otb::StandardFilterWatcher watcher(connected->GetStreamer(),"Segmentation");
  connected->Update();

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(connected->GetFilter()->GetOutputVectorData());
  vdwriter->SetFileName(parseResult->GetParameterString("OutputVectorData"));
  vdwriter->Update();

  return EXIT_SUCCESS;
}

}
