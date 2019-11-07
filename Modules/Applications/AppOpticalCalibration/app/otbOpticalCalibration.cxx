/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbImageToRadianceImageFilter.h"
#include "otbRadianceToReflectanceImageFilter.h"
#include "otbRadianceToImageImageFilter.h"
#include "otbReflectanceToRadianceImageFilter.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "otbClampImageFilter.h"
#include "otbSurfaceAdjacencyEffectCorrectionSchemeFilter.h"
#include "otbGroundSpacingImageFunction.h"
#include "vnl/vnl_random.h"


#include <fstream>
#include <sstream>
#include <vector>
#include <itkVariableLengthVector.h>


namespace otb
{

enum
{
  Level_IM_TOA,
  Level_TOA_IM,
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

  typedef ImageToRadianceImageFilter<FloatVectorImageType, DoubleVectorImageType> ImageToRadianceImageFilterType;

  typedef RadianceToReflectanceImageFilter<DoubleVectorImageType, DoubleVectorImageType> RadianceToReflectanceImageFilterType;

  typedef RadianceToImageImageFilter<DoubleVectorImageType, DoubleVectorImageType> RadianceToImageImageFilterType;

  typedef ReflectanceToRadianceImageFilter<FloatVectorImageType, DoubleVectorImageType> ReflectanceToRadianceImageFilterType;

  typedef itk::MultiplyImageFilter<DoubleVectorImageType, DoubleImageType, DoubleVectorImageType> ScaleFilterOutDoubleType;

  typedef otb::ClampImageFilter<DoubleVectorImageType, DoubleVectorImageType> ClampFilterType;

  typedef ReflectanceToSurfaceReflectanceImageFilter<DoubleVectorImageType, DoubleVectorImageType> ReflectanceToSurfaceReflectanceImageFilterType;
  typedef ReflectanceToSurfaceReflectanceImageFilterType::FilterFunctionValuesType FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                               ValuesVectorType;

  typedef otb::AtmosphericCorrectionParameters           AtmoCorrectionParametersType;
  typedef otb::AtmosphericCorrectionParameters::Pointer  AtmoCorrectionParametersPointerType;
  typedef AtmoCorrectionParametersType::AerosolModelType AerosolModelType;

  typedef otb::ImageMetadataCorrectionParameters          AcquiCorrectionParametersType;
  typedef otb::ImageMetadataCorrectionParameters::Pointer AcquiCorrectionParametersPointerType;

  typedef otb::SurfaceAdjacencyEffectCorrectionSchemeFilter<DoubleVectorImageType, DoubleVectorImageType> SurfaceAdjacencyEffectCorrectionSchemeFilterType;

  typedef otb::GroundSpacingImageFunction<FloatVectorImageType> GroundSpacingImageType;

  typedef DoubleVectorImageType::IndexType  IndexType;
  typedef GroundSpacingImageType::FloatType FloatType;
  typedef GroundSpacingImageType::ValueType ValueType;

  typedef IndexType::IndexValueType IndexValueType;


private:
  std::string m_inImageName;
  bool        m_currentEnabledStateOfFluxParam;
  bool        m_currentEnabledStateOfSolarDistanceParam;

