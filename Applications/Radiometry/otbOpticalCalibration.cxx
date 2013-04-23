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
#include "otbClampVectorImageFilter.h"
#include "otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter.h"
#include "otbGroundSpacingImageFunction.h"
#include "vnl/vnl_random.h"

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

  typedef otb::ClampVectorImageFilter<DoubleVectorImageType,
                                      DoubleVectorImageType>              ClampFilterType;

  typedef ReflectanceToSurfaceReflectanceImageFilter<DoubleVectorImageType,
                                                     DoubleVectorImageType>         ReflectanceToSurfaceReflectanceImageFilterType;
  typedef ReflectanceToSurfaceReflectanceImageFilterType::FilterFunctionValuesType  FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                                ValuesVectorType;
  typedef AtmosphericCorrectionParameters                                           AtmosphericCorrectionParametersType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType                     AerosolModelType;

  typedef otb::SurfaceAdjacencyEffect6SCorrectionSchemeFilter<DoubleVectorImageType,DoubleVectorImageType>
  SurfaceAdjacencyEffect6SCorrectionSchemeFilterType;

  typedef otb::GroundSpacingImageFunction<UInt16VectorImageType> GroundSpacingImageType;

  typedef UInt16VectorImageType::IndexType   IndexType;
  typedef GroundSpacingImageType::FloatType  FloatType;
  typedef GroundSpacingImageType::ValueType  ValueType;

  typedef IndexType::IndexValueType                IndexValueType;


