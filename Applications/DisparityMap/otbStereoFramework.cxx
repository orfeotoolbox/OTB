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
#include "otbInverseDeformationFieldImageFilter.h"

#include "itkRescaleIntensityImageFilter.h"
#include "otbStreamingMinMaxImageFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbExtractROI.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbMultiToMonoChannelExtractROI.h"

#include "otbMultiDisparityMapTo3DFilter.h"
#include "otbMulti3DMapToDEMFilter.h"
#include "otbDisparityTranslateFilter.h"


#include "otbWrapperElevationParametersHandler.h"
#include "otbWrapperMapProjectionParametersHandler.h"


#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"



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

  typedef otb::InverseDeformationFieldImageFilter
   <DeformationFieldType,DeformationFieldType> InverseDeformationFieldFilterType;

  

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
  
  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                         FloatImageType::PixelType>               ExtractROIFilterType;

  typedef otb::MultiDisparityMapTo3DFilter
    <FloatImageType,
      FloatVectorImageType,
      FloatImageType>                              MultiDisparityTo3DFilterType;
 typedef MultiDisparityTo3DFilterType::Pointer     MultiDisparityTo3DFilterPointerType;
     typedef otb::Multi3DMapToDEMFilter<FloatVectorImageType,FloatImageType,FloatImageType>                              Multi3DFilterType;

  typedef otb::DisparityTranslateFilter
    <FloatImageType,
      DeformationFieldType,
      FloatImageType,
      FloatImageType>                              DisparityTranslateFilter;

  typedef itk::VectorIndexSelectionCastImageFilter<DeformationFieldType,
                                                     FloatImageType> IndexSelectionCastFilterType;

    typedef otb::ImageListToVectorImageFilter<ImageListType,FloatVectorImageType> ImageListFilterType;

    typedef ExtractROIFilterType::Pointer               ExtractROIFilterPointerType;
    typedef std::vector<ExtractROIFilterPointerType>            ExtractorListType;
    typedef std::vector<MultiDisparityTo3DFilterPointerType>           MultiDisparityTo3DFilterListType;