  void DoInit() override
  {
    SetName("OpticalCalibration");
    SetDescription(
        "Perform optical calibration TOA/TOC (Top Of Atmosphere/Top Of Canopy). Supported sensors: QuickBird, Ikonos, WorldView2, Formosat, Spot5, Pleiades, "
        "Spot6, Spot7. For other sensors the application also allows providing calibration parameters manually.");
    // Documentation
    SetDocLongDescription(
        "The application allows converting pixel values from DN (for Digital Numbers) to reflectance. Calibrated values are called surface reflectivity and "
        "its values lie in the range [0, 1].\nThe first level is called Top Of Atmosphere (TOA) reflectivity. It takes into account the sensor gain, sensor "
        "spectral response and the solar illuminations.\nThe second level is called Top Of Canopy (TOC) reflectivity. In addition to sensor gain and solar "
        "illuminations, it takes into account the optical thickness of the atmosphere, the atmospheric pressure, the water vapor amount, the ozone amount, as "
        "well as the composition and amount of aerosol gasses.\nIt is also possible to indicate an AERONET file which contains atmospheric parameters (version "
        "1 and version 2 of Aeronet file are supported. Note that computing TOC reflectivity will internally compute first TOA and then TOC reflectance. \n"
        "\n--------------------------\n\n"
        "If the sensor is not supported by the metadata interface factory of OTB, users still have the possibility to give the needed parameters to the "
        "application.\n"
        "For TOA conversion, these parameters are: \n\n"
        "- day and month of acquisition, or flux normalization coefficient, or solar distance (in AU);\n"
        "- sun elevation angle;\n"
        "- gains and biases, one pair of values for each band (passed by a file);\n"
        "- solar illuminations, one value for each band (passed by a file).\n\n"
        "For the conversion from DN (for Digital Numbers) to spectral radiance (or 'TOA radiance') L, the following formula is used:\n\n"

        "(1)\tL(b) = DN(b)/gain(b)+bias(b)\t(in W/m2/steradians/micrometers)\twith b being a band ID.\n\n"

        "These values are provided by the user thanks to a simple txt file with two lines, one for the gains and one for the biases.\n"
        "Each value must be separated with colons (:), with eventual spaces. Blank lines are not allowed. If a line begins with the '#' symbol, then it is "
        "considered as comments.\n"
        "Note that sometimes, the values provided by certain metadata files assume the formula L(b) = gain(b)*DC(b)+bias(b).\n"
        "In this case, be sure to provide the inverse gain values so that the application can correctly interpret them.\n\n"

        "In order to convert TOA radiance to TOA reflectance, the following formula is used:\n\n"

        "(2)\tR(b) = (pi*L(b)*d*d) / (ESUN(b)*cos(θ))\t(no dimension)\twhere: \n\n"

        "- L(b) is the spectral radiance for band b \n"
        "- pi is the famous mathematical constant (3.14159...) \n"
        "- d is the earth-sun distance (in astronomical units) and depends on the acquisition's day and month \n"
        "- ESUN(b) is the mean TOA solar irradiance (or solar illumination) in W/m2/micrometers\n"
        "- θ is the solar zenith angle in degrees.\n\n"
        "Note that the application asks for the solar elevation angle, and will perform the conversion to the zenith angle itself (zenith_angle = 90 - "
        "elevation_angle , units: degrees).\n"
        "Note also that ESUN(b) not only depends on the band b, but also on the spectral sensitivity of the sensor in this particular band. "
        "In other words, the influence of spectral sensitivities is included within the ESUN different values.\n"
        "These values are provided by the user thanks to a txt file following the same convention as before.\n"
        "Instead of providing the date of acquisition, the user can also provide a flux normalization coefficient or a solar distance (in AU) 'fn'. "
        "The formula used instead will be the following : \n\n"
        "(3) \tR(b) = (pi*L(b)) / (ESUN(b)*fn*fn*cos(θ)) \n\n"
        "Whatever the formula used (2 or 3), the user should pay attention to the interpretation of the parameters he will provide to the application, "
        "by taking into account the original formula that the metadata files assumes.\n\n"

        "Below, we give two examples of txt files containing information about gains/biases and solar illuminations :\n\n"
        "- gainbias.txt :\n\n"
        "# Gain values for each band. Each value must be separated with colons (:), with eventual spaces. Blank lines not allowed.\n"
        "10.4416 : 9.529 : 8.5175 : 14.0063\n"
        "# Bias values for each band.\n"
        "0.0 : 0.0 : 0.0 : 0.0\n\n"
        "- solarillumination.txt : \n\n"
        "# Solar illumination values in watt/m2/micron ('micron' means actually 'for each band').\n"
        "# Each value must be separated with colons (:), with eventual spaces. Blank lines not allowed.\n"
        "1540.494123 : 1826.087443 : 1982.671954 : 1094.747446\n\n"

        "Finally, the 'Logs' tab provides useful messages that can help the user in knowing the process different status.");

    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("The OTB CookBook");

    AddDocTag(Tags::Calibration);

    AddParameter(ParameterType_InputImage, "in", "Input");
    SetParameterDescription("in", "Input image filename");

    AddParameter(ParameterType_OutputImage, "out", "Output");
    SetParameterDescription("out", "Output calibrated image filename");

    AddParameter(ParameterType_Choice, "level", "Calibration Level");
    AddChoice("level.toa", "Image to Top Of Atmosphere reflectance");
    AddChoice("level.toatoim", "TOA reflectance to Image");
    AddChoice("level.toc", "Image to Top Of Canopy reflectance (atmospheric corrections)");
    SetParameterString("level", "toa");

    AddParameter(ParameterType_Bool, "milli", "Convert to milli reflectance");
    SetParameterDescription("milli",
                            "Flag to use milli-reflectance instead of reflectance.\n"
                            "This allows saving the image with integer pixel type (in the range [0, 1000]  instead of floating point in the range [0, 1]. In "
                            "order to do that, use this option and set the output pixel type (-out filename double for example)");

    AddParameter(ParameterType_Bool, "clamp", "Clamp of reflectivity values between [0, 1]");
    SetParameterDescription("clamp", "Clamping in the range [0, 1]. It can be useful to preserve area with specular reflectance.");
    SetParameterInt("clamp", 1);

    // Acquisition parameters
    AddParameter(ParameterType_Group, "acqui", "Acquisition parameters");
    SetParameterDescription("acqui", "This group allows setting the parameters related to the acquisition conditions.");
    // Minute
    AddParameter(ParameterType_Int, "acqui.minute", "Minute");
    SetParameterDescription("acqui.minute", "Minute (0-59)");
    SetMinimumParameterIntValue("acqui.minute", 0);
    SetMaximumParameterIntValue("acqui.minute", 59);
    SetDefaultParameterInt("acqui.minute", 0);
    // Hour
    AddParameter(ParameterType_Int, "acqui.hour", "Hour");
    SetParameterDescription("acqui.hour", "Hour (0-23)");
    SetMinimumParameterIntValue("acqui.hour", 0);
    SetMaximumParameterIntValue("acqui.hour", 23);
    SetDefaultParameterInt("acqui.hour", 12);
    // Day
    AddParameter(ParameterType_Int, "acqui.day", "Day");
    SetParameterDescription("acqui.day", "Day (1-31)");
    SetMinimumParameterIntValue("acqui.day", 1);
    SetMaximumParameterIntValue("acqui.day", 31);
    SetDefaultParameterInt("acqui.day", 1);
    // Month
    AddParameter(ParameterType_Int, "acqui.month", "Month");
    SetParameterDescription("acqui.month", "Month (1-12)");
    SetMinimumParameterIntValue("acqui.month", 1);
    SetMaximumParameterIntValue("acqui.month", 12);
    SetDefaultParameterInt("acqui.month", 1);
    // Year
    AddParameter(ParameterType_Int, "acqui.year", "Year");
    SetParameterDescription("acqui.year", "Year");
    SetDefaultParameterInt("acqui.year", 2000);
    // Flux normalization coefficient
    AddParameter(ParameterType_Float, "acqui.fluxnormcoeff", "Flux Normalization");
    SetParameterDescription("acqui.fluxnormcoeff", "Flux Normalization Coefficient");
    SetMinimumParameterFloatValue("acqui.fluxnormcoeff", 0.);
    MandatoryOff("acqui.fluxnormcoeff");
    // Solar distance
    AddParameter(ParameterType_Float, "acqui.solardistance", "Solar distance");
    SetParameterDescription("acqui.solardistance", "Solar distance (in AU)");
    SetMinimumParameterFloatValue("acqui.solardistance", 0.);
    SetMaximumParameterFloatValue("acqui.solardistance", 2.);
    MandatoryOff("acqui.solardistance");

    AddParameter(ParameterType_Group, "acqui.sun", "Sun angles");
    SetParameterDescription("acqui.sun", "This group contains the sun angles");
    // Sun elevation angle
    AddParameter(ParameterType_Float, "acqui.sun.elev", "Sun elevation angle (deg)");
    SetParameterDescription("acqui.sun.elev", "Sun elevation angle (in degrees)");
    SetMinimumParameterFloatValue("acqui.sun.elev", 0.);
    SetMaximumParameterFloatValue("acqui.sun.elev", 120.);
    SetDefaultParameterFloat("acqui.sun.elev", 90.0);
    // Sun azimuth angle
    AddParameter(ParameterType_Float, "acqui.sun.azim", "Sun azimuth angle (deg)");
    SetParameterDescription("acqui.sun.azim", "Sun azimuth angle (in degrees)");
    SetMinimumParameterFloatValue("acqui.sun.azim", 0.);
    SetMaximumParameterFloatValue("acqui.sun.azim", 360.);
    SetDefaultParameterFloat("acqui.sun.azim", 0.0);

    AddParameter(ParameterType_Group, "acqui.view", "Viewing angles");
    SetParameterDescription("acqui.view", "This group contains the sensor viewing angles");
    // Viewing elevation angle
    AddParameter(ParameterType_Float, "acqui.view.elev", "Viewing elevation angle (deg)");
    SetParameterDescription("acqui.view.elev", "Viewing elevation angle (in degrees)");
    SetMinimumParameterFloatValue("acqui.view.elev", 0.);
    SetMaximumParameterFloatValue("acqui.view.elev", 120.);
    SetDefaultParameterFloat("acqui.view.elev", 90.0);
    // Viewing azimuth angle
    AddParameter(ParameterType_Float, "acqui.view.azim", "Viewing azimuth angle (deg)");
    SetParameterDescription("acqui.view.azim", "Viewing azimuth angle (in degrees)");
    SetMinimumParameterFloatValue("acqui.view.azim", 0.);
    SetMaximumParameterFloatValue("acqui.view.azim", 360.);
    SetDefaultParameterFloat("acqui.view.azim", 0.0);

    // Gain & bias
    AddParameter(ParameterType_InputFilename, "acqui.gainbias", "Gains or biases");
    SetParameterDescription("acqui.gainbias", "Gains or biases");
    MandatoryOff("acqui.gainbias");
    // Solar illuminations
    AddParameter(ParameterType_InputFilename, "acqui.solarilluminations", "Solar illuminations");
    SetParameterDescription("acqui.solarilluminations", "Solar illuminations (one value per band, in W/m^2/micron)");
    MandatoryOff("acqui.solarilluminations");

    // Atmospheric parameters (TOC)
    AddParameter(ParameterType_Group, "atmo", "Atmospheric parameters (for TOC)");
    SetParameterDescription("atmo", "This group allows setting the atmospheric parameters.");
    AddParameter(ParameterType_Choice, "atmo.aerosol", "Aerosol Model");
    AddChoice("atmo.aerosol.noaersol", "No Aerosol Model");
    AddChoice("atmo.aerosol.continental", "Continental");
    AddChoice("atmo.aerosol.maritime", "Maritime");
    AddChoice("atmo.aerosol.urban", "Urban");
    AddChoice("atmo.aerosol.desertic", "Desertic");

    AddParameter(ParameterType_Float, "atmo.oz", "Ozone Amount (cm-atm)");
    SetParameterDescription("atmo.oz", "Stratospheric ozone layer content (in cm-atm)");

    AddParameter(ParameterType_Float, "atmo.wa", "Water Vapor Amount (g/cm2)");
    SetParameterDescription("atmo.wa", "Total water vapor content over vertical atmospheric column (in g/cm2)");

    AddParameter(ParameterType_Float, "atmo.pressure", "Atmospheric Pressure (hPa)");
    SetParameterDescription("atmo.pressure", "Atmospheric Pressure (in hPa)");

    AddParameter(ParameterType_Float, "atmo.opt", "Aerosol Optical Thickness");
    SetParameterDescription("atmo.opt", "Aerosol Optical Thickness (unitless)");

    SetDefaultParameterFloat("atmo.oz", 0.);
    SetDefaultParameterFloat("atmo.wa", 2.5);
    SetDefaultParameterFloat("atmo.pressure", 1030.);

    SetDefaultParameterFloat("atmo.opt", 0.2);
    MandatoryOff("atmo.oz");
    MandatoryOff("atmo.wa");
    MandatoryOff("atmo.pressure");
    MandatoryOff("atmo.opt");

    AddParameter(ParameterType_InputFilename, "atmo.aeronet", "Aeronet File");
    SetParameterDescription("atmo.aeronet", "Aeronet file containing atmospheric parameters");
    MandatoryOff("atmo.aeronet");

    AddParameter(ParameterType_InputFilename, "atmo.rsr", "Relative Spectral Response File");
    std::ostringstream oss;
    oss << "Sensor relative spectral response file" << std::endl;
    oss << "By default the application gets this information in the metadata";
    SetParameterDescription("atmo.rsr", oss.str());
    MandatoryOff("atmo.rsr");

    // Window radius for adjacency effects correction
    AddParameter(ParameterType_Int, "atmo.radius", "Window radius (adjacency effects)");
    SetParameterDescription("atmo.radius",
                            "Window radius for adjacency effects corrections"
                            "Setting this parameters will enable the correction of"
                            "adjacency effects");
    MandatoryOff("atmo.radius");
    SetDefaultParameterInt("atmo.radius", 2);
    DisableParameter("atmo.radius");

    // Pixel spacing
    AddParameter(ParameterType_Float, "atmo.pixsize", "Pixel size (in km)");
    SetParameterDescription("atmo.pixsize",
                            "Pixel size (in km) used to"
                            "compute adjacency effects, it doesn't have to"
                            "match the image spacing");
    SetMinimumParameterFloatValue("atmo.pixsize", 0.0);
    SetDefaultParameterFloat("atmo.pixsize", 1.);

    MandatoryOff("atmo.pixsize");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("level", "toa");
    SetDocExampleParameterValue("out", "OpticalCalibration.tif");

    SetOfficialDocLink();

    m_inImageName                             = "";
    m_currentEnabledStateOfFluxParam          = false;
    m_currentEnabledStateOfSolarDistanceParam = false;
  }

