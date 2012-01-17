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

#include "otbVectorImageToIntensityImageFilter.h"
#include "otbFineRegistrationImageFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbStreamingWarpImageFilter.h"

#include "otbImageFileReader.h"

#include "itkDiscreteGaussianImageFilter.h"
#include "itkAbsImageFilter.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkMeanReciprocalSquareDifferenceImageToImageMetric.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkFixedArray.h"


namespace otb
{
namespace Wrapper
{

template<class TVLV, class TFixedArray>
class VLVToFixedArray
{
public:
  TFixedArray operator()(const TVLV& vlv)
  {
    TFixedArray output;
    output[0] = vlv[0];
    output[1] = vlv[1];
    return output;
  }
};

class FineRegistration : public Application
{
public:
  /** Standard class typedefs. */
  typedef FineRegistration              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(FineRegistration, otb::Application);

  /** Filters typedef */
  static const unsigned int Dimension = 2;
  typedef double     PixelType;

  typedef itk::FixedArray<PixelType, Dimension>         DeformationValueType;
  typedef otb::Image< PixelType,  Dimension >           ScalarImageType;
  typedef otb::VectorImage<PixelType, Dimension>        VectorImageType;
  typedef otb::ImageList<ScalarImageType>               ImageListType;
  typedef otb::ImageListToVectorImageFilter
                <ImageListType, VectorImageType>        IL2VIFilterType;
  typedef otb::Image<DeformationValueType, Dimension>   FieldImageType;
  typedef otb::FineRegistrationImageFilter
    <ScalarImageType, ScalarImageType, FieldImageType>  RegistrationFilterType;
  typedef itk::DiscreteGaussianImageFilter
                <ScalarImageType, ScalarImageType>      GaussianFilterType;
  typedef itk::VectorIndexSelectionCastImageFilter
                <FieldImageType, ScalarImageType>       VectorImageToImageFilterType;
  typedef itk::AbsImageFilter
                <ScalarImageType, ScalarImageType>      AbsFilterType;
  typedef itk::BinaryThresholdImageFilter
                <ScalarImageType, ScalarImageType>      BinaryThresholdImageFilterType;
  typedef otb::VectorImageToIntensityImageFilter
                <FloatVectorImageType, ScalarImageType>  IntensityFilterType;
  typedef itk::NormalizedCorrelationImageToImageMetric
                <ScalarImageType, ScalarImageType>      NCCType;
  typedef itk::MeanSquaresImageToImageMetric
                <ScalarImageType, ScalarImageType>      MeanSquareType;
  typedef itk::MeanReciprocalSquareDifferenceImageToImageMetric
                <ScalarImageType, ScalarImageType>      MRSDType;
  typedef itk::MattesMutualInformationImageToImageMetric
                <ScalarImageType, ScalarImageType>      MIType;
  typedef otb::MultiChannelExtractROI
                <PixelType, PixelType>                  ExtractROIFilterType;
  typedef VLVToFixedArray
    <VectorImageType::PixelType, FieldImageType::PixelType> VLVToFixedArrayType;
  typedef itk::UnaryFunctorImageFilter
    <VectorImageType, FieldImageType, VLVToFixedArrayType>  CastFilterType;
  typedef StreamingWarpImageFilter
    <FloatVectorImageType, FloatVectorImageType, FieldImageType> WarpFilterType;
  typedef otb::ImageFileReader<VectorImageType>         InternalReaderType;

private:
  void DoInit()
  {
    SetName("FineRegistration");
    SetDescription("Estimate disparity map between two images.");

    SetDocName("Fine Registration");
    SetDocLongDescription("Estimate disparity map between two images. Output image contain x offset, y offset and metric value.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Stereo);

    AddParameter(ParameterType_InputImage,  "ref",   "Reference Image");
    SetParameterDescription( "ref", "The reference image." );
    
    AddParameter(ParameterType_InputImage,  "sec",   "Secondary Image");
    SetParameterDescription( "sec", "The secondary image." );
    
    AddParameter(ParameterType_OutputImage,  "out",   "Output Image");
    SetParameterDescription( "out", "The output image." );
    
    AddParameter(ParameterType_Int,  "erx",   "Exploration Radius X");
    SetParameterDescription( "erx", "The exploration radius along x (in pixels)" );
    SetMinimumParameterIntValue("erx", 0);
    
    AddParameter(ParameterType_Int,  "ery",   "Exploration Radius Y");
    SetParameterDescription( "ery", "The exploration radius along y (in pixels)" );
    SetMinimumParameterIntValue("ery", 0);
    
    AddParameter(ParameterType_Int,  "mrx",   "Metric Radius X");
    SetParameterDescription( "mrx", "Radius along x (in pixels) of the metric computation window" );
    SetMinimumParameterIntValue("mrx", 0);
    
    AddParameter(ParameterType_Int,  "mry",   "Metric Radius Y");
    SetParameterDescription( "mry", "Radius along y (in pixels) of the metric computation window" );
    SetMinimumParameterIntValue("mry", 0);
    
    AddParameter(ParameterType_InputImage,  "w",   "Image To Warp");
    SetParameterDescription( "w", "The image to warp after disparity estimation is completed" );
    MandatoryOff("w");
    
    AddParameter(ParameterType_OutputImage,  "wo",   "Output Warped Image");
    SetParameterDescription( "wo", "The output warped image" );
    MandatoryOff("wo");
    
    AddParameter(ParameterType_Float,  "cox",   "Coarse Offset X");
    SetParameterDescription( "cox", "Coarse offset along x (in physical space) between the two images" );
    SetDefaultParameterFloat("cox", 0.0);
    MandatoryOff("cox");
    
    AddParameter(ParameterType_Float,  "coy",   "Coarse Offset Y");
    SetParameterDescription( "coy", "Coarse offset along y (in physical space) between the two images" );
    SetDefaultParameterFloat("coy", 0.0);
    MandatoryOff("coy");
    
    AddParameter(ParameterType_Float,  "ssrx",   "Sub-Sampling Rate X");
    SetParameterDescription( "ssrx", "Generates a result at a coarser resolution with a given sub-sampling rate along X" );
    SetDefaultParameterFloat("ssrx", 1.0);
    SetMinimumParameterFloatValue("ssrx", 1.0);
    MandatoryOff("ssrx");
    
    AddParameter(ParameterType_Float,  "ssry",   "Sub-Sampling Rate Y");
    SetParameterDescription( "ssry", "Generates a result at a coarser resolution with a given sub-sampling rate along Y" );
    SetDefaultParameterFloat("ssry", 1.0);
    SetMinimumParameterFloatValue("ssry", 1.0);
    MandatoryOff("ssry");
    
    AddParameter(ParameterType_Float,  "rgsx",   "Reference Gaussian Smoothing X");
    SetParameterDescription( "rgsx", "Performs a gaussian smoothing of the reference image. Parameter is gaussian sigma (in pixels) in X direction." );
    MandatoryOff("rgsx");
    
    AddParameter(ParameterType_Float,  "rgsy",   "Reference Gaussian Smoothing Y");
    SetParameterDescription( "rgsy", "Performs a gaussian smoothing of the reference image. Parameter is gaussian sigma (in pixels) in Y direction." );
    MandatoryOff("rgsy");
    
    AddParameter(ParameterType_Float,  "sgsx",   "Secondary Gaussian Smoothing X");
    SetParameterDescription( "sgsx", "Performs a gaussian smoothing of the secondary image. Parameter is gaussian sigma (in pixels) in X direction." );
    MandatoryOff("sgsx");
    
    AddParameter(ParameterType_Float,  "sgsy",   "Secondary Gaussian Smoothing Y");
    SetParameterDescription( "sgsy", "Performs a gaussian smoothing of the secondary image. Parameter is gaussian sigma (in pixels) in Y direction." );
    MandatoryOff("sgsy");
    
    AddParameter(ParameterType_String,  "m",   "Metric");
    SetParameterDescription( "m", "Choose the metric used for block matching. Available metrics are cross-correlation (CC), cross-correlation with subtracted mean (CCSM), mean-square difference (MSD), mean reciprocal square difference (MRSD) and mutual information (MI). Default is cross-correlation" );
    MandatoryOff("m");
    
    AddParameter(ParameterType_Float,  "spa",   "SubPixelAccuracy");
    SetParameterDescription( "spa", "Metric extrema location will be refined up to the given accuracy. Default is 0.01" );
    SetDefaultParameterFloat("spa", 0.01);
    SetMinimumParameterFloatValue("spa", 0.0);
    MandatoryOff("spa");
    
    AddParameter(ParameterType_Float,  "vmlt",   "Validity Mask Lower Threshold");
    SetParameterDescription( "vmlt", "Lower threshold to obtain a validity mask." );
    MandatoryOff("vmlt");
    
    AddParameter(ParameterType_Float,  "vmut",   "Validity Mask Upper Threshold");
    SetParameterDescription( "vmut", "Upper threshold to obtain a validity mask." );
    MandatoryOff("vmut");
    
    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

    // Doc example parameter settings
    SetDocExampleParameterValue("ref", "StereoFixed.png");
    SetDocExampleParameterValue("sec", "StereoMoving.png");
    SetDocExampleParameterValue("out", "FineRegistration.tif");
    SetDocExampleParameterValue("erx", "2");
    SetDocExampleParameterValue("ery", "2");
    SetDocExampleParameterValue("mrx", "3");
    SetDocExampleParameterValue("mry", "3");
  }

  void DoUpdateParameters()
  {
    // Nothing to do
  }

  void DoExecute()
  {
    FloatVectorImageType::Pointer refImage = GetParameterImage("ref"); // fixed
    FloatVectorImageType::Pointer secImage = GetParameterImage("sec"); // moved
    
    m_IntensityRef = IntensityFilterType::New();
    m_IntensitySec = IntensityFilterType::New();
    
    m_IntensityRef->SetInput(refImage);
    m_IntensitySec->SetInput(secImage);
    
    // Retrieve main registration parameters
    RegistrationFilterType::SizeType radius, sradius;
    RegistrationFilterType::OffsetType ssrate;
    sradius[0] = GetParameterInt("erx");
    sradius[1] = GetParameterInt("ery");
    radius[0] = GetParameterInt("mrx");
    radius[1] = GetParameterInt("mry");
  
    double accuracy = static_cast<double>(GetParameterFloat("spa"));
    
    ssrate[0] = GetParameterFloat("ssrx");
    ssrate[1] = GetParameterFloat("ssry");
    
    RegistrationFilterType::SpacingType initialOffset;
    initialOffset[0] = GetParameterFloat("cox");
    initialOffset[1] = GetParameterFloat("coy");
  
    // Display info
    otbAppLogINFO("Reference image   : "<<GetParameterString("ref"));
    otbAppLogINFO("Secondary image   : "<<GetParameterString("sec"));
    otbAppLogINFO("Exploration radius: "<<sradius<<" (pixels)");
    otbAppLogINFO("Metric radius     : "<<radius<<" (pixels)");
    otbAppLogINFO("Sub-sampling rate : "<<ssrate<<" (pixels)");
    otbAppLogINFO("Coarse offset     : "<<initialOffset<<" (physical unit)");
    otbAppLogINFO("Accuracy          : "<<accuracy<<" (physical unit)");
    
    m_Registration = RegistrationFilterType::New();
    m_Registration->SetRadius(radius);
    m_Registration->SetSearchRadius(sradius);
    m_Registration->SetSubPixelAccuracy(accuracy);
    m_Registration->SetGridStep(ssrate);
    m_Registration->SetInitialOffset(initialOffset);
  
    if(HasValue("rgsx") && HasValue("rgsy"))
      {
      m_RefGaussianFilter = GaussianFilterType::New();
      m_RefGaussianFilter->SetInput(m_IntensityRef->GetOutput());
      GaussianFilterType::ArrayType sigma;
      sigma[0] = GetParameterFloat("rgsx");
      sigma[1] = GetParameterFloat("rgsy");
      m_RefGaussianFilter->SetVariance(sigma);
      m_RefGaussianFilter->SetUseImageSpacingOff();
      otbAppLogINFO("Reference image gaussian smoothing : ON");
      otbAppLogINFO("variance        : "<<sigma<<" (pixels)");
      m_Registration->SetFixedInput(m_RefGaussianFilter->GetOutput());
      }
    else
      {
      otbAppLogINFO("Reference image gaussian smoothing : OFF");
      m_Registration->SetFixedInput(m_IntensityRef->GetOutput());
      }
  
    if(HasValue("sgsx") && HasValue("sgsy"))
        {
        m_SecGaussianFilter = GaussianFilterType::New();
        m_SecGaussianFilter->SetInput(m_IntensitySec->GetOutput());
        GaussianFilterType::ArrayType sigma;
        sigma[0] = GetParameterFloat("sgsx");
        sigma[1] = GetParameterFloat("sgsy");
        m_SecGaussianFilter->SetVariance(sigma);
        m_SecGaussianFilter->SetUseImageSpacingOff();
        otbAppLogINFO("Secondary image gaussian smoothing : ON");
        otbAppLogINFO("variance : "<<sigma<<" (pixels)");
        m_Registration->SetMovingInput(m_SecGaussianFilter->GetOutput());
        }
      else
        {
        otbAppLogINFO("Secondary image gaussian smoothing : OFF");
        m_Registration->SetMovingInput(m_IntensitySec->GetOutput());
        }
  
    // Retrieve metric name
    std::string metricId = "CC";
    if(HasValue("m"))
      {
      metricId = GetParameterString("m");
      }
  
    if(metricId == "CC")
      {
      otbAppLogINFO("Metric : Cross-correlation");
      m_NCCMetricPtr = NCCType::New();
      m_NCCMetricPtr->SubtractMeanOff();
      m_Registration->SetMetric(m_NCCMetricPtr);
      m_Registration->MinimizeOn();
      }
    else if(metricId == "CCSM")
      {
      otbAppLogINFO("Metric : Cross-correlation (mean subtracted)");
      m_NCCMetricPtr = NCCType::New();
      m_NCCMetricPtr->SubtractMeanOn();
      m_Registration->SetMetric(m_NCCMetricPtr);
      m_Registration->MinimizeOn();
      }
    else if(metricId == "MSD")
      {
      otbAppLogINFO("Metric : Mean square difference");
      m_MetricPtr = MeanSquareType::New();
      m_Registration->SetMetric(m_MetricPtr);
      m_Registration->MinimizeOn();
      }
    else if(metricId == "MRSD")
      {
      otbAppLogINFO("Metric : Mean reciprocal square difference");
      m_MetricPtr = MRSDType::New();
      m_Registration->SetMetric(m_MetricPtr);
      m_Registration->MinimizeOff();
      }
    else if(metricId == "MI")
      {
      otbAppLogINFO("Metric : Mutual information");
      m_MetricPtr = MIType::New();
      m_Registration->SetMetric(m_MetricPtr);
      m_Registration->MinimizeOn();
      }
    else
      {
      itkExceptionMacro("Metric not recognized. Possible choices are: CC, CCSM, MSD, MRSD, MI");
      }
    
    m_XExtractor = VectorImageToImageFilterType::New();
    m_XExtractor->SetInput(m_Registration->GetOutputDeformationField());
    m_XExtractor->SetIndex(0);
  
    m_YExtractor = VectorImageToImageFilterType::New();
    m_YExtractor->SetInput(m_Registration->GetOutputDeformationField());
    m_YExtractor->SetIndex(1);
  
    m_ImgList = ImageListType::New();
    m_ImgList->PushBack(m_XExtractor->GetOutput());
    m_ImgList->PushBack(m_YExtractor->GetOutput());
    
    // Invert correlation to get classical rendering
    if(metricId == "CC" || metricId == "CCSM")
      {
      m_AbsFilter = AbsFilterType::New();
      m_AbsFilter->SetInput(m_Registration->GetOutput());
      m_ImgList->PushBack(m_AbsFilter->GetOutput());
      }
    else
      {
      m_ImgList->PushBack(m_Registration->GetOutput());
      }
  
    m_Registration->UpdateOutputInformation();
  
    if(HasUserValue("vmlt") || HasUserValue("vmut"))
      {
      m_Threshold = BinaryThresholdImageFilterType::New();
  
      if(metricId == "CC" || metricId == "CCSM")
        {
        m_Threshold->SetInput(m_AbsFilter->GetOutput());
        }
      else
        {
        m_Threshold->SetInput(m_Registration->GetOutput());
        }
      
      otbAppLogINFO("A validity mask will be produced as the 4th band (valid pixels = 1.0, not valid = 0.0).");
      if(HasUserValue("vmlt"))
        {
        m_Threshold->SetLowerThreshold(GetParameterFloat("vmlt"));
        otbAppLogINFO("The lower threshold is set to "<<GetParameterFloat("vmlt"));
        }
      if(HasUserValue("vmut"))
        {
        m_Threshold->SetUpperThreshold(GetParameterFloat("vmut"));
        otbAppLogINFO("The upper threshold is set to "<<GetParameterFloat("vmut"));
        }
  
      m_Threshold->SetInsideValue(1.0);
      m_Threshold->SetOutsideValue(0.);
      m_ImgList->PushBack(m_Threshold->GetOutput());
      }
  
    m_Il2vi = IL2VIFilterType::New();
    m_Il2vi->SetInput(m_ImgList);
    
    AddProcess(m_Registration,"Fine Registration");
    
    SetParameterOutputImage<VectorImageType>("out", m_Il2vi->GetOutput());
    
    // If an image to warp has been given
    if (HasValue("w") && HasValue("wo"))
      {
      otbAppLogINFO("Doing warping : ON");
      
      m_outputReader = InternalReaderType::New();
      m_outputReader->SetFileName(GetParameterString("out"));
      
      m_ExtractROIFilter = ExtractROIFilterType::New();
      m_ExtractROIFilter->SetChannel(1);
      m_ExtractROIFilter->SetChannel(2);
      m_ExtractROIFilter->SetInput(m_outputReader->GetOutput());
  
      m_Cast = CastFilterType::New();
      m_Cast->SetInput(m_ExtractROIFilter->GetOutput());
      
      FloatVectorImageType::Pointer imageToWarp = GetParameterImage("w");
       
      m_Warp = WarpFilterType::New();
      m_Warp->SetDeformationField(m_Cast->GetOutput());
      m_Warp->SetInput(imageToWarp);
      m_Warp->SetOutputParametersFromImage(refImage);
      
      AddProcess(m_Warp,"Warp");
      
      SetParameterOutputImage("wo", m_Warp->GetOutput());
      
      }
    else
      {
      otbAppLogINFO("Doing warping : OFF");
      }
    
  }

  IntensityFilterType::Pointer m_IntensityRef;
  IntensityFilterType::Pointer m_IntensitySec;
  RegistrationFilterType::Pointer m_Registration;
  GaussianFilterType::Pointer m_RefGaussianFilter, m_SecGaussianFilter;
  itk::ImageToImageMetric<ScalarImageType, ScalarImageType>::Pointer m_MetricPtr;
  NCCType::Pointer m_NCCMetricPtr; // special case because of the SubtractMeanOn/Off method
  
  VectorImageToImageFilterType::Pointer m_XExtractor;
  VectorImageToImageFilterType::Pointer m_YExtractor;
  
  AbsFilterType::Pointer m_AbsFilter;
  ImageListType::Pointer m_ImgList;
  
  BinaryThresholdImageFilterType::Pointer m_Threshold;
  
  IL2VIFilterType::Pointer m_Il2vi;
  
  InternalReaderType::Pointer m_outputReader;
  ExtractROIFilterType::Pointer m_ExtractROIFilter;
  CastFilterType::Pointer m_Cast;
  WarpFilterType::Pointer m_Warp;
  
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::FineRegistration)
