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
#include "otbRasterization.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

//Image
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkRGBAPixel.h"

//VectorData
#include "otbVectorData.h"
#include "otbVectorDataExtractROI.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataProperties.h"

//Rasterization
#include "otbVectorDataToImageFilter.h"

//Misc
#include "otbRemoteSensingRegion.h"
#include "otbStandardWriterWatcher.h"
#include "otbPipelineMemoryPrintCalculator.h"

namespace otb
{

int Rasterization::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Rasterization");
  descriptor->SetDescription("Reproject and Rasterize a Vector Data.");
  descriptor->AddOption("InputVData", "The input vector data to be rasterized",
                        "in",  1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputImage", "An output image containing the rasterized vector data",
                        "out", 1, true, ApplicationDescriptor::OutputImage);
  descriptor->AddOption("SizeX", "OutputSize[0]",
                        "szx", 1, true, ApplicationDescriptor::Real);
  descriptor->AddOption("SizeY", "OutputSize[1]",
                        "szy", 1, true, ApplicationDescriptor::Real);

  // Optional
  descriptor->AddOption("OriginX", "OutputOrigin[0] (optional)",
                        "orx", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("OriginY", "OutputOrigin[1] (optional)",
                        "ory", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("SpacingX", "OutputSpacing[0] (optional)",
                        "spx", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("SpacingY", "OutputSpacing[1] (optional)",
                        "spy", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("ProjRef", "Projection (optional)",
                        "pr",  1, false, ApplicationDescriptor::String);
  descriptor->AddOption("AvailableMemory", "Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)",
                        "ram", 1, false, otb::ApplicationDescriptor::Integer);
  
  return EXIT_SUCCESS;
}

int Rasterization::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Images
  //typedef itk::RGBAPixel<unsigned char>                   PixelType;
  typedef unsigned char                                   PixelType;
  typedef otb::Image<PixelType, 2>                        ImageType;
  typedef ImageType::PointType                            PointType;
  typedef ImageType::SizeType                             SizeType;
  typedef ImageType::SpacingType                          SpacingType;
  typedef ImageType::IndexType                            IndexType;
  typedef otb::StreamingImageFileWriter<ImageType>        WriterType;

  // VectorData
  typedef otb::VectorData<>                               VectorDataType;
  typedef VectorDataType::DataNodeType                    DataNodeType;
  typedef VectorDataType::DataTreeType                    DataTreeType;
  typedef DataNodeType::PointType                         PointType;
  typedef otb::VectorDataFileReader<VectorDataType>       VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>       VectorDataWriterType;
  typedef VectorDataProjectionFilter<
    VectorDataType, VectorDataType>                        VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<VectorDataType>            VectorDataExtractROIType;
  typedef VectorDataProperties<VectorDataType>            VectorDataPropertiesType;

  // Rasterization
  typedef otb::VectorDataToImageFilter<VectorDataType,
    ImageType>                                      VectorDataToImageFilterType;

  // Misc
  typedef otb::RemoteSensingRegion<double>                RemoteSensingRegionType;
  typedef RemoteSensingRegionType::SizeType               SizePhyType;
  typedef otb::PipelineMemoryPrintCalculator              MemoryCalculatorType;
  
  // Reading the VectorData
  std::string vdFilename = parseResult->GetParameterString("InputVData");
  std::cout<<"Processing vector data : "<<vdFilename<<std::endl;
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(vdFilename);
  vdReader->Update();

  // Reprojecting the VectorData
  std::string projectionRef;
  VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
  vproj->SetInput(vdReader->GetOutput());
  if(parseResult->IsOptionPresent("ProjRef"))
    {
    projectionRef = parseResult->GetParameterString("ProjRef");
    }
  else
    {
    projectionRef = vdReader->GetOutput()->GetProjectionRef();
    }
  vproj->SetOutputProjectionRef(projectionRef);

  // Converting the VectorData
  VectorDataPropertiesType::Pointer vdProperties = VectorDataPropertiesType::New();
  vdProperties->SetVectorDataObject(vdReader->GetOutput());
  vdProperties->ComputeBoundingRegion();

  SizeType size;
  size[0] = parseResult->GetParameterDouble("SizeX");
  size[1] = parseResult->GetParameterDouble("SizeY");

  PointType origin;
  if(parseResult->IsOptionPresent("OriginX") && parseResult->IsOptionPresent("OriginY"))
    {
    origin[0] = parseResult->GetParameterDouble("OriginX");
    origin[1] = parseResult->GetParameterDouble("OriginY");
    }
  else
    {
    origin = vdProperties->GetBoundingRegion().GetIndex();
    }
  
  SpacingType spacing;
  if(parseResult->IsOptionPresent("SpacingX") && parseResult->IsOptionPresent("SpacingY"))
    {
    spacing[0] = parseResult->GetParameterDouble("SpacingX");
    spacing[1] = parseResult->GetParameterDouble("SpacingY");
    }
  else
    {
    spacing[0] = vdProperties->GetBoundingRegion().GetSize()[0]/size[0];
    spacing[1] = vdProperties->GetBoundingRegion().GetSize()[1]/size[1];
    }

  
  SizePhyType               sizePhy;
  sizePhy[0] = size[0] * spacing[0];
  sizePhy[1] = size[1] * spacing[1];

  RemoteSensingRegionType   region;
  region.SetSize(sizePhy);
  region.SetOrigin(origin);
  region.SetRegionProjection(projectionRef);
 
  VectorDataExtractROIType::Pointer vdextract = VectorDataExtractROIType::New();
  vdextract->SetRegion(region);
  vdextract->SetInput(vproj->GetOutput());

  VectorDataToImageFilterType::Pointer vectorDataRendering = VectorDataToImageFilterType::New();
  vectorDataRendering->SetInput(vdextract->GetOutput());
  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetVectorDataProjectionWKT(projectionRef);
  vectorDataRendering->SetRenderingStyleType(VectorDataToImageFilterType::Binary);

  // Instantiate the writer
  std::string oFilename = parseResult->GetParameterString("OutputImage");
  
  WriterType::Pointer oWriter = WriterType::New();
  oWriter->SetFileName(oFilename);
  oWriter->SetInput(vectorDataRendering->GetOutput());
  

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
  
  calculator->SetDataToWrite(vectorDataRendering->GetOutput());
  calculator->Compute();
      
  oWriter->SetTilingStreamDivisions(calculator->GetOptimalNumberOfStreamDivisions());
      
  otbMsgDevMacro(<< "Guess the pipeline memory print " << calculator->GetMemoryPrint()*byteToMegabyte << " Mo");
  otbMsgDevMacro(<< "Number of stream divisions : " << calculator->GetOptimalNumberOfStreamDivisions());

  otb::StandardWriterWatcher watcher(oWriter,"Rasterization");
  
  oWriter->Update();

  return EXIT_SUCCESS;
}
}