  void DoUpdateParameters() override
  {
    std::ostringstream ossOutput;
    // ossOutput << std::endl << "--DoUpdateParameters--" << std::endl;

    // Manage the case where a new input is provided: we should try to retrieve image metadata
    if (HasValue("in"))
    {
      bool        newInputImage = false;
      std::string tempName      = GetParameterString("in");

      // Check if the input image change
      if (tempName != m_inImageName)
      {
        m_inImageName = tempName;
        newInputImage = true;
      }

      if (newInputImage)
      {
        ossOutput << std::endl << "File: " << m_inImageName << std::endl;

        // Check if valid metadata information are available to compute ImageToRadiance and RadianceToReflectance
        FloatVectorImageType::Pointer          inImage                 = GetParameterFloatVectorImage("in");
        itk::MetaDataDictionary                dict                    = inImage->GetMetaDataDictionary();
        OpticalImageMetadataInterface::Pointer lImageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);

        std::string IMIName(lImageMetadataInterface->GetNameOfClass()), IMIOptDfltName("OpticalDefaultImageMetadataInterface");
        if ((IMIName != IMIOptDfltName))
        {
          ossOutput << "Sensor detected: " << lImageMetadataInterface->GetSensorID() << std::endl;

          itk::VariableLengthVector<double> vlvector;
          std::stringstream                 ss;

          ossOutput << "Parameters extract from input image: " << std::endl
                    << "\tAcquisition Day: " << lImageMetadataInterface->GetDay() << std::endl
                    << "\tAcquisition Month: " << lImageMetadataInterface->GetMonth() << std::endl
                    << "\tAcquisition Year: " << lImageMetadataInterface->GetYear() << std::endl
                    << "\tAcquisition Sun Elevation Angle: " << lImageMetadataInterface->GetSunElevation() << std::endl
                    << "\tAcquisition Sun Azimuth Angle: " << lImageMetadataInterface->GetSunAzimuth() << std::endl
                    << "\tAcquisition Viewing Elevation Angle: " << lImageMetadataInterface->GetSatElevation() << std::endl
                    << "\tAcquisition Viewing Azimuth Angle: " << lImageMetadataInterface->GetSatAzimuth() << std::endl;

          vlvector = lImageMetadataInterface->GetPhysicalGain();
          ossOutput << "\tAcquisition gain (per band): ";
          for (unsigned int k = 0; k < vlvector.Size(); k++)
            ossOutput << vlvector[k] << " ";
          ossOutput << std::endl;

          vlvector = lImageMetadataInterface->GetPhysicalBias();
          ossOutput << "\tAcquisition bias (per band): ";
          for (unsigned int k = 0; k < vlvector.Size(); k++)
            ossOutput << vlvector[k] << " ";
          ossOutput << std::endl;
          DisableParameter("acqui.gainbias");
          MandatoryOff("acqui.gainbias");

          vlvector = lImageMetadataInterface->GetSolarIrradiance();
          ossOutput << "\tSolar Irradiance (per band): ";
          for (unsigned int k = 0; k < vlvector.Size(); k++)
            ossOutput << vlvector[k] << " ";
          ossOutput << std::endl;
          DisableParameter("acqui.solarilluminations");
          MandatoryOff("acqui.solarilluminations");

          if (HasUserValue("acqui.minute"))
            ossOutput << "Acquisition Minute already set by user: no overload" << std::endl;
          else
          {
            SetParameterInt("acqui.minute", lImageMetadataInterface->GetMinute());
          }

          if (HasUserValue("acqui.hour"))
            ossOutput << "Acquisition Hour already set by user: no overload" << std::endl;
          else
          {
            SetParameterInt("acqui.hour", lImageMetadataInterface->GetHour());
          }

          if (HasUserValue("acqui.day"))
            ossOutput << "Acquisition Day already set by user: no overload" << std::endl;
          else
          {
            SetParameterInt("acqui.day", lImageMetadataInterface->GetDay());
            if (IsParameterEnabled("acqui.fluxnormcoeff") || IsParameterEnabled("acqui.solardistance"))
              DisableParameter("acqui.day");
          }

          if (HasUserValue("acqui.month"))
            ossOutput << "Acquisition Month already set by user: no overload" << std::endl;
          else
          {
            SetParameterInt("acqui.month", lImageMetadataInterface->GetMonth());
            if (IsParameterEnabled("acqui.fluxnormcoeff") || IsParameterEnabled("acqui.solardistance"))
              DisableParameter("acqui.month");
          }

          if (HasUserValue("acqui.year"))
            ossOutput << "Acquisition Year already set by user: no overload" << std::endl;
          else
          {
            SetParameterInt("acqui.year", lImageMetadataInterface->GetYear());
          }

          if (HasUserValue("acqui.sun.elev"))
            ossOutput << "Acquisition Sun Elevation Angle already set by user: no overload" << std::endl;
          else
            SetParameterFloat("acqui.sun.elev", lImageMetadataInterface->GetSunElevation());

          if (HasUserValue("acqui.sun.azim"))
            ossOutput << "Acquisition Sun Azimuth Angle already set by user: no overload" << std::endl;
          else
            SetParameterFloat("acqui.sun.azim", lImageMetadataInterface->GetSunAzimuth());

          if (HasUserValue("acqui.view.elev"))
            ossOutput << "Acquisition Viewing Elevation Angle already set by user: no overload" << std::endl;
          else
            SetParameterFloat("acqui.view.elev", lImageMetadataInterface->GetSatElevation());

          if (HasUserValue("acqui.view.azim"))
            ossOutput << "Acquisition Viewing Azimuth Angle already set by user: no overload" << std::endl;
          else
            SetParameterFloat("acqui.view.azim", lImageMetadataInterface->GetSatAzimuth());

          // Set default value so that they are stored somewhere even if
          // they are overloaded by user values
          SetDefaultParameterInt("acqui.minute", lImageMetadataInterface->GetMinute());
          SetDefaultParameterInt("acqui.hour", lImageMetadataInterface->GetHour());
          SetDefaultParameterInt("acqui.day", lImageMetadataInterface->GetDay());
          SetDefaultParameterInt("acqui.month", lImageMetadataInterface->GetMonth());
          SetDefaultParameterInt("acqui.year", lImageMetadataInterface->GetYear());
          SetDefaultParameterFloat("acqui.sun.elev", lImageMetadataInterface->GetSunElevation());
          SetDefaultParameterFloat("acqui.sun.azim", lImageMetadataInterface->GetSunAzimuth());
          SetDefaultParameterFloat("acqui.view.elev", lImageMetadataInterface->GetSatElevation());
          SetDefaultParameterFloat("acqui.view.azim", lImageMetadataInterface->GetSatAzimuth());
        }
        else
        {
          // Switch gain , bias and solar illumination to mandatory since
          // they are not given in the image loaded
          EnableParameter("acqui.gainbias");
          EnableParameter("acqui.solarilluminations");
          MandatoryOn("acqui.gainbias");
          MandatoryOn("acqui.solarilluminations");

          ossOutput << "Sensor unknown!" << std::endl;
          ossOutput << "Additional parameters are necessary, please provide them (cf. documentation)!" << std::endl;

          /*GetLogger()->Info("\n-------------------------------------------------------------\n"
          "Sensor ID : unknown...\n"
          "The application didn't manage to find an appropriate metadata interface; "
          "custom values must be provided in order to perform TOA conversion.\nPlease, set the following fields :\n"
          "- day and month of acquisition, or flux normalization coefficient;\n"
          "- sun elevation angle;\n"
          "- gains and biases for each band (passed by a file, see documentation);\n"
          "- solar illuminationss for each band (passed by a file, see documentation).\n"
          "-------------------------------------------------------------\n"); */
        }
        // Estimate ground spacing in kilometers
        GroundSpacingImageType::Pointer groundSpacing = GroundSpacingImageType::New();
        groundSpacing->SetInputImage(inImage);
        IndexType  index;
        vnl_random rand;
        index[0]                        = static_cast<IndexValueType>(rand.lrand32(0, inImage->GetLargestPossibleRegion().GetSize()[0]));
        index[1]                        = static_cast<IndexValueType>(rand.lrand32(0, inImage->GetLargestPossibleRegion().GetSize()[1]));
        FloatType   tmpSpacing          = groundSpacing->EvaluateAtIndex(index);
        const float spacingInKilometers = (std::max(tmpSpacing[0], tmpSpacing[1])) / 1000.;
        SetDefaultParameterFloat("atmo.pixsize", spacingInKilometers);
        if (!HasUserValue("atmo.pixsize"))
          SetParameterFloat("atmo.pixsize", spacingInKilometers);
      }
    }

