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
                                      DoubleVectorImageType>              ImageToLuminanceImageFilterType;
  
  typedef LuminanceToReflectanceImageFilter<DoubleVectorImageType,
                                            DoubleVectorImageType>        LuminanceToReflectanceImageFilterType;

  typedef otb::MultiplyByScalarImageFilter<DoubleVectorImageType,
                                           DoubleVectorImageType>         ScaleFilterType;

  typedef ReflectanceToSurfaceReflectanceImageFilter<DoubleVectorImageType,
                                                     DoubleVectorImageType>         ReflectanceToSurfaceReflectanceImageFilterType;
  typedef ReflectanceToSurfaceReflectanceImageFilterType::FilterFunctionValuesType  FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                                ValuesVectorType;
  typedef AtmosphericCorrectionParameters                                           AtmosphericCorrectionParametersType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType                     AerosolModelType;

private:
  void DoInit()
  {
    SetName("OpticalCalibration");
    SetDescription("Perform optical calibration TOA/TOC (Top Of Atmosphere/Top Of Canopy). Supported sensors: QuickBird, Ikonos, WorldView2, Formosat, Spot5");
    // Documentation
    SetDocName("Optical calibration");
    SetDocLongDescription("The application allows to convert pixel values from DN (for Digital Numbers) to physically interpretable and comparable values.Calibrated values are called surface reflectivity and its values lie in the range [0, 1].\nThe first level is called Top Of Atmosphere (TOA) reflectivity. It takes into account the sensor gain, sensor spectral response and the solar illumination.\nThe second level is called Top Of Canopy (TOC) reflectivity. In addition to sensor gain and solar illumination, it takes into account the optical thickness of the atmosphere, the atmospheric pressure, the water vapor amount, the ozone amount, as well as the composition and amount of aerosol gasses.\nIt is also possible to indicate an AERONET file which contains atmospheric parameters (version 1 and version 2 of Aeronet file are supported.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("The OTB CookBook");
 
    AddDocTag(Tags::Calibration);

    AddParameter(ParameterType_InputImage,  "in",  "Input Image Filename");

    AddParameter(ParameterType_OutputImage, "out", "Output Image Filename");
    SetParameterDescription("out","Calibrated Image Filename");

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

    AddParameter(ParameterType_Choice,   "level", "Calibration Level");
    AddChoice("level.toa",     "TOA : Top Of Atmosphere");
    AddChoice("level.toc",     "TOC : Top Of Canopy (EXPERIMENTAL)");
    SetParameterString("level", "toa");

    AddParameter(ParameterType_Empty, "milli", "Convert to milli reflectance");
    SetParameterDescription("milli", "Output milli-reflectance instead of reflectance.\n"
                                     "This allows to put save the image in integer pixel type (in the range [0, 1000]  instead of floating point in the range [0, 1]. In order to do that, use this option and set the output pixel type (-out filename uint16 for example)");
    DisableParameter("milli");
    MandatoryOff("milli");

    AddParameter(ParameterType_Filename,   "rsr", "Relative Spectral Response File");
    std::ostringstream oss;
    oss << "Sensor relative spectral response file"<<std::endl;
    oss << "By default the application gets these informations in the metadata";
    SetParameterDescription("rsr", oss.str());
    MandatoryOff("rsr");

    AddParameter(ParameterType_Group,"atmo","Atmospheric parameters");
    SetParameterDescription("atmo","This group allows to set the atmospheric parameters.");
    AddParameter(ParameterType_Choice,   "atmo.aerosol", "Aerosol Model");
    AddChoice("atmo.aerosol.noaersol",    "No Aerosol Model");
    AddChoice("atmo.aerosol.continental", "Continental");
    AddChoice("atmo.aerosol.maritime",    "Maritime");
    AddChoice("atmo.aerosol.urban",       "Urban");
    AddChoice("atmo.aerosol.desertic",    "Desertic");

    AddParameter(ParameterType_Float, "atmo.oz",   "Ozone Amount");
    AddParameter(ParameterType_Float, "atmo.wa",   "Water Vapor Amount");
    AddParameter(ParameterType_Float, "atmo.pressure", "Atmospheric Pressure");
    AddParameter(ParameterType_Float, "atmo.opt",  "Aerosol Optical Thickness");

    SetDefaultParameterFloat("atmo.oz", 0.);
    SetDefaultParameterFloat("atmo.wa",  2.5);
    SetDefaultParameterFloat("atmo.pressure", 1030.);

    SetDefaultParameterFloat("atmo.opt", 0.2);
    MandatoryOff("atmo.oz");
    MandatoryOff("atmo.wa");
    MandatoryOff("atmo.pressure");
    MandatoryOff("atmo.opt");

    AddParameter(ParameterType_Filename,   "atmo.aeronet", "Aeronet File");
    SetParameterDescription("atmo.aeronet","Aeronet file containing atmospheric parameters");
    MandatoryOff("atmo.aeronet");

   // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("level", "toa");
    SetDocExampleParameterValue("out", "OpticalCalibration.tif");
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
      //AerosolModelType aeroMod = AtmosphericCorrectionParametersType::NO_AEROSOL;

      switch ( GetParameterInt("atmo.aerosol") )
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
        m_AtmosphericParam->SetAerosolModel(static_cast<AerosolModelType>(GetParameterInt("atmo.aerosol")));
        }
        break;
        }
      // Set the atmospheric param
      m_AtmosphericParam->SetOzoneAmount(GetParameterFloat("atmo.oz"));
      m_AtmosphericParam->SetWaterVaporAmount(GetParameterFloat("atmo.wa"));
      m_AtmosphericParam->SetAtmosphericPressure(GetParameterFloat("atmo.pressure"));
      m_AtmosphericParam->SetAerosolOptical(GetParameterFloat("atmo.opt"));
      
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
      if (IsParameterEnabled("atmo.aeronet"))
        {
        m_ReflectanceToSurfaceReflectanceFilter->SetAeronetFileName(GetParameterString("atmo.aeronet"));
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
