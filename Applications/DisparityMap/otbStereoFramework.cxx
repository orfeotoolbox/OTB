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
#include "otbDisparityMapTo3DFilter.h"
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
#include "otbBijectionCoherencyFilter.h"

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
  
   typedef otb::Functor::SSDBlockMatching<FloatImageType,FloatImageType> SSDBlockMatchingFunctorType;
   typedef otb::Functor::SSDDivMeanBlockMatching<FloatImageType,FloatImageType> SSDDivMeanBlockMatchingFunctorType;

   typedef otb::Functor::NCCBlockMatching<FloatImageType,FloatImageType> NCCBlockMatchingFunctorType;
   typedef otb::Functor::LPBlockMatching<FloatImageType,FloatImageType>  LPBlockMatchingFunctorType;

   typedef otb::PixelWiseBlockMatchingImageFilter<FloatImageType,
                                                              FloatImageType,
                                                              FloatImageType,
                                                              FloatImageType,
                                                              SSDBlockMatchingFunctorType> SSDBlockMatchingFilterType;
   typedef otb::PixelWiseBlockMatchingImageFilter<FloatImageType,
                                                                FloatImageType,
                                                                FloatImageType,
                                                                FloatImageType,
                                                                SSDDivMeanBlockMatchingFunctorType> SSDDivMeanBlockMatchingFilterType;

     typedef otb::PixelWiseBlockMatchingImageFilter<FloatImageType,
                                                              FloatImageType,
                                                              FloatImageType,
                                                              FloatImageType,
                                                              NCCBlockMatchingFunctorType> NCCBlockMatchingFilterType;

     typedef otb::PixelWiseBlockMatchingImageFilter<FloatImageType,
                                                              FloatImageType,
                                                              FloatImageType,
                                                              FloatImageType,
                                                              LPBlockMatchingFunctorType> LPBlockMatchingFilterType;

  typedef otb::BandMathImageFilter
    <FloatImageType>                          BandMathFilterType;

  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                                FloatImageType,
                                                FloatImageType,
                                                FloatImageType,
                                                SSDBlockMatchingFunctorType> SSDSubPixelFilterType;

  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                                  FloatImageType,
                                                  FloatImageType,
                                                  FloatImageType,
                                                  SSDDivMeanBlockMatchingFunctorType> SSDDivMeanSubPixelFilterType;

   typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                                FloatImageType,
                                                FloatImageType,
                                                FloatImageType,
                                                LPBlockMatchingFunctorType> LPSubPixelFilterType;

  typedef otb::SubPixelDisparityImageFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatImageType,
     NCCBlockMatchingFunctorType>             NCCSubPixelFilterType;

  typedef otb::DisparityMapMedianFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType>                          MedianFilterType;


  typedef otb::VarianceImageFilter<FloatImageType,FloatImageType> VarianceFilterType;

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

  typedef otb::DisparityMapTo3DFilter
    <FloatImageType,
     FloatVectorImageType,DeformationFieldType,FloatImageType>                         DisparityTo3DFilter;

  typedef itk::VectorIndexSelectionCastImageFilter<DeformationFieldType,
                                                     FloatImageType> IndexSelectionCastFilterType;

    typedef otb::ImageListToVectorImageFilter<ImageListType,FloatVectorImageType> ImageListFilterType;

    typedef ExtractROIFilterType::Pointer               ExtractROIFilterPointerType;
    typedef std::vector<ExtractROIFilterPointerType>            ExtractorListType;
    typedef std::vector<MultiDisparityTo3DFilterPointerType>           MultiDisparityTo3DFilterListType;

    typedef otb::BijectionCoherencyFilter
      <FloatImageType,
        FloatImageType>                             BijectionFilterType;

    typedef itk::ImageToImageFilter<FloatImageType,FloatImageType>  FilterType;