    // Manage the case where fluxnormcoeff is modified by user
    if (m_currentEnabledStateOfFluxParam != IsParameterEnabled("acqui.fluxnormcoeff"))
    {
      if (IsParameterEnabled("acqui.fluxnormcoeff"))
      {
        ossOutput << std::endl << "Flux Normalization Coefficient will be used" << std::endl;
        DisableParameter("acqui.day");
        DisableParameter("acqui.month");
        DisableParameter("acqui.solardistance");
        MandatoryOff("acqui.day");
        MandatoryOff("acqui.month");
        MandatoryOff("acqui.solardistance");
        MandatoryOn("acqui.fluxnormcoeff");
        m_currentEnabledStateOfFluxParam          = true;
        m_currentEnabledStateOfSolarDistanceParam = false;
      }
      else
      {
        ossOutput << std::endl << "Day and Month will be used" << std::endl;
        EnableParameter("acqui.day");
        EnableParameter("acqui.month");
        MandatoryOn("acqui.day");
        MandatoryOn("acqui.month");
        MandatoryOff("acqui.fluxnormcoeff");
        m_currentEnabledStateOfFluxParam = false;
      }
    }

    // Manage the case where solardistance is modified by user
    if (m_currentEnabledStateOfSolarDistanceParam != IsParameterEnabled("acqui.solardistance"))
    {
      if (IsParameterEnabled("acqui.solardistance"))
      {
        ossOutput << std::endl << "Solar distance Coefficient will be used" << std::endl;
        DisableParameter("acqui.day");
        DisableParameter("acqui.month");
        DisableParameter("acqui.fluxnormcoeff");
        MandatoryOff("acqui.day");
        MandatoryOff("acqui.month");
        MandatoryOff("acqui.fluxnormcoeff");
        MandatoryOn("acqui.solardistance");
        m_currentEnabledStateOfFluxParam          = false;
        m_currentEnabledStateOfSolarDistanceParam = true;
      }
      else
      {
        ossOutput << std::endl << "Day and Month will be used" << std::endl;
        EnableParameter("acqui.day");
        EnableParameter("acqui.month");
        MandatoryOn("acqui.day");
        MandatoryOn("acqui.month");
        MandatoryOff("acqui.solardistance");
        m_currentEnabledStateOfSolarDistanceParam = false;
      }
    }

