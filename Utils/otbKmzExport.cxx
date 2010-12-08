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

#include "otbKmzExport.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbKmzProductWriter.h"

namespace otb
{

int KmzExport::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("KmzExport");
  descriptor->SetDescription("Chain that Estimate a sensor model in order to export the input image to Google Earth understandable format Kmz");
  descriptor->AddInputImage(); 
  descriptor->AddOption("OutputProductName", "Output Kmz product ", "kmz", 1, true,ApplicationDescriptor::FileName);
  descriptor->AddOption("TileSize", "Set the size of the tiles ", "s", 1, false,ApplicationDescriptor::Integer);
  descriptor->AddOption("LogoImage", "Add a logo to the final Kmz ", "lo", 1, false,ApplicationDescriptor::FileName);
  descriptor->AddOption("LegendImage", "Add a legend to the image exported ", "le", 1, false,ApplicationDescriptor::FileName);
 
  return EXIT_SUCCESS;
}

int KmzExport::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorImage<float, 2>                      ImageType;
  typedef otb::KmzProductWriter<ImageType>                KmzProductWriterType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;

  // Instanciate reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());
  reader->UpdateOutputInformation();
  
  // Second part : Image To Kmz
  KmzProductWriterType::Pointer    kmzWriter  = KmzProductWriterType::New();
  kmzWriter->SetInput(reader->GetOutput());
  kmzWriter->SetPath(parseResult->GetParameterString("OutputProductName"));

  // If the tilesize is set
  if(parseResult->IsOptionPresent("TileSize"))
    {
    kmzWriter->SetTileSize(parseResult->GetParameterUInt("TileSize"));
    }
  // Read the logo if any
  if(parseResult->IsOptionPresent("LogoImage"))
    {
    ReaderType::Pointer logoReader  = ReaderType::New();
    logoReader->SetFileName(parseResult->GetParameterString("LogoImage"));
    logoReader->Update();
    kmzWriter->SetLogo(logoReader->GetOutput());
    }
  // Read the legend if any
  if(parseResult->IsOptionPresent("LegendImage"))
    {
    ReaderType::Pointer legendReader  = ReaderType::New();
    legendReader->SetFileName(parseResult->GetParameterString("LegendImage"));
    legendReader->Update();
    kmzWriter->AddLegend("Input Legend",legendReader->GetOutput());
    kmzWriter->AddLegend(legendReader->GetOutput());
    } 
  
  // trigger the writing
  kmzWriter->Update();

  return EXIT_SUCCESS;
}
}
