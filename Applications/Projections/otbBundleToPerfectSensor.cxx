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

#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "itkPixelBuilder.h"
#include "itkFixedArray.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{


class BundleToPerfectSensor : public Application
{
public:
  /** Standard class typedefs. */
  typedef BundleToPerfectSensor         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BundleToPerfectSensor, otb::Application);

private:

  void DoInit()
  {
    SetName("BundleToPerfectSensor");
    SetDescription("Perform P+XS pansharpening");

    // Documentation
    SetDocName("Bundle to perfect sensor");
    SetDocLongDescription("This application performs P+XS pansharpening.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
   
    AddDocTag(Tags::Geometry);
    AddDocTag(Tags::Pansharpening);

    AddParameter(ParameterType_InputImage,   "inp",   "Input PAN Image");
    SetParameterDescription("inp"," Input panchromatic image.");
    AddParameter(ParameterType_InputImage,   "inxs",  "Input XS Image");
    SetParameterDescription("inxs"," Input XS image.");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_Float,        "lms",   "Spacing of the deformation field");
    SetParameterDescription("lms"," Spacing of the deformation field. Default is 10 times the PAN image spacing.");
    AddParameter(ParameterType_OutputImage,  "out",   "Output image");
    SetParameterDescription("out"," Output image.");
    AddRAMParameter();

    MandatoryOff("lms");
   
    // Doc example parameter settings
    SetDocExampleParameterValue("inp", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("inxs", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "BundleToPerfectSensor.png uchar");
    
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    FloatVectorImageType* panchroV = GetParameterImage("inp");
    if ( panchroV->GetNumberOfComponentsPerPixel() != 1 )
      {
      itkExceptionMacro(<< "The panchromatic image must be a single channel image")
      }

    // Transform the PAN image to otb::Image
    typedef otb::Image<FloatVectorImageType::InternalPixelType> FloatImageType;
    typedef itk::VectorIndexSelectionCastImageFilter<FloatVectorImageType, FloatImageType> VectorIndexSelectionCastImageFilterType;

    VectorIndexSelectionCastImageFilterType::Pointer channelSelect = VectorIndexSelectionCastImageFilterType::New();
    m_Ref.push_back(channelSelect.GetPointer());
    channelSelect->SetIndex(0);
    channelSelect->SetInput(panchroV);
    channelSelect->UpdateOutputInformation();
    FloatImageType::Pointer panchro = channelSelect->GetOutput();


    FloatVectorImageType* xs = GetParameterImage("inxs");

    typedef otb::BCOInterpolateImageFunction<FloatVectorImageType> InterpolatorType;
    typedef otb::GenericRSResampleImageFilter<FloatVectorImageType, FloatVectorImageType>  ResamplerType;
    typedef otb::SimpleRcsPanSharpeningFusionImageFilter<FloatImageType, FloatVectorImageType, FloatVectorImageType> FusionFilterType;

    // Resample filter
    ResamplerType::Pointer    resampler = ResamplerType::New();
    m_Ref.push_back(resampler.GetPointer());

    //Elevation handler is faster calling OSSIM elevation source
    resampler->SetDeformationFilterNumberOfThreads(1);

    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    resampler->SetInterpolator(interpolator);
    
    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // Set up output image informations
    FloatVectorImageType::SpacingType spacing = panchro->GetSpacing();
    FloatVectorImageType::IndexType   start = panchro->GetLargestPossibleRegion().GetIndex();
    FloatVectorImageType::SizeType    size = panchro->GetLargestPossibleRegion().GetSize();
    FloatVectorImageType::PointType   origin = panchro->GetOrigin();

    if(IsParameterEnabled("lms") && HasValue("lms"))
      {
      double defScalarSpacing = GetParameterFloat("lms");
      otbAppLogINFO(<< "Generating coarse deformation field (spacing="<<defScalarSpacing<<")" << std::endl);
      FloatVectorImageType::SpacingType defSpacing;

      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = defScalarSpacing;
      
      resampler->SetDeformationFieldSpacing(defSpacing);
      }
    else
      {
      FloatVectorImageType::SpacingType defSpacing;
      defSpacing[0]=10*spacing[0];
      defSpacing[1]=10*spacing[1];
      resampler->SetDeformationFieldSpacing(defSpacing);
      }
    
    FloatVectorImageType::PixelType defaultValue;
    itk::PixelBuilder<FloatVectorImageType::PixelType>::Zero(defaultValue,
                                                             xs->GetNumberOfComponentsPerPixel());

    resampler->SetInput(xs);
    resampler->SetOutputOrigin(origin);
    resampler->SetOutputSpacing(spacing);
    resampler->SetOutputSize(size);
    resampler->SetOutputStartIndex(start);
    resampler->SetOutputKeywordList(panchro->GetImageKeywordlist());
    resampler->SetOutputProjectionRef(panchro->GetProjectionRef());
    resampler->SetEdgePaddingValue(defaultValue);

    resampler->UpdateOutputInformation();
    FloatVectorImageType::Pointer xsResample = resampler->GetOutput();

    FusionFilterType::Pointer  fusionFilter = FusionFilterType::New();
    m_Ref.push_back(fusionFilter.GetPointer());

    fusionFilter->SetPanInput(panchro);
    fusionFilter->SetXsInput(resampler->GetOutput());
    fusionFilter->UpdateOutputInformation();
    
    SetParameterOutputImage("out", fusionFilter->GetOutput());
  }

  std::vector<itk::ProcessObject::Pointer> m_Ref;

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::BundleToPerfectSensor)