    if (!ossOutput.str().empty())
      otbAppLogINFO(<< ossOutput.str());
  }

  void DoExecute() override
  {
    // Main filters instantiations
    m_ImageToRadianceFilter                 = ImageToRadianceImageFilterType::New();
    m_RadianceToReflectanceFilter           = RadianceToReflectanceImageFilterType::New();
    m_ReflectanceToSurfaceReflectanceFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();
    m_ReflectanceToRadianceFilter           = ReflectanceToRadianceImageFilterType::New();
    m_RadianceToImageFilter                 = RadianceToImageImageFilterType::New();

    // Other instantiations
    m_ScaleFilter = ScaleFilterOutDoubleType::New();
    // m_ScaleFilter->InPlaceOn();
    m_ClampFilter = ClampFilterType::New();
    m_paramAcqui  = AcquiCorrectionParametersType::New();
    m_paramAtmo   = AtmoCorrectionParametersType::New();

    FloatVectorImageType::Pointer inImage = GetParameterFloatVectorImage("in");

    // Prepare a metadata interface on the input image.
    itk::MetaDataDictionary                dict                    = inImage->GetMetaDataDictionary();
    OpticalImageMetadataInterface::Pointer lImageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);
    std::string                            IMIName(lImageMetadataInterface->GetNameOfClass());
    std::string                            IMIOptDfltName("OpticalDefaultImageMetadataInterface");