private:
  
  StereoFramework()
    {

     m_Interpolator = InterpolatorType::New();
     m_Multi3DMapToDEMFilter =  Multi3DFilterType::New();
    }
  
  void DoInit()
  {
    SetName("StereoFramework");
    SetDescription("Compute the ground elevation based on a stereo pair");

    SetDocName("Stereo Framework");
    SetDocLongDescription("Compute the ground elevation with a stereo block matching algorithm "
                          "between a stereo pair in sensor geometry. The output is projected in "
                          "WGS84. The pipeline is made of the following steps:\n"
                          "\t- compute the epipolar deformation grids from the stereo pair\n"
                          "\t- resample the stereo pair into epipolar images using BCO interpolation\n"
                          "\t- create masks for each epipolar image : remove black borders and resample"
                          " input masks\n"
                          "\t- compute horizontal disparities with a NCC block matching algorithm\n"
                          "\t- refine disparities to sub-pixel precision with a dichotomy algorithm\n"
                          "\t- apply a median filter\n"
                          "\t- filter disparites based on the correlation score (must be greater than "
                          "0.6) and exploration bounds\n"
                          "\t- project disparities on a regular grid in WGS84, for each cell the "
                          "maximum elevation is kept\n");
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Stereo);

    AddParameter(ParameterType_InputImageList,  "il",   "Input images list");
    SetParameterDescription("il", "The list of images. First image is used as left sensor image. Other images are used to complete stereo couple.");

    ElevationParametersHandler::AddElevationParameters(this, "elev");
    
    AddParameter(ParameterType_Float, "res","Output resolution");
    SetParameterDescription("res","Spatial sampling distance of the output elevation (in m)");
    
    AddParameter(ParameterType_OutputImage,"out","Output image");
    SetParameterDescription("out","Output elevation image");
    
    /*AddParameter(ParameterType_OutputImage,"out2","Output image");
        SetParameterDescription("out2","Output elevation image"); */

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
    
    //TODO JGT new framework disparity map handling
    // is it useful to store disp in epipolar geometry ?
   /* AddParameter(ParameterType_OutputFilename,"disp","Disparity map output");
    SetParameterDescription("disp","Image filename to store the disparity map (it can improve the processing time)");
    MandatoryOff("disp");
    DisableParameter("disp"); */
    
    AddRAMParameter();
    

    SetDocExampleParameterValue("il","sensor_stereo_left.tif sensor_stereo_right.tif");
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
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");

    m_Filters.clear();

    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    if (inList->Size() < 2)
      {
      itkExceptionMacro("at least two input images must be set...");

      }

    unsigned int stereoCouples = inList->Size() - 1;
    otbAppLogINFO(<<stereoCouples<<" stereo Couples will be computed");
    m_ExtractorList.resize(inList->Size());
    //TODO JGT check the mutli stereo fusion step
    //     N MultiDisp which gives N 3D Map Then fuse N 3D Map to DEM
    // or
    //     1 MultiDisp which fuse N Disp Couples to 1 3D Map and then convert one 3D Map into DEM
    m_MultiDisparityTo3DFilterList.resize(stereoCouples);

    for (unsigned int i = 0; i < inList->Size(); i++)
      {
      m_ExtractorList[i] = ExtractROIFilterType::New();
      m_ExtractorList[i]->SetInput(inList->GetNthElement(i));
      m_ExtractorList[i]->SetChannel(1);
      m_ExtractorList[i]->UpdateOutputInformation();

      }

    //create BCO interpolator with radius 2
    // used by Left and Right Resampler and Left and Right Mask REsampler
    m_Interpolator->SetRadius(2);

    m_Multi3DMapToDEMFilter->SetNumberOf3DMaps(stereoCouples);

    //create pipeline for each stereo couple
    for (unsigned int i = 0; i < stereoCouples; i++)
      {

      FloatImageType::Pointer inleft = m_ExtractorList[0]->GetOutput();
      FloatImageType::Pointer inright = m_ExtractorList[i + 1]->GetOutput();

      m_MultiDisparityTo3DFilterList[i] = MultiDisparityTo3DFilterType::New();
      DeformationFieldSourceType::Pointer epipolarGridSource = DeformationFieldSourceType::New();
      epipolarGridSource->SetLeftImage(inleft);
      epipolarGridSource->SetRightImage(inright);
      epipolarGridSource->SetGridStep(16);
      epipolarGridSource->SetScale(1.0);

      if (otb::Wrapper::ElevationParametersHandler::IsDEMUsed(this, "elev")
          && otb::Wrapper::ElevationParametersHandler::IsGeoidUsed(this, "elev"))
        {
        epipolarGridSource->SetUseDEM(true);
        }

      epipolarGridSource->UpdateOutputInformation();
      // check that deformation grids fit in 1/4 of available RAM
      double ram = 0.25 * static_cast<double> (this->GetParameterInt("ram"));
      FloatVectorImageType::SizeType
          grid_size = epipolarGridSource->GetLeftDeformationFieldOutput()->GetLargestPossibleRegion().GetSize();
      double storage_size = static_cast<double> (grid_size[0]) * static_cast<double> (grid_size[1]) * 4.0 * 8.0
          / 1000000.0;
      if (ram < storage_size)
        {
        double newStep = vcl_ceil(vcl_sqrt(storage_size * 16.0 * 16.0 / ram));
        epipolarGridSource->SetGridStep(newStep);
        otbAppLogINFO(<<"Change grid step to "<<newStep);
        }

      AddProcess(epipolarGridSource, "Compute epipolar grids...");
      epipolarGridSource->Update();

      FloatImageType::SpacingType epiSpacing;
      epiSpacing[0] = 0.5 * (vcl_abs(inleft->GetSpacing()[0]) + vcl_abs(inleft->GetSpacing()[1]));
      epiSpacing[1] = 0.5 * (vcl_abs(inleft->GetSpacing()[0]) + vcl_abs(inleft->GetSpacing()[1]));

      FloatImageType::SizeType epiSize;
      epiSize = epipolarGridSource->GetRectifiedImageSize();

      FloatImageType::PointType epiOrigin;
      epiOrigin[0] = 0.0;
      epiOrigin[1] = 0.0;

      FloatImageType::PixelType defaultValue = 0;

      double meanBaseline = epipolarGridSource->GetMeanBaselineRatio();

      // Compute rectification grids (lef/right and left inverse (for disparity translate filter)).
      DeformationFieldCastFilterType::Pointer leftGridCaster = DeformationFieldCastFilterType::New();
      leftGridCaster->SetInput(epipolarGridSource->GetLeftDeformationFieldOutput());
      leftGridCaster->Update();

      DeformationFieldType::Pointer leftDeformation;
      leftDeformation = leftGridCaster->GetOutput();
      leftDeformation->DisconnectPipeline();
      m_Filters.push_back(leftDeformation.GetPointer());

      InverseDeformationFieldFilterType::Pointer
          leftInverseDeformationFieldFilter = InverseDeformationFieldFilterType::New();
      leftInverseDeformationFieldFilter->SetInput(leftDeformation);

      FloatVectorImageType::PointType lorigin = inleft->GetOrigin();
      FloatVectorImageType::SpacingType lspacing = inleft->GetSpacing();
      FloatVectorImageType::SizeType lsize = inleft->GetLargestPossibleRegion().GetSize();
      double gridStep = epipolarGridSource->GetGridStep();
      lspacing[0] *= gridStep;
      lspacing[1] *= gridStep;

      lsize[0] /= gridStep;
      lsize[1] /= gridStep;

      lsize[0] += 1;
      lsize[1] += 1;

      leftInverseDeformationFieldFilter->SetOutputOrigin(lorigin);
      leftInverseDeformationFieldFilter->SetOutputSpacing(lspacing);
      leftInverseDeformationFieldFilter->SetSize(lsize);
      // change value
      leftInverseDeformationFieldFilter->SetSubsamplingFactor(1);
      AddProcess(leftInverseDeformationFieldFilter, "Inverting left deformation field ...");
      leftInverseDeformationFieldFilter->Update();
      DeformationFieldType::Pointer leftInverseDeformation;
      leftInverseDeformation = leftInverseDeformationFieldFilter->GetOutput();
      leftInverseDeformation->DisconnectPipeline();
      m_Filters.push_back(leftInverseDeformation.GetPointer());

      ResampleFilterType::Pointer leftResampleFilter = ResampleFilterType::New();
      leftResampleFilter->SetInput(inleft);
      leftResampleFilter->SetDeformationField(leftDeformation);
      leftResampleFilter->SetInterpolator(m_Interpolator);
      leftResampleFilter->SetOutputSize(epiSize);
      leftResampleFilter->SetOutputSpacing(epiSpacing);
      leftResampleFilter->SetOutputOrigin(epiOrigin);
      leftResampleFilter->SetEdgePaddingValue(defaultValue);
      m_Filters.push_back(leftResampleFilter.GetPointer());

      DeformationFieldCastFilterType::Pointer rightGridCaster = DeformationFieldCastFilterType::New();
      rightGridCaster->SetInput(epipolarGridSource->GetRightDeformationFieldOutput());
      rightGridCaster->Update();

      DeformationFieldType::Pointer rightDeformation;
      rightDeformation = rightGridCaster->GetOutput();
      rightDeformation->DisconnectPipeline();
      m_Filters.push_back(rightDeformation.GetPointer());

      ResampleFilterType::Pointer rightResampleFilter = ResampleFilterType::New();
      rightResampleFilter->SetInput(inright);
      rightResampleFilter->SetDeformationField(rightDeformation);
      rightResampleFilter->SetInterpolator(m_Interpolator);
      rightResampleFilter->SetOutputSize(epiSize);
      rightResampleFilter->SetOutputSpacing(epiSpacing);
      rightResampleFilter->SetOutputOrigin(epiOrigin);
      rightResampleFilter->SetEdgePaddingValue(defaultValue);
      m_Filters.push_back(rightResampleFilter.GetPointer());

      // Compute masks
      FloatImageType::Pointer leftmask;
      FloatImageType::Pointer rightmask;
      BandMathFilterType::Pointer lBandMathFilter = BandMathFilterType::New();
      BandMathFilterType::Pointer rBandMathFilter = BandMathFilterType::New();

      lBandMathFilter->SetNthInput(0, leftResampleFilter->GetOutput(), "inleft");
      std::ostringstream leftFormula;
      leftFormula << "if((inleft > 0)";

      ResampleFilterType::Pointer leftMaskResampleFilter = ResampleFilterType::New();

      if (IsParameterEnabled("mask.left") && HasValue("mask.left"))
        {
        leftmask = this->GetParameterFloatImage("mask.left");

        leftMaskResampleFilter->SetInput(leftmask);
        leftMaskResampleFilter->SetDeformationField(leftDeformation);
        leftMaskResampleFilter->SetInterpolator(m_Interpolator);
        leftMaskResampleFilter->SetOutputSize(epiSize);
        leftMaskResampleFilter->SetOutputSpacing(epiSpacing);
        leftMaskResampleFilter->SetOutputOrigin(epiOrigin);
        leftMaskResampleFilter->SetEdgePaddingValue(defaultValue);

        lBandMathFilter->SetNthInput(1, leftMaskResampleFilter->GetOutput(), "maskleft");
        leftFormula << " and (maskleft > 0)";
        }
      leftFormula << ",255,0)";
      lBandMathFilter->SetExpression(leftFormula.str());

      m_Filters.push_back(leftMaskResampleFilter.GetPointer());

      rBandMathFilter->SetNthInput(0, rightResampleFilter->GetOutput(), "inright");

      std::ostringstream rightFormula;
      rightFormula << "if((inright > 0)";
      ResampleFilterType::Pointer rightMaskResampleFilter = ResampleFilterType::New();

      if (IsParameterEnabled("mask.right") && HasValue("mask.right"))
        {
        rightmask = this->GetParameterFloatImage("mask.right");

        rightMaskResampleFilter->SetInput(rightmask);
        rightMaskResampleFilter->SetDeformationField(rightDeformation);
        rightMaskResampleFilter->SetInterpolator(m_Interpolator);
        rightMaskResampleFilter->SetOutputSize(epiSize);
        rightMaskResampleFilter->SetOutputSpacing(epiSpacing);
        rightMaskResampleFilter->SetOutputOrigin(epiOrigin);
        rightMaskResampleFilter->SetEdgePaddingValue(defaultValue);

        rBandMathFilter->SetNthInput(1, rightMaskResampleFilter->GetOutput(), "maskright");
        rightFormula << " and (maskright > 0)";
        }
      rightFormula << ",255,0)";

      rBandMathFilter->SetExpression(rightFormula.str());

      m_Filters.push_back(rightMaskResampleFilter.GetPointer());
      m_Filters.push_back(rBandMathFilter.GetPointer());
      m_Filters.push_back(lBandMathFilter.GetPointer());
      // Compute disparities
      BlockMatchingFilterType::Pointer blockMatcherFilter = BlockMatchingFilterType::New();
      blockMatcherFilter->SetLeftInput(leftResampleFilter->GetOutput());
      blockMatcherFilter->SetRightInput(rightResampleFilter->GetOutput());
      blockMatcherFilter->SetLeftMaskInput(lBandMathFilter->GetOutput());
      blockMatcherFilter->SetRightMaskInput(rBandMathFilter->GetOutput());
      blockMatcherFilter->SetRadius(4);
      blockMatcherFilter->MinimizeOff();
      m_Filters.push_back(blockMatcherFilter.GetPointer());

      SubPixelFilterType::Pointer subPixelFilter = SubPixelFilterType::New();
      subPixelFilter = SubPixelFilterType::New();
      subPixelFilter->SetInputsFromBlockMatchingFilter(blockMatcherFilter);
      subPixelFilter->SetRefineMethod(SubPixelFilterType::DICHOTOMY);
      subPixelFilter->UpdateOutputInformation();
      m_Filters.push_back(subPixelFilter.GetPointer());

      MedianFilterType::Pointer hMedianFilter = MedianFilterType::New();
      hMedianFilter->SetInput(subPixelFilter->GetHorizontalDisparityOutput());
      hMedianFilter->SetRadius(2);
      hMedianFilter->SetIncoherenceThreshold(2.0);
      hMedianFilter->SetMaskInput(lBandMathFilter->GetOutput());
      hMedianFilter->UpdateOutputInformation();
      m_Filters.push_back(hMedianFilter.GetPointer());

      DisparityTranslateFilter::Pointer disparityTranslateFilter = DisparityTranslateFilter::New();
      disparityTranslateFilter->SetHorizontalDisparityMapInput(hMedianFilter->GetOutput());
      disparityTranslateFilter->SetVerticalDisparityMapInput(subPixelFilter->GetVerticalDisparityOutput());
      disparityTranslateFilter->SetInverseEpipolarLeftGrid(leftInverseDeformation);
      disparityTranslateFilter->SetDirectEpipolarRightGrid(rightDeformation);
      // disparityTranslateFilter->SetDisparityMaskInput()
      disparityTranslateFilter->SetLeftSensorImageInput(inleft);
      disparityTranslateFilter->UpdateOutputInformation();
      m_Filters.push_back(disparityTranslateFilter.GetPointer());

      MedianFilterType::Pointer hMedianFilter2 = MedianFilterType::New();
      MedianFilterType::Pointer vMedianFilter2 = MedianFilterType::New();

      //TODO JGT Check if medianfiltering is necessary after disparitytranslate
      hMedianFilter2->SetInput(disparityTranslateFilter->GetHorizontalDisparityMapOutput());
      hMedianFilter2->SetRadius(2);
      hMedianFilter2->SetIncoherenceThreshold(2.0);
      //hMedianFilter2->SetMaskInput(lBandMathFilter->GetOutput());
      hMedianFilter2->UpdateOutputInformation();
      m_Filters.push_back(hMedianFilter2.GetPointer());

      vMedianFilter2->SetInput(disparityTranslateFilter->GetVerticalDisparityMapOutput());
      vMedianFilter2->SetRadius(2);
      vMedianFilter2->SetIncoherenceThreshold(2.0);
      //vMedianFilter2->SetMaskInput(lBandMathFilter->GetOutput());
      vMedianFilter2->UpdateOutputInformation();
      m_Filters.push_back(vMedianFilter2.GetPointer());

      // transform disparity into 3D map
      m_MultiDisparityTo3DFilterList[i]->SetReferenceKeywordList(inleft->GetImageKeywordlist());
      m_MultiDisparityTo3DFilterList[i]->SetNumberOfMovingImages(1);
      m_MultiDisparityTo3DFilterList[i]->SetHorizontalDisparityMapInput(0, hMedianFilter2->GetOutput());
      m_MultiDisparityTo3DFilterList[i]->SetVerticalDisparityMapInput(0, vMedianFilter2->GetOutput());
      m_MultiDisparityTo3DFilterList[i]->SetMovingKeywordList(0, inright->GetImageKeywordlist());
      m_MultiDisparityTo3DFilterList[i]->UpdateOutputInformation();

      // PARAMETER ESTIMATION

      double underElev = this->GetParameterFloat("below");
      double overElev = this->GetParameterFloat("above");

      double minElev = 0.0;
      double maxElev = 0.0;

      // Compute min/max elevation on DEM
      if (otb::Wrapper::ElevationParametersHandler::IsDEMUsed(this, "elev"))
        {
      DEMToImageGeneratorType::Pointer demToImageFilter = DEMToImageGeneratorType::New();
      demToImageFilter->SetOutputParametersFromImage((m_MultiDisparityTo3DFilterList[i]->GetOutput()));
      MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();
      minMaxFilter->SetInput(demToImageFilter->GetOutput());
      minMaxFilter->GetStreamer()->SetAutomaticTiledStreaming(this->GetParameterInt("ram"));

      AddProcess(minMaxFilter->GetStreamer(), "Estimating min/max elevation...");
      minMaxFilter->Update();

      minElev = minMaxFilter->GetMinimum();
      maxElev = minMaxFilter->GetMaximum();
      }
      else
      {
        minElev =otb::Wrapper::ElevationParametersHandler::GetDefaultElevation(this, "elev");
        maxElev =otb::Wrapper::ElevationParametersHandler::GetDefaultElevation(this, "elev");
        otbAppLogINFO(<<"Default elevation set for Min/Max elevation : "<<minElev);
      }

      otbAppLogINFO(<<"Minimum elevation found : "<<minElev);
      otbAppLogINFO(<<"Maximum elevation found : "<<maxElev);

      //check under and over for each couple
      if(i==0)
        {
        m_Multi3DMapToDEMFilter->SetElevationMin(minElev + underElev);
        m_Multi3DMapToDEMFilter->SetNoDataValue(minElev);
        m_Multi3DMapToDEMFilter->SetElevationMax(maxElev + overElev);
        }
      else
        {
         if(minElev<(m_Multi3DMapToDEMFilter->GetElevationMin()-underElev))
           {
           m_Multi3DMapToDEMFilter->SetElevationMin(minElev + underElev);
           m_Multi3DMapToDEMFilter->SetNoDataValue(minElev);
           }
         if(maxElev<(m_Multi3DMapToDEMFilter->GetElevationMax()-overElev))
          {
           m_Multi3DMapToDEMFilter->SetElevationMax(maxElev + overElev);
          }
        }

      double minDisp = vcl_floor((-1.0) * overElev * meanBaseline / epiSpacing[0]);
      double maxDisp = vcl_ceil((-1.0) * underElev * meanBaseline / epiSpacing[0]);
      otbAppLogINFO(<<"Minimum disparity : "<<minDisp);
      otbAppLogINFO(<<"Maximum disparity : "<<maxDisp);
      blockMatcherFilter->SetMinimumHorizontalDisparity(minDisp);
      blockMatcherFilter->SetMaximumHorizontalDisparity(maxDisp);

      blockMatcherFilter->SetMinimumVerticalDisparity(0);
      blockMatcherFilter->SetMaximumVerticalDisparity(0);

      // Compute disparity mask
      BandMathFilterType::Pointer dispMaskFilter = BandMathFilterType::New();
      dispMaskFilter->SetNthInput(0, hMedianFilter->GetOutput(), "hdisp");
      dispMaskFilter->SetNthInput(1, subPixelFilter->GetMetricOutput(), "metric");
      std::ostringstream maskFormula;
      maskFormula << "if((hdisp > " << minDisp << ") and (hdisp < " << maxDisp << ") and (metric > 0.6),255,0)";
      dispMaskFilter->SetExpression(maskFormula.str());
      m_Filters.push_back(dispMaskFilter.GetPointer());

      //TODO to check
      disparityTranslateFilter->SetDisparityMaskInput(dispMaskFilter->GetOutput());

      m_Multi3DMapToDEMFilter->Set3DMapInput(i, m_MultiDisparityTo3DFilterList[i]->GetOutput());
      //TODO JGT Check if mask is necessary

      }

    m_Multi3DMapToDEMFilter->SetOutputParametersFrom3DMap();
    m_Multi3DMapToDEMFilter->SetDEMGridStep(this->GetParameterFloat("res"));
    m_Multi3DMapToDEMFilter ->SetCellFusionMode(1); //maxfiltering
    m_Multi3DMapToDEMFilter->UpdateOutputInformation();

    SetParameterOutputImage("out", m_Multi3DMapToDEMFilter->GetOutput());

  }
  
  // private filters
  std::vector<itk::LightObject::Pointer> m_Filters;

  InterpolatorType::Pointer m_Interpolator;
  
  MultiDisparityTo3DFilterListType  m_MultiDisparityTo3DFilterList;
  Multi3DFilterType::Pointer  m_Multi3DMapToDEMFilter;

  ExtractorListType                            m_ExtractorList;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::StereoFramework)
