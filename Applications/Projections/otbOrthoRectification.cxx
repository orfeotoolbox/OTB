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

  typedef otb::PipelineMemoryPrintCalculator                       MemoryCalculatorType;

private:
  OrthoRectification()
  {
    SetName("OrthoRectification");
    SetDescription("Using available image metadata to determine the sensor model, \ncomputes a cartographic projection of the image");
  }

  void DoCreateParameters()
  {
    // Set the parameters
    AddParameter(ParameterType_InputImage, "in", "Input Image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Projected image");


    // Add the output paramters in a group
    AddParameter(ParameterType_Group, "outputs", "Output Parameters Group");
    MandatoryOn("outputs");

    // UserDefined values
    AddParameter(ParameterType_Empty, "outputs.uservalues",  "User defined Parameters");

    // Upper left point coordinates
    AddParameter(ParameterType_Float, "outputs.ulx", "Upper Left X");    
    SetParameterDescription("outputs.ulx","Cartographic X coordinate of upper left corner");
    //MandatoryOn("outputs.ulx");

    AddParameter(ParameterType_Float, "outputs.uly", "Upper Left Y");
    SetParameterDescription("outputs.uly","Cartographic Y coordinate of upper left corner");
    //MandatoryOn("outputs.uly");

    // Size of the output image
    AddParameter(ParameterType_Int, "outputs.sizex", "Size X");
    SetParameterDescription("outputs.sizex","Size of projected image along X");
    MandatoryOn("outputs.sizex");

    AddParameter(ParameterType_Int, "outputs.sizey", "Size Y");
    SetParameterDescription("outputs.sizey","Size of projected image along Y");
    MandatoryOn("outputs.sizey");
    
    // Spacing of the output image
    AddParameter(ParameterType_Float, "outputs.spacingx", "Pixel Size X");
    SetParameterDescription("outputs.spacingx","Size of each pixel along X axis");
    MandatoryOn("outputs.spacingx");

    AddParameter(ParameterType_Float, "outputs.spacingy", "Pixel Size Y");
    SetParameterDescription("outputs.spacingy","Size of each pixel along Y axis");
    MandatoryOn("outputs.spacingy");

    // DEM
    AddParameter(ParameterType_Directory, "dem",   "DEM directory");
    MandatoryOff("dem");

    // Estimate a RPC model (for spot image for instance)
    AddParameter(ParameterType_Group, "rpc", "Estimate RPC model");
    AddParameter(ParameterType_Int, "rpc.ncp", "Nb control Points");
    SetParameterDescription("rpc","Activate RPC sensor model estimation. Parameter is the number of control points per axis");
    MandatoryOff("rpc");
    MandatoryOff("rpc.ncp");

    // Interpolators
    AddParameter(ParameterType_Choice,   "interpolator", "Interpolator");
    AddChoice("interpolator.linear",     "Linear");
    AddChoice("interpolator.nn", "Neareast Neighbor");
    AddChoice("interpolator.bco",   "BCO");
    AddParameter(ParameterType_Radius, "interpolator.bco.radius", "Radius");
    SetParameterInt("interpolator.bco.radius", 2);

    // Built the Output Map Projection
    AddParameter(ParameterType_Choice, "map", "Map Projection");
    
    AddChoice("map.utm",   "UTM");   // OK
    AddParameter(ParameterType_Int, "map.utm.zone", "Zone number");
    AddParameter(ParameterType_Empty, "map.utm.hem",  "Hemisphere North");
    MandatoryOff("map.utm.zone");
    MandatoryOff("map.utm.hem");    
    

    AddChoice("map.lambert2",     "Lambert II Etendu"); // OK

    AddChoice("map.epsg","EPSG");
    AddParameter(ParameterType_Int, "map.epsg.code", "EPSG Code");    
    SetParameterInt("map.epsg.code",32631);
    SetParameterString("map", "epsg");

   //     descriptor->AddOption("LocMapSpacing",
   //     "Generate a coarser deformation field with the given spacing.","lmSpacing", 
   //     1, false, otb::ApplicationDescriptor::Real);
  }

  void DoUpdateParameters()
  {
    if (HasValue("in")) 
      {
      // input image 
      FloatVectorImageType* inImage = GetParameterImage("in");

      // Get the output projection Ref
      this->UpdateOutputProjectionRef();

      

      // Compute the output image spacing and size
      typedef otb::ImageToGenericRSOutputParameters<FloatVectorImageType> OutputParametersEstimatorType;
      OutputParametersEstimatorType::Pointer genericRSEstimator = OutputParametersEstimatorType::New();
      genericRSEstimator->SetInput(inImage);
      genericRSEstimator->SetOutputProjectionRef(m_OutputProjectionRef);
      genericRSEstimator->Compute();
      
      // TODO : Add a ParameterGroup with the output Parameters
      //        Add an option user defined param or automatic params

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

//       // Mode : user defined
//       if (!IsParameterEnabled("outputs.uservalues"))
//         {
//         // Enable UL corners widget
//         DisableParameter("outputs.ulx");
//         DisableParameter("outputs.uly");
//         // Force size or spacing to user defined params
//         if (HasUserValue("outputs.sizex") || HasUserValue("outputs.sizey"))
//           {
//           ResampleFilterType::SizeType size;        
//           size[0] = GetParameterInt("outputs.sizex");
//           size[1] = GetParameterInt("outputs.sizey");
//           genericRSEstimator->ForceSizeTo(size);
//           genericRSEstimator->Compute();
        
//           std::cout <<"Size Forced to "<<size  << " --> implies Sapcing : "
//                     <<genericRSEstimator->GetOutputSpacing()  << std::endl;
        
//           // Set the  processed spacing relative to this forced size
//           SetParameterFloat("outputs.spacingx", genericRSEstimator->GetOutputSpacing()[0]);
//           SetParameterFloat("outputs.spacingy", genericRSEstimator->GetOutputSpacing()[1]);
//           }
       
//         if (HasUserValue("outputs.spacingy") || HasUserValue("outputs.spacingx"))
//           {
//           ResampleFilterType::SpacingType spacing;
//           spacing[0] = GetParameterFloat("outputs.spacingx");
//           spacing[1] = GetParameterFloat("outputs.spacingy");

//           genericRSEstimator->ForceSpacingTo(spacing);
//           genericRSEstimator->Compute();

//           std::cout <<"Spacing Forced to "<< spacing<< " --> implies Size : "
//                     <<genericRSEstimator->GetOutputSize()  << std::endl;
        
//           // Set the  processed size relative to this forced spacing
//           SetParameterInt("outputs.sizex", genericRSEstimator->GetOutputSize()[0]);
//           SetParameterInt("outputs.sizey", genericRSEstimator->GetOutputSize()[1]);
//           }
//         }
//       else
//         {
//         // Enable UL corners widget
//         EnableParameter("outputs.ulx");
//         EnableParameter("outputs.uly");

//         }
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
      typedef itk::LinearInterpolateImageFunction<FloatVectorImageType, double>          LinearInterpolationType;
      LinearInterpolationType::Pointer interpolator = LinearInterpolationType::New();
      m_ResampleFilter->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_NNeighbor:
      {
      typedef itk::NearestNeighborInterpolateImageFunction<FloatVectorImageType, double> NearestNeighborInterpolationType;
      NearestNeighborInterpolationType::Pointer interpolator = NearestNeighborInterpolationType::New();
      m_ResampleFilter->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_BCO:
      {
      typedef otb::BCOInterpolateImageFunction<FloatVectorImageType>                     BCOInterpolationType;
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
    if (IsParameterEnabled("outputs.sizex") && IsParameterEnabled("outputs.sizey"))
      {
      size[0] = GetParameterInt("outputs.sizex");
      size[1] = GetParameterInt("outputs.sizey");
      m_ResampleFilter->SetOutputSize(size);
      }

    ResampleFilterType::SpacingType spacing;
    if (IsParameterEnabled("outputs.spacingx") && IsParameterEnabled("outputs.spacingy"))
      {
      spacing[0] = GetParameterFloat("outputs.spacingx");
      spacing[1] = GetParameterFloat("outputs.spacingy");
      m_ResampleFilter->SetOutputSpacing(spacing);
      }
    
    ResampleFilterType::OriginType ul;
    if (IsParameterEnabled("outputs.ulx") && IsParameterEnabled("outputs.uly"))
      {
      ul[0] = GetParameterFloat("outputs.ulx");
      ul[1] = GetParameterFloat("outputs.uly");
      m_ResampleFilter->SetOutputOrigin(ul);
      }

//     std::cout <<"Resampler : Output Origin "<<m_ResampleFilter->GetOutputOrigin()  << std::endl;
//     std::cout <<"Resampler : Output Spacing "<<m_ResampleFilter->GetOutputSpacing()  << std::endl;
//     std::cout <<"Resampler : Output Size "<<  m_ResampleFilter->GetOutputSize()<< std::endl;

    // Output Image 
    SetParameterOutputImage("out", m_ResampleFilter->GetOutput());
    }

  ResampleFilterType::Pointer  m_ResampleFilter;
  std::string                  m_OutputProjectionRef;
  };



  } // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OrthoRectification)