    // Set (Date and Day) OR FluxNormalizationCoef to corresponding filters OR solardistance
    if (IsParameterEnabled("acqui.fluxnormcoeff"))
    {
      m_RadianceToReflectanceFilter->SetFluxNormalizationCoefficient(GetParameterFloat("acqui.fluxnormcoeff"));

      m_ReflectanceToRadianceFilter->SetFluxNormalizationCoefficient(GetParameterFloat("acqui.fluxnormcoeff"));
    }
    else if (IsParameterEnabled("acqui.solardistance"))
    {
      m_RadianceToReflectanceFilter->SetSolarDistance(GetParameterFloat("acqui.solardistance"));

      m_ReflectanceToRadianceFilter->SetSolarDistance(GetParameterFloat("acqui.solardistance"));
    }
    else
    {
      m_RadianceToReflectanceFilter->SetDay(GetParameterInt("acqui.day"));
      m_RadianceToReflectanceFilter->SetMonth(GetParameterInt("acqui.month"));

      m_ReflectanceToRadianceFilter->SetDay(GetParameterInt("acqui.day"));
      m_ReflectanceToRadianceFilter->SetMonth(GetParameterInt("acqui.month"));
    }

    // Set Sun Elevation Angle to corresponding filters
    m_RadianceToReflectanceFilter->SetElevationSolarAngle(GetParameterFloat("acqui.sun.elev"));
    m_ReflectanceToRadianceFilter->SetElevationSolarAngle(GetParameterFloat("acqui.sun.elev"));

    // Set Gain and Bias to corresponding filters
    if (IsParameterEnabled("acqui.gainbias") && HasValue("acqui.gainbias"))
    {
      // Try to retrieve information from file provided by user
      std::string filename(GetParameterString("acqui.gainbias"));

      std::ifstream file(filename, std::ios::in);
      if (file)
      {
        std::string  line;
        unsigned int numLine = 0;
        while (getline(file, line))
        {
          // clean line
          std::string::size_type startPos = line.find_first_not_of(std::string(" \t\n\r"));
          if (startPos == std::string::npos)
            continue;

          line = line.substr(startPos);
          if (line[0] != '#')
          {
            numLine++;
            std::vector<double> values;
            std::istringstream  iss(line);
            std::string         value;
            double              dvalue;
            while (getline(iss, value, ':'))
            {
              std::istringstream iss2(value);
              iss2 >> dvalue;
              values.push_back(dvalue);
            }

            itk::VariableLengthVector<double> vlvector;
            vlvector.SetData(values.data(), values.size(), false);

            switch (numLine)
            {
            case 1:
              m_ImageToRadianceFilter->SetAlpha(vlvector);
              m_RadianceToImageFilter->SetAlpha(vlvector);
              GetLogger()->Info("Trying to get gains/biases information... OK (1/2)\n");
              break;

            case 2:
              m_ImageToRadianceFilter->SetBeta(vlvector);
              m_RadianceToImageFilter->SetBeta(vlvector);
              GetLogger()->Info("Trying to get gains/biases information... OK (2/2)\n");
              break;

            default:
              itkExceptionMacro(<< "File : " << filename << " contains wrong number of lines (needs two, one for gains and one for biases)");
            }
          }
        }
        file.close();
      }
      else
        itkExceptionMacro(<< "File : " << filename << " couldn't be opened");
    }
    else
    {
      // Try to retrieve information from image metadata
      if (IMIName != IMIOptDfltName)
      {
        m_ImageToRadianceFilter->SetAlpha(lImageMetadataInterface->GetPhysicalGain());
        m_RadianceToImageFilter->SetAlpha(lImageMetadataInterface->GetPhysicalGain());

        m_ImageToRadianceFilter->SetBeta(lImageMetadataInterface->GetPhysicalBias());
        m_RadianceToImageFilter->SetBeta(lImageMetadataInterface->GetPhysicalBias());
      }
      else
        itkExceptionMacro(<< "Please, provide a type of sensor supported by OTB for automatic metadata extraction! ");
    }

