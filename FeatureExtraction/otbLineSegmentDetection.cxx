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
#include "otbVectorImage.h"
#include "otbVectorImageToAmplitudeImageFilter.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStandardFilterWatcher.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "itkShiftScaleImageFilter.h"

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

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
  descriptor->AddOption("DEMDirectory", "DEM directory (used to reproject in WGS84 if input is in sensor model geometry)",
                        "dem", 1, false, ApplicationDescriptor::DirectoryName);
  return EXIT_SUCCESS;
}


int LineSegmentDetection::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension>       ImageType;
  typedef otb::VectorImage<InputPixelType, Dimension> VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>       ReaderType;

  typedef otb::VectorData<double, Dimension>          VectorDataType;
  typedef VectorDataType::Pointer                     VectorDataPointerType;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataFileWriterType;
  typedef VectorDataFileWriterType::Pointer           VectorDataFileWriterPointerType;

  typedef otb::VectorImageToAmplitudeImageFilter<VectorImageType, ImageType>
    VectorImageToAmplitudeImageFilterType;

  typedef otb::StreamingStatisticsImageFilter<ImageType>
    StreamingStatisticsImageFilterType;

  typedef itk::ShiftScaleImageFilter<ImageType, ImageType>
    ShiftScaleImageFilterType;

  typedef otb::StreamingLineSegmentDetector
    < ImageType >::FilterType LSDFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());
  reader->UpdateOutputInformation();

  VectorImageToAmplitudeImageFilterType::Pointer amplitudeConverter
    = VectorImageToAmplitudeImageFilterType::New();
  amplitudeConverter->SetInput(reader->GetOutput());

  StreamingStatisticsImageFilterType::Pointer stats
    = StreamingStatisticsImageFilterType::New();
  stats->SetInput(amplitudeConverter->GetOutput());
  stats->Update();
  InputPixelType min = stats->GetMinimum();
  InputPixelType max = stats->GetMaximum();

  ShiftScaleImageFilterType::Pointer shiftScale
    = ShiftScaleImageFilterType::New();
  shiftScale->SetInput(amplitudeConverter->GetOutput());
  shiftScale->SetShift( -min );
  shiftScale->SetScale( 255.0 / (max - min) );

  LSDFilterType::Pointer lsd
    = LSDFilterType::New();
  lsd->GetFilter()->SetInput(shiftScale->GetOutput());

  otb::StandardFilterWatcher watcher(lsd->GetStreamer(),"Line Segment Detection");
  lsd->Update();

  /*
   * Reprojection of the output VectorData
   *
   * The output of LSDFilterType is in image physical coordinates,
   * projection WKT applied if the input image has one
   *
   * We need to reproject in WGS84 if the input image is in sensor model geometry
   */

  std::string projRef = reader->GetOutput()->GetProjectionRef();
  ImageKeywordlist kwl = reader->GetOutput()->GetImageKeywordlist();

  VectorDataType::Pointer vd = lsd->GetFilter()->GetOutputVectorData();
  VectorDataType::Pointer projectedVD = vd;

  if ( projRef.empty() && kwl.GetSize() > 0 )
    {
    // image is in sensor model geometry

    // Reproject VectorData in image projection
    typedef otb::VectorDataProjectionFilter
      <VectorDataType, VectorDataType>                     VectorDataProjectionFilterType;

    VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
    vproj->SetInput(vd);
    vproj->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
    vproj->SetInputOrigin(reader->GetOutput()->GetOrigin());
    vproj->SetInputSpacing(reader->GetOutput()->GetSpacing());

    if( parseResult->IsOptionPresent("DEMDirectory") )
      {
      vproj->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
      }

    vproj->Update();

    projectedVD = vproj->GetOutput();
    }

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(projectedVD);
  vdwriter->SetFileName(parseResult->GetParameterString("OutputVectorData"));
  vdwriter->Update();

  return EXIT_SUCCESS;
}

}
