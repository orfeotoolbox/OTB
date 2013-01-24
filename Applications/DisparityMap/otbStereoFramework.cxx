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

#include "otbStereorectificationDeformationFieldSource.h"
#include "otbStreamingWarpImageFilter.h"
#include "otbPixelWiseBlockMatchingImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "otbSubPixelDisparityImageFilter.h"
#include "otbDisparityMapMedianFilter.h"
#include "otbDisparityMapToDEMFilter.h"
#include "otbDEMToImageGenerator.h"

#include "otbVarianceImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkVectorCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbStreamingMinMaxImageFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbExtractROI.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbWrapperElevationParametersHandler.h"
#include "otbWrapperMapProjectionParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class StereoFramework : public Application
{
public:
  /** Standard class typedefs. */
  typedef StereoFramework                  Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(StereoFramework, otb::Application);
  
  /** Filters typedefs */
  typedef FloatImageType::PixelType           FloatPixelType;
  
  typedef otb::StereorectificationDeformationFieldSource
    <FloatImageType,FloatVectorImageType>     DeformationFieldSourceType;
  
  typedef itk::Vector<double,2>               DeformationType;
  typedef otb::Image<DeformationType>         DeformationFieldType;

  typedef itk::VectorCastImageFilter
    <FloatVectorImageType,
     DeformationFieldType>                    DeformationFieldCastFilterType;
  
  typedef otb::StreamingWarpImageFilter
    <FloatImageType,
     FloatImageType,
     DeformationFieldType>                    ResampleFilterType;

  typedef otb::BCOInterpolateImageFunction
    <FloatImageType>                          InterpolatorType;
  
  typedef otb::Functor::NCCBlockMatching
    <FloatImageType,
     FloatImageType>                          NCCBlockMatchingFunctorType;
  
  typedef otb::PixelWiseBlockMatchingImageFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatImageType,
     NCCBlockMatchingFunctorType>             BlockMatchingFilterType;

  typedef otb::BandMathImageFilter
    <FloatImageType>                          BandMathFilterType;

  typedef otb::SubPixelDisparityImageFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatImageType,
     NCCBlockMatchingFunctorType>             SubPixelFilterType;

  typedef otb::DisparityMapMedianFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType>                          MedianFilterType;

  typedef otb::DisparityMapToDEMFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     DeformationFieldType,
     FloatImageType>                          DisparityToElevationFilterType;
  
  typedef otb::DEMToImageGenerator
    <FloatImageType>                          DEMToImageGeneratorType;
  
  typedef otb::StreamingMinMaxImageFilter
    <FloatImageType>                          MinMaxFilterType;
  
  typedef otb::StreamingStatisticsImageFilter
    <FloatImageType>                          StatisticsFilterType;
   
  typedef otb::ExtractROI
    <FloatPixelType,FloatPixelType>           ExtractFilterType;
  
  typedef otb::ImageList<FloatImageType>      ImageListType;
  
  typedef otb::ImageListToVectorImageFilter
    <ImageListType,
     FloatVectorImageType>                    ImageListToVectorImageFilterType;
  
  typedef otb::ImageFileReader
    <FloatVectorImageType>                    ReaderType;
  
  typedef otb::ImageFileWriter
    <FloatVectorImageType>                    WriterType;
  
  typedef otb::VectorImageToImageListFilter
    <FloatVectorImageType,
     ImageListType>                           VectorImageToListFilterType;
  
private:
  
  StereoFramework()
    {
    m_LeftInterpolator = InterpolatorType::New();
    m_RightInterpolator = InterpolatorType::New();
    
    m_LeftResampleFilter = ResampleFilterType::New();
    m_RightResampleFilter = ResampleFilterType::New();
    
    m_LBandMathFilter = BandMathFilterType::New();
    m_RBandMathFilter = BandMathFilterType::New();
    
    m_BlockMatcher = BlockMatchingFilterType::New();
    m_SubPixFilter = SubPixelFilterType::New();
    
    m_HMedianFilter = MedianFilterType::New();
    
    m_ElevationFilter = DisparityToElevationFilterType::New();
    
    m_LeftMaskInterpolator = InterpolatorType::New();
    m_RightMaskInterpolator = InterpolatorType::New();
    
    m_LeftMaskResampleFilter = ResampleFilterType::New();
    m_RightMaskResampleFilter = ResampleFilterType::New();
    
    m_DispMaskFilter = BandMathFilterType::New();
    
    m_ReaderTmp = ReaderType::New();
    
    m_VectorToList = VectorImageToListFilterType::New();
    }
  
  void DoInit()
  {
    SetName("StereoFramework");
    SetDescription("Compute the ground elevation based on a stereo pair");

    SetDocName("Stereo Framework");
    SetDocLongDescription("Compute the ground elevation with a stereo block matching algorithm between a stereo pair in sensor geometry. The output is projected in WGS84");
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
    
    AddParameter(ParameterType_InputImage,"inleft","Left input image");
    SetParameterDescription("inleft", "Left sensor image of the stereo pair");
    
    AddParameter(ParameterType_InputImage,"inright","Right input image");
    SetParameterDescription("inright", "Right sensor image of the stereo pair");
    
    ElevationParametersHandler::AddElevationParameters(this, "elev");
    
    AddParameter(ParameterType_Float, "res","Output resolution");
    SetParameterDescription("res","Spatial sampling distance of the output elevation (in m)");
    
    AddParameter(ParameterType_OutputImage,"out","Output image");
    SetParameterDescription("out","Output elevation image");
    
    AddParameter(ParameterType_Float,"above","Maximum altitude offset");
    SetParameterDescription("above","Maximum altitude above the selected elevation source (in m)");
    MandatoryOff("above");
    SetDefaultParameterFloat("above",300.0);
    DisableParameter("above");
    
    AddParameter(ParameterType_Float,"below","Minimum altitude offset");
    SetParameterDescription("below","Minimum altitude below the selected elevation source (in m)");
    MandatoryOff("below");
    SetDefaultParameterFloat("below",-20.0);
    DisableParameter("below");
    
    AddParameter(ParameterType_Group,"mask","Masks");
    
    AddParameter(ParameterType_InputImage, "mask.left","Input left mask");
    SetParameterDescription("mask.left","Mask for left input image");
    MandatoryOff("mask.left");
    DisableParameter("mask.left");
    
    AddParameter(ParameterType_InputImage, "mask.right","Input right mask");
    SetParameterDescription("mask.right","Mask for right input image");
    MandatoryOff("mask.right");
    DisableParameter("mask.right");
    
    AddParameter(ParameterType_OutputFilename,"disp","Disparity map output");
    SetParameterDescription("disp","Image filename to store the disparity map (it can improve the processing time)");
    MandatoryOff("disp");
    DisableParameter("disp");
    
    AddRAMParameter();
    
    SetDocExampleParameterValue("inleft","sensor_stereo_left.tif");
    SetDocExampleParameterValue("inright","sensor_stereo_right.tif");
    SetDocExampleParameterValue("res","2.5");
    SetDocExampleParameterValue("out","dem.tif");
    
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do
  }
  
  void DoExecute()
  {
    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");
    
    // PIPELINE SETUP
    FloatImageType::Pointer inleft = this->GetParameterImage<FloatImageType>("inleft");
    FloatImageType::Pointer inright = this->GetParameterImage<FloatImageType>("inright");
    
    DeformationFieldSourceType::Pointer epipolarGridSource = DeformationFieldSourceType::New();
    epipolarGridSource->SetLeftImage(inleft);
    epipolarGridSource->SetRightImage(inright);
    epipolarGridSource->SetGridStep(16);
    epipolarGridSource->SetScale(1.0);
    epipolarGridSource->SetUseDEM(true);
    
    epipolarGridSource->UpdateOutputInformation();
    // check that deformation grids fit in 1/4 of available RAM
    double ram = 0.25 * static_cast<double>(this->GetParameterInt("ram"));
    FloatVectorImageType::SizeType grid_size = epipolarGridSource->GetLeftDeformationFieldOutput()->GetLargestPossibleRegion().GetSize();
    double storage_size = static_cast<double>(grid_size[0])*static_cast<double>(grid_size[1])*4.0*8.0/1000000.0;
    if (ram < storage_size)
    {
      double newStep = vcl_ceil(vcl_sqrt(storage_size*16.0*16.0/ram));
      epipolarGridSource->SetGridStep(newStep);
      otbAppLogINFO(<<"Change grid step to "<<newStep);
    }
    
    AddProcess(epipolarGridSource, "Compute epipolar grids...");
    epipolarGridSource->Update();
    
    FloatImageType::SpacingType epiSpacing;
    epiSpacing[0] = 0.5 * (vcl_abs(inleft->GetSpacing()[0])+vcl_abs(inleft->GetSpacing()[1]));
    epiSpacing[1] = 0.5 * (vcl_abs(inleft->GetSpacing()[0])+vcl_abs(inleft->GetSpacing()[1]));

    FloatImageType::SizeType epiSize;
    epiSize = epipolarGridSource->GetRectifiedImageSize();

    FloatImageType::PointType epiOrigin;
    epiOrigin[0] = 0.0;
    epiOrigin[1] = 0.0;

    FloatImageType::PixelType defaultValue = 0;
    
    double meanBaseline = epipolarGridSource->GetMeanBaselineRatio();
    
    // Compute rectification grids.
    DeformationFieldCastFilterType::Pointer leftGridCaster = DeformationFieldCastFilterType::New();
    leftGridCaster->SetInput(epipolarGridSource->GetLeftDeformationFieldOutput());
    leftGridCaster->Update();
    
    m_LeftDeformation = leftGridCaster->GetOutput();
    m_LeftDeformation->DisconnectPipeline();
    
    m_LeftInterpolator->SetRadius(2);
    
    m_LeftResampleFilter->SetInput(inleft);
    m_LeftResampleFilter->SetDeformationField(m_LeftDeformation);
    m_LeftResampleFilter->SetInterpolator(m_LeftInterpolator);
    m_LeftResampleFilter->SetOutputSize(epiSize);
    m_LeftResampleFilter->SetOutputSpacing(epiSpacing);
    m_LeftResampleFilter->SetOutputOrigin(epiOrigin);
    m_LeftResampleFilter->SetEdgePaddingValue(defaultValue);

    DeformationFieldCastFilterType::Pointer rightGridCaster = DeformationFieldCastFilterType::New();
    rightGridCaster->SetInput(epipolarGridSource->GetRightDeformationFieldOutput());
    rightGridCaster->Update();
    
    m_RightDeformation = rightGridCaster->GetOutput();
    m_RightDeformation->DisconnectPipeline();
    
    m_RightInterpolator->SetRadius(2);
    
    m_RightResampleFilter->SetInput(inright);
    m_RightResampleFilter->SetDeformationField(m_RightDeformation);
    m_RightResampleFilter->SetInterpolator(m_RightInterpolator);
    m_RightResampleFilter->SetOutputSize(epiSize);
    m_RightResampleFilter->SetOutputSpacing(epiSpacing);
    m_RightResampleFilter->SetOutputOrigin(epiOrigin);
    m_RightResampleFilter->SetEdgePaddingValue(defaultValue);

    // Compute masks
    FloatImageType::Pointer leftmask;
    FloatImageType::Pointer rightmask;
    
    m_LBandMathFilter->SetNthInput(0,m_LeftResampleFilter->GetOutput(),"inleft");
    std::ostringstream leftFormula;
    leftFormula<<"if((inleft > 0)";
    if (IsParameterEnabled("mask.left") && HasValue("mask.left"))
      {
      leftmask = this->GetParameterFloatImage("mask.left");
      m_LeftMaskInterpolator->SetRadius(2);
      
      m_LeftMaskResampleFilter->SetInput(leftmask);
      m_LeftMaskResampleFilter->SetDeformationField(m_LeftDeformation);
      m_LeftMaskResampleFilter->SetInterpolator(m_LeftMaskInterpolator);
      m_LeftMaskResampleFilter->SetOutputSize(epiSize);
      m_LeftMaskResampleFilter->SetOutputSpacing(epiSpacing);
      m_LeftMaskResampleFilter->SetOutputOrigin(epiOrigin);
      m_LeftMaskResampleFilter->SetEdgePaddingValue(defaultValue);
      
      m_LBandMathFilter->SetNthInput(1,m_LeftMaskResampleFilter->GetOutput(),"maskleft");
      leftFormula << " and (maskleft > 0)";
      }
    leftFormula << ",255,0)";
    m_LBandMathFilter->SetExpression(leftFormula.str());
    
    m_RBandMathFilter->SetNthInput(0,m_RightResampleFilter->GetOutput(),"inright");
    std::ostringstream rightFormula;
    rightFormula<<"if((inright > 0)";
    if (IsParameterEnabled("mask.right") && HasValue("mask.right"))
    {
      rightmask = this->GetParameterFloatImage("mask.right");
      m_RightMaskInterpolator->SetRadius(2);
      
      m_RightMaskResampleFilter->SetInput(rightmask);
      m_RightMaskResampleFilter->SetDeformationField(m_RightDeformation);
      m_RightMaskResampleFilter->SetInterpolator(m_RightMaskInterpolator);
      m_RightMaskResampleFilter->SetOutputSize(epiSize);
      m_RightMaskResampleFilter->SetOutputSpacing(epiSpacing);
      m_RightMaskResampleFilter->SetOutputOrigin(epiOrigin);
      m_RightMaskResampleFilter->SetEdgePaddingValue(defaultValue);
      
      m_RBandMathFilter->SetNthInput(1,m_RightMaskResampleFilter->GetOutput(),"maskright");
      rightFormula << " and (maskright > 0)";
    }
    rightFormula << ",255,0)";
    m_RBandMathFilter->SetExpression(rightFormula.str());

    // Compute disparities
    m_BlockMatcher->SetLeftInput(m_LeftResampleFilter->GetOutput());
    m_BlockMatcher->SetRightInput(m_RightResampleFilter->GetOutput());
    m_BlockMatcher->SetLeftMaskInput(m_LBandMathFilter->GetOutput());
    m_BlockMatcher->SetRightMaskInput(m_RBandMathFilter->GetOutput());
    m_BlockMatcher->SetRadius(4);
    m_BlockMatcher->MinimizeOff();
    
    m_SubPixFilter->SetInputsFromBlockMatchingFilter(m_BlockMatcher);
    m_SubPixFilter->SetRefineMethod(SubPixelFilterType::DICHOTOMY);
    
    
    m_HMedianFilter->SetInput(m_SubPixFilter->GetHorizontalDisparityOutput());
    m_HMedianFilter->SetRadius(2);
    m_HMedianFilter->SetIncoherenceThreshold(2.0);
    m_HMedianFilter->SetMaskInput(m_LBandMathFilter->GetOutput());
    
    // Compute elevation
    m_ElevationFilter->SetHorizontalDisparityMapInput(m_HMedianFilter->GetOutput());
    m_ElevationFilter->SetVerticalDisparityMapInput(m_SubPixFilter->GetVerticalDisparityOutput());
    m_ElevationFilter->SetLeftInput(inleft);
    m_ElevationFilter->SetRightInput(inright);
    m_ElevationFilter->SetLeftEpipolarGridInput(m_LeftDeformation);
    m_ElevationFilter->SetRightEpipolarGridInput(m_RightDeformation);
    m_ElevationFilter->SetDEMGridStep(this->GetParameterFloat("res"));
    m_ElevationFilter->UpdateOutputInformation();
    
    // set the EPSG:4326
    itk::MetaDataDictionary & dict = m_ElevationFilter->GetOutput()->GetMetaDataDictionary();
    itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                           static_cast<std::string>(otb::GeoInformationConversion::ToWKT(4326)));
    
    SetParameterOutputImage("out",m_ElevationFilter->GetOutput());
    
    // PARAMETER ESTIMATION
    double underElev = this->GetParameterFloat("below");
    double overElev = this->GetParameterFloat("above");
    
    // Compute min/max elevation on DEM
    FloatImageType::Pointer elevationOutput = m_ElevationFilter->GetOutput();
    otbAppLogINFO(<<"Output origin : "<<elevationOutput->GetOrigin());
    otbAppLogINFO(<<"Output spacing : "<<elevationOutput->GetSpacing());
    otbAppLogINFO(<<"Output size : "<<elevationOutput->GetLargestPossibleRegion().GetSize());
    
    DEMToImageGeneratorType::Pointer demToImageFilter = DEMToImageGeneratorType::New();
    demToImageFilter->SetOutputOrigin(elevationOutput->GetOrigin());
    demToImageFilter->SetOutputSpacing(elevationOutput->GetSpacing());
    demToImageFilter->SetOutputSize(elevationOutput->GetLargestPossibleRegion().GetSize());
    demToImageFilter->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(4326));
    
    MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();
    minMaxFilter->SetInput(demToImageFilter->GetOutput());
    minMaxFilter->GetStreamer()->SetAutomaticTiledStreaming(this->GetParameterInt("ram"));
    
    AddProcess(minMaxFilter->GetStreamer(), "Estimating min/max elevation...");
    minMaxFilter->Update();
    
    double minElev = minMaxFilter->GetMinimum();
    double maxElev = minMaxFilter->GetMaximum();
    otbAppLogINFO(<<"Minimum elevation found : "<<minElev);
    otbAppLogINFO(<<"Maximum elevation found : "<<maxElev);
    
    //fill min/max elevations
    m_ElevationFilter->SetElevationMin(minElev+underElev);
    m_ElevationFilter->SetElevationMax(maxElev+overElev);
    
    // fill min/max disparities
    double minDisp = vcl_floor((-1.0)*overElev*meanBaseline/epiSpacing[0]);
    double maxDisp = vcl_ceil((-1.0)*underElev*meanBaseline/epiSpacing[0]);
    otbAppLogINFO(<<"Minimum disparity : "<<minDisp);
    otbAppLogINFO(<<"Maximum disparity : "<<maxDisp);
    m_BlockMatcher->SetMinimumHorizontalDisparity(minDisp);
    m_BlockMatcher->SetMaximumHorizontalDisparity(maxDisp);
    
    m_BlockMatcher->SetMinimumVerticalDisparity(0);
    m_BlockMatcher->SetMaximumVerticalDisparity(0);
    
    // Compute disparity mask
    m_DispMaskFilter->SetNthInput(0,m_HMedianFilter->GetOutput(),"hdisp");
    m_DispMaskFilter->SetNthInput(1,m_SubPixFilter->GetMetricOutput(),"metric");
    std::ostringstream maskFormula;
    maskFormula << "if((hdisp > "<<minDisp<<") and (hdisp < "<<maxDisp<<") and (metric > 0.6),255,0)";
    m_DispMaskFilter->SetExpression(maskFormula.str());
    m_ElevationFilter->SetDisparityMaskInput(m_DispMaskFilter->GetOutput());
    
    // If given, store the disparity map to a file
    if (IsParameterEnabled("disp") && HasValue("disp"))
      {
      ImageListType::Pointer imageList = ImageListType::New();
      imageList->Clear();
      imageList->PushBack(m_HMedianFilter->GetOutput());
      imageList->PushBack(m_SubPixFilter->GetVerticalDisparityOutput());
      imageList->PushBack(m_SubPixFilter->GetMetricOutput());
      
      ImageListToVectorImageFilterType::Pointer listToVector = ImageListToVectorImageFilterType::New();
      listToVector->SetInput(imageList);
      
      WriterType::Pointer writerTmp = WriterType::New();
      writerTmp->SetInput(listToVector->GetOutput());
      writerTmp->SetFileName(this->GetParameterString("disp"));
      writerTmp->SetAutomaticTiledStreaming(this->GetParameterInt("ram"));
      AddProcess(writerTmp, "Compute disparity map...");
      writerTmp->Update();
      
      m_ReaderTmp->SetFileName(this->GetParameterString("disp"));
      
      m_VectorToList->SetInput(m_ReaderTmp->GetOutput());
      m_VectorToList->UpdateOutputInformation();
      
      m_ElevationFilter->SetHorizontalDisparityMapInput(m_VectorToList->GetOutput()->GetNthElement(0));
      m_ElevationFilter->SetVerticalDisparityMapInput(m_VectorToList->GetOutput()->GetNthElement(1));
      
      m_DispMaskFilter->SetNthInput(0,m_VectorToList->GetOutput()->GetNthElement(0),"hdisp");
      m_DispMaskFilter->SetNthInput(1,m_VectorToList->GetOutput()->GetNthElement(2),"metric");
      }
    
  }
  
  // private filters
  InterpolatorType::Pointer m_LeftInterpolator;
  InterpolatorType::Pointer m_RightInterpolator;
  
  ResampleFilterType::Pointer m_LeftResampleFilter;
  ResampleFilterType::Pointer m_RightResampleFilter;
  
  BandMathFilterType::Pointer m_LBandMathFilter;
  BandMathFilterType::Pointer m_RBandMathFilter;
  
  BlockMatchingFilterType::Pointer m_BlockMatcher;
  SubPixelFilterType::Pointer m_SubPixFilter;
  
  MedianFilterType::Pointer m_HMedianFilter;
  
  DisparityToElevationFilterType::Pointer m_ElevationFilter;
  
  InterpolatorType::Pointer m_LeftMaskInterpolator;
  InterpolatorType::Pointer m_RightMaskInterpolator;
  
  ResampleFilterType::Pointer m_LeftMaskResampleFilter;
  ResampleFilterType::Pointer m_RightMaskResampleFilter;
  
  BandMathFilterType::Pointer m_DispMaskFilter;
  
  ReaderType::Pointer m_ReaderTmp;
  
  VectorImageToListFilterType::Pointer m_VectorToList;
  // private data
  DeformationFieldType::Pointer m_LeftDeformation;
  DeformationFieldType::Pointer m_RightDeformation;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::StereoFramework)
