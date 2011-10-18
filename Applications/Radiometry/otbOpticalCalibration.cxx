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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbImageToLuminanceImageFilter.h"
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"

namespace otb
{

enum
{
  Level_TOC,
  Level_TOA
};

enum
{
  Aerosol_NoAerosol,
  Aerosol_Continental,
  Aerosol_Maritime,
  Aerosol_Urban,
  Aerosol_Desertic,
};

namespace Wrapper
{

class OpticalCalibration : public Application
{

public:
/** Standard class typedefs. */
  typedef OpticalCalibration            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(OpticalCalibration, Application);

  typedef ImageToLuminanceImageFilter<UInt16VectorImageType, 
                                      FloatVectorImageType>              ImageToLuminanceImageFilterType;
  
  typedef LuminanceToReflectanceImageFilter<FloatVectorImageType,
                                            FloatVectorImageType>        LuminanceToReflectanceImageFilterType;

  typedef otb::MultiplyByScalarImageFilter<FloatVectorImageType, 
                                           UInt16VectorImageType>        ScaleFilterType;

  typedef ReflectanceToSurfaceReflectanceImageFilter<FloatVectorImageType,
                                                     FloatVectorImageType>          ReflectanceToSurfaceReflectanceImageFilterType;
  typedef ReflectanceToSurfaceReflectanceImageFilterType::FilterFunctionValuesType  FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                                ValuesVectorType;
  typedef AtmosphericCorrectionParameters                                           AtmosphericCorrectionParametersType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType                     AerosolModelType;

private:
  OpticalCalibration()
  {
    SetName("OpticalCalibration");
    SetDescription("Perform optical calibration TOA/TOC(Top Of Atmosphere/Top Of Canopy). Output image is in milli-reflectance.");
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",  "Input Image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Projected image");

    AddParameter(ParameterType_Choice,   "level", "Calibration Level");
    AddChoice("level.toa",     "TOA : Top Of Atmosphere");
    AddChoice("level.toc",     "TOC : Top Of Canopy");
    
    AddParameter(ParameterType_Filename,   "rsr", "Relative Spectral Response File");
    std::ostringstream oss;
    oss << "Sensor relative spectral response file"<<std::endl;
    oss << "By default the application gets these informations in the metadata";
    SetParameterDescription("rsr",oss.str());
    MandatoryOff("rsr");

    AddParameter(ParameterType_Choice,   "aerosol", "Aerosol Model");
    AddChoice("aerosol.noaersol",    "No Aerosol");
    AddChoice("aerosol.continental", "Continental");
    AddChoice("aerosol.maritime",    "Maritime");
    AddChoice("aerosol.urban",       "Urban");
    AddChoice("aerosol.desertic",    "Desertic");

    AddParameter(ParameterType_Float, "oz",   "Amount of Ozone");
    AddParameter(ParameterType_Float, "wa",   "Water Vapor Amount");
    AddParameter(ParameterType_Float, "atmo", "Atmospheric Pressure");
    AddParameter(ParameterType_Float, "opt",  "Aerosol Optical");

    SetParameterFloat("oz", 0.);
    SetParameterFloat("wa",  2.5);
    SetParameterFloat("atmo", 1030.);

    SetParameterFloat("opt", 0.2);
    MandatoryOff("oz");
    MandatoryOff("wa");
    MandatoryOff("atmo");
    MandatoryOff("opt");

    AddParameter(ParameterType_Filename,   "aeronet", "Aeronet File");
    SetParameterDescription("aeronet","Aeronet file to get atmospheric parameters");
    MandatoryOff("aeronet");
  }

  void DoUpdateParameters()
  {
    // Nothing to update
  }

  void DoExecute()
  {
    UInt16VectorImageType::Pointer inImage = GetParameterUInt16VectorImage("in");

    //Check if valid metadata informations are available to compute ImageToLuminance and LuminanceToReflectance
    itk::MetaDataDictionary             dict = inImage->GetMetaDataDictionary();
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
      itkGenericExceptionMacro("Invalid input image medadata. The parsing returns the following error");
      }

