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
#include "otbMultiplyByScalarImageFilter.h"

namespace otb
{

int OpticalCalibration::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("OpticalCalibration");
  descriptor->SetDescription("Perform optical calibration TOA/TOC(Top Of Atmosphere/Top Of Canopy). Output image is in milli-reflectance.");
  descriptor->AddInputImage();
  descriptor->AddOutputImage();
  descriptor->AddOptionNParams("Level",
                               "Level of calibration TOA(Top Of Atmosphere) or TOC(Top Of Canopy) (default is TOA)",
                               "level", false, otb::ApplicationDescriptor::String);
  descriptor->AddOption("RelativeSpectralResponseFile","Sensor relative spectral response file(by default the application gets these informations in the metadata)","rsr", 1, false, otb::ApplicationDescriptor::FileName);
  descriptor->AddOption("AerosolModel","AerosolModel: NO_AEROSOL(0), CONTINENTAL(1), MARITIME(2), URBAN(3), DESERTIC(5); default 0","aerosol", 1, false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOption("OzoneAmount","Amount of Ozone ","oz", 1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("WaterVaporAmount","WaterVaporAmount ","wa", 1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("AtmosphericPressure","Atmospheric pressure ","atmo", 1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("AerosolOptical","AerosolOptical ","opt", 1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("AeronetFile","Aeronet file to get atmospheric parameters","aeronet", 1, false, otb::ApplicationDescriptor::FileName);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram", 1, false, otb::ApplicationDescriptor::Integer);
  return EXIT_SUCCESS;
}

int OpticalCalibration::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorImage<unsigned short int, 2>       ImageType;
  typedef otb::VectorImage<float, 2>                     FloatImageType;
  typedef otb::ImageFileReader<ImageType>               ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType>      WriterType;

  typedef ImageToLuminanceImageFilter<ImageType, FloatImageType> ImageToLuminanceImageFilterType;
  typedef LuminanceToReflectanceImageFilter<FloatImageType,
      FloatImageType>                      LuminanceToReflectanceImageFilterType;
  typedef otb::MultiplyByScalarImageFilter<FloatImageType, ImageType> ScaleFilterType;
  typedef ReflectanceToSurfaceReflectanceImageFilter<FloatImageType,
      FloatImageType>             ReflectanceToSurfaceReflectanceImageFilterType;
  typedef ReflectanceToSurfaceReflectanceImageFilterType::FilterFunctionValuesType  FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                                ValuesVectorType;
  typedef AtmosphericCorrectionParameters AtmosphericCorrectionParametersType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType AerosolModelType;

  // Read input image information
  ReaderType::Pointer reader=ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage().c_str());
  reader->GenerateOutputInformation();

  //Check if valid metadata informations are available to compute ImageToLuminance and LuminanceToReflectance
  itk::MetaDataDictionary             dict = reader->GetOutput()->GetMetaDataDictionary();
  OpticalImageMetadataInterface::Pointer lImageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);
  // Test if needed data are available.
  try
  {
    // ImageToLuminance
    lImageMetadataInterface->GetPhysicalGain();
    lImageMetadataInterface->GetPhysicalBias();

    // LuminanceToReflectance
    lImageMetadataInterface->GetDay();
    lImageMetadataInterface->GetMonth();

    lImageMetadataInterface->GetSolarIrradiance();
    lImageMetadataInterface->GetSunElevation();

  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "Invalid input image medadata. The parsing returns the following error:\n" << err << std::endl;
    return EXIT_FAILURE;
  }

  ImageToLuminanceImageFilterType ::Pointer imageToLuminanceFilter                = ImageToLuminanceImageFilterType::New();
  LuminanceToReflectanceImageFilterType::Pointer luminanceToReflectanceFilter          = LuminanceToReflectanceImageFilterType::New();
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer reflectanceToSurfaceReflectanceFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();

  imageToLuminanceFilter->SetInput(reader->GetOutput());
  luminanceToReflectanceFilter->SetInput(imageToLuminanceFilter->GetOutput());
  reflectanceToSurfaceReflectanceFilter->SetInput(luminanceToReflectanceFilter->GetOutput());

  ScaleFilterType::Pointer scaleFilter = ScaleFilterType::New();
  scaleFilter->SetCoef(1000.);

  if(parseResult->GetParameterString("Level") == "toc")
    {
    AtmosphericCorrectionParametersType::Pointer atmosphericParam = reflectanceToSurfaceReflectanceFilter->GetCorrectionParameters();

    AerosolModelType aeroMod = AtmosphericCorrectionParametersType::NO_AEROSOL;

    if(parseResult->IsOptionPresent("AerosolModel"))
      {
      aeroMod = static_cast<AerosolModelType>(parseResult->GetParameterUInt("AerosolModel"));
      }
    atmosphericParam->SetAerosolModel(static_cast<AerosolModelType>(aeroMod));

    double ozoneAmount = 0.;
    double waterVaporAmount = 2.5;
    double atmosphericPressure = 1030.;
    double aerosolOptical = 0.2;

    if (parseResult->IsOptionPresent("OzoneAmount"))
      {
      ozoneAmount = parseResult->GetParameterFloat("OzoneAmount");
      }
    atmosphericParam->SetOzoneAmount(ozoneAmount);

    if (parseResult->IsOptionPresent("WaterVaporAmount"))
      {
      waterVaporAmount = parseResult->GetParameterFloat("WaterVaporAmount");
      }
    atmosphericParam->SetWaterVaporAmount(waterVaporAmount);

    if (parseResult->IsOptionPresent("AtmosphericPressure"))
      {
      atmosphericPressure = parseResult->GetParameterFloat("AtmosphericPressure");
      }
    atmosphericParam->SetAtmosphericPressure(atmosphericPressure);

    if (parseResult->IsOptionPresent("AerosolOptical"))
      {
      aerosolOptical = parseResult->GetParameterFloat("AerosolOptical");
      }
    atmosphericParam->SetAerosolOptical(aerosolOptical);

    if (parseResult->IsOptionPresent("RelativeSpectralResponseFile"))
      {
      reflectanceToSurfaceReflectanceFilter->SetFilterFunctionValuesFileName(parseResult->GetParameterString("RelativeSpectralResponseFile", 0));
      }
    else
      {
      atmosphericParam->SetWavelengthSpectralBand(lImageMetadataInterface->GetSpectralSensitivity());
      }

    if (parseResult->IsOptionPresent("AeronetFile"))
      {
      reflectanceToSurfaceReflectanceFilter->SetAeronetFileName(parseResult->GetParameterString("AeronetFile", 0));
      }

    AtmosphericRadiativeTerms::Pointer radTerms = AtmosphericRadiativeTerms::New();
    radTerms->ValuesInitialization(reader->GetOutput()->GetNumberOfComponentsPerPixel());
    reflectanceToSurfaceReflectanceFilter->SetAtmosphericRadiativeTerms(radTerms);

    reflectanceToSurfaceReflectanceFilter->SetIsSetAtmosphericRadiativeTerms(true);
    reflectanceToSurfaceReflectanceFilter->GenerateAtmosphericRadiativeTerms();
    reflectanceToSurfaceReflectanceFilter->GenerateParameters();
    reflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(false);

    //rescale the surface reflectance in milli-reflectance
    scaleFilter->SetInput(reflectanceToSurfaceReflectanceFilter->GetOutput());
    }
  else
    {
    //Rescale luminanceToReflectance filter output (TOA level)
    scaleFilter->SetInput(luminanceToReflectanceFilter->GetOutput());
    }

  //Instantiate the writer
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(parseResult->GetOutputImage());
  writer->SetInput(scaleFilter->GetOutput());
  writer->SetWriteGeomFile(true);

  unsigned int ram = 256;
  if (parseResult->IsOptionPresent("AvailableMemory"))
    {
    ram = parseResult->GetParameterUInt("AvailableMemory");
    }
  writer->SetAutomaticTiledStreaming(ram);

  otb::StandardWriterWatcher watcher(writer,"OpticalCalibration");
  writer->Update();

  return EXIT_SUCCESS;
}
}