    // Set Solar Illumination to corresponding filters
    if (IsParameterEnabled("acqui.solarilluminations") && HasValue("acqui.solarilluminations"))
    {
      // Try to retrieve information from file provided by user
      std::string filename(GetParameterString("acqui.solarilluminations"));

      std::ifstream file(filename, std::ios::in);
      if (file)
      {
        std::string line;
        while (getline(file, line))
        {
          // clean line
          std::string::size_type startPos = line.find_first_not_of(std::string(" \t\n\r"));
          if (startPos == std::string::npos)
            continue;

          line = line.substr(startPos);
          if (line[0] != '#')
          {
            std::vector<double> values;
            std::istringstream  iss(line);
            std::string         value;
            double              dvalue;
            while (getline(iss, value, ':'))
            {
              std::istringstream iss2(value);
              iss2 >> dvalue;
              values.push_back(dvalue);
            }

            itk::VariableLengthVector<double> vlvector;
            vlvector.SetData(values.data(), values.size(), false);

            m_RadianceToReflectanceFilter->SetSolarIllumination(vlvector);
            m_ReflectanceToRadianceFilter->SetSolarIllumination(vlvector);
          }
        }
        file.close();
      }
      else
        itkExceptionMacro(<< "File : " << filename << " couldn't be opened");
    }
    else
    {
      // Try to retrieve information from image metadata
      if (IMIName != IMIOptDfltName)
      {
        m_RadianceToReflectanceFilter->SetSolarIllumination(lImageMetadataInterface->GetSolarIrradiance());
        m_ReflectanceToRadianceFilter->SetSolarIllumination(lImageMetadataInterface->GetSolarIrradiance());
      }
      else
        itkExceptionMacro(<< "Please, provide a type of sensor supported by OTB for automatic metadata extraction! ");
    }

    // Set acquisition parameters
    m_paramAcqui->SetYear(GetParameterInt("acqui.year"));
    m_paramAcqui->SetMonth(GetParameterInt("acqui.month"));
    m_paramAcqui->SetDay(GetParameterInt("acqui.day"));
    m_paramAcqui->SetSolarZenithalAngle(90.0 - GetParameterFloat("acqui.sun.elev"));
    m_paramAcqui->SetSolarAzimutalAngle(GetParameterFloat("acqui.sun.azim"));
    m_paramAcqui->SetViewingZenithalAngle(90.0 - GetParameterFloat("acqui.view.elev"));
    m_paramAcqui->SetViewingAzimutalAngle(GetParameterFloat("acqui.view.azim"));

