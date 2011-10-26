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
#include "otbWrapperNumericalParameter.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbImageToGenericRSOutputParameters.h"
#include "otbMapProjections.h"

#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

#include "otbMacro.h"

namespace otb
{

enum
{
  Map_Utm,
  Map_Lambert2,
  Map_Epsg
};

enum
{
  Interpolator_Linear,
  Interpolator_NNeighbor,
  Interpolator_BCO
};

enum
{
  Mode_UserDefined,
  Mode_AutomaticSize,
  Mode_AutomaticSpacing
};

namespace Wrapper
{

class OrthoRectification : public Application
{
public:
/** Standard class typedefs. */
  typedef OrthoRectification            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(OrthoRectification, otb::Application);

  /** Generic Remote Sensor Resampler */
  typedef otb::GenericRSResampleImageFilter<FloatVectorImageType,
                                            FloatVectorImageType>       ResampleFilterType;
  
  /** Interpolators typedefs*/
  typedef itk::LinearInterpolateImageFunction<FloatVectorImageType,
                                              double>              LinearInterpolationType;
  typedef itk::NearestNeighborInterpolateImageFunction<FloatVectorImageType,
                                                       double>     NearestNeighborInterpolationType;
  typedef otb::BCOInterpolateImageFunction<FloatVectorImageType>   BCOInterpolationType;

private:
  OrthoRectification()
  {
    SetName("OrthoRectification");
    std::ostringstream oss;
    oss << "Using available image metadata to determine the sensor model," << std::endl;
    oss << "computes a cartographic projection of the image";
    SetDescription(oss.str());
    // Documentation
    SetDocName("Ortho Rectification application");
    SetDocLongDescription(oss.str());
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample(" ");
    AddDocTag("Image manipulation");
    AddDocTag("Projection");
  }

  void DoCreateParameters()
  {
    // Set the parameters
    AddParameter(ParameterType_InputImage, "in", "Input Image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Projected image");

    // Add the output paramters in a group
    AddParameter(ParameterType_Group, "outputs", "Output Parameters");

    // UserDefined values
    AddParameter(ParameterType_Choice, "outputs.mode", "Mode Type");
    AddChoice("outputs.mode.auto", "User Defined");
    AddChoice("outputs.mode.autosize", "Automatic Size ");
    AddChoice("outputs.mode.autospacing", "Automatic Spacing");    

    // Upper left point coordinates
    AddParameter(ParameterType_Float, "outputs.ulx", "Upper Left X");
    SetParameterDescription("outputs.ulx","Cartographic X coordinate of upper left corner");

    AddParameter(ParameterType_Float, "outputs.uly", "Upper Left Y");
    SetParameterDescription("outputs.uly","Cartographic Y coordinate of upper left corner");

    // Size of the output image
    AddParameter(ParameterType_Int, "outputs.sizex", "Size X");
    SetParameterDescription("outputs.sizex","Size of projected image along X");

    AddParameter(ParameterType_Int, "outputs.sizey", "Size Y");
    SetParameterDescription("outputs.sizey","Size of projected image along Y");
    
    // Spacing of the output image
    AddParameter(ParameterType_Float, "outputs.spacingx", "Pixel Size X");
    SetParameterDescription("outputs.spacingx","Size of each pixel along X axis");


    AddParameter(ParameterType_Float, "outputs.spacingy", "Pixel Size Y");
    SetParameterDescription("outputs.spacingy","Size of each pixel along Y axis");

    AddParameter(ParameterType_Empty,"outputs.isotropic","Force isotropic spacing by default");
    std::ostringstream isotropOss;
    isotropOss << "Default spacing values are estimated from the sensor modelling of the image. It can therefore result in a non-isotropic spacing. ";
    isotropOss << "This option allows you to force default values to be isotropic (in this case, the minimum of spacing in both direction is applied. ";
    isotropOss << "Values overriden by user are not affected by this option.";
    SetParameterDescription("outputs.isotropic",isotropOss.str());
    EnableParameter("outputs.isotropic");
    
    // DEM
    AddParameter(ParameterType_Directory, "dem",   "DEM directory");
    MandatoryOff("dem");

    // Estimate a RPC model (for spot image for instance)
    AddParameter(ParameterType_Group, "rpc", "Estimate RPC model");
    AddParameter(ParameterType_Int, "rpc.ncp", "Nb control Points");
    std::ostringstream oss;
    oss << "Activate RPC sensor model estimation"<<std::endl;
    oss << "Parameter is the number of control points per axis";
    SetParameterDescription("rpc",oss.str().c_str());
    MandatoryOff("rpc");

    // Interpolators
    AddParameter(ParameterType_Choice,   "interpolator", "Interpolator");
    AddChoice("interpolator.linear", "Linear");
    AddChoice("interpolator.nn",     "Nearest Neighbor");
    AddChoice("interpolator.bco",    "BCO");
    AddParameter(ParameterType_Radius, "interpolator.bco.radius", "Radius");
    SetParameterInt("interpolator.bco.radius", 2);

    // Built the Output Map Projection
    AddParameter(ParameterType_Choice, "map", "Map Projection");
    
    AddChoice("map.utm",   "UTM");
    AddParameter(ParameterType_Int, "map.utm.zone", "Zone number");
    AddParameter(ParameterType_Empty, "map.utm.hem",  "Hemisphere North");

    AddChoice("map.lambert2",  "Lambert II Etendu"); 

    AddChoice("map.epsg","EPSG");
    AddParameter(ParameterType_Int, "map.epsg.code", "EPSG Code");    
    SetParameterInt("map.epsg.code",32631);
    SetParameterString("map", "epsg");

    // Deformation Field Spacing
    AddParameter(ParameterType_Float, "gridspacing", "Deformation Field Spacing");
    SetParameterInt("gridspacing", 4.);
    SetParameterDescription("gridspacing", "Generate a coarser deformation field with the given spacing");
    MandatoryOff("gridspacing");
  }

