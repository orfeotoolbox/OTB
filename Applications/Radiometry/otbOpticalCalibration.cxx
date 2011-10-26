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
  Level_TOA,
  Level_TOC
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
                                           FloatVectorImageType>        ScaleFilterType;

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
    std::ostringstream oss;
    oss << "Perform optical calibration TOA/TOC (Top Of Atmosphere/Top Of Canopy)" << std::endl;
    oss << "Supported sensors : QuickBird, Ikonos, WorldView2, Formosat, Spot5";
    SetDescription(oss.str());
    // Documentation
    SetDocName("Optical calibration application");
    SetDocLongDescription(oss.str());
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine OpticalCalibration ${OTB-BIN}/bin "
      "--in --out --milli --level toa");
    AddDocTag("Calibration");
}

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",  "Input Image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Calibrated image");

    AddParameter(ParameterType_Empty, "milli", "Convert to milli reflectance");
    SetParameterDescription("milli", "Output milli-reflectance instead of reflectance.\n"
                                     "This allows to put save the image in integer pixel type instead of floating point.");
    DisableParameter("milli");
    MandatoryOff("milli");

    AddParameter(ParameterType_Choice,   "level", "Calibration Level");
    AddChoice("level.toa",     "TOA : Top Of Atmosphere");
    AddChoice("level.toc",     "TOC : Top Of Canopy (EXPERIMENTAL)");
    SetParameterString("level", "toa");
    
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

    SetDefaultParameterFloat("oz", 0.);
    SetDefaultParameterFloat("wa",  2.5);
    SetDefaultParameterFloat("atmo", 1030.);

    SetDefaultParameterFloat("opt", 0.2);
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

    // Test if needed data are available : an exception will be thrown
    // if one the following Get* return failure. the exception is then
    // caught in the Wrapper::Application class which redirect it to
    // the logger
    // ImageToLuminance
    lImageMetadataInterface->GetPhysicalGain();
    lImageMetadataInterface->GetPhysicalBias();

    // LuminanceToReflectance
    lImageMetadataInterface->GetDay();
    lImageMetadataInterface->GetMonth();
    
    lImageMetadataInterface->GetSolarIrradiance();
    lImageMetadataInterface->GetSunElevation();

    m_ImageToLuminanceFilter                = ImageToLuminanceImageFilterType::New();
    m_LuminanceToReflectanceFilter          = LuminanceToReflectanceImageFilterType::New();
    m_ReflectanceToSurfaceReflectanceFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();

    m_ImageToLuminanceFilter->SetInput(inImage);
    m_LuminanceToReflectanceFilter->SetInput(m_ImageToLuminanceFilter->GetOutput());
    m_ReflectanceToSurfaceReflectanceFilter->SetInput(m_LuminanceToReflectanceFilter->GetOutput());

    m_ScaleFilter = ScaleFilterType::New();
    m_ScaleFilter->InPlaceOn();

    switch ( GetParameterInt("level") )
      {
      case Level_TOA:
      {
      m_LuminanceToReflectanceFilter->UpdateOutputInformation();
      m_ScaleFilter->SetInput(m_LuminanceToReflectanceFilter->GetOutput());
      }
      break;
      case Level_TOC:
      {
      m_AtmosphericParam = m_ReflectanceToSurfaceReflectanceFilter->GetCorrectionParameters();
      AerosolModelType aeroMod = AtmosphericCorrectionParametersType::NO_AEROSOL;

      switch ( GetParameterInt("aerosol") )
        {
        case Aerosol_Desertic:
        {
        // Aerosol_Desertic correspond to 4 in the enum but actually in
        // the class atmosphericParam it is known as parameter 5
        m_AtmosphericParam->SetAerosolModel(static_cast<AerosolModelType>(5));
        }
        break;
        default:
        {
        m_AtmosphericParam->SetAerosolModel(static_cast<AerosolModelType>(GetParameterInt("aerosol")));
        }
        break;
        }
      // Set the atmospheric param 
      m_AtmosphericParam->SetOzoneAmount(GetParameterFloat("oz"));
      m_AtmosphericParam->SetWaterVaporAmount(GetParameterFloat("wa"));
      m_AtmosphericParam->SetAtmosphericPressure(GetParameterFloat("atmo"));
      m_AtmosphericParam->SetAerosolOptical(GetParameterFloat("opt"));
      
      // Relative Spectral Response File
      if (IsParameterEnabled("rsr"))
        {
        m_ReflectanceToSurfaceReflectanceFilter->SetFilterFunctionValuesFileName(GetParameterString("rsr"));
        }
      else
        {
        m_ReflectanceToSurfaceReflectanceFilter->SetFilterFunctionCoef(lImageMetadataInterface->GetSpectralSensitivity());
        }

      // Aeronet file
      if (IsParameterEnabled("aeronet"))
        {
        m_ReflectanceToSurfaceReflectanceFilter->SetAeronetFileName(GetParameterString("AeronetFile"));
        }
      
      // 
      AtmosphericRadiativeTerms::Pointer radTerms = AtmosphericRadiativeTerms::New();
      radTerms->ValuesInitialization(inImage->GetNumberOfComponentsPerPixel());
      m_ReflectanceToSurfaceReflectanceFilter->SetAtmosphericRadiativeTerms(radTerms);
      m_ReflectanceToSurfaceReflectanceFilter->SetIsSetAtmosphericRadiativeTerms(true);
      m_ReflectanceToSurfaceReflectanceFilter->GenerateAtmosphericRadiativeTerms();
      m_ReflectanceToSurfaceReflectanceFilter->GenerateParameters();
      m_ReflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(false);

      //rescale the surface reflectance in milli-reflectance
      m_ReflectanceToSurfaceReflectanceFilter->UpdateOutputInformation();
      m_ScaleFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
      }
      break;
      }

    // Output Image
    const double scale = IsParameterEnabled("milli") ? 1000.0 : 1.0;
    m_ScaleFilter->SetCoef(scale);

    SetParameterOutputImage("out", m_ScaleFilter->GetOutput());
  }

  ImageToLuminanceImageFilterType ::Pointer               m_ImageToLuminanceFilter;
  LuminanceToReflectanceImageFilterType::Pointer          m_LuminanceToReflectanceFilter;
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer m_ReflectanceToSurfaceReflectanceFilter;
  ScaleFilterType::Pointer                                m_ScaleFilter;
  AtmosphericCorrectionParametersType::Pointer            m_AtmosphericParam;
};

}// namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OpticalCalibration)

  
  
