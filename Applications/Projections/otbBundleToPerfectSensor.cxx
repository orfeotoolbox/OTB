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
#include "itkFixedArray.h"
#include "itkScalableAffineTransform.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

#include "otbDateTimeAdapter.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbPleiadesImageMetadataInterface.h"

namespace otb
{
namespace Wrapper
{

enum
{
  Mode_Default,
  Mode_PHR
};

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
    if (!HasUserValue("mode") &&
        GetParameterInt("mode") == Mode_Default &&
        HasValue("inp") &&
        HasValue("inxs"))
      {
      FloatVectorImageType* refImage = GetParameterImage("inp");
      FloatVectorImageType* movingImage = GetParameterImage("inxs");
      bool isRefPHR = false;
      bool isMovingPHR = false;
      
      otb::PleiadesImageMetadataInterface::Pointer phrIMI =
        otb::PleiadesImageMetadataInterface::New();
      phrIMI->SetMetaDataDictionary(refImage->GetMetaDataDictionary());
      isRefPHR = phrIMI->CanRead();
      
      phrIMI->SetMetaDataDictionary(movingImage->GetMetaDataDictionary());
      isMovingPHR = phrIMI->CanRead();
      
      if (isRefPHR && isMovingPHR)
        {
        ImageKeywordlist kwlPan;
        ImageKeywordlist kwlXS;
        
        itk::ExposeMetaData<ImageKeywordlist>(
          refImage->GetMetaDataDictionary(),
          MetaDataKey::OSSIMKeywordlistKey,
          kwlPan);
        
        itk::ExposeMetaData<ImageKeywordlist>(
          movingImage->GetMetaDataDictionary(),
          MetaDataKey::OSSIMKeywordlistKey,
          kwlXS);
        
        // Get geometric processing
        std::string panProcessing = kwlPan.GetMetadataByKey("support_data.processing_level");
        std::string xsProcessing = kwlXS.GetMetadataByKey("support_data.processing_level");
        
        if (panProcessing.compare("SENSOR") == 0 &&
            xsProcessing.compare("SENSOR") == 0)
          {
          SetParameterInt("mode",Mode_PHR);
          }
        }
      }
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
    typedef otb::StreamingResampleImageFilter<FloatVectorImageType, FloatVectorImageType>  BasicResamplerType;
    typedef otb::SimpleRcsPanSharpeningFusionImageFilter<FloatImageType, FloatVectorImageType, FloatVectorImageType> FusionFilterType;

    // Resample filter
    ResamplerType::Pointer    resampler = ResamplerType::New();
    m_Ref.push_back(resampler.GetPointer());
    
    BasicResamplerType::Pointer basicResampler = BasicResamplerType::New();
    m_Ref.push_back(basicResampler.GetPointer());

    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    resampler->SetInterpolator(interpolator);
    basicResampler->SetInterpolator(interpolator);

    // Fusion filter
    FusionFilterType::Pointer  fusionFilter = FusionFilterType::New();
    m_Ref.push_back(fusionFilter.GetPointer());
    fusionFilter->SetPanInput(panchro);
    
    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // Set up output image informations
    FloatVectorImageType::SpacingType spacing = panchro->GetSpacing();
    FloatVectorImageType::IndexType   start = panchro->GetLargestPossibleRegion().GetIndex();
    FloatVectorImageType::SizeType    size = panchro->GetLargestPossibleRegion().GetSize();
    FloatVectorImageType::PointType   origin = panchro->GetOrigin();

    FloatVectorImageType::PixelType defaultValue;
    itk::NumericTraits<FloatVectorImageType::PixelType>::SetLength(defaultValue, xs->GetNumberOfComponentsPerPixel());