  void DoUpdateParameters()
  {
    if (HasValue("in"))
      {
      // input image 
      FloatVectorImageType::Pointer inImage = GetParameterImage("in");

      // Update the UTM zone params
      InitializeUTMParameters();
      // Get the output projection Ref
      this->UpdateOutputProjectionRef();

      // Compute the output image spacing and size
      typedef otb::ImageToGenericRSOutputParameters<FloatVectorImageType> OutputParametersEstimatorType;
      OutputParametersEstimatorType::Pointer genericRSEstimator = OutputParametersEstimatorType::New();
      
      if(IsParameterEnabled("outputs.isotropic"))
        {
        genericRSEstimator->EstimateIsotropicSpacingOn();
        }
      else
        {
        genericRSEstimator->EstimateIsotropicSpacingOff();
        }
      
      genericRSEstimator->SetInput(inImage);
      genericRSEstimator->SetOutputProjectionRef(m_OutputProjectionRef);
      genericRSEstimator->Compute();
      
      // Update the GUI
      dynamic_cast< NumericalParameter<float> * >(GetParameterByKey("outputs.spacingy"))->SetMinimumValue(-50000.0);
      dynamic_cast< NumericalParameter<float> * >(GetParameterByKey("outputs.spacingy"))->SetMaximumValue(10000.0);
      
      // Fill the Gui with the computed parameters      
      if (!HasUserValue("outputs.ulx"))
        SetParameterFloat("outputs.ulx", genericRSEstimator->GetOutputOrigin()[0]);
      
      if (!HasUserValue("outputs.uly"))
        SetParameterFloat("outputs.uly", genericRSEstimator->GetOutputOrigin()[1]);

      if (!HasUserValue("outputs.sizex"))
        SetParameterInt("outputs.sizex", genericRSEstimator->GetOutputSize()[0]);

      if (!HasUserValue("outputs.sizey"))
        SetParameterInt("outputs.sizey", genericRSEstimator->GetOutputSize()[1]);

      if (!HasUserValue("outputs.spacingx"))
        SetParameterFloat("outputs.spacingx", genericRSEstimator->GetOutputSpacing()[0]);

      if (!HasUserValue("outputs.spacingy"))
        SetParameterFloat("outputs.spacingy", genericRSEstimator->GetOutputSpacing()[1]);

      // Handle the spacing and size field following the mode 
      // choosed by the user
      switch (GetParameterInt("outputs.mode") )
        {
        case Mode_UserDefined:
        {
        // Automatic set to off
        AutomaticValueOff("outputs.sizex");
        AutomaticValueOff("outputs.sizey");
        AutomaticValueOff("outputs.spacingx");
        AutomaticValueOff("outputs.spacingy");

        // Enable add the parameters
        EnableParameter("outputs.spacingx");
        EnableParameter("outputs.spacingy");
        EnableParameter("outputs.sizex");
        EnableParameter("outputs.sizey");

        // Make all the parameters in this mode mandatory
        MandatoryOn("outputs.spacingx");
        MandatoryOn("outputs.spacingy");
        MandatoryOn("outputs.sizex");
        MandatoryOn("outputs.sizey");
        }
        break;
        case Mode_AutomaticSize:
        {
        // Disable the size fields
        DisableParameter("outputs.sizex");
        DisableParameter("outputs.sizey");
        EnableParameter("outputs.spacingx");
        EnableParameter("outputs.spacingy");

        // Update the automatic value mode of each filed
        AutomaticValueOn("outputs.sizex");
        AutomaticValueOn("outputs.sizey");
        AutomaticValueOff("outputs.spacingx");
        AutomaticValueOff("outputs.spacingy");

        // Adapat the status of the param to this mode
        MandatoryOn("outputs.spacingx");
        MandatoryOn("outputs.spacingy");
        MandatoryOff("outputs.sizex");
        MandatoryOff("outputs.sizey");

        ResampleFilterType::SpacingType spacing;
        spacing[0] = GetParameterFloat("outputs.spacingx");
        spacing[1] = GetParameterFloat("outputs.spacingy");
        
        genericRSEstimator->ForceSpacingTo(spacing);
        genericRSEstimator->Compute();
        
        // Set the  processed size relative to this forced spacing
        SetParameterInt("outputs.sizex", genericRSEstimator->GetOutputSize()[0]);
        SetParameterInt("outputs.sizey", genericRSEstimator->GetOutputSize()[1]);
        }
        break;
        case Mode_AutomaticSpacing:
        {
        // Disable the spacing fields and enable the size fields
        DisableParameter("outputs.spacingx");
        DisableParameter("outputs.spacingy");
        EnableParameter("outputs.sizex");
        EnableParameter("outputs.sizey");

        // Update the automatic value mode of each filed
        AutomaticValueOn("outputs.spacingx");
        AutomaticValueOn("outputs.spacingy");
        AutomaticValueOff("outputs.sizex");
        AutomaticValueOff("outputs.sizey");

        // Adapat the status of the param to this mode
        MandatoryOff("outputs.spacingx");
        MandatoryOff("outputs.spacingy");
        MandatoryOn("outputs.sizex");
        MandatoryOn("outputs.sizey");

        ResampleFilterType::SizeType size;        
        size[0] = GetParameterInt("outputs.sizex");
        size[1] = GetParameterInt("outputs.sizey");

        genericRSEstimator->ForceSizeTo(size);
        genericRSEstimator->Compute();
        
        // Set the  processed spacing relative to this forced size
        SetParameterFloat("outputs.spacingx", genericRSEstimator->GetOutputSpacing()[0]);
        SetParameterFloat("outputs.spacingy", genericRSEstimator->GetOutputSpacing()[1]);
        }
        break;
        }
      }
  }

