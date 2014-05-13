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
#include "otbLuminanceToImageImageFilter.h"
#include "otbReflectanceToLuminanceImageFilter.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "otbClampVectorImageFilter.h"
#include "otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter.h"
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

  typedef ImageToLuminanceImageFilter<FloatVectorImageType,
                                      DoubleVectorImageType>              ImageToLuminanceImageFilterType;

  typedef LuminanceToReflectanceImageFilter<DoubleVectorImageType,
                                            DoubleVectorImageType>        LuminanceToReflectanceImageFilterType;

  typedef LuminanceToImageImageFilter<DoubleVectorImageType,
                                            DoubleVectorImageType>        LuminanceToImageImageFilterType;

  typedef ReflectanceToLuminanceImageFilter<FloatVectorImageType,
                                            DoubleVectorImageType>        ReflectanceToLuminanceImageFilterType;

  typedef otb::MultiplyByScalarImageFilter<DoubleVectorImageType,
                                           DoubleVectorImageType>         ScaleFilterOutDoubleType;

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

  typedef otb::GroundSpacingImageFunction<FloatVectorImageType> GroundSpacingImageType;

  typedef DoubleVectorImageType::IndexType   IndexType;
  typedef GroundSpacingImageType::FloatType  FloatType;
  typedef GroundSpacingImageType::ValueType  ValueType;

  typedef IndexType::IndexValueType          IndexValueType;