    switch ( GetParameterInt("mode") )
    {
    case Mode_Default:
      {
      if(IsParameterEnabled("lms") && HasValue("lms"))
        {
        double defScalarSpacing = GetParameterFloat("lms");
        otbAppLogINFO(<< "Generating coarse deformation field (spacing="<<defScalarSpacing<<")" << std::endl);
        FloatVectorImageType::SpacingType defSpacing;

        defSpacing[0] = defScalarSpacing;
        defSpacing[1] = defScalarSpacing;

        resampler->SetDisplacementFieldSpacing(defSpacing);
        }
      else
        {
        FloatVectorImageType::SpacingType defSpacing;
        defSpacing[0]=10*spacing[0];
        defSpacing[1]=10*spacing[1];
        resampler->SetDisplacementFieldSpacing(defSpacing);
        }
      
      resampler->SetInput(xs);
      resampler->SetOutputOrigin(origin);
      resampler->SetOutputSpacing(spacing);
      resampler->SetOutputSize(size);
      resampler->SetOutputStartIndex(start);
      resampler->SetOutputKeywordList(panchro->GetImageKeywordlist());
      resampler->SetOutputProjectionRef(panchro->GetProjectionRef());
      resampler->SetEdgePaddingValue(defaultValue);
      //resampler->UpdateOutputInformation();
      // TODO : set the Input keywordlist and ProjRef manually ??
      fusionFilter->SetXsInput(resampler->GetOutput());
      //fusionFilter->UpdateOutputInformation();
      }
      break;
    case Mode_PHR:
      {
      otbAppLogINFO("Using the PHR mode");
      // Setup a simple affine transform using PHR support data
      ImageKeywordlist kwlPan;
      ImageKeywordlist kwlXS;
      
      itk::ExposeMetaData<ImageKeywordlist>(
        panchroV->GetMetaDataDictionary(),
        MetaDataKey::OSSIMKeywordlistKey,
        kwlPan);
      
      itk::ExposeMetaData<ImageKeywordlist>(
        xs->GetMetaDataDictionary(),
        MetaDataKey::OSSIMKeywordlistKey,
        kwlXS);
      
      // Compute time delta
      std::string strStartTimePan = kwlPan.GetMetadataByKey("support_data.time_range_start");
      std::string strStartTimeXS = kwlXS.GetMetadataByKey("support_data.time_range_start");
      
      DateTimeAdapter::Pointer startTimePan = DateTimeAdapter::New();
      DateTimeAdapter::Pointer startTimeXS = DateTimeAdapter::New();
      
      startTimePan->SetFromIso8601(strStartTimePan);
      startTimeXS->SetFromIso8601(strStartTimeXS);
      
      double timeDelta = startTimeXS->GetDeltaInSeconds(startTimePan);
      
      // Retrieve line period in Pan
      std::string tmpStr = kwlPan.GetMetadataByKey("support_data.line_period");
      double linePeriodPan = atof(tmpStr.c_str());
      
      // Retrieve column start
      tmpStr = kwlPan.GetMetadataByKey("support_data.swath_first_col");
      int colStartPan = atoi(tmpStr.c_str());
      tmpStr = kwlXS.GetMetadataByKey("support_data.swath_first_col");
      int colStartXS = atoi(tmpStr.c_str());
      
      // Compute shift between MS and P (in Pan pixels)
      int lineShift_MS_P =int(vcl_floor((timeDelta/(linePeriodPan/1000))  + 0.5));
      int colShift_MS_P = colStartXS*4 - colStartPan - 4;
      
      // Apply the scaling
      typedef itk::ScalableAffineTransform<double, 2>  TransformType;
      TransformType::Pointer transform = TransformType::New();
      transform->Scale(4.0);
      
      // Resample filter assumes the origin is attached to the pixel center
      // in order to keep the top left corners unchanged, apply a 3/2 pixels
      // shift in each direction
      // TODO : clarify formula, the '-1.0' for columns is strange
      TransformType::OutputVectorType offset;
      offset[0] = 1.5 + static_cast<double>(colShift_MS_P);
      offset[1] = 1.5 + static_cast<double>(lineShift_MS_P);
      transform->Translate(offset);
      
      // Invert the transform as the resampler filter expect an output-to-input
      // transform (we have computed the input-to-output transform)
      TransformType::Pointer realTransform = TransformType::New();
      transform->GetInverse(realTransform);
      basicResampler->SetTransform(realTransform);
      
      basicResampler->SetInput(xs);
      basicResampler->SetOutputOrigin(origin);
      basicResampler->SetOutputSpacing(spacing);
      basicResampler->SetOutputSize(size);
      basicResampler->SetOutputStartIndex(start);
      basicResampler->SetEdgePaddingValue(defaultValue);
      
      fusionFilter->SetXsInput(basicResampler->GetOutput());

      // Set the profRef & Keywordlist from Pan into the resampled XS image
      basicResampler->UpdateOutputInformation();
      itk::MetaDataDictionary& dict = basicResampler->GetOutput()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey,
                                            panchro->GetProjectionRef());
      itk::EncapsulateMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey,
                                                 panchro->GetImageKeywordlist());
      }
      break;
    default:
      {
      otbAppLogWARNING("Unknown mode");
      }
      break;
    }
    
    SetParameterOutputImage("out", fusionFilter->GetOutput());
  }

  std::vector<itk::ProcessObject::Pointer> m_Ref;

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::BundleToPerfectSensor)