  void InitializeUTMParameters()
  {
    // Compute the zone and the hemisphere if not UserValue defined
    if(!HasUserValue("map.utm.zone")
       && HasValue("in")
       && !HasAutomaticValue("map.utm.zone"))
      {
      // Compute the Origin lat/long coordinate
      typedef otb::ImageToGenericRSOutputParameters<FloatVectorImageType> OutputParametersEstimatorType;
      OutputParametersEstimatorType::Pointer genericRSEstimator = OutputParametersEstimatorType::New();
      genericRSEstimator->SetInput(GetParameterImage("in"));
      genericRSEstimator->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(4326));
      genericRSEstimator->Compute();

      int zone = otb::Utils::GetZoneFromGeoPoint(genericRSEstimator->GetOutputOrigin()[0],
                                                 genericRSEstimator->GetOutputOrigin()[1]);
      // Update the UTM Gui fields
      SetParameterInt("map.utm.zone",zone);
      if(genericRSEstimator->GetOutputOrigin()[0] > 0.)
        {
        EnableParameter("map.utm.hem");
        }

      AutomaticValueOn("map.utm.zone");
      AutomaticValueOn("map.utm.hem");
      }
  }

  void UpdateOutputProjectionRef()
  {
    switch ( GetParameterInt("map") )
      {
      case Map_Utm:
      {
      typedef UtmInverseProjection  UtmProjectionType;
      UtmProjectionType::Pointer    utmProjection = UtmProjectionType::New();

      // Set the zone
      utmProjection->SetZone(GetParameterInt("map.utm.zone"));

      // Set the hem
      std::string hem = "N";
      if (!IsParameterEnabled("map.utm.hem"))
        hem = "S";
      utmProjection->SetHemisphere(hem[0]);            
        
      // Get the projection ref
      m_OutputProjectionRef = utmProjection->GetWkt();
      }
      break;
      case Map_Lambert2:
      {
      typedef Lambert2EtenduForwardProjection Lambert2ProjectionType;
      Lambert2ProjectionType::Pointer lambert2Projection = Lambert2ProjectionType::New();
      m_OutputProjectionRef = lambert2Projection->GetWkt();
      }
      break;
      case Map_Epsg:
      {
      m_OutputProjectionRef = otb::GeoInformationConversion::ToWKT(GetParameterInt("map.epsg.code"));
      }
      break;
      }
  }