    ImageToLuminanceImageFilterType ::Pointer imageToLuminanceFilter                = ImageToLuminanceImageFilterType::New();
    LuminanceToReflectanceImageFilterType::Pointer luminanceToReflectanceFilter     = LuminanceToReflectanceImageFilterType::New();
    ReflectanceToSurfaceReflectanceImageFilterType::Pointer reflectanceToSurfaceReflectanceFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();

    imageToLuminanceFilter->SetInput(inImage);
    luminanceToReflectanceFilter->SetInput(imageToLuminanceFilter->GetOutput());
    reflectanceToSurfaceReflectanceFilter->SetInput(luminanceToReflectanceFilter->GetOutput());

    ScaleFilterType::Pointer scaleFilter = ScaleFilterType::New();
    scaleFilter->SetCoef(1000.);

    
    switch ( GetParameterInt("level") )
      {
      case Level_TOA:
      {
      AtmosphericCorrectionParametersType::Pointer atmosphericParam = reflectanceToSurfaceReflectanceFilter->GetCorrectionParameters();
      AerosolModelType aeroMod = AtmosphericCorrectionParametersType::NO_AEROSOL;

      switch ( GetParameterInt("aerosol") )
        {
        case Aerosol_Desertic:
        {
        // Aerosol_Desertic correspond to 4 in the enum but actually in
        // the class atmosphericParam it is known as parameter 5
        atmosphericParam->SetAerosolModel(static_cast<AerosolModelType>(5));
        }
        break;
        default:
        {
        atmosphericParam->SetAerosolModel(static_cast<AerosolModelType>(GetParameterInt("aerosol")));
        }
        break;
        }

      // Set the atmospheric param 
      atmosphericParam->SetOzoneAmount(GetParameterFloat("oz"));
      atmosphericParam->SetWaterVaporAmount(GetParameterFloat("wa"));
      atmosphericParam->SetAtmosphericPressure(GetParameterFloat("atmo"));
      atmosphericParam->SetAerosolOptical(GetParameterFloat("opt"));
      
      // Relative Spectral Response File
      if (IsParameterEnabled("rsr"))
        {
        reflectanceToSurfaceReflectanceFilter->SetFilterFunctionValuesFileName(GetParameterString("rsr"));
        }
      else
        {
        reflectanceToSurfaceReflectanceFilter->SetFilterFunctionCoef(lImageMetadataInterface->GetSpectralSensitivity());
        }

      // Aeronet file
      if (IsParameterEnabled("aeronet"))
        {
        reflectanceToSurfaceReflectanceFilter->SetAeronetFileName(GetParameterString("AeronetFile"));
        }
      
      // 
      AtmosphericRadiativeTerms::Pointer radTerms = AtmosphericRadiativeTerms::New();
      radTerms->ValuesInitialization(inImage->GetNumberOfComponentsPerPixel());
      reflectanceToSurfaceReflectanceFilter->SetAtmosphericRadiativeTerms(radTerms);

      reflectanceToSurfaceReflectanceFilter->SetIsSetAtmosphericRadiativeTerms(true);
      reflectanceToSurfaceReflectanceFilter->GenerateAtmosphericRadiativeTerms();
      reflectanceToSurfaceReflectanceFilter->GenerateParameters();
      reflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(false);

      //rescale the surface reflectance in milli-reflectance
      scaleFilter->SetInput(reflectanceToSurfaceReflectanceFilter->GetOutput());      
      }
      break;
      case Level_TOC:
      {
      scaleFilter->SetInput(luminanceToReflectanceFilter->GetOutput());
      }
      break;
      }

    // Output Image 
    SetParameterOutputImage("out", scaleFilter->GetOutput());
  }
};

}// namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OpticalCalibration)

  
  