private:

  string m_inImageName;
  bool m_currentEnabledStateOfFluxParam;

  void DoInit()
  {
    SetName("OpticalCalibration");
    SetDescription("Perform optical calibration TOA/TOC (Top Of Atmosphere/Top Of Canopy). Supported sensors: QuickBird, Ikonos, WorldView2, Formosat, Spot5, Pleiades, Spot6");
    // Documentation
    SetDocName("Optical calibration");
    SetDocLongDescription("The application allows to convert pixel values from DN (for Digital Numbers) to physically interpretable and comparable values. Calibrated values are called surface reflectivity and its values lie in the range [0, 1].\nThe first level is called Top Of Atmosphere (TOA) reflectivity. It takes into account the sensor gain, sensor spectral response and the solar illuminations.\nThe second level is called Top Of Canopy (TOC) reflectivity. In addition to sensor gain and solar illuminations, it takes into account the optical thickness of the atmosphere, the atmospheric pressure, the water vapor amount, the ozone amount, as well as the composition and amount of aerosol gasses.\nIt is also possible to indicate an AERONET file which contains atmospheric parameters (version 1 and version 2 of Aeronet file are supported.\n"
"\n--------------------------\n\n"
"If the sensor is not supported by the metadata interface factory of OTB, users still have the possibility to give the needed parameters to the application.\n"
"For TOA conversion, these parameters are : \n"
"- day and month of acquisition, or flux normalization coefficient;\n"
"- sun elevation angle;\n"
"- gains and biases, one pair of values for each band (passed by a file);\n"
"- solar illuminations, one value for each band (passed by a file).\n\n"
"For the conversion from DN (for Digital Numbers) to spectral radiance (or 'TOA radiance') L, the following formula is used :\n\n"

"(1)\tL(b) = DN(b)/gain(b)+bias(b)\t(in W/m²/steradians/micrometers)\twith b being a band ID.\n\n"

"These values are provided by the user thanks to a simple txt file with two lines, one for the gains and one for the biases.\n"
"Each value must be separated with colons (:), with eventual spaces. Blank lines are not allowed. If a line begins with the '#' symbol, then it is considered as comments.\n"
"Note that sometimes, the values provided by certain metadata files assume the formula L(b) = gain(b)*DC(b)+bias(b).\n"
"In this case, be sure to provide the inverse gain values so that the application can correctly interpret them.\n\n"

"In order to convert TOA radiance to TOA reflectance, the following formula is used :\n\n"

"(2)\tR(b) = (pi*L(b)*d²) / (ESUN(b)*cos(θ))\t(no dimension)\twhere : \n\n"

"- L(b) is the spectral radiance for band b \n"
"- pi is the famous mathematical constant (3.14159...) \n"
"- d is the earth-sun distance (in astronomical units) and depends on the acquisition's day and month \n"
"- ESUN(b) is the mean TOA solar irradiance (or solar illumination) in W/m²/micrometers\n"
"- θ is the solar zenith angle in degrees. \n"
"Note that the application asks for the solar elevation angle, and will perfom the conversion to the zenith angle itself (ze. angle = 90° - el. angle).\n"
"Note also that ESUN(b) not only depends on the band b, but also on the spectral sensitivity of the sensor in this particular band. "
"In other words, the influence of spectral sensitivities is included within the ESUN different values.\n"
"These values are provided by the user thanks to a txt file following the same convention as before.\n"
"Instead of providing the date of acquisition, the user can also provide a flux normalization coefficient 'fn'. "
"The formula used instead will be the following : \n\n"
"(3) \tR(b) = (pi*L(b)) / (ESUN(b)*fn²*cos(θ)) \n\n"
"Whatever the formula used (2 or 3), the user should pay attention to the interpretation of the parameters he will provide to the application, "
"by taking into account the original formula that the metadata files assum.\n\n"

"Below, we give two examples of txt files containing information about gains/biases and solar illuminations :\n\n"
"- gainbias.txt :\n"
"# Gain values for each band. Each value must be separated with colons (:), with eventual spaces. Blank lines not allowed.\n"
"10.4416 : 9.529 : 8.5175 : 14.0063\n"
"# Bias values for each band.\n"
"0.0 : 0.0 : 0.0 : 0.0\n\n"
"- solarillumination.txt : \n"
"# Solar illumination values in watt/m2/micron ('micron' means actually 'for each band').\n"
"# Each value must be separated with colons (:), with eventual spaces. Blank lines not allowed.\n"
"1540.494123 : 1826.087443 : 1982.671954 : 1094.747446\n\n"

"Finally, the 'Logs' tab provides usefull messages that can help the user in knowing the process different status." );
    
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
    AddChoice("level.toa",     "Image to TOA reflectance");
    AddChoice("level.toatoim",     "TOA reflectance to Image");
    AddChoice("level.toc",     "TOC : Top Of Canopy (EXPERIMENTAL)");
    SetParameterString("level", "toa");

    AddParameter(ParameterType_Empty, "milli", "Convert to milli reflectance");
    SetParameterDescription("milli", "Flag to use milli-reflectance instead of reflectance.\n"
                            "This allows to save the image with integer pixel type (in the range [0, 1000]  instead of floating point in the range [0, 1]. In order to do that, use this option and set the output pixel type (-out filename double for example)");
    DisableParameter("milli");
    MandatoryOff("milli");

    AddParameter(ParameterType_Empty, "clamp", "Clamp of reflectivity values between [0, 100]");
    SetParameterDescription("clamp", "Clamping in the range [0, 100]. It can be useful to preserve area with specular reflectance.");
    EnableParameter("clamp");
    MandatoryOff("clamp");

    //Acquisition parameters (TOA)
    AddParameter(ParameterType_Group,"acquisition","Acquisition parameters (TOA)");
    SetParameterDescription("acquisition","This group allows to set the parameters related to the acquisition conditions.");
    //Day
    AddParameter(ParameterType_Int, "acquisition.day",   "Day");
    SetParameterDescription("acquisition.day", "Day (1-31)");
    SetMinimumParameterIntValue("acquisition.day", 1);
    SetMaximumParameterIntValue("acquisition.day", 31);
    EnableParameter("acquisition.day");
    MandatoryOn("acquisition.day");
    //Month
    AddParameter(ParameterType_Int, "acquisition.month",   "Month");
    SetParameterDescription("acquisition.month", "Month (1-12)");
    SetMinimumParameterIntValue("acquisition.month", 1);
    SetMaximumParameterIntValue("acquisition.month", 12);
    MandatoryOn("acquisition.month");
    //Flux normalization coefficient
    AddParameter(ParameterType_Float, "acquisition.fluxnormalizationcoefficient",   "Flux Normalization");
    SetParameterDescription("acquisition.fluxnormalizationcoefficient", "Flux Normalization Coefficient");
    SetMinimumParameterFloatValue("acquisition.fluxnormalizationcoefficient", 0.);
    MandatoryOff("acquisition.fluxnormalizationcoefficient");
    //Sun elevation angle
    AddParameter(ParameterType_Float, "acquisition.sunelevationangle",   "Sun elevation angle (°)");
    SetParameterDescription("acquisition.sunelevationangle", "Sun elevation angle");
    SetMinimumParameterFloatValue("acquisition.sunelevationangle", 0.);
    SetMaximumParameterFloatValue("acquisition.sunelevationangle", 120.);
    MandatoryOn("acquisition.sunelevationangle");
    //Gain & bias
    AddParameter(ParameterType_InputFilename, "acquisition.gainbias",   "Gains | biases");
    SetParameterDescription("acquisition.gainbias", "Gains | biases");
    MandatoryOn("acquisition.gainbias");
    //Solar illuminations
    AddParameter(ParameterType_InputFilename, "acquisition.solarilluminations",   "Solar illuminations");
    SetParameterDescription("acquisition.solarilluminations", "Solar illuminations (one value per band)");
    MandatoryOn("acquisition.solarilluminations");

    //Atmospheric parameters (TOC)
    AddParameter(ParameterType_Group,"atmo","Atmospheric parameters (TOC)");
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
 
    AddParameter(ParameterType_InputFilename, "atmo.rsr", "Relative Spectral Response File");
    std::ostringstream oss;
    oss << "Sensor relative spectral response file"<<std::endl;
    oss << "By default the application gets these informations in the metadata";
    SetParameterDescription("atmo.rsr", oss.str());
    MandatoryOff("atmo.rsr");

    // Window radius for adjacency effects correction
    AddParameter(ParameterType_Int, "atmo.radius", "Window radius (adjacency effects)");
    SetParameterDescription("atmo.radius","Window radius for adjacency effects corrections");
    MandatoryOff("atmo.radius");
    SetDefaultParameterInt("atmo.radius", 2);
    DisableParameter("atmo.radius");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("level", "toa");
    SetDocExampleParameterValue("out", "OpticalCalibration.tif");

    m_inImageName = "";
    m_currentEnabledStateOfFluxParam=false;
  }

  void DoUpdateParameters()
  {
    std::ostringstream ossOutput;
    //ossOutput << std::endl << "--DoUpdateParameters--" << std::endl;

    // Manage the case where a new input is provided: we should try to retrieve image metadata
    if (HasValue("in"))
    {
      bool newInputImage = false;
      string tempName = GetParameterString("in");

      // Check if the input image change
      if (tempName != m_inImageName)
      {
        m_inImageName = tempName;
        newInputImage = true;
      }

      if (newInputImage)
      {
        ossOutput << std::endl << "File: " << m_inImageName << std::endl;
        
        //Check if valid metadata informations are available to compute ImageToLuminance and LuminanceToReflectance
        FloatVectorImageType::Pointer inImage = GetParameterFloatVectorImage("in");
        itk::MetaDataDictionary             dict = inImage->GetMetaDataDictionary();
        OpticalImageMetadataInterface::Pointer lImageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);

        string IMIName( lImageMetadataInterface->GetNameOfClass() ) , IMIOptDfltName("OpticalDefaultImageMetadataInterface");
        if ( (IMIName != IMIOptDfltName))
        {
           ossOutput << "Sensor detected: " << lImageMetadataInterface->GetSensorID() << std::endl;

           itk::VariableLengthVector<double> vlvector;
           std::stringstream ss;

           ossOutput << "Parameters extract from input image: "<< std::endl
                     << "\tAcquisition Day: " << lImageMetadataInterface->GetDay() << std::endl
                     << "\tAcquisition Month: " << lImageMetadataInterface->GetMonth() << std::endl
                     << "\tAcquisition Sun Elevation Angle: " << lImageMetadataInterface->GetSunElevation() << std::endl;
         
           vlvector = lImageMetadataInterface->GetPhysicalGain();
           ossOutput << "\tAcquisition gain (per band): ";
           for(unsigned int k=0; k<vlvector.Size(); k++)
             ossOutput << vlvector[k] << " ";
           ossOutput << std::endl;

           vlvector = lImageMetadataInterface->GetPhysicalBias();
           ossOutput << "\tAcquisition bias (per band): ";
           for(unsigned int k=0; k<vlvector.Size(); k++)
             ossOutput << vlvector[k] << " ";
           ossOutput << std::endl;
           MandatoryOff("acquisition.gainbias");

           vlvector = lImageMetadataInterface->GetSolarIrradiance();
           ossOutput << "\tSolar Irradiance (per band): ";
           for(unsigned int k=0; k<vlvector.Size(); k++)
             ossOutput << vlvector[k] << " ";
           ossOutput << std::endl;
           MandatoryOff("acquisition.solarilluminations");

           if (HasUserValue("acquisition.day"))
             ossOutput << "Acquisition Day already set by user: no overload" <<std::endl;
           else
           {
             SetParameterInt("acquisition.day", lImageMetadataInterface->GetDay());
             if (IsParameterEnabled("acquisition.fluxnormalizationcoefficient"))
               DisableParameter("acquisition.day");
           }
         
           if (HasUserValue("acquisition.month"))
             ossOutput << "Acquisition Month already set by user: no overload" <<std::endl;
           else
           {
             SetParameterInt("acquisition.month", lImageMetadataInterface->GetMonth());
             if (IsParameterEnabled("acquisition.fluxnormalizationcoefficient"))
               DisableParameter("acquisition.month");
           }

           if (HasUserValue("acquisition.sunelevationangle"))
             ossOutput << "Acquisition Sun Elevation Angle already set by user: no overload" <<std::endl;
           else
             SetParameterFloat("acquisition.sunelevationangle", lImageMetadataInterface->GetSunElevation());
        }
        else
        {
          ossOutput << "Sensor unknown!"<< std::endl;
          ossOutput << "Additional parameters are necessary, please provide them (cf. documentation)!"<< std::endl;

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
      }
    }

    // Manage the case where fluxnormalizationcoefficient is modified by user
    if (m_currentEnabledStateOfFluxParam != IsParameterEnabled("acquisition.fluxnormalizationcoefficient"))
    {
      if (IsParameterEnabled("acquisition.fluxnormalizationcoefficient"))
      {
        ossOutput << std::endl << "Flux Normalization Coefficient will be used" << std::endl;
        DisableParameter("acquisition.day");
        DisableParameter("acquisition.month");
        MandatoryOff("acquisition.day");
        MandatoryOff("acquisition.month");
        MandatoryOn("acquisition.fluxnormalizationcoefficient");
        m_currentEnabledStateOfFluxParam = true;
      }
      else
      {
        ossOutput << std::endl << "Day and Month will be used" << std::endl;
        EnableParameter("acquisition.day");
        EnableParameter("acquisition.month");
        MandatoryOn("acquisition.day");
        MandatoryOn("acquisition.month");
        MandatoryOff("acquisition.fluxnormalizationcoefficient");
        m_currentEnabledStateOfFluxParam = false;
      }
    }

    if (!ossOutput.str().empty())
      otbAppLogINFO(<< ossOutput.str());

  }

  void DoExecute()
  {
    //Main filters instanciations
    m_ImageToLuminanceFilter                = ImageToLuminanceImageFilterType::New();
    m_LuminanceToReflectanceFilter          = LuminanceToReflectanceImageFilterType::New();
    m_ReflectanceToSurfaceReflectanceFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();
    m_ReflectanceToLuminanceFilter          = ReflectanceToLuminanceImageFilterType::New();
    m_LuminanceToImageFilter                = LuminanceToImageImageFilterType::New();

    //Other instanciations
    m_ScaleFilter = ScaleFilterOutDoubleType::New();
    m_ScaleFilter->InPlaceOn();
    m_ClampFilter = ClampFilterType::New();

    FloatVectorImageType::Pointer inImage = GetParameterFloatVectorImage("in");

    // Set (Date and Day) OR FluxNormalizationCoef to corresponding filters
    if ( !IsParameterEnabled("acquisition.fluxnormalizationcoefficient") )
    {
      m_LuminanceToReflectanceFilter->SetDay(GetParameterInt("acquisition.day"));
      m_LuminanceToReflectanceFilter->SetMonth(GetParameterInt("acquisition.month"));

      m_ReflectanceToLuminanceFilter->SetDay(GetParameterInt("acquisition.day"));
      m_ReflectanceToLuminanceFilter->SetMonth(GetParameterInt("acquisition.month"));
    }
    else
    {
      m_LuminanceToReflectanceFilter->SetFluxNormalizationCoefficient(GetParameterFloat("acquisition.fluxnormalizationcoefficient"));

      m_ReflectanceToLuminanceFilter->SetFluxNormalizationCoefficient(GetParameterFloat("acquisition.fluxnormalizationcoefficient"));
    }

    // Set Sun Elevation Angle to corresponding filters
    m_LuminanceToReflectanceFilter->SetElevationSolarAngle(GetParameterFloat("acquisition.sunelevationangle"));
    m_ReflectanceToLuminanceFilter->SetElevationSolarAngle(GetParameterFloat("acquisition.sunelevationangle"));

    // Set Gain and Bias to corresponding filters
    if (HasValue("acquisition.gainbias"))
    {
      // Try to retrieve information from file provided by user
      string filename(GetParameterString("acquisition.gainbias"));
      
      std::ifstream file(filename.c_str(), std::ios::in);
      if(file)
      {
        string line;
        unsigned int numLine = 0;
        while (getline(file, line))
        {
          if (line[0]!='#')
          {
            numLine++;
            std::vector<double> values;
            std::istringstream  iss(line);
            string value; double dvalue;
            while ( getline( iss, value, ':' ) )
            {
              std::istringstream  iss2(value);
              iss2 >> dvalue;
              values.push_back(dvalue);
            }

            itk::VariableLengthVector<double> vlvector;
            vlvector.SetData(values.data(),values.size(),false);

            switch (numLine)
            {
              case 1 :
              m_ImageToLuminanceFilter->SetAlpha(vlvector);
              m_LuminanceToImageFilter->SetAlpha(vlvector);
              GetLogger()->Info("Trying to get gains/biases information... OK (1/2)\n");
              break;
              
              case 2 :
              m_ImageToLuminanceFilter->SetBeta(vlvector);
              m_LuminanceToImageFilter->SetBeta(vlvector);
              GetLogger()->Info("Trying to get gains/biases information... OK (2/2)\n");
              break;
              
              default : itkExceptionMacro(<< "File : " << filename << " contains wrong number of lines (needs two, one for gains and one for biases)");
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
      //Try to retrieve information from image metadata
      itk::MetaDataDictionary           dict = inImage->GetMetaDataDictionary();
      OpticalImageMetadataInterface::Pointer lImageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);
      string IMIName( lImageMetadataInterface->GetNameOfClass() ) , IMIOptDfltName("OpticalDefaultImageMetadataInterface");
      if (IMIName != IMIOptDfltName)
      {
        m_ImageToLuminanceFilter->SetAlpha(lImageMetadataInterface->GetPhysicalGain());
        m_LuminanceToImageFilter->SetAlpha(lImageMetadataInterface->GetPhysicalGain());
     
        m_ImageToLuminanceFilter->SetBeta(lImageMetadataInterface->GetPhysicalBias());
        m_LuminanceToImageFilter->SetBeta(lImageMetadataInterface->GetPhysicalBias());
      }
      else
        itkExceptionMacro(<< "Please, provide a type of sensor supported by OTB for automatic metadata extraction! ");
    }

    // Set Solar Illumination to corresponding filters
    if (HasValue("acquisition.solarilluminations"))
    {
      // Try to retrieve information from file provided by user
      string filename(GetParameterString("acquisition.solarilluminations"));
      
      std::ifstream file(filename.c_str(), std::ios::in);
      if(file)
      {
        string line;
        while (getline(file, line))
        {
          if (line[0]!='#')
          {
            std::vector<double> values;
            std::istringstream  iss(line);
            string value; double dvalue;
            while ( getline( iss, value, ':' ) )
            {
              std::istringstream  iss2(value);
              iss2 >> dvalue;
              values.push_back(dvalue);
            }

            itk::VariableLengthVector<double> vlvector;
            vlvector.SetData(values.data(),values.size(),false);

            m_LuminanceToReflectanceFilter->SetSolarIllumination(vlvector);
            m_ReflectanceToLuminanceFilter->SetSolarIllumination(vlvector);
          }
        }
        file.close();
      }
      else
        itkExceptionMacro(<< "File : " << filename << " couldn't be opened");
    }
    else
    {
      //Try to retrieve information from image metadata
      itk::MetaDataDictionary           dict = inImage->GetMetaDataDictionary();
      OpticalImageMetadataInterface::Pointer lImageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);
      string IMIName( lImageMetadataInterface->GetNameOfClass() ) , IMIOptDfltName("OpticalDefaultImageMetadataInterface");
      if (IMIName != IMIOptDfltName)
      {
        m_LuminanceToReflectanceFilter->SetSolarIllumination(lImageMetadataInterface->GetSolarIrradiance());
        m_ReflectanceToLuminanceFilter->SetSolarIllumination(lImageMetadataInterface->GetSolarIrradiance());
      }
      else
        itkExceptionMacro(<< "Please, provide a type of sensor supported by OTB for automatic metadata extraction! ");
    }

    switch ( GetParameterInt("level") )
    {
      case Level_IM_TOA:
      {
        GetLogger()->Info("Compute Top of Atmosphere reflectance\n");
    
        //Pipeline
        m_ImageToLuminanceFilter->SetInput(inImage);
        m_LuminanceToReflectanceFilter->SetInput(m_ImageToLuminanceFilter->GetOutput());
        m_LuminanceToReflectanceFilter->SetUseClamp(IsParameterEnabled("clamp"));
        m_LuminanceToReflectanceFilter->UpdateOutputInformation();
        m_ScaleFilter->SetInput(m_LuminanceToReflectanceFilter->GetOutput());
      }
      break;
      case Level_TOA_IM:
      {
        GetLogger()->Info("Convert Top of Atmosphere reflectance to image DN\n");
    
        //Pipeline
        m_ReflectanceToLuminanceFilter->SetInput(inImage);
        m_LuminanceToImageFilter->SetInput(m_ReflectanceToLuminanceFilter->GetOutput());
        m_LuminanceToImageFilter->UpdateOutputInformation();
        m_ScaleFilter->SetInput(m_LuminanceToImageFilter->GetOutput());
      }
      break;
      case Level_TOC:
      {
        GetLogger()->Info("Convert Top of Canopy reflectance\n");

        //Pipeline
        m_ImageToLuminanceFilter->SetInput(inImage);
        m_LuminanceToReflectanceFilter->SetInput(m_ImageToLuminanceFilter->GetOutput());
        m_ReflectanceToSurfaceReflectanceFilter->SetInput(m_LuminanceToReflectanceFilter->GetOutput());
         
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
        if (IsParameterEnabled("atmo.rsr"))
        {
          m_ReflectanceToSurfaceReflectanceFilter->SetFilterFunctionValuesFileName(GetParameterString("atmo.rsr"));
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

        bool adjComputation=false;
        if (IsParameterEnabled("atmo.radius"))
        {
          adjComputation=true;
          //Compute adjacency effect
          m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter
            = SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::New();

          m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
          m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->
            SetAtmosphericRadiativeTerms(
              m_ReflectanceToSurfaceReflectanceFilter->GetAtmosphericRadiativeTerms());
          m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->SetZenithalViewingAngle(
            m_AtmosphericParam->GetViewingZenithalAngle());
          m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->SetWindowRadius(GetParameterInt("atmo.radius"));

          //Estimate ground spacing in kilometers
          GroundSpacingImageType::Pointer groundSpacing = GroundSpacingImageType::New();

          groundSpacing->SetInputImage(inImage);
          IndexType  index;
          vnl_random rand;

          index[0] = static_cast<IndexValueType>(rand.lrand32(0, inImage->GetLargestPossibleRegion().GetSize()[0]));
          index[1] = static_cast<IndexValueType>(rand.lrand32(0, inImage->GetLargestPossibleRegion().GetSize()[1]));
          FloatType tmpSpacing = groundSpacing->EvaluateAtIndex(index);

          const float spacingInKilometers = (std::max(tmpSpacing[0], tmpSpacing[1])) / 1000.;
            // std::ostringstream oss2;
            //  oss2.str("");
            //  oss2 << spacingInKilometers;
            //  GetLogger()->Info("Spacing in kilometers " + oss2.str());
          m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->
            SetPixelSpacingInKilometers(spacingInKilometers);

        
          m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->UpdateOutputInformation();
        }

        //Rescale the surface reflectance in milli-reflectance
        if (!IsParameterEnabled("clamp"))
        {
          if (!adjComputation)
            m_ScaleFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
          else
            m_ScaleFilter->SetInput(m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->GetOutput());
        }
        else
        {
          GetLogger()->Info("Clamp values between [0, 100]");
          
          if (!adjComputation)
            m_ClampFilter->SetInput(m_ReflectanceToSurfaceReflectanceFilter->GetOutput());
          else
            m_ClampFilter->SetInput(m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter->GetOutput());
          
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
      if ( (GetParameterInt("level") == Level_IM_TOA) || (GetParameterInt("level") == Level_TOC) )
        scale = 1000.;
      if (GetParameterInt("level") == Level_TOA_IM)
        scale = 1. / 1000.;
    }
    m_ScaleFilter->SetCoef(scale);

    SetParameterOutputImage("out", m_ScaleFilter->GetOutput());
  }

  //Keep object references as a members of the class, else the pipeline will be broken after exiting DoExecute().
  ImageToLuminanceImageFilterType ::Pointer               m_ImageToLuminanceFilter;
  LuminanceToReflectanceImageFilterType::Pointer          m_LuminanceToReflectanceFilter;
  ReflectanceToLuminanceImageFilterType::Pointer          m_ReflectanceToLuminanceFilter;
  LuminanceToImageImageFilterType::Pointer                m_LuminanceToImageFilter;
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer m_ReflectanceToSurfaceReflectanceFilter;
  ScaleFilterOutDoubleType::Pointer                       m_ScaleFilter;
  AtmosphericCorrectionParametersType::Pointer            m_AtmosphericParam;
  ClampFilterType::Pointer                                m_ClampFilter;

  SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::Pointer m_SurfaceAdjacencyEffect6SCorrectionSchemeFilter;
};

}// namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OpticalCalibration)
