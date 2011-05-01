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
#include "otbVectorDataExtractROIApplication.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

//Image
#include "otbImage.h"
#include "otbImageFileReader.h"

//VectorData
#include "otbVectorData.h"
#include "otbVectorDataExtractROI.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataProperties.h"

//Misc
#include "otbRemoteSensingRegion.h"
#include "otbStandardWriterWatcher.h"

namespace otb
{

int VectorDataExtractROIApplication::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("VectorDataExtractROIApplication");
  descriptor->SetDescription("Perform an extract ROI on the input vector data according to the input image extent");
  descriptor->AddOption("InputVData", "The input vector data",
                        "in",  1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("InputImage", "The support image",
                        "img",  1, true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("DEMPath", "The DEM directory",
                        "dem",  1, false, ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("OutputVData", "The output vector data",
                        "out",  1, true, ApplicationDescriptor::FileName);

  return EXIT_SUCCESS;
}

int VectorDataExtractROIApplication::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Images
  typedef unsigned char                                   PixelType;
  typedef otb::Image<PixelType, 2>                        ImageType;
  typedef ImageType::PointType                            PointType;
  typedef ImageType::IndexType                            IndexType;
  typedef ImageType::SizeType                             SizeType;
  typedef ImageType::SpacingType                          SpacingType;
  typedef otb::ImageFileReader<ImageType>                 ImageReaderType;
  
  // VectorData
  typedef otb::VectorData<>                               VectorDataType;
  
  typedef otb::VectorDataFileReader<VectorDataType>       VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>       VectorDataWriterType;
  typedef VectorDataProjectionFilter<
    VectorDataType, VectorDataType>                        VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<VectorDataType>            VectorDataExtractROIType;

  // Misc
  typedef otb::RemoteSensingRegion<double>                RemoteSensingRegionType;

  // Reading the VectorData
  std::string vdFilename = parseResult->GetParameterString("InputVData");
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(vdFilename);
  vdReader->Update();

  // Reading the Image
  std::string imgFilename = parseResult->GetParameterString("InputImage");
  ImageReaderType::Pointer imgReader = ImageReaderType::New();
  imgReader->SetFileName(imgFilename);
  imgReader->UpdateOutputInformation();
  
  // Extracting the VectorData
  VectorDataExtractROIType::Pointer vdExtract = VectorDataExtractROIType::New();
  vdExtract->SetInput(vdReader->GetOutput());
    
  // Find the geographic region of interest
  // Get the index of the corner of the image
  IndexType ul, ur, ll, lr;
  PointType pul, pur, pll, plr;
  ul = imgReader->GetOutput()->GetLargestPossibleRegion().GetIndex();
  ur = ul;
  ll = ul;
  lr = ul;
  ur[0] += imgReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  lr[0] += imgReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  lr[1] += imgReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
  ll[1] += imgReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

  // Transform to physical point
  imgReader->GetOutput()->TransformIndexToPhysicalPoint(ul, pul);
  imgReader->GetOutput()->TransformIndexToPhysicalPoint(ur, pur);
  imgReader->GetOutput()->TransformIndexToPhysicalPoint(ll, pll);
  imgReader->GetOutput()->TransformIndexToPhysicalPoint(lr, plr);

  // Get DEM directory
  std::string demPath;
  if(parseResult->IsOptionPresent("DEMPath"))
    {
    demPath = parseResult->GetParameterString("DEMPath");
    }

  // Build the cartographic region
  RemoteSensingRegionType            rsRegion;
  RemoteSensingRegionType::IndexType rsOrigin;
  RemoteSensingRegionType::SizeType  rsSize;
  rsOrigin[0] = std::min(pul[0], plr[0]);
  rsOrigin[1] = std::min(pul[1], plr[1]);
  rsSize[0] = vcl_abs(pul[0] - plr[0]);
  rsSize[1] = vcl_abs(pul[1] - plr[1]);
  
  rsRegion.SetOrigin(rsOrigin);
  rsRegion.SetSize(rsSize);
  rsRegion.SetRegionProjection(imgReader->GetOutput()->GetProjectionRef());
  rsRegion.SetKeywordList(imgReader->GetOutput()->GetImageKeywordlist());

  // Set the cartographic region to the extract roi filter
  vdExtract->SetRegion(rsRegion);
  if(parseResult->IsOptionPresent("DEMPath"))
    {
    vdExtract->SetDEMDirectory(demPath);
    }

  // Reprojecting the VectorData
  vdReader->GetOutput()->GetProjectionRef();
  VectorDataProjectionFilterType::Pointer vdProj = VectorDataProjectionFilterType::New();
  vdProj->SetInput(vdExtract->GetOutput());
  vdProj->SetInputProjectionRef(vdReader->GetOutput()->GetProjectionRef());
  vdProj->SetOutputKeywordList(imgReader->GetOutput()->GetImageKeywordlist());
  vdProj->SetOutputProjectionRef(imgReader->GetOutput()->GetProjectionRef());
  vdProj->SetOutputOrigin(imgReader->GetOutput()->GetOrigin());
  vdProj->SetOutputSpacing(imgReader->GetOutput()->GetSpacing());
  if(parseResult->IsOptionPresent("DEMPath"))
    {
    vdProj->SetDEMDirectory(demPath);
    }

  // Instantiate the writer
  std::string outFilename = parseResult->GetParameterString("OutputVData");
  
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  vdWriter->SetFileName(outFilename);
  vdWriter->SetInput(vdExtract->GetOutput());
  vdWriter->Update();

  return EXIT_SUCCESS;
}

}