    switch (GetParameterInt("level"))
    {
    case Level_IM_TOA:
    {
      GetLogger()->Info("Compute Top of Atmosphere reflectance\n");

      // Pipeline
      m_ImageToRadianceFilter->SetInput(inImage);
      m_RadianceToReflectanceFilter->SetInput(m_ImageToRadianceFilter->GetOutput());

      if (GetParameterInt("clamp"))
      {
        GetLogger()->Info("Clamp values between [0, 100]\n");
      }

      m_RadianceToReflectanceFilter->SetUseClamp(GetParameterInt("clamp"));
      m_RadianceToReflectanceFilter->UpdateOutputInformation();
      m_ScaleFilter->SetInput(m_RadianceToReflectanceFilter->GetOutput());
    }
    break;
    case Level_TOA_IM:
    {
      GetLogger()->Info("Convert Top of Atmosphere reflectance to image DN\n");

      // Pipeline
      m_ReflectanceToRadianceFilter->SetInput(inImage);
      m_RadianceToImageFilter->SetInput(m_ReflectanceToRadianceFilter->GetOutput());
      m_RadianceToImageFilter->UpdateOutputInformation();
      m_ScaleFilter->SetInput(m_RadianceToImageFilter->GetOutput());
    }
    break;
    case Level_TOC:
    {
      GetLogger()->Info("Compute Top of Canopy reflectance\n");

      // Pipeline
      m_ImageToRadianceFilter->SetInput(inImage);
      m_RadianceToReflectanceFilter->SetInput(m_ImageToRadianceFilter->GetOutput());
      m_ReflectanceToSurfaceReflectanceFilter->SetInput(m_RadianceToReflectanceFilter->GetOutput());
      m_ReflectanceToSurfaceReflectanceFilter->SetAcquiCorrectionParameters(m_paramAcqui);
      m_ReflectanceToSurfaceReflectanceFilter->SetAtmoCorrectionParameters(m_paramAtmo);

      // AerosolModelType aeroMod = AtmosphericCorrectionParametersType::NO_AEROSOL;

      switch (GetParameterInt("atmo.aerosol"))
      {
      case Aerosol_Desertic:
      {
        // Aerosol_Desertic correspond to 4 in the enum but actually in
        // the class atmosphericParam it is known as parameter 5
        m_paramAtmo->SetAerosolModel(static_cast<AerosolModelType>(5));
      }
      break;
      default:
      {
        m_paramAtmo->SetAerosolModel(static_cast<AerosolModelType>(GetParameterInt("atmo.aerosol")));
      }
      break;
      }

      // Set the atmospheric param
      m_paramAtmo->SetOzoneAmount(GetParameterFloat("atmo.oz"));
      m_paramAtmo->SetWaterVaporAmount(GetParameterFloat("atmo.wa"));
      m_paramAtmo->SetAtmosphericPressure(GetParameterFloat("atmo.pressure"));
      m_paramAtmo->SetAerosolOptical(GetParameterFloat("atmo.opt"));

      // Relative Spectral Response File
      if (IsParameterEnabled("atmo.rsr"))
      {
        if (!(GetParameterString("atmo.rsr") == ""))
          m_paramAcqui->LoadFilterFunctionValue(GetParameterString("atmo.rsr"));
        else
          otbAppLogFATAL("Please, set a sensor relative spectral response file.");
      }
      else if (IMIName != IMIOptDfltName)
      {
        // Avoid to call GetSpectralSensitivity() multiple times
        OpticalImageMetadataInterface::WavelengthSpectralBandVectorType spectralSensitivity = lImageMetadataInterface->GetSpectralSensitivity();

        if (spectralSensitivity->Size() > 0)
          m_paramAcqui->SetWavelengthSpectralBand(spectralSensitivity);
      }
      // Check that m_paramAcqui contains a real spectral profile.
      if (m_paramAcqui->GetWavelengthSpectralBand()->Size() == 0)
      {
        otbAppLogWARNING(
            "No relative spectral response found, using "
            "default response (constant between 0.3 and 1.0µm)");
        AcquiCorrectionParametersType::WavelengthSpectralBandVectorType spectralDummy =
            AcquiCorrectionParametersType::InternalWavelengthSpectralBandVectorType::New();
        spectralDummy->Clear();
        for (unsigned int i = 0; i < inImage->GetNumberOfComponentsPerPixel(); ++i)
        {
          spectralDummy->PushBack(FilterFunctionValues::New());
        }
        m_paramAcqui->SetWavelengthSpectralBand(spectralDummy);
      }

      // Aeronet file
      if (IsParameterEnabled("atmo.aeronet"))
      {
        GetLogger()->Info("Use Aeronet file to retrieve atmospheric parameters\n");
        m_paramAtmo->SetAeronetFileName(GetParameterString("atmo.aeronet"));
        m_paramAtmo->UpdateAeronetData(GetParameterInt("acqui.year"), GetParameterInt("acqui.month"), GetParameterInt("acqui.day"),
                                       GetParameterInt("acqui.hour"), GetParameterInt("acqui.minute"), 0.4);
      }

      m_ReflectanceToSurfaceReflectanceFilter->UpdateOutputInformation();
      m_ReflectanceToSurfaceReflectanceFilter->SetIsSetAtmosphericRadiativeTerms(false);
      m_ReflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(true);
      m_ReflectanceToSurfaceReflectanceFilter->GenerateParameters();
      m_ReflectanceToSurfaceReflectanceFilter->SetUseGenerateParameters(false);

      // std::ostringstream oss_atmo;
      // oss_atmo << "Atmospheric parameters: " << std::endl;
      // oss_atmo << m_AtmosphericParam;
      // GetLogger()->Info(oss_atmo.str());

      std::ostringstream oss;
      oss.str("");
      oss << std::endl << m_paramAtmo;

      AtmosphericRadiativeTerms::Pointer atmoTerms = m_ReflectanceToSurfaceReflectanceFilter->GetAtmosphericRadiativeTerms();
      oss << std::endl << std::endl << atmoTerms << std::endl;

      GetLogger()->Info("Atmospheric correction parameters compute by 6S : " + oss.str());

      bool adjComputation = false;
      if (IsParameterEnabled("atmo.radius"))
      {
        GetLogger()->Info("Compute adjacency effects\n");
        adjComputation = true;
        // Compute adjacency effect
        m_SurfaceAdjacencyEffectCorrectionSchemeFilter = SurfaceAdjacencyEffectCorrectionSchemeFilterType::New();

        m_SurfaceAdjacencyEffectCorrectionSchemeFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
        m_SurfaceAdjacencyEffectCorrectionSchemeFilter->SetAtmosphericRadiativeTerms(m_ReflectanceToSurfaceReflectanceFilter->GetAtmosphericRadiativeTerms());
        m_SurfaceAdjacencyEffectCorrectionSchemeFilter->SetZenithalViewingAngle(m_paramAcqui->GetViewingZenithalAngle());
        m_SurfaceAdjacencyEffectCorrectionSchemeFilter->SetWindowRadius(GetParameterInt("atmo.radius"));
        m_SurfaceAdjacencyEffectCorrectionSchemeFilter->SetPixelSpacingInKilometers(GetParameterFloat("atmo.pixsize"));

        m_SurfaceAdjacencyEffectCorrectionSchemeFilter->UpdateOutputInformation();
      }

      // Rescale the surface reflectance in milli-reflectance
      if (!GetParameterInt("clamp"))
      {
        if (!adjComputation)
          m_ScaleFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
        else
          m_ScaleFilter->SetInput(m_SurfaceAdjacencyEffectCorrectionSchemeFilter->GetOutput());
      }
      else
      {
        GetLogger()->Info("Clamp values between [0, 100]\n");

        if (!adjComputation)
          m_ClampFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
        else
          m_ClampFilter->SetInput(m_SurfaceAdjacencyEffectCorrectionSchemeFilter->GetOutput());

        m_ClampFilter->ClampOutside(0.0, 1.0);
        m_ScaleFilter->SetInput(m_ClampFilter->GetOutput());
      }
    }
    break;
    }

    // Output Image
    double scale = 1.;

    if (GetParameterInt("milli"))
    {
      GetLogger()->Info("Use milli-reflectance\n");
      if ((GetParameterInt("level") == Level_IM_TOA) || (GetParameterInt("level") == Level_TOC))
        scale = 1000.;
      if (GetParameterInt("level") == Level_TOA_IM)
        scale = 1. / 1000.;
    }
    m_ScaleFilter->SetConstant(scale);

    SetParameterOutputImage("out", m_ScaleFilter->GetOutput());
  }

  // Keep object references as a members of the class, else the pipeline will be broken after exiting DoExecute().
  ImageToRadianceImageFilterType::Pointer                 m_ImageToRadianceFilter;
  RadianceToReflectanceImageFilterType::Pointer           m_RadianceToReflectanceFilter;
  ReflectanceToRadianceImageFilterType::Pointer           m_ReflectanceToRadianceFilter;
  RadianceToImageImageFilterType::Pointer                 m_RadianceToImageFilter;
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer m_ReflectanceToSurfaceReflectanceFilter;
  ScaleFilterOutDoubleType::Pointer                       m_ScaleFilter;
  AtmoCorrectionParametersPointerType                     m_paramAtmo;
  AcquiCorrectionParametersPointerType                    m_paramAcqui;
  ClampFilterType::Pointer                                m_ClampFilter;

  SurfaceAdjacencyEffectCorrectionSchemeFilterType::Pointer m_SurfaceAdjacencyEffectCorrectionSchemeFilter;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OpticalCalibration)