  void DoExecute()
    {
    GetLogger()->Debug("Entering DoExecute");
    
    // Get the input image
    FloatVectorImageType* inImage = GetParameterImage("in");

    // Resampler Instanciation
    m_ResampleFilter = ResampleFilterType::New();
    m_ResampleFilter->SetInput(inImage);

    // Set the output projection Ref 
    m_ResampleFilter->SetInputProjectionRef(inImage->GetProjectionRef());
    m_ResampleFilter->SetInputKeywordList(inImage->GetImageKeywordlist());
    m_ResampleFilter->SetOutputProjectionRef(m_OutputProjectionRef);

    // Get Interpolator
    switch ( GetParameterInt("interpolator") )
      {
      case Interpolator_Linear:
      {
      typedef itk::LinearInterpolateImageFunction<FloatVectorImageType,
        double>          LinearInterpolationType;
      LinearInterpolationType::Pointer interpolator = LinearInterpolationType::New();
      m_ResampleFilter->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_NNeighbor:
      {
      typedef itk::NearestNeighborInterpolateImageFunction<FloatVectorImageType,
        double> NearestNeighborInterpolationType;
      NearestNeighborInterpolationType::Pointer interpolator = NearestNeighborInterpolationType::New();
      m_ResampleFilter->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_BCO:
      {
      typedef otb::BCOInterpolateImageFunction<FloatVectorImageType>     BCOInterpolationType;
      BCOInterpolationType::Pointer interpolator = BCOInterpolationType::New();
      interpolator->SetRadius(GetParameterInt("interpolator.bco.radius"));
      m_ResampleFilter->SetInterpolator(interpolator);
      }
      break;
      }

    // DEM 
    if (IsParameterEnabled("dem") && HasValue("dem"))
      {
      m_ResampleFilter->SetDEMDirectory(GetParameterString("dem"));
      }
    else
      {
      if ( otb::ConfigurationFile::GetInstance()->IsValid() )
        {
        m_ResampleFilter->SetDEMDirectory(otb::ConfigurationFile::GetInstance()->GetDEMDirectory());
        }
      }

    // If activated, generate RPC model
    if(IsParameterEnabled("rpc.ncp"))
      {
      //std::cout <<"Estimating the rpc Model " <<std::endl;
      m_ResampleFilter->EstimateInputRpcModelOn();
      m_ResampleFilter->SetInputRpcGridSize( GetParameterInt("rpc.ncp"));
      }
    
    // Set Output information
    ResampleFilterType::SizeType size;
    size[0] = GetParameterInt("outputs.sizex");
    size[1] = GetParameterInt("outputs.sizey");
    m_ResampleFilter->SetOutputSize(size);

    ResampleFilterType::SpacingType spacing;
    spacing[0] = GetParameterFloat("outputs.spacingx");
    spacing[1] = GetParameterFloat("outputs.spacingy");
    m_ResampleFilter->SetOutputSpacing(spacing);
    
    ResampleFilterType::OriginType ul;
    ul[0] = GetParameterFloat("outputs.ulx");
    ul[1] = GetParameterFloat("outputs.uly");
    m_ResampleFilter->SetOutputOrigin(ul);

    // Deformation Field spacing
    ResampleFilterType::SpacingType gridSpacing;
    if (IsParameterEnabled("gridspacing"))
      {
      gridSpacing[0] = GetParameterFloat("gridspacing");
      gridSpacing[1] = -GetParameterFloat("gridspacing");
      std::cout <<"grid spacing is "<<  gridSpacing << std::endl;
      m_ResampleFilter->SetDeformationFieldSpacing(gridSpacing);
      }

    // Output Image 
    SetParameterOutputImage("out", m_ResampleFilter->GetOutput());
    }

  ResampleFilterType::Pointer  m_ResampleFilter;
  std::string                  m_OutputProjectionRef;
  };

  } // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OrthoRectification)