private:
  
  StereoFramework()
    {

     m_Interpolator = InterpolatorType::New();
     m_Multi3DMapToDEMFilter =  Multi3DFilterType::New();
    }
  
  std::vector<std::vector<int> > CreateCouplesList(string couples)
    {
    std::vector<std::vector<int> > couplesList;

    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    unsigned int imageListNb = inList->Size();

    if (!couples.empty())
      {

      unsigned int i;
      std::vector<int> couple;
      std::stringstream couplesStream(couples);
      while (couplesStream >> i)
        {
        if (i >= imageListNb)
          {
          otbAppLogFATAL(<<i<<" is not a valid image index.");
          }
        couple.push_back(i);
        if (couplesStream.peek() == ',')
          {
          couplesStream.ignore();
          couplesList.push_back(couple);
          couple.clear();
          }
        }
       if (couplesStream.eof() == 0)
        {
        otbAppLogFATAL(<<couplesStream.str()<<" contain non valid character for couples string. (""comma"" and ""spaces"" are only valid delimiters) ");
        }

       couplesList.push_back(couple);

       otbAppLogINFO(<<couplesList.size()<<"  Stereo couples will be processed");

      }
    else
      {
      if (imageListNb % 2 != 0)
        {
        otbAppLogFATAL(<<" input image list number must be even with empty couple string parameter. please add couple string parameter or input image(s).");

        }
      const unsigned int couplesNb = (imageListNb)/2;

      otbAppLogINFO(<<couplesNb<<"  Stereo couples will be processed");

      for (unsigned int index = 0; index <= couplesNb; index = index + 2)
        {
        std::vector<int> couple;
        couple.push_back(index);
        couple.push_back(index + 1);
        couplesList.push_back(couple);
        }
      }

    return couplesList;
  }


  void DoInit()
  {
    SetName("StereoFramework");
    SetDescription("Compute the ground elevation based on a stereo pair");

    SetDocName("Stereo Framework");
    SetDocLongDescription("Compute the ground elevation with a stereo block matching algorithm "
                          "between a stereo pair in sensor geometry. The output is projected in "
                          "desired geographic or cartographic projection (UTM by default). The pipeline is made of the following steps:\n"
                          "\t- compute the epipolar deformation grids from the stereo pair\n"
                          "\t- resample the stereo pair into epipolar images using BCO interpolation\n"
                          "\t- create masks for each epipolar image : remove black borders and resample"
                          " input masks\n"
                          "\t- compute horizontal disparities with a NCC block matching algorithm\n"
                          "\t- refine disparities to sub-pixel precision with a dichotomy algorithm\n"
                          "\t- apply an optional median filter\n"
                          "\t- filter disparities based on the correlation score (must be greater than "
                          "0.6) and exploration bounds\n"
                          "\t- project disparities on a regular grid for each cell the "
                          "maximum elevation is kept.\n"
                          "NoData values are filled with -32768\n");
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Stereo);

    AddParameter(ParameterType_InputImageList,  "il",   "Input images list");
    SetParameterDescription("il", "The list of images. First image is used as left sensor image. Other images are used to complete stereo couple.");

    AddParameter(ParameterType_String, "co", "Couples list");
    SetParameterDescription("co","list of index of couples. couples must be separated by a comma. (index start at 0). for example : 0 1,1 2. note that images are handled by pairs."
        " if left empty couples are created from input index (in this case image list must be even).");
    MandatoryOff("co");
    SetParameterString("co","");
    DisableParameter("co");


    ElevationParametersHandler::AddElevationParameters(this, "elev");
    
    // Add the output paramters in a group
    AddParameter(ParameterType_Group, "output", "Output parameters");
    SetParameterDescription("output","This group of parameters allows to choose the DEM resolution and projection parameters.");

    // Build the Output Map Projection
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "map");

    SetParameterString("map","wgs");

    AddParameter(ParameterType_Float, "output.res","Output resolution");
    SetParameterDescription("output.res","Spatial sampling distance of the output elevation (in m)");
    SetDefaultParameterFloat("output.res",1.);

    AddParameter(ParameterType_Float, "output.nodata","NoData value");
    SetParameterDescription("output.nodata","DEM empty cells are filled with tihs value (optional -32768 by default)");
    SetDefaultParameterFloat("output.nodata",-32768);
    MandatoryOff("output.nodata");
    
    AddParameter(ParameterType_OutputImage,"output.out","Output image");
    SetParameterDescription("output.out","Output elevation image");


    // UserDefined values
    AddParameter(ParameterType_Choice, "output.mode", "Parameters estimation modes");
    AddChoice("output.mode.fit", "Fit to sensor image");
       SetParameterDescription("output.mode.fit", "Fit the size, origin and spacing to an existing ortho image (uses the value of outputs.ortho)");
    AddChoice("output.mode.user", "User Defined");
    SetParameterDescription("output.mode.user","This mode allows you to fully modify default values.");
    // Upper left point coordinates
    AddParameter(ParameterType_Float, "output.mode.user.ulx", "Upper Left X");
    SetParameterDescription("output.mode.user.ulx","Cartographic X coordinate of upper-left corner (meters for cartographic projections, degrees for geographic ones)");

    AddParameter(ParameterType_Float, "output.mode.user.uly", "Upper Left Y");
    SetParameterDescription("output.mode.user.uly","Cartographic Y coordinate of the upper-left corner (meters for cartographic projections, degrees for geographic ones)");

    // Size of the output image
    AddParameter(ParameterType_Int, "output.mode.user.sizex", "Size X");
    SetParameterDescription("output.mode.user.sizex","Size of projected image along X (in pixels)");

    AddParameter(ParameterType_Int, "output.mode.user.sizey", "Size Y");
    SetParameterDescription("output.mode.user.sizey","Size of projected image along Y (in pixels)");

    // Spacing of the output image
    AddParameter(ParameterType_Float, "output.mode.user.spacingx", "Pixel Size X");
    SetParameterDescription("output.mode.user.spacingx","Size of each pixel along X axis (meters for cartographic projections, degrees for geographic ones)");


    AddParameter(ParameterType_Float, "output.mode.user.spacingy", "Pixel Size Y");
    SetParameterDescription("output.mode.user.spacingy","Size of each pixel along Y axis (meters for cartographic projections, degrees for geographic ones)");


    AddParameter(ParameterType_Int,"step","Step of the deformation grid (in nb. of pixels)");
    SetParameterDescription("step","Stereo-rectification deformation grid only varies slowly. Therefore, it is recommanded to use a coarser grid (higher step value) in case of large images");
    SetDefaultParameterInt("step",16);
    MandatoryOff("step");

    AddParameter(ParameterType_Int, "ssfactor", "Sub-sampling rate for epipolar grid inversion");
    SetParameterDescription("ssfactor","Grid inversion is an heavy process that implies spline regression on control points. To avoid eating to much memory, this parameter allows to first sub-sample the field to invert.");
    SetDefaultParameterInt("ssfactor",1);
    SetMinimumParameterIntValue("ssfactor",1);
    MandatoryOff("ssfactor");

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
    
    AddParameter(ParameterType_Group,"bm","Block matching parameters");
    SetParameterDescription("bm","This group of parameters allow to tune the block-matching behaviour");

     AddParameter(ParameterType_Choice,   "bm.metric", "Block-matching metric");
     AddChoice("bm.metric.ncc","Normalized Cross-Correlation");
     SetParameterDescription("bm.metric.ncc","Normalized Cross-Correlation between the left and right windows");


     AddChoice("bm.metric.ssd","Sum of Squared Distances");
     SetParameterDescription("bm.metric.ssd","Sum of squared distances between pixels value in the metric window");

     AddChoice("bm.metric.ssdmean","Sum of Squared Distances divided by mean of block");
     SetParameterDescription("bm.metric.ssdmean","derived version of Sum of squared distances between pixels value in the metric window (SSD divided by mean over window)");

     AddChoice("bm.metric.lp","Lp pseudo-norm");
     SetParameterDescription("bm.metric.lp","Lp pseudo-norm between the left and right windows");

     AddParameter(ParameterType_Float,"bm.metric.lp.p","p value" );
     SetParameterDescription("bm.metric.lp.p", "Value of the p parameter in Lp pseudo-norm (must be positive)");
     SetDefaultParameterFloat("bm.metric.lp.p", 1.0);
     SetMinimumParameterFloatValue("bm.metric.lp.p", 0.0);

    AddParameter(ParameterType_Int,"bm.radius","Radius of blocks for matching filter");
    SetParameterDescription("bm.radius","The radius (in pixels) of blocks in Block-Matching");
    SetDefaultParameterInt("bm.radius",4);
    SetMinimumParameterIntValue("bm.radius",1);
    MandatoryOff("bm.radius");

    AddParameter(ParameterType_Empty,"bij","Use bijection consistency in block matching strategy");
    SetParameterDescription("bij","use bijection consistency. Right to Left correlation is computed to validate Left to Right disparities. If bijection is not found pixel is rejected.");
    MandatoryOff("bij");
    DisableParameter("bij");

    AddParameter(ParameterType_Empty,"med","Use median disparities filtering");
    SetParameterDescription("med","disparities output can be filtered using median post filtering (disabled by default).");
    MandatoryOff("med");
    DisableParameter("med");


    AddParameter(ParameterType_Float,"metrict","correlation metric threshold.");
    SetParameterDescription("metrict","Use block matching metric output to discard pixels with low correlation value.(disabled by default)");
    MandatoryOff("metrict");
    SetDefaultParameterFloat("metrict",0.6);
    DisableParameter("metrict");


    AddParameter(ParameterType_Group,"mask","Masks");
    
    AddParameter(ParameterType_InputImage, "mask.left","Input left mask");
    SetParameterDescription("mask.left","Mask for left input image");
    MandatoryOff("mask.left");
    DisableParameter("mask.left");
    
    AddParameter(ParameterType_InputImage, "mask.right","Input right mask");
    SetParameterDescription("mask.right","Mask for right input image");
    MandatoryOff("mask.right");
    DisableParameter("mask.right");
    
    AddParameter(ParameterType_Float,"mask.variancet","Discard pixels with low local variance");
    SetParameterDescription("mask.variancet","This parameter allows to discard pixels whose local variance is too small (the size of the neighborhood is given by the radius parameter)");
    MandatoryOff("mask.variancet");
    SetDefaultParameterFloat("mask.variancet",100.);
    DisableParameter("mask.variancet");
    
    AddRAMParameter();
    

    SetDocExampleParameterValue("il","sensor_stereo_left.tif sensor_stereo_right.tif");
    SetDocExampleParameterValue("output.res","2.5");
    SetDocExampleParameterValue("output.out","dem.tif");
    
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do
  }
  

  template<class TInputImage, class TMetricFunctor>
    void
    SetBlockMatchingParameters(otb::PixelWiseBlockMatchingImageFilter<TInputImage,TInputImage,TInputImage,TInputImage,
                             TMetricFunctor>*blockMatcherFilter, otb::PixelWiseBlockMatchingImageFilter<TInputImage,TInputImage,TInputImage,TInputImage,
                             TMetricFunctor>*invBlockMatcherFilter,otb::SubPixelDisparityImageFilter<TInputImage,TInputImage,TInputImage,TInputImage,
                             TMetricFunctor>*subPixelFilter, TInputImage * leftImage, TInputImage * rightImage,TInputImage * leftMask,TInputImage * rightMask,TInputImage * finalMask, const bool minimize,double minDisp,double maxDisp)
    {
    typedef TMetricFunctor MetricFunctorType;
    typedef otb::PixelWiseBlockMatchingImageFilter<TInputImage, TInputImage, TInputImage, TInputImage,
        MetricFunctorType> BlockMatchingFilterType;
    typedef otb::SubPixelDisparityImageFilter<TInputImage, TInputImage, TInputImage, TInputImage, MetricFunctorType>
        SubPixelFilterType;

    blockMatcherFilter->SetLeftInput(leftImage);
    blockMatcherFilter->SetRightInput(rightImage);
    blockMatcherFilter->SetLeftMaskInput(leftMask);
    blockMatcherFilter->SetRightMaskInput(rightMask);
    blockMatcherFilter->SetRadius(this->GetParameterInt("bm.radius"));
    blockMatcherFilter->SetMinimumHorizontalDisparity(minDisp);
    blockMatcherFilter->SetMaximumHorizontalDisparity(maxDisp);
    blockMatcherFilter->SetMinimumVerticalDisparity(0);
    blockMatcherFilter->SetMaximumVerticalDisparity(0);

    if (minimize)
      {
      blockMatcherFilter->MinimizeOn();
      }
    else blockMatcherFilter->MinimizeOff();

    if (IsParameterEnabled("bij"))
      {
      invBlockMatcherFilter->SetLeftInput(rightImage);
      invBlockMatcherFilter->SetRightInput(leftImage);
      invBlockMatcherFilter->SetLeftMaskInput(rightMask);
      invBlockMatcherFilter->SetRightMaskInput(leftMask);
      invBlockMatcherFilter->SetRadius(this->GetParameterInt("bm.radius"));
      invBlockMatcherFilter->SetMinimumHorizontalDisparity(-maxDisp);
      invBlockMatcherFilter->SetMaximumHorizontalDisparity(-minDisp);
      invBlockMatcherFilter->SetMinimumVerticalDisparity(0);
      invBlockMatcherFilter->SetMaximumVerticalDisparity(0);

      if (minimize)
        {
        invBlockMatcherFilter->MinimizeOn();
        }
      else invBlockMatcherFilter->MinimizeOff();
      }

    subPixelFilter->SetInputsFromBlockMatchingFilter(blockMatcherFilter);
    subPixelFilter->SetRefineMethod(SubPixelFilterType::DICHOTOMY);
    subPixelFilter->SetLeftMaskInput(finalMask);
    subPixelFilter->UpdateOutputInformation();
  }


  void DoExecute()
  {
    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");
    double underElev = this->GetParameterFloat("below");
    double overElev = this->GetParameterFloat("above");

    m_Filters.clear();

    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    if (inList->Size() < 2)
      {
      itkExceptionMacro("at least two input images must be set...");
      }

    //create pipeline for each stereo couple


    std::vector<std::vector<int> > couples = this->CreateCouplesList(GetParameterString("co"));

    unsigned int stereoCouples = couples.size();
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
    // Update the UTM zone params
    MapProjectionParametersHandler::InitializeUTMParameters(this, "il", "map");
    // Get the output projection Ref
    m_OutputProjectionRef = MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "map");

    //create BCO interpolator with radius 2
    // used by Left and Right Resampler and Left and Right Mask REsampler
    m_Interpolator->SetRadius(2);

    m_Multi3DMapToDEMFilter->SetNumberOf3DMaps(stereoCouples);
    m_Multi3DMapToDEMFilter->SetNoDataValue(this->GetParameterFloat("output.nodata"));

    // value of ram used to compute epipolar grid
    double globalEpiStorageSize = 0;

    for (unsigned int i = 0; i < stereoCouples; i++)
      {

      std::vector<int> couple = couples[i];
      if (couple.size() > 2)
        {
        otbAppLogWARNING(<<" current couple contain more than 2 value. only first and second values will be associated to create next couple.");
        }
      otbAppLogINFO("\n \n \n /****    process image index "<<couple[0]<<" and "<<couple[1]<<"    ****/ \n \n");
      FloatImageType::Pointer inleft = m_ExtractorList[couple[0]]->GetOutput();
      FloatImageType::Pointer inright = m_ExtractorList[couple[1]]->GetOutput();

      m_MultiDisparityTo3DFilterList[i] = MultiDisparityTo3DFilterType::New();
      DeformationFieldSourceType::Pointer epipolarGridSource = DeformationFieldSourceType::New();
      epipolarGridSource->SetLeftImage(inleft);
      epipolarGridSource->SetRightImage(inright);
      epipolarGridSource->SetGridStep(this->GetParameterInt("step"));
      epipolarGridSource->SetScale(1.0);

      if (otb::Wrapper::ElevationParametersHandler::IsDEMUsed(this, "elev")
          && otb::Wrapper::ElevationParametersHandler::IsGeoidUsed(this, "elev"))
        {
        epipolarGridSource->SetUseDEM(true);
        }

      epipolarGridSource->UpdateOutputInformation();
      // check that deformation grids fit in 1/4 of available RAM
      FloatVectorImageType::SizeType
          gridSize = epipolarGridSource->GetLeftDeformationFieldOutput()->GetLargestPossibleRegion().GetSize();
      double storageSize = static_cast<double> (gridSize[0]) * static_cast<double> (gridSize[1]) * 4.0 * 8.0
          / 1000000.0;
      globalEpiStorageSize += storageSize;
      if (globalEpiStorageSize > (static_cast<double> (this->GetParameterInt("ram"))))
        {
        otbAppLogINFO(<<"grid step for couple  "<<couple[0]<<" and "<<couple[1]<<" memory usage "<<storageSize<<" MO.");
        otbAppLogWARNING(<<"Deformation grid step value"<<this->GetParameterInt("step")<<" seems too be to high.");
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

      double minDisp = vcl_floor((-1.0) * overElev * meanBaseline / epiSpacing[0]);
      double maxDisp = vcl_ceil((-1.0) * underElev * meanBaseline / epiSpacing[0]);
      otbAppLogINFO(<<"Minimum disparity : "<<minDisp);
      otbAppLogINFO(<<"Maximum disparity : "<<maxDisp);

      // Compute rectification grids (left/right and left inverse (for disparity translate filter)).
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
      leftInverseDeformationFieldFilter->SetSubsamplingFactor(this->GetParameterInt("ssfactor"));
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
      unsigned int inputIdLeft = 0;
      unsigned int inputIdRight = 0;

      lBandMathFilter->SetNthInput(inputIdLeft, leftResampleFilter->GetOutput(), "inleft");
      ++inputIdLeft;
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

        lBandMathFilter->SetNthInput(inputIdLeft, leftMaskResampleFilter->GetOutput(), "maskleft");
        ++inputIdLeft;
        leftFormula << " and (maskleft > 0)";
        }
      // Handle variance threshold if present
      if (IsParameterEnabled("mask.variancet"))
        {
        // Left side
        VarianceFilterType::Pointer leftVarianceFilter = VarianceFilterType::New();
        leftVarianceFilter->SetInput(leftResampleFilter->GetOutput());
        VarianceFilterType::InputSizeType vradius;
        vradius.Fill(this->GetParameterInt("bm.radius"));
        leftVarianceFilter->SetRadius(vradius);

        lBandMathFilter->SetNthInput(inputIdLeft, leftVarianceFilter->GetOutput(), "variance");
        ++inputIdLeft;
        leftFormula << " and variance > " << GetParameterFloat("mask.variancet");
        m_Filters.push_back(leftVarianceFilter.GetPointer());
        }

      leftFormula << ",255,0)";
      lBandMathFilter->SetExpression(leftFormula.str());

      m_Filters.push_back(leftMaskResampleFilter.GetPointer());

      rBandMathFilter->SetNthInput(inputIdRight, rightResampleFilter->GetOutput(), "inright");
      ++inputIdRight;
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

        rBandMathFilter->SetNthInput(inputIdRight, rightMaskResampleFilter->GetOutput(), "maskright");
        ++inputIdRight;
        rightFormula << " and (maskright > 0)";
        }
      if (IsParameterEnabled("mask.variancet"))
        {
        // right side
        VarianceFilterType::Pointer rightVarianceFilter = VarianceFilterType::New();
        rightVarianceFilter->SetInput(rightResampleFilter->GetOutput());
        VarianceFilterType::InputSizeType vradius;
        vradius.Fill(this->GetParameterInt("bm.radius"));
        rightVarianceFilter->SetRadius(vradius);

        rBandMathFilter->SetNthInput(inputIdRight, rightVarianceFilter->GetOutput(), "variance");
        ++inputIdRight;
        rightFormula << " and variance > " << GetParameterFloat("mask.variancet");
        m_Filters.push_back(rightVarianceFilter.GetPointer());
        }

      rightFormula << ",255,0)";
      rBandMathFilter->SetExpression(rightFormula.str());

      m_Filters.push_back(rightMaskResampleFilter.GetPointer());
      m_Filters.push_back(rBandMathFilter.GetPointer());
      m_Filters.push_back(lBandMathFilter.GetPointer());

      BandMathFilterType::Pointer finalMaskFilter;
      if (IsParameterEnabled("bij"))
        {
        finalMaskFilter = BandMathFilterType::New();
        }
      else
        {
        finalMaskFilter = lBandMathFilter;
        }

      // Compute disparities
      FilterType* blockMatcherFilterPointer;
      FilterType* invBlockMatcherFilterPointer = NULL;
      FilterType* subPixelFilterPointer;
      BijectionFilterType::Pointer bijectFilter;

      // pointer
      bool minimize = false;
      //switch

      NCCBlockMatchingFilterType::Pointer NCCBlockMatcherFilter;
      NCCBlockMatchingFilterType::Pointer invNCCBlockMatcherFilter;
      NCCSubPixelFilterType::Pointer NCCSubPixelFilter;

      SSDBlockMatchingFilterType::Pointer SSDBlockMatcherFilter;
      SSDBlockMatchingFilterType::Pointer invSSDBlockMatcherFilter;
      SSDSubPixelFilterType::Pointer SSDSubPixelFilter;

      SSDDivMeanBlockMatchingFilterType::Pointer SSDDivMeanBlockMatcherFilter;
      SSDDivMeanBlockMatchingFilterType::Pointer invSSDDivMeanBlockMatcherFilter;
      SSDDivMeanSubPixelFilterType::Pointer SSDDivMeanSubPixelFilter;

      LPBlockMatchingFilterType::Pointer LPBlockMatcherFilter;
      LPBlockMatchingFilterType::Pointer invLPBlockMatcherFilter;
      LPSubPixelFilterType::Pointer LPSubPixelFilter;

      switch (GetParameterInt("bm.metric"))
        {
        case 0: //NCC
          otbAppLogINFO(<<"use NCC Metric for BlockMatching.")
        ;
          NCCBlockMatcherFilter = NCCBlockMatchingFilterType::New();
          blockMatcherFilterPointer = NCCBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("bij"))
            {
            //Reverse correlation
            invNCCBlockMatcherFilter = NCCBlockMatchingFilterType::New();
            invBlockMatcherFilterPointer = invNCCBlockMatcherFilter.GetPointer();
            m_Filters.push_back(invBlockMatcherFilterPointer);
            }
          NCCSubPixelFilter = NCCSubPixelFilterType::New();
          subPixelFilterPointer = NCCSubPixelFilter.GetPointer();
          m_Filters.push_back(NCCSubPixelFilter.GetPointer());

          minimize = false;
          this->SetBlockMatchingParameters<FloatImageType, NCCBlockMatchingFunctorType> (
                                                                                         NCCBlockMatcherFilter,
                                                                                         invNCCBlockMatcherFilter,
                                                                                         NCCSubPixelFilter,
                                                                                         leftResampleFilter->GetOutput(),
                                                                                         rightResampleFilter->GetOutput(),
                                                                                         lBandMathFilter->GetOutput(),
                                                                                         rBandMathFilter->GetOutput(),
                                                                                         finalMaskFilter->GetOutput(),
                                                                                         minimize, minDisp, maxDisp);
          break;
        case 1: //SSD
          otbAppLogINFO(<<"use SSD Metric for BlockMatching.")
        ;

          SSDBlockMatcherFilter = SSDBlockMatchingFilterType::New();
          blockMatcherFilterPointer = SSDBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("bij"))
            {
            //Reverse correlation
            invSSDBlockMatcherFilter = SSDBlockMatchingFilterType::New();
            invBlockMatcherFilterPointer = invSSDBlockMatcherFilter.GetPointer();
            m_Filters.push_back(invBlockMatcherFilterPointer);
            }
          SSDSubPixelFilter = SSDSubPixelFilterType::New();
          subPixelFilterPointer = SSDSubPixelFilter.GetPointer();
          m_Filters.push_back(SSDSubPixelFilter.GetPointer());

          minimize = true;
          this->SetBlockMatchingParameters<FloatImageType, SSDBlockMatchingFunctorType> (
                                                                                         SSDBlockMatcherFilter,
                                                                                         invSSDBlockMatcherFilter,
                                                                                         SSDSubPixelFilter,
                                                                                         leftResampleFilter->GetOutput(),
                                                                                         rightResampleFilter->GetOutput(),
                                                                                         lBandMathFilter->GetOutput(),
                                                                                         rBandMathFilter->GetOutput(),
                                                                                         finalMaskFilter->GetOutput(),
                                                                                         minimize, minDisp, maxDisp);

          break;
        case 2: //SSDDivMean
          otbAppLogINFO(<<"use robust SSD Metric for BlockMatching.")
        ;

          SSDDivMeanBlockMatcherFilter = SSDDivMeanBlockMatchingFilterType::New();
          blockMatcherFilterPointer = SSDDivMeanBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("bij"))
            {
            //Reverse correlation
            invSSDDivMeanBlockMatcherFilter = SSDDivMeanBlockMatchingFilterType::New();
            invBlockMatcherFilterPointer = invSSDDivMeanBlockMatcherFilter.GetPointer();
            m_Filters.push_back(invBlockMatcherFilterPointer);
            }
          SSDDivMeanSubPixelFilter = SSDDivMeanSubPixelFilterType::New();
          subPixelFilterPointer = SSDDivMeanSubPixelFilter.GetPointer();
          m_Filters.push_back(SSDDivMeanSubPixelFilter.GetPointer());

          minimize = true;
          this->SetBlockMatchingParameters<FloatImageType, SSDDivMeanBlockMatchingFunctorType> (
                                                                                                SSDDivMeanBlockMatcherFilter,
                                                                                                invSSDDivMeanBlockMatcherFilter,
                                                                                                SSDDivMeanSubPixelFilter,
                                                                                                leftResampleFilter->GetOutput(),
                                                                                                rightResampleFilter->GetOutput(),
                                                                                                lBandMathFilter->GetOutput(),
                                                                                                rBandMathFilter->GetOutput(),
                                                                                                finalMaskFilter->GetOutput(),
                                                                                                minimize, minDisp,
                                                                                                maxDisp);

          break;
        case 3: //LP
          otbAppLogINFO(<<"use Lp Metric for BlockMatching.")
        ;

          LPBlockMatcherFilter = LPBlockMatchingFilterType::New();
          LPBlockMatcherFilter->GetFunctor().SetP(static_cast<double> (GetParameterFloat("bm.metric.lp.p")));

          blockMatcherFilterPointer = LPBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("bij"))
            {
            //Reverse correlation
            invLPBlockMatcherFilter = LPBlockMatchingFilterType::New();
            invLPBlockMatcherFilter->GetFunctor().SetP(static_cast<double> (GetParameterFloat("bm.metric.lp.p")));
            invBlockMatcherFilterPointer = invLPBlockMatcherFilter.GetPointer();
            m_Filters.push_back(invBlockMatcherFilterPointer);
            }
          LPSubPixelFilter = LPSubPixelFilterType::New();
          subPixelFilterPointer = LPSubPixelFilter.GetPointer();
          m_Filters.push_back(LPSubPixelFilter.GetPointer());

          minimize = false;
          this->SetBlockMatchingParameters<FloatImageType, LPBlockMatchingFunctorType> (
                                                                                        LPBlockMatcherFilter,
                                                                                        invLPBlockMatcherFilter,
                                                                                        LPSubPixelFilter,
                                                                                        leftResampleFilter->GetOutput(),
                                                                                        rightResampleFilter->GetOutput(),
                                                                                        lBandMathFilter->GetOutput(),
                                                                                        rBandMathFilter->GetOutput(),
                                                                                        finalMaskFilter->GetOutput(),
                                                                                        minimize, minDisp, maxDisp);

          break;
        default:
          break;
        }

       if (IsParameterEnabled("bij"))
        {
        otbAppLogINFO(<<"Use reverse blockMatcher to validate direct Horizontal disparities ");
        bijectFilter = BijectionFilterType::New();
        //bijectFilter->SetDirectHorizontalDisparityMapInput(blockMatcherFilter->GetHorizontalDisparityOutput());
        bijectFilter->SetDirectHorizontalDisparityMapInput(blockMatcherFilterPointer->GetOutput(1));
        bijectFilter->SetReverseHorizontalDisparityMapInput(invBlockMatcherFilterPointer->GetOutput(1));
        //bijectFilter->SetTolerance(2);
        bijectFilter->SetMinHDisp(minDisp);
        bijectFilter->SetMaxHDisp(maxDisp);
        bijectFilter->SetMinVDisp(0);
        bijectFilter->SetMaxVDisp(0);
        m_Filters.push_back(bijectFilter.GetPointer());

        //finalMaskFilter = BandMathFilterType::New();
        finalMaskFilter->SetNthInput(0, lBandMathFilter->GetOutput(), "inmask");
        finalMaskFilter->SetNthInput(1, bijectFilter->GetOutput(), "lrrl");
        finalMaskFilter->SetExpression("if(inmask > 0 and lrrl > 0, 255, 0)");
        //
        m_Filters.push_back(finalMaskFilter.GetPointer());
        }


      FloatImageType::Pointer hDispOutput = subPixelFilterPointer->GetOutput(0);
      if (IsParameterEnabled("med"))
        {
        MedianFilterType::Pointer hMedianFilter = MedianFilterType::New();
        hMedianFilter->SetInput(subPixelFilterPointer->GetOutput(0));
        hMedianFilter->SetRadius(2);
        hMedianFilter->SetIncoherenceThreshold(2.0);
        hMedianFilter->SetMaskInput(finalMaskFilter->GetOutput());
        hMedianFilter->UpdateOutputInformation();
        hDispOutput = hMedianFilter->GetOutput();
        m_Filters.push_back(hMedianFilter.GetPointer());
        }

      DisparityTranslateFilter::Pointer disparityTranslateFilter = DisparityTranslateFilter::New();
      disparityTranslateFilter->SetHorizontalDisparityMapInput(hDispOutput);
      disparityTranslateFilter->SetVerticalDisparityMapInput(subPixelFilterPointer->GetOutput(1));
      disparityTranslateFilter->SetInverseEpipolarLeftGrid(leftInverseDeformation);
      disparityTranslateFilter->SetDirectEpipolarRightGrid(rightDeformation);
      // disparityTranslateFilter->SetDisparityMaskInput()
      disparityTranslateFilter->SetLeftSensorImageInput(inleft);
      disparityTranslateFilter->SetNoDataValue(-32768);
      disparityTranslateFilter->UpdateOutputInformation();

      m_Filters.push_back(disparityTranslateFilter.GetPointer());

      BandMathFilterType::Pointer dispTranslateMaskFilter = BandMathFilterType::New();
      dispTranslateMaskFilter->SetNthInput(0, disparityTranslateFilter->GetHorizontalDisparityMapOutput(), "hdisp");
      dispTranslateMaskFilter->SetExpression("hdisp!=-32768");
      m_Filters.push_back(dispTranslateMaskFilter.GetPointer());



      FloatImageType::Pointer hDispOutput2 = disparityTranslateFilter->GetHorizontalDisparityMapOutput();
      FloatImageType::Pointer vDispOutput2 = disparityTranslateFilter->GetVerticalDisparityMapOutput();

      if (IsParameterEnabled("med"))
        {
        MedianFilterType::Pointer hMedianFilter2 = MedianFilterType::New();
        MedianFilterType::Pointer vMedianFilter2 = MedianFilterType::New();

        //TODO JGT Check if medianfiltering is necessary after disparitytranslate
        hMedianFilter2->SetInput(disparityTranslateFilter->GetHorizontalDisparityMapOutput());
        hMedianFilter2->SetRadius(2);
        hMedianFilter2->SetIncoherenceThreshold(2.0);
        //hMedianFilter2->SetMaskInput(lBandMathFilter->GetOutput());
        hMedianFilter2->UpdateOutputInformation();
        hDispOutput2 = hMedianFilter2->GetOutput();
        m_Filters.push_back(hMedianFilter2.GetPointer());

        vMedianFilter2->SetInput(disparityTranslateFilter->GetVerticalDisparityMapOutput());
        vMedianFilter2->SetRadius(2);
        vMedianFilter2->SetIncoherenceThreshold(2.0);
        //vMedianFilter2->SetMaskInput(lBandMathFilter->GetOutput());
        vMedianFilter2->UpdateOutputInformation();
        vDispOutput2 = vMedianFilter2->GetOutput();
        m_Filters.push_back(vMedianFilter2.GetPointer());
        }
      // transform disparity into 3D map
      m_MultiDisparityTo3DFilterList[i]->SetReferenceKeywordList(inleft->GetImageKeywordlist());
      m_MultiDisparityTo3DFilterList[i]->SetNumberOfMovingImages(1);
      m_MultiDisparityTo3DFilterList[i]->SetHorizontalDisparityMapInput(0, hDispOutput2);
      m_MultiDisparityTo3DFilterList[i]->SetVerticalDisparityMapInput(0, vDispOutput2);
      m_MultiDisparityTo3DFilterList[i]->SetMovingKeywordList(0, inright->GetImageKeywordlist());
      m_MultiDisparityTo3DFilterList[i]->UpdateOutputInformation();
      m_MultiDisparityTo3DFilterList[i]->SetDisparityMaskInput(0,dispTranslateMaskFilter->GetOutput());

      // PARAMETER ESTIMATION

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
        minElev = otb::Wrapper::ElevationParametersHandler::GetDefaultElevation(this, "elev");
        maxElev = otb::Wrapper::ElevationParametersHandler::GetDefaultElevation(this, "elev");
        otbAppLogINFO(<<"Default elevation set for Min/Max elevation : "<<minElev);
        }

      otbAppLogINFO(<<"Minimum elevation found : "<<minElev);
      otbAppLogINFO(<<"Maximum elevation found : "<<maxElev);

      //check under and over for each couple
      if (i == 0)
        {
        m_Multi3DMapToDEMFilter->SetElevationMin(minElev + underElev);
        //m_Multi3DMapToDEMFilter->SetNoDataValue(minElev);
        m_Multi3DMapToDEMFilter->SetElevationMax(maxElev + overElev);
        }
      else
        {
        if (minElev < (m_Multi3DMapToDEMFilter->GetElevationMin() - underElev))
          {
          m_Multi3DMapToDEMFilter->SetElevationMin(minElev + underElev);
          //m_Multi3DMapToDEMFilter->SetNoDataValue(minElev);
          }
        if (maxElev < (m_Multi3DMapToDEMFilter->GetElevationMax() - overElev))
          {
          m_Multi3DMapToDEMFilter->SetElevationMax(maxElev + overElev);
          }
        }

      // blockMatcherFilter->SetMinimumHorizontalDisparity(minDisp);
      // blockMatcherFilter->SetMaximumHorizontalDisparity(maxDisp);
      // blockMatcherFilter->SetMinimumVerticalDisparity(0);
      // blockMatcherFilter->SetMaximumVerticalDisparity(0);


      // Compute disparity mask
      BandMathFilterType::Pointer dispMaskFilter = BandMathFilterType::New();
      dispMaskFilter->SetNthInput(0, hDispOutput, "hdisp");
      dispMaskFilter->SetNthInput(1, finalMaskFilter->GetOutput(), "mask");

      std::ostringstream maskFormula;
      maskFormula << "if((hdisp > " << minDisp << ") and (hdisp < " << maxDisp << ") and (mask>0";
      if (IsParameterEnabled("metrict"))
        {
        dispMaskFilter->SetNthInput(2, subPixelFilterPointer->GetOutput(2), "metric");
        maskFormula << ") and (metric ";
        if (minimize == true)
          {
          maskFormula << " < " << this->GetParameterFloat("metrict");
          }
        else
          {
          maskFormula << " > " << this->GetParameterFloat("metrict");

          }
        }
      maskFormula << "),255,0)";
      otbAppLogINFO(<<"disparity mask formula :"<<std::endl<<maskFormula.str());
      dispMaskFilter->SetExpression(maskFormula.str());
      m_Filters.push_back(dispMaskFilter.GetPointer());
      //TODO to check
      disparityTranslateFilter->SetDisparityMaskInput(dispMaskFilter->GetOutput());
      // disparityTo3DFilter->SetDisparityMaskInput(dispMaskFilter->GetOutput());

      m_Multi3DMapToDEMFilter->Set3DMapInput(i, m_MultiDisparityTo3DFilterList[i]->GetOutput());

      //TODO JGT Check if mask is necessary

      }

    if(GetParameterInt("output.mode")==0)
      {
      otbAppLogINFO(<<"DEM parameters are deduced from sensor input data");

      m_Multi3DMapToDEMFilter->SetOutputParametersFrom3DMap();
      }
    else
      {
      otbAppLogINFO(<<"DEM parameters are user defined");
      FloatVectorImageType::IndexType start;
      start[0] = 0;
      start[1] = 0;
      m_Multi3DMapToDEMFilter->SetOutputStartIndex(start);

      FloatVectorImageType::SizeType size;
      size[0] = this->GetParameterInt("output.mode.user.sizex"); // X size
      size[1] = this->GetParameterInt("output.mode.user.sizey"); //Y size
      m_Multi3DMapToDEMFilter->SetOutputSize(size);

      FloatVectorImageType::SpacingType spacing;
      spacing[0] = this->GetParameterFloat("output.mode.user.spacingx");
      spacing[1] = this->GetParameterFloat("output.mode.user.spacingy");
      m_Multi3DMapToDEMFilter->SetOutputSpacing(spacing);

      FloatVectorImageType::PointType origin;
      origin[0] = this->GetParameterFloat("output.mode.user.ulx");
      origin[1] = this->GetParameterFloat("output.mode.user.uly");
      m_Multi3DMapToDEMFilter->SetOutputOrigin(origin);

      }
    otbAppLogINFO(<<"DEM OutputProjectionRef :"<<std::endl<<m_OutputProjectionRef);
    m_Multi3DMapToDEMFilter->SetProjectionRef(m_OutputProjectionRef);

    m_Multi3DMapToDEMFilter->SetDEMGridStep(this->GetParameterFloat("output.res"));
    m_Multi3DMapToDEMFilter ->SetCellFusionMode(otb::CellFusionMode::MAX);
    m_Multi3DMapToDEMFilter->UpdateOutputInformation();

    SetParameterOutputImage("output.out", m_Multi3DMapToDEMFilter->GetOutput());

  }
  
  // private filters
  std::vector<itk::LightObject::Pointer> m_Filters;

  InterpolatorType::Pointer m_Interpolator;
  
  MultiDisparityTo3DFilterListType  m_MultiDisparityTo3DFilterList;
  Multi3DFilterType::Pointer        m_Multi3DMapToDEMFilter;

  ExtractorListType               m_ExtractorList;
  std::string                     m_OutputProjectionRef;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::StereoFramework)
