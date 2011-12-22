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

#include "otbGenericRSResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{

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

  typedef otb::BCOInterpolateImageFunction<UInt16VectorImageType>   InterpolatorType;
  typedef otb::GenericRSResampleImageFilter<UInt16VectorImageType,
                                            UInt16VectorImageType>  ResamplerType;

private:
  void DoInit()
  {
    SetName("Superimpose");
    SetDescription("Using available image metadata, project one image onto another one");

    // Documentation
    SetDocName("Superimpose sensor application");
    SetDocLongDescription("This application performs /....");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
 
    AddDocTag(Tags::Geometry);
    AddDocTag("Superimposition");

    AddParameter(ParameterType_InputImage,   "inr",   "Reference input");
    AddParameter(ParameterType_InputImage,   "inm",   "The image to reproject");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_Float,        "lms",   "Spacing of the deformation field");
    SetParameterDescription("lms","Generate a coarser deformation field with the given spacing");
    SetDefaultParameterFloat("lms", 4.);

    AddParameter(ParameterType_OutputImage,  "out",   "Output image");
    AddParameter(ParameterType_RAM,          "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);

    MandatoryOff("lms");
    MandatoryOff("ram");

    // Doc example parameter settings
    SetDocExampleParameterValue("inr", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("inm", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "SuperimposedXS_to_PAN.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  
  void DoExecute()
  {
    // Get the inputs
    UInt16VectorImageType* refImage = GetParameterUInt16VectorImage("inr");
    UInt16VectorImageType* movingImage = GetParameterUInt16VectorImage("inm");
    
    // Resample filter
    m_Resampler    = ResamplerType::New();
    m_Interpolator = InterpolatorType::New();
    m_Resampler->SetInterpolator(m_Interpolator);
    
    // Elevation through the elevation handler
    if (ElevationParametersHandler::IsElevationEnabled(this, "elev"))
      {
      switch(ElevationParametersHandler::GetElevationType(this, "elev"))
        {
        case Elevation_DEM:
        {
        m_Resampler->SetDEMDirectory(ElevationParametersHandler::GetDEMDirectory(this, "elev"));
        m_Resampler->SetGeoidFile(ElevationParametersHandler::GetGeoidFile(this, "elev"));
        }
        break;
        case Elevation_Average:
        {
        m_Resampler->SetAverageElevation(ElevationParametersHandler::GetAverageElevation(this, "elev"));
        }
        break;
        //   Commented cause using a tiff file is not implemented yet
        //  case Elevation_Tiff:
        //  {
        //  }
        //  break;
        }
      }
    
    // Set up output image informations
    UInt16VectorImageType::SpacingType spacing = refImage->GetSpacing();
    UInt16VectorImageType::IndexType   start   = refImage->GetLargestPossibleRegion().GetIndex();
    UInt16VectorImageType::SizeType    size    = refImage->GetLargestPossibleRegion().GetSize();
    UInt16VectorImageType::PointType   origin  = refImage->GetOrigin();

    if(IsParameterEnabled("lms"))
      {
      float defScalarSpacing = GetParameterFloat("lms");
      std::cout<<"Generating coarse deformation field (spacing="<<defScalarSpacing<<")"<<std::endl;
      UInt16VectorImageType::SpacingType defSpacing;

      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = defScalarSpacing;
      
      m_Resampler->SetDeformationFieldSpacing(defSpacing);
      }
    
    UInt16VectorImageType::PixelType defaultValue;
    itk::PixelBuilder<UInt16VectorImageType::PixelType>::Zero(defaultValue,
                                                              movingImage->GetNumberOfComponentsPerPixel());

    m_Resampler->SetInput(movingImage);
    m_Resampler->SetOutputOrigin(origin);
    m_Resampler->SetOutputSpacing(spacing);
    m_Resampler->SetOutputSize(size);
    m_Resampler->SetOutputStartIndex(start);
    m_Resampler->SetOutputKeywordList(refImage->GetImageKeywordlist());
    m_Resampler->SetOutputProjectionRef(refImage->GetProjectionRef());
    m_Resampler->SetEdgePaddingValue(defaultValue);
    
    // Set the output image
    SetParameterOutputImage("out", m_Resampler->GetOutput());
  }

  ResamplerType::Pointer           m_Resampler;
  InterpolatorType::Pointer        m_Interpolator;
};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::Superimpose)

  
