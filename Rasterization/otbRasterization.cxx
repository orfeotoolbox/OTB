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

#include "otbGeoInformationConversion.h"

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
  descriptor->AddOption("InputImage", "A reference image for extraction region and projection information (optional)",
                          "im", 1, false, ApplicationDescriptor::InputImage);
  descriptor->AddOption("SizeX", "OutputSize[0] (useless if support image is given)",
                        "szx", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("SizeY", "OutputSize[1] (useless if support image is given)",
                        "szy", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("RSID", "Projection System RSID number (RSID 4326 for WGS84 32631 for UTM31N)  (useless if support image is given)",
                         "epsg",  1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("OriginX", "OutputOrigin[0] (optional if support image is given )",
                        "orx", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("OriginY", "OutputOrigin[1] (optional, useless if support image is given)",
                        "ory", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("SpacingX", "OutputSpacing[0] (optional, useless if support image is given )",
                        "spx", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("SpacingY", "OutputSpacing[1] (optional, useless if support image is given)",
                        "spy", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("AvailableMemory", "Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)",
                        "ram", 1, false, otb::ApplicationDescriptor::Integer);
  
  return EXIT_SUCCESS;
}

int Rasterization::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Images
  //typedef itk::RGBAPixel<unsigned char>                   PixelType;
  typedef unsigned char PixelType;
  typedef otb::Image<PixelType, 2> ImageType;
  typedef ImageType::PointType PointType;
  typedef ImageType::SizeType SizeType;
  typedef ImageType::SpacingType SpacingType;
  typedef ImageType::IndexType IndexType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;

  // VectorData
  typedef otb::VectorData<> VectorDataType;
  typedef VectorDataType::DataNodeType DataNodeType;
  typedef VectorDataType::DataTreeType DataTreeType;
  typedef DataNodeType::PointType PointType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> VectorDataWriterType;
  typedef VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<VectorDataType> VectorDataExtractROIType;
  typedef VectorDataProperties<VectorDataType> VectorDataPropertiesType;

  // Image

  typedef otb::ImageFileReader<ImageType> ImageReaderType;

  // Rasterization
  typedef otb::VectorDataToImageFilter<VectorDataType, ImageType> VectorDataToImageFilterType;

  // Misc
  typedef otb::RemoteSensingRegion<double> RemoteSensingRegionType;
  typedef RemoteSensingRegionType::SizeType SizePhyType;
  typedef otb::PipelineMemoryPrintCalculator MemoryCalculatorType;

  ImageReaderType::Pointer imReader;
  ImageType::Pointer referenceImage;

  // Reading the VectorData
  std::string vdFilename = parseResult->GetParameterString("InputVData");
  std::cout << "Processing vector data : " << vdFilename << std::endl;
  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  vdReader->SetFileName(vdFilename);
  vdReader->Update();

  // region information
  SizeType size;
  PointType origin;
  SpacingType spacing;
  SizePhyType sizePhy;

  // reading projection information
  // two choice :
  std::string outputProjectionRef;
  // a reference image is given as input
  if (parseResult->IsOptionPresent("InputImage"))
    {
    if (parseResult->IsOptionPresent("SizeX") || parseResult->IsOptionPresent("SizeY")
        || parseResult->IsOptionPresent("OriginX") || parseResult->IsOptionPresent("OriginY")
        || parseResult->IsOptionPresent("SpacingX") || parseResult->IsOptionPresent("SpacingY")
        || parseResult->IsOptionPresent("RSID"))
      {
      std::cerr << "Input entry problem " << std::endl
          << " reference image can't be used with other projection parameters. " << std::endl;
      return EXIT_FAILURE;
      }

    std::string imFilename = parseResult->GetParameterString("InputImage");

    imReader = ImageReaderType::New();
    imReader->SetFileName(imFilename);
    imReader->Update();

    referenceImage = imReader->GetOutput();
    outputProjectionRef = referenceImage->GetProjectionRef();

    size[0] = referenceImage->GetLargestPossibleRegion().GetSize(0);
    size[1] = referenceImage->GetLargestPossibleRegion().GetSize(1);

    origin = referenceImage->GetOrigin();

    spacing = referenceImage->GetSpacing();

    sizePhy[0] = size[0] * spacing[0];
    sizePhy[1] = size[1] * spacing[1];

    }

  else
    if (parseResult->IsOptionPresent("SizeX") && parseResult->IsOptionPresent("SizeY"))
      {

      if (parseResult->IsOptionPresent("RSID"))
        {
        unsigned int RSID = parseResult->GetParameterInt("RSID");
        outputProjectionRef = otb::GeoInformationConversion::ToWKT(RSID);
        }
      else
        {
        outputProjectionRef = vdReader->GetOutput()->GetProjectionRef();
        }

      VectorDataPropertiesType::Pointer vdProperties = VectorDataPropertiesType::New();
      vdProperties->SetVectorDataObject(vdReader->GetOutput());
      vdProperties->ComputeBoundingRegion();

      size[0] = parseResult->GetParameterDouble("SizeX");
      size[1] = parseResult->GetParameterDouble("SizeY");

      if (parseResult->IsOptionPresent("OriginX") && parseResult->IsOptionPresent("OriginY"))
        {
        origin[0] = parseResult->GetParameterDouble("OriginX");
        origin[1] = parseResult->GetParameterDouble("OriginY");
        }
      else
        {
        origin = vdProperties->GetBoundingRegion().GetIndex();
        }

      if (parseResult->IsOptionPresent("SpacingX") && parseResult->IsOptionPresent("SpacingY"))
        {
        spacing[0] = parseResult->GetParameterDouble("SpacingX");
        spacing[1] = parseResult->GetParameterDouble("SpacingY");
        }
      else
        {
        spacing[0] = vdProperties->GetBoundingRegion().GetSize()[0] / size[0];
        spacing[1] = vdProperties->GetBoundingRegion().GetSize()[1] / size[1];
        }

      sizePhy[0] = size[0] * spacing[0];
      sizePhy[1] = size[1] * spacing[1];

      }
    else
      {
      std::cerr << "Input entry problem " << std::endl << " if no reference image given, region size is needed "
          << std::endl;
      return EXIT_FAILURE;
      }

  // Reprojecting the VectorData

  VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
  vproj->SetInput(vdReader->GetOutput());

  //
  vproj->SetInputProjectionRef(vdReader->GetOutput()->GetProjectionRef());

  vproj->SetOutputProjectionRef(outputProjectionRef);
  if (parseResult->IsOptionPresent("InputImage"))
    {
    vproj->SetOutputKeywordList(referenceImage->GetImageKeywordlist());
    }

  RemoteSensingRegionType region;
  region.SetSize(sizePhy);
  region.SetOrigin(origin);
  region.SetRegionProjection(outputProjectionRef);
  if (parseResult->IsOptionPresent("InputImage"))
    {
    region.SetKeywordList(referenceImage->GetImageKeywordlist());
    }
  VectorDataExtractROIType::Pointer vdextract = VectorDataExtractROIType::New();
  vdextract->SetRegion(region);

  vdextract->SetInput(vproj->GetOutput());
  VectorDataToImageFilterType::Pointer vectorDataRendering = VectorDataToImageFilterType::New();
  vectorDataRendering->SetInput(vdextract->GetOutput());
  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetVectorDataProjectionWKT(outputProjectionRef);
  vectorDataRendering->SetRenderingStyleType(VectorDataToImageFilterType::Binary);

  ImageType::Pointer outputImage = vectorDataRendering->GetOutput();
  if (parseResult->IsOptionPresent("InputImage"))
    {
    outputImage->SetMetaDataDictionary(referenceImage->GetMetaDataDictionary());
    }

  // Instantiate the writer
  std::string oFilename = parseResult->GetParameterString("OutputImage");

  WriterType::Pointer oWriter = WriterType::New();
  oWriter->SetFileName(oFilename);
  oWriter->SetInput(outputImage);

  unsigned int ram = 256;
  if (parseResult->IsOptionPresent("AvailableMemory"))
    {
    ram = parseResult->GetParameterUInt("AvailableMemory");
    }
  oWriter->SetAutomaticTiledStreaming(ram);

  otb::StandardWriterWatcher watcher(oWriter, "Rasterization");
  oWriter->Update();

  return EXIT_SUCCESS;
}
}
