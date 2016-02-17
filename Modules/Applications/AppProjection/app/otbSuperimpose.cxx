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
#include "otbWrapperApplicationFactory.h"

#include "otbGenericRSResampleImageFilter.h"

#include "otbBCOInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"
#include "otbPleiadesPToXSAffineTransformCalculator.h"

namespace otb
{

enum
{
  Interpolator_BCO,
  Interpolator_NNeighbor,
  Interpolator_Linear
};

namespace Wrapper
{

class Superimpose : public Application
{
public:
  /** Standard class typedefs. */
  typedef Superimpose         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Superimpose, Application);

  typedef unsigned short int PixelType;

  typedef itk::LinearInterpolateImageFunction
    <FloatVectorImageType,
     double>                                                      LinInterpolatorType;
  typedef itk::NearestNeighborInterpolateImageFunction
    <FloatVectorImageType,
     double>                                                      NNInterpolatorType;
  typedef otb::BCOInterpolateImageFunction
    <FloatVectorImageType>                                        BCOInterpolatorType;

  typedef otb::GenericRSResampleImageFilter<FloatVectorImageType,
                                            FloatVectorImageType>  ResamplerType;

  typedef itk::ScalableAffineTransform<double, 2>                 TransformType;
  
  typedef otb::StreamingResampleImageFilter
    <FloatVectorImageType,
     FloatVectorImageType>                                        BasicResamplerType;
  
private:
  void DoInit()
  {
    SetName("Superimpose");
    SetDescription("Using available image metadata, project one image onto another one");

    // Documentation
    SetDocName("Superimpose sensor");
    SetDocLongDescription("This application performs the projection of an image into the geometry of another one.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
    AddDocTag("Superimposition");

    AddParameter(ParameterType_InputImage,   "inr",   "Reference input");
    SetParameterDescription("inr","The input reference image.");
    AddParameter(ParameterType_InputImage,   "inm",   "The image to reproject");
    SetParameterDescription("inm","The image to reproject into the geometry of the reference input.");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_Float,        "lms",   "Spacing of the deformation field");
    SetParameterDescription("lms","Generate a coarser deformation field with the given spacing");
    SetDefaultParameterFloat("lms", 4.);
    MandatoryOff("lms");

    AddParameter(ParameterType_OutputImage,  "out",   "Output image");
    SetParameterDescription("out","Output reprojected image.");

    // Superposition mode
    AddParameter(ParameterType_Choice,"mode", "Mode");
    SetParameterDescription("mode", "Superimposition mode");
    
    AddChoice("mode.default", "Default mode");
    SetParameterDescription("mode.default", "Default superimposition mode : "
      "uses any projection reference or sensor model found in the images");
    
    AddChoice("mode.phr", "Pleiades mode");
    SetParameterDescription("mode.phr", "Pleiades superimposition mode, "
      "designed for the case of a P+XS bundle in SENSOR geometry. It uses"
      " a simple transform on the XS image : a scaling and a residual "
      "translation.");
    
    // Interpolators
    AddParameter(ParameterType_Choice,   "interpolator", "Interpolation");
    SetParameterDescription("interpolator","This group of parameters allows defining how the input image will be interpolated during resampling.");

    AddChoice("interpolator.bco",    "Bicubic interpolation");
    SetParameterDescription("interpolator.bco", "Bicubic interpolation leads to very good image quality but is slow.");

    AddParameter(ParameterType_Radius, "interpolator.bco.radius", "Radius for bicubic interpolation");
    SetParameterDescription("interpolator.bco.radius","This parameter allows controling the size of the bicubic interpolation filter. If the target pixel size is higher than the input pixel size, increasing this parameter will reduce aliasing artefacts.");
    SetDefaultParameterInt("interpolator.bco.radius", 2);

    AddChoice("interpolator.nn",     "Nearest Neighbor interpolation");
    SetParameterDescription("interpolator.nn","Nearest neighbor interpolation leads to poor image quality, but it is very fast.");

    AddChoice("interpolator.linear", "Linear interpolation");
    SetParameterDescription("interpolator.linear","Linear interpolation leads to average image quality but is quite fast");
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("inr", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("inm", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "SuperimposedXS_to_PAN.tif");
  }

  void DoUpdateParameters()
  {
    if(!HasUserValue("mode") && HasValue("inr") && HasValue("inm") && otb::PleiadesPToXSAffineTransformCalculator::CanCompute(GetParameterImage("inr"),GetParameterImage("inm")))
      {
      otbAppLogWARNING("Forcing PHR mode with PHR data. You need to add \"-mode default\" to force the default mode with PHR images.");
      SetParameterString("mode","phr");
      }
  }


  void DoExecute()
  {
    // Get the inputs
    FloatVectorImageType* refImage = GetParameterImage("inr");
    FloatVectorImageType* movingImage = GetParameterImage("inm");

    // Resample filter
    m_Resampler = ResamplerType::New();
    
    m_BasicResampler = BasicResamplerType::New();

    // Get Interpolator
    switch ( GetParameterInt("interpolator") )
      {
      case Interpolator_Linear:
      {
      LinInterpolatorType::Pointer interpolator = LinInterpolatorType::New();
      m_Resampler->SetInterpolator(interpolator);
      m_BasicResampler->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_NNeighbor:
      {
      NNInterpolatorType::Pointer interpolator = NNInterpolatorType::New();
      m_Resampler->SetInterpolator(interpolator);
      m_BasicResampler->SetInterpolator(interpolator);
      }
      break;
      case Interpolator_BCO:
      {
      BCOInterpolatorType::Pointer interpolator = BCOInterpolatorType::New();
      interpolator->SetRadius(GetParameterInt("interpolator.bco.radius"));
      m_Resampler->SetInterpolator(interpolator);
      m_BasicResampler->SetInterpolator(interpolator);
      }
      break;
      }


    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // Set up output image informations
    FloatVectorImageType::SpacingType spacing = refImage->GetSpacing();
    FloatVectorImageType::IndexType   start   = refImage->GetLargestPossibleRegion().GetIndex();
    FloatVectorImageType::SizeType    size    = refImage->GetLargestPossibleRegion().GetSize();
    FloatVectorImageType::PointType   origin  = refImage->GetOrigin();
    
    FloatVectorImageType::PixelType defaultValue;
    itk::NumericTraits<FloatVectorImageType::PixelType>::SetLength(defaultValue, movingImage->GetNumberOfComponentsPerPixel());

    
    if(GetParameterString("mode")=="default")
      {
      if(IsParameterEnabled("lms"))
        {
        float defScalarSpacing = vcl_abs(GetParameterFloat("lms"));
        otbAppLogDEBUG("Generating coarse deformation field (spacing="<<defScalarSpacing<<")");
        FloatVectorImageType::SpacingType defSpacing;

        defSpacing[0] = defScalarSpacing;
        defSpacing[1] = defScalarSpacing;

        if (spacing[0]<0.0) defSpacing[0] *= -1.0;
        if (spacing[1]<0.0) defSpacing[1] *= -1.0;

        m_Resampler->SetDisplacementFieldSpacing(defSpacing);
        }
      
      // Setup transform through projRef and Keywordlist
      m_Resampler->SetInputKeywordList(movingImage->GetImageKeywordlist());
      m_Resampler->SetInputProjectionRef(movingImage->GetProjectionRef());
      
      m_Resampler->SetOutputKeywordList(refImage->GetImageKeywordlist());
      m_Resampler->SetOutputProjectionRef(refImage->GetProjectionRef());
      
      m_Resampler->SetInput(movingImage);
      
      m_Resampler->SetOutputOrigin(origin);
      m_Resampler->SetOutputSpacing(spacing);
      m_Resampler->SetOutputSize(size);
      m_Resampler->SetOutputStartIndex(start);
      
      m_Resampler->SetEdgePaddingValue(defaultValue);

      // Set the output image
      SetParameterOutputImage("out", m_Resampler->GetOutput());
      }
    else if(GetParameterString("mode")=="phr")
      {
      otbAppLogINFO("Using the PHR mode");
      
      otb::PleiadesPToXSAffineTransformCalculator::TransformType::Pointer transform
        = otb::PleiadesPToXSAffineTransformCalculator::Compute(GetParameterImage("inr"),
                                                               GetParameterImage("inm"));

      m_BasicResampler->SetTransform(transform);
      
      m_BasicResampler->SetInput(movingImage);
      
      m_BasicResampler->SetOutputOrigin(origin);
      m_BasicResampler->SetOutputSpacing(spacing);
      m_BasicResampler->SetOutputSize(size);
      m_BasicResampler->SetOutputStartIndex(start);
      
      m_BasicResampler->SetEdgePaddingValue(defaultValue);

      // Set the output image
      SetParameterOutputImage("out", m_BasicResampler->GetOutput());
      }
    else
      {
      otbAppLogWARNING("Unknown mode");
      }
  }
   
  ResamplerType::Pointer           m_Resampler;
  
  BasicResamplerType::Pointer      m_BasicResampler;
  
};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::Superimpose)