private:
  void DoInit()
  {
    SetName("OpticalCalibration");
    SetDescription("Perform optical calibration TOA/TOC (Top Of Atmosphere/Top Of Canopy). Supported sensors: QuickBird, Ikonos, WorldView2, Formosat, Spot5, Pleiades");
    // Documentation
    SetDocName("Optical calibration");
    SetDocLongDescription("The application allows to convert pixel values from DN (for Digital Numbers) to physically interpretable and comparable values. Calibrated values are called surface reflectivity and its values lie in the range [0, 1].\nThe first level is called Top Of Atmosphere (TOA) reflectivity. It takes into account the sensor gain, sensor spectral response and the solar illumination.\nThe second level is called Top Of Canopy (TOC) reflectivity. In addition to sensor gain and solar illumination, it takes into account the optical thickness of the atmosphere, the atmospheric pressure, the water vapor amount, the ozone amount, as well as the composition and amount of aerosol gasses.\nIt is also possible to indicate an AERONET file which contains atmospheric parameters (version 1 and version 2 of Aeronet file are supported.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("The OTB CookBook");

    AddDocTag(Tags::Calibration);

    AddParameter(ParameterType_InputImage,  "in",  "Input");
    SetParameterDescription("in", "Input image filename (values in DN)");

    AddParameter(ParameterType_OutputImage, "out", "Output");
    SetParameterDescription("out","Output calibrated image filename");

    AddRAMParameter();

    AddParameter(ParameterType_Choice,   "level", "Calibration Level");
    AddChoice("level.toa",     "TOA : Top Of Atmosphere");
    AddChoice("level.toc",     "TOC : Top Of Canopy (EXPERIMENTAL)");
    SetParameterString("level", "toa");

    AddParameter(ParameterType_Empty, "milli", "Convert to milli reflectance");
    SetParameterDescription("milli", "Flag to use milli-reflectance instead of reflectance.\n"
                            "This allows to save the image with integer pixel type (in the range [0, 1000]  instead of floating point in the range [0, 1]. In order to do that, use this option and set the output pixel type (-out filename uint16 for example)");
    DisableParameter("milli");
    MandatoryOff("milli");

    AddParameter(ParameterType_Empty, "clamp", "Clamp of reflectivity values between [0% , 100%]");
    SetParameterDescription("clamp", "Clamping in the range [0% , 100%]. It can be usefull to preserve area with specular reflectance.");
    EnableParameter("clamp");
    MandatoryOff("clamp");

    AddParameter(ParameterType_InputFilename, "rsr", "Relative Spectral Response File");
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
    SetParameterDescription("atmo.oz", "Ozone Amount");

    AddParameter(ParameterType_Float, "atmo.wa",   "Water Vapor Amount");
    SetParameterDescription("atmo.wa", "Water Vapor Amount (in saturation fraction of water)");

    AddParameter(ParameterType_Float, "atmo.pressure", "Atmospheric Pressure");
    SetParameterDescription("atmo.pressure", "Atmospheric Pressure (in hPa)");

    AddParameter(ParameterType_Float, "atmo.opt",  "Aerosol Optical Thickness");
    SetParameterDescription("atmo.opt", "Aerosol Optical Thickness");

    SetDefaultParameterFloat("atmo.oz", 0.);
    SetDefaultParameterFloat("atmo.wa",  2.5);
    SetDefaultParameterFloat("atmo.pressure", 1030.);

    SetDefaultParameterFloat("atmo.opt", 0.2);
    MandatoryOff("atmo.oz");
    MandatoryOff("atmo.wa");
    MandatoryOff("atmo.pressure");
    MandatoryOff("atmo.opt");

    AddParameter(ParameterType_InputFilename, "atmo.aeronet", "Aeronet File");
    SetParameterDescription("atmo.aeronet","Aeronet file containing atmospheric parameters");
    MandatoryOff("atmo.aeronet");

    // Window radius for adjacency effects correction
    AddParameter(ParameterType_Int, "radius", "Window radius");
    SetParameterDescription("radius","Window radius for adjacency effects corrections");
    MandatoryOff("radius");
    SetDefaultParameterInt("radius", 2);

    AddRAMParameter();

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

    m_ClampFilter = ClampFilterType::New();

    switch ( GetParameterInt("level") )
      {
      case Level_TOA:
      {
      GetLogger()->Info("Compute Top of Atmosphere reflectance\n");
      m_LuminanceToReflectanceFilter->SetUseClamp(IsParameterEnabled("clamp"));

      m_LuminanceToReflectanceFilter->UpdateOutputInformation();
      m_ScaleFilter->SetInput(m_LuminanceToReflectanceFilter->GetOutput());
      }
      break;
      case Level_TOC:
      {
      GetLogger()->Info("Compute Top of Canopy reflectance\n");
      m_ReflectanceToSurfaceReflectanceFilter->SetIsSetAtmosphericRadiativeTerms(false);
      m_ReflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(true);
      m_ReflectanceToSurfaceReflectanceFilter->UpdateOutputInformation();
      m_ReflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(false);

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

      // Aeronet file
      if (IsParameterEnabled("atmo.aeronet"))
        {
        GetLogger()->Info("Use aeronet file to retrieve atmospheric parameters");
        m_ReflectanceToSurfaceReflectanceFilter->SetAeronetFileName(GetParameterString("atmo.aeronet"));
        }

      m_ReflectanceToSurfaceReflectanceFilter->SetIsSetAtmosphericRadiativeTerms(false);
      m_ReflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(true);
      m_ReflectanceToSurfaceReflectanceFilter->GenerateParameters();
      m_ReflectanceToSurfaceReflectanceFilter->UpdateOutputInformation();
      m_ReflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(false);

      // std::ostringstream oss_atmo;
      // oss_atmo << "Atmospheric parameters: " << std::endl;
      // oss_atmo << m_AtmosphericParam;

      // GetLogger()->Info(oss_atmo.str());

      std::ostringstream oss;
      oss.str("");
      oss << std::endl << m_AtmosphericParam;

      AtmosphericRadiativeTerms::Pointer atmoTerms =  m_ReflectanceToSurfaceReflectanceFilter->GetAtmosphericRadiativeTerms();
      oss << std::endl << std::endl << atmoTerms;

      GetLogger()->Info("Atmospheric correction parameters compute by 6S : " + oss.str());

      //Compute adjacency effect
      //   m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter
      //   = SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::New();

      //   m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
      // m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->
      //   SetAtmosphericRadiativeTerms(
      //     m_ReflectanceToSurfaceReflectanceFilter->GetAtmosphericRadiativeTerms());
      // m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->SetZenithalViewingAngle(
      //   m_AtmosphericParam->GetViewingZenithalAngle());
      // m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->SetWindowRadius(GetParameterInt("radius"));

      // //estimate ground spacing in kilometers
      // GroundSpacingImageType::Pointer groundSpacing = GroundSpacingImageType::New();

      // groundSpacing->SetInputImage(inImage);
      // IndexType  index;

      // vnl_random rand;

      // index[0] = static_cast<IndexValueType>(rand.lrand32(0, inImage->GetLargestPossibleRegion().GetSize()[0]));
      // index[1] = static_cast<IndexValueType>(rand.lrand32(0, inImage->GetLargestPossibleRegion().GetSize()[1]));
      // FloatType tmpSpacing = groundSpacing->EvaluateAtIndex(index);

      // const float spacingInKilometers = (std::max(tmpSpacing[0], tmpSpacing[1])) / 1000.;

      // // std::ostringstream oss2;
      // //  oss2.str("");
      // //  oss2 << spacingInKilometers;

      // //  GetLogger()->Info("Spacing in kilometers " + oss2.str());
      // m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->
      //   SetPixelSpacingInKilometers(spacingInKilometers);

      // //rescale the surface reflectance in milli-reflectance
      // m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->UpdateOutputInformation();
      // //m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->Update();
      // m_ScaleFilter->SetInput(m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->GetOutput());
      if (!IsParameterEnabled("clamp"))
        {
        m_ScaleFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
        }
      else
        {
        GetLogger()->Info("Clamp values between [0%,100%]");
        m_ClampFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
        m_ClampFilter->ClampOutside(0.0, 1.0);
        m_ScaleFilter->SetInput(m_ClampFilter->GetOutput());
        }
      }
      break;
      }

    // Output Image
    double scale = 1.0;

    if (IsParameterEnabled("milli"))
      {
      GetLogger()->Info("Use milli-reflectance");
      scale = 1000.;
      }

    m_ScaleFilter->SetCoef(scale);

    SetParameterOutputImage("out", m_ScaleFilter->GetOutput());
  }

  ImageToLuminanceImageFilterType ::Pointer               m_ImageToLuminanceFilter;
  LuminanceToReflectanceImageFilterType::Pointer          m_LuminanceToReflectanceFilter;
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer m_ReflectanceToSurfaceReflectanceFilter;
  ScaleFilterType::Pointer                                m_ScaleFilter;
  AtmosphericCorrectionParametersType::Pointer            m_AtmosphericParam;
  ClampFilterType::Pointer                                m_ClampFilter;

  SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::Pointer m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter;
};

}// namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OpticalCalibration)
