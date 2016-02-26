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

#include "otbStereorectificationDisplacementFieldSource.h"
#include "otbStreamingWarpImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "otbSubPixelDisparityImageFilter.h"
#include "otbDisparityMapMedianFilter.h"
#include "otbDisparityMapToDEMFilter.h"
#include "otbDisparityMapTo3DFilter.h"
#include "otbDEMToImageGenerator.h"

#include "otbVarianceImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbImageToNoDataMaskFilter.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "itkInverseDisplacementFieldImageFilter.h"

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
  typedef StereoFramework                     Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(StereoFramework, otb::Application);

  /** Filters typedefs */
  typedef FloatImageType::PixelType           FloatPixelType;

  typedef otb::StereorectificationDisplacementFieldSource
    <FloatImageType,FloatVectorImageType>     DisplacementFieldSourceType;

  typedef itk::Vector<float,2>               DisplacementType;
  typedef otb::Image<DisplacementType>         DisplacementFieldType;

  typedef itk::VectorCastImageFilter
    <FloatVectorImageType,
     DisplacementFieldType>                    DisplacementFieldCastFilterType;

  typedef itk::InverseDisplacementFieldImageFilter
   <DisplacementFieldType,DisplacementFieldType> InverseDisplacementFieldFilterType;


  typedef otb::StreamingWarpImageFilter
    <FloatImageType,
     FloatImageType,
     DisplacementFieldType>                    ResampleFilterType;

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
     DisplacementFieldType,
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
      DisplacementFieldType,
      FloatImageType,
      FloatImageType>                              DisparityTranslateFilter;

  typedef otb::DisparityMapTo3DFilter
    <FloatImageType,
     FloatVectorImageType,DisplacementFieldType,FloatImageType>                         DisparityTo3DFilter;

  typedef itk::VectorIndexSelectionCastImageFilter<DisplacementFieldType,
                                                     FloatImageType> IndexSelectionCastFilterType;

    typedef otb::ImageListToVectorImageFilter<ImageListType,FloatVectorImageType> ImageListFilterType;

    typedef ExtractROIFilterType::Pointer               ExtractROIFilterPointerType;
    typedef std::vector<ExtractROIFilterPointerType>            ExtractorListType;
    typedef std::vector<MultiDisparityTo3DFilterPointerType>           MultiDisparityTo3DFilterListType;

    typedef otb::BijectionCoherencyFilter
      <FloatImageType,
        FloatImageType>                             BijectionFilterType;

    typedef itk::ImageToImageFilter<FloatImageType,FloatImageType>  FilterType;

    typedef otb::ImageToNoDataMaskFilter<FloatImageType,FloatImageType> NoDataMaskFilterType;
private:

  StereoFramework()
    {

     m_Interpolator = InterpolatorType::New();
     m_Multi3DMapToDEMFilter =  Multi3DFilterType::New();
    }

  std::vector<std::vector<int> > CreateCouplesList(std::string couples)
    {
    std::vector<std::vector<int> > couplesList;

    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("input.il");

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

       otbAppLogINFO(<<couplesList.size()<<" Stereo couples will be processed");

      }
    else
      {
      if (imageListNb % 2 != 0)
        {
        otbAppLogFATAL(<<" input image list number must be even with empty couple string parameter. please add couple string parameter or input image(s).");

        }
      const unsigned int couplesNb = (imageListNb)/2;

      otbAppLogINFO(<<couplesNb<<" Stereo couples will be processed");

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
    SetDescription("Compute the ground elevation based on one or multiple stereo pair(s)");

    SetDocName("Stereo Framework");
    SetDocLongDescription("Compute the ground elevation with a stereo block matching algorithm "
                          "between one or mulitple stereo pair in sensor geometry. The output is projected in "
                          "desired geographic or cartographic map projection (UTM by default). The pipeline is made of the following steps:\n"
                          "for each sensor pair :\n"
                          "\t- compute the epipolar displacement grids from the stereo pair (direct and inverse)\n"
                          "\t- resample the stereo pair into epipolar geometry using BCO interpolation\n"
                          "\t- create masks for each epipolar image : remove black borders and resample"
                          " input masks\n"
                          "\t- compute horizontal disparities with a block matching algorithm\n"
                          "\t- refine disparities to sub-pixel precision with a dichotomy algorithm\n"
                          "\t- apply an optional median filter\n"
                          "\t- filter disparities based on the correlation score  and exploration bounds\n"
                          "\t- translate disparities in sensor geometry\n"
                          "\t  convert disparity to 3D Map.\n"
                          "Then fuse all 3D maps to produce DSM.");
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Stereo);

    // Add the output paramters in a group
    AddParameter(ParameterType_Group, "input", "Input parameters");
    SetParameterDescription("input","This group of parameters allows one to parametrize input data.");

    AddParameter(ParameterType_InputImageList,  "input.il",   "Input images list");
    SetParameterDescription("input.il", "The list of images.");

    AddParameter(ParameterType_String, "input.co", "Couples list");
    SetParameterDescription("input.co","List of index of couples im image list. Couples must be separated by a comma. (index start at 0). for example : 0 1,1 2 will process a first couple composed of the first and the second image in image list, then the first and the third image\n. note that images are handled by pairs."
        " if left empty couples are created from input index i.e. a first couple will be composed of the first and second image, a second couple with third and fourth image etc. (in this case image list must be even).");
    MandatoryOff("input.co");
    SetParameterString("input.co","");
    DisableParameter("input.co");

    AddParameter(ParameterType_Int,  "input.channel",   "Image channel used for the block matching");
    SetParameterDescription("input.channel", "Used channel for block matching (used for all images)");
    SetDefaultParameterInt("input.channel", 1);
    SetMinimumParameterIntValue("input.channel", 1);

    ElevationParametersHandler::AddElevationParameters(this, "elev");

    // Add the output paramters in a group
    AddParameter(ParameterType_Group, "output", "Output parameters");
    SetParameterDescription("output","This group of parameters allows one to choose the DSM resolution, nodata value, and projection parameters.");

    // // Build the Output Map Projection
    // for custom map projection
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "map");
    SetParameterString("map","wgs");

    AddParameter(ParameterType_Float, "output.res","Output resolution");
    SetParameterDescription("output.res","Spatial sampling distance of the output elevation : the cell size (in m)");
    SetDefaultParameterFloat("output.res",1.);

    AddParameter(ParameterType_Float, "output.nodata","NoData value");
    SetParameterDescription("output.nodata","DSM empty cells are filled with this value (optional -32768 by default)");
    SetDefaultParameterFloat("output.nodata",-32768);
    MandatoryOff("output.nodata");

    // UserDefined values
    AddParameter(ParameterType_Choice, "output.fusionmethod", "Method to fuse measures in each DSM cell");
    SetParameterDescription("output.fusionmethod","This parameter allows one to choose the method used to fuse elevation measurements in each output DSM cell");
    AddChoice("output.fusionmethod.max", "The cell is filled with the maximum measured elevation values");
    AddChoice("output.fusionmethod.min", "The cell is filled with the minimum measured elevation values");
    AddChoice("output.fusionmethod.mean","The cell is filled with the mean of measured elevation values");
    AddChoice("output.fusionmethod.acc", "accumulator mode. The cell is filled with the the number of values (for debugging purposes).");

    AddParameter(ParameterType_OutputImage,"output.out","Output DSM");
    SetParameterDescription("output.out","Output elevation image");

    // UserDefined values
    AddParameter(ParameterType_Choice, "output.mode", "Parameters estimation modes");
    AddChoice("output.mode.fit", "Fit to sensor image");
       SetParameterDescription("output.mode.fit", "Fit the size, origin and spacing to an existing ortho image (uses the value of outputs.ortho)");
    AddChoice("output.mode.user", "User Defined");
    SetParameterDescription("output.mode.user","This mode allows you to fully modify default values.");
    // Upper left point coordinates
    AddParameter(ParameterType_Float, "output.mode.user.ulx", "Upper Left X ");
    SetParameterDescription("output.mode.user.ulx","Cartographic X coordinate of upper-left corner (meters for cartographic projections, degrees for geographic ones)");

    AddParameter(ParameterType_Float, "output.mode.user.uly", "Upper Left Y ");
    SetParameterDescription("output.mode.user.uly","Cartographic Y coordinate of the upper-left corner (meters for cartographic projections, degrees for geographic ones)");

    // Size of the output image
    AddParameter(ParameterType_Int, "output.mode.user.sizex", "Size X ");
    SetParameterDescription("output.mode.user.sizex","Size of projected image along X (in pixels)");

    AddParameter(ParameterType_Int, "output.mode.user.sizey", "Size Y ");
    SetParameterDescription("output.mode.user.sizey","Size of projected image along Y (in pixels)");

    // Spacing of the output image
    AddParameter(ParameterType_Float, "output.mode.user.spacingx", "Pixel Size X ");
    SetParameterDescription("output.mode.user.spacingx","Size of each pixel along X axis (meters for cartographic projections, degrees for geographic ones)");


    AddParameter(ParameterType_Float, "output.mode.user.spacingy", "Pixel Size Y ");
    SetParameterDescription("output.mode.user.spacingy","Size of each pixel along Y axis (meters for cartographic projections, degrees for geographic ones)");

    // Add the output paramters in a group
    AddParameter(ParameterType_Group, "stereorect", "Stereorectification Grid parameters");
    SetParameterDescription("stereorect","This group of parameters allows one to choose direct and inverse grid subsampling. These parameters are very useful to tune time and memory consumption.");

    AddParameter(ParameterType_Int,"stereorect.fwdgridstep","Step of the displacement grid (in pixels)");
    SetParameterDescription("stereorect.fwdgridstep","Stereo-rectification displacement grid only varies slowly. Therefore, it is recommended to use a coarser grid (higher step value) in case of large images");
    SetDefaultParameterInt("stereorect.fwdgridstep",16);
    MandatoryOff("stereorect.fwdgridstep");

    AddParameter(ParameterType_Int, "stereorect.invgridssrate", "Sub-sampling rate for epipolar grid inversion");
    SetParameterDescription("stereorect.invgridssrate","Grid inversion is an heavy process that implies spline regression on control points. To avoid eating to much memory, this parameter allows one to first sub-sample the field to invert.");
    SetDefaultParameterInt("stereorect.invgridssrate",10);
    SetMinimumParameterIntValue("stereorect.invgridssrate",1);
    MandatoryOff("stereorect.invgridssrate");

    AddParameter(ParameterType_Group,"bm","Block matching parameters");
    SetParameterDescription("bm","This group of parameters allow tuning the block-matching behavior");

    AddParameter(ParameterType_Choice,   "bm.metric", "Block-matching metric");
    //SetDefaultParameterInt("bm.metric",3);

    AddChoice("bm.metric.ssdmean","Sum of Squared Distances divided by mean of block");
    SetParameterDescription("bm.metric.ssdmean","derived version of Sum of Squared Distances between pixels value in the metric window (SSD divided by mean over window)");

    AddChoice("bm.metric.ssd","Sum of Squared Distances");
    SetParameterDescription("bm.metric.ssd","Sum of squared distances between pixels value in the metric window");

    AddChoice("bm.metric.ncc","Normalized Cross-Correlation");
    SetParameterDescription("bm.metric.ncc","Normalized Cross-Correlation between the left and right windows");

    AddChoice("bm.metric.lp","Lp pseudo-norm");
    SetParameterDescription("bm.metric.lp","Lp pseudo-norm between the left and right windows");

    AddParameter(ParameterType_Float,"bm.metric.lp.p","p value" );
    SetParameterDescription("bm.metric.lp.p", "Value of the p parameter in Lp pseudo-norm (must be positive)");
    SetDefaultParameterFloat("bm.metric.lp.p", 1.0);
    SetMinimumParameterFloatValue("bm.metric.lp.p", 0.0);

    AddParameter(ParameterType_Int,"bm.radius","Radius of blocks for matching filter (in pixels)");
    SetParameterDescription("bm.radius","The radius of blocks in Block-Matching (in pixels)");
    SetDefaultParameterInt("bm.radius",2);
    SetMinimumParameterIntValue("bm.radius",1);
    MandatoryOff("bm.radius");

    AddParameter(ParameterType_Float,"bm.minhoffset","Minimum altitude offset (in meters)");
    SetParameterDescription("bm.minhoffset","Minimum altitude below the selected elevation source (in meters)");
    //MandatoryOff("bm.minhoffset");
    SetDefaultParameterFloat("bm.minhoffset",-20.0);
    DisableParameter("bm.minhoffset");

    AddParameter(ParameterType_Float,"bm.maxhoffset","Maximum altitude offset (in meters)");
    SetParameterDescription("bm.maxhoffset","Maximum altitude above the selected elevation source (in meters)");
    //MandatoryOff("bm.maxhoffset");
    SetDefaultParameterFloat("bm.maxhoffset",20.0);
    DisableParameter("bm.maxhoffset");

    AddParameter(ParameterType_Group,"postproc","Postprocessing parameters");
    SetParameterDescription("postproc","This group of parameters allow use optional filters.");

    AddParameter(ParameterType_Empty,"postproc.bij","Use bijection consistency in block matching strategy");
    SetParameterDescription("postproc.bij","use bijection consistency. Right to Left correlation is computed to validate Left to Right disparities. If bijection is not found pixel is rejected.");
    MandatoryOff("postproc.bij");
    EnableParameter("postproc.bij");

    AddParameter(ParameterType_Empty,"postproc.med","Use median disparities filtering");
    SetParameterDescription("postproc.med","disparities output can be filtered using median post filtering (disabled by default).");
    MandatoryOff("postproc.med");
    DisableParameter("postproc.med");


    AddParameter(ParameterType_Float,"postproc.metrict","Correlation metric threshold");
    SetParameterDescription("postproc.metrict","Use block matching metric output to discard pixels with low correlation value (disabled by default, float value)");
    MandatoryOff("postproc.metrict");
    SetDefaultParameterFloat("postproc.metrict",0.6);
    DisableParameter("postproc.metrict");

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
    SetParameterDescription("mask.variancet","This parameter allows one to discard pixels whose local variance is too small (the size of the neighborhood is given by the radius parameter)");
    MandatoryOff("mask.variancet");
    SetDefaultParameterFloat("mask.variancet",50.);
    //DisableParameter("mask.variancet");

    AddRAMParameter();


    SetDocExampleParameterValue("input.il","sensor_stereo_left.tif sensor_stereo_right.tif");
    SetDocExampleParameterValue("elev.default","200");
    SetDocExampleParameterValue("stereorect.fwdgridstep", "8");
    SetDocExampleParameterValue("stereorect.invgridssrate", "4");
    SetDocExampleParameterValue("postproc.med","1");
    SetDocExampleParameterValue("output.res","2.5");
    SetDocExampleParameterValue("output.out","dem.tif");


  }

  void DoUpdateParameters()
  {
    if( HasValue("input.il") )
      {
      // Get the input image list
      FloatVectorImageListType::Pointer inList = this->GetParameterImageList("input.il");

      // Set channel interval
      SetMaximumParameterIntValue("input.channel", inList->GetNthElement(0)->GetNumberOfComponentsPerPixel());
      }
  }


  template<class TInputImage, class TMetricFunctor>
    void
    SetBlockMatchingParameters(otb::PixelWiseBlockMatchingImageFilter<TInputImage,TInputImage,TInputImage,TInputImage,
                             TMetricFunctor>*blockMatcherFilter, otb::PixelWiseBlockMatchingImageFilter<TInputImage,TInputImage,TInputImage,TInputImage,
                             TMetricFunctor>*invBlockMatcherFilter,otb::SubPixelDisparityImageFilter<TInputImage,TInputImage,TInputImage,TInputImage,
                             TMetricFunctor>*subPixelFilter, TInputImage * leftImage, TInputImage * rightImage,TInputImage * leftMask,TInputImage * rightMask,TInputImage * finalMask, const bool minimize,double minDisp,double maxDisp)
    {
    typedef TMetricFunctor MetricFunctorType;
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

    if (IsParameterEnabled("postproc.bij"))
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
    // Setup the DSM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");
    double underElev = this->GetParameterFloat("bm.minhoffset");
    double overElev = this->GetParameterFloat("bm.maxhoffset");

    m_Filters.clear();

    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("input.il");

    if (inList->Size() < 2)
      {
      itkExceptionMacro("at least two input images must be set...");
      }

    //create pipeline for each stereo couple


    std::vector<std::vector<int> > couples = this->CreateCouplesList(GetParameterString("input.co"));

    unsigned int stereoCouples = couples.size();
    m_ExtractorList.resize(inList->Size());
    //TODO JGT check the mutli stereo fusion step
    //     N MultiDisp which gives N 3D Map Then fuse N 3D Map to DSM
    // or
    //     1 MultiDisp which fuse N Disp Couples to 1 3D Map and then convert one 3D Map into DSM
    m_MultiDisparityTo3DFilterList.resize(stereoCouples);

    for (unsigned int i = 0; i < inList->Size(); i++)
      {
      m_ExtractorList[i] = ExtractROIFilterType::New();
      m_ExtractorList[i]->SetInput(inList->GetNthElement(i));
      m_ExtractorList[i]->SetChannel(this->GetParameterInt("input.channel"));
      m_ExtractorList[i]->UpdateOutputInformation();

      }
    // Update the UTM zone params - Commented until bug fix on custom
    // map projections
     MapProjectionParametersHandler::InitializeUTMParameters(this, "input.il", "map");
    // Get the output projection Ref
    m_OutputProjectionRef = MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "map");

   // m_OutputProjectionRef = "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]]";

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
      otbAppLogINFO("Processing stereo couple "<<i<<" (images index "<<couple[0]<<" and "<<couple[1]<<")");
      FloatImageType::Pointer inleft = m_ExtractorList[couple[0]]->GetOutput();
      FloatImageType::Pointer inright = m_ExtractorList[couple[1]]->GetOutput();

      m_MultiDisparityTo3DFilterList[i] = MultiDisparityTo3DFilterType::New();
      DisplacementFieldSourceType::Pointer epipolarGridSource = DisplacementFieldSourceType::New();
      epipolarGridSource->SetLeftImage(inleft);
      epipolarGridSource->SetRightImage(inright);
      epipolarGridSource->SetGridStep(this->GetParameterInt("stereorect.fwdgridstep"));
      epipolarGridSource->SetScale(1.0);

      if (otb::Wrapper::ElevationParametersHandler::IsDEMUsed(this, "elev")
          && otb::Wrapper::ElevationParametersHandler::IsGeoidUsed(this, "elev"))
        {
        epipolarGridSource->SetUseDEM(true);
        }

      epipolarGridSource->UpdateOutputInformation();
      // check that displacement grids fit in 1/4 of available RAM
      FloatVectorImageType::SizeType
        gridSize = epipolarGridSource->GetLeftDisplacementFieldOutput()->GetLargestPossibleRegion().GetSize();
      double storageSize = static_cast<double> (gridSize[0]) * static_cast<double> (gridSize[1]) * 4.0 * 8.0
          / 1000000.0;
      globalEpiStorageSize += storageSize;
      if (globalEpiStorageSize > (static_cast<double> (this->GetParameterInt("ram"))))
        {
        otbAppLogINFO(<<"Grid step for couple  "<<couple[0]<<" and "<<couple[1]<<" (memory usage: "<<storageSize<<" Mo)");
        otbAppLogWARNING(<<"Grid step value"<<this->GetParameterInt("stereorect.fwdgridstep")<<" seems too be to high.");
        }

      AddProcess(epipolarGridSource, "Computing epipolar grids...");
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
      DisplacementFieldCastFilterType::Pointer leftGridCaster = DisplacementFieldCastFilterType::New();
      leftGridCaster->SetInput(epipolarGridSource->GetLeftDisplacementFieldOutput());
      leftGridCaster->Update();

      DisplacementFieldType::Pointer leftDisplacement;
      leftDisplacement = leftGridCaster->GetOutput();
      leftDisplacement->DisconnectPipeline();
      m_Filters.push_back(leftDisplacement.GetPointer());

      InverseDisplacementFieldFilterType::Pointer
          leftInverseDisplacementFieldFilter = InverseDisplacementFieldFilterType::New();
      leftInverseDisplacementFieldFilter->SetInput(leftDisplacement);

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
      leftInverseDisplacementFieldFilter->SetOutputOrigin(lorigin);
      leftInverseDisplacementFieldFilter->SetOutputSpacing(lspacing);
      leftInverseDisplacementFieldFilter->SetSize(lsize);
      // change value
      leftInverseDisplacementFieldFilter->SetSubsamplingFactor(this->GetParameterInt("stereorect.invgridssrate"));
      AddProcess(leftInverseDisplacementFieldFilter, "Inverting left displacement field ...");
      leftInverseDisplacementFieldFilter->Update();
      DisplacementFieldType::Pointer leftInverseDisplacement;
      leftInverseDisplacement = leftInverseDisplacementFieldFilter->GetOutput();
      leftInverseDisplacement->DisconnectPipeline();
      m_Filters.push_back(leftInverseDisplacement.GetPointer());

      ResampleFilterType::Pointer leftResampleFilter = ResampleFilterType::New();
      leftResampleFilter->SetInput(inleft);
      leftResampleFilter->SetDisplacementField(leftDisplacement);
      leftResampleFilter->SetInterpolator(m_Interpolator);
      leftResampleFilter->SetOutputSize(epiSize);
      leftResampleFilter->SetOutputSpacing(epiSpacing);
      leftResampleFilter->SetOutputOrigin(epiOrigin);
      leftResampleFilter->SetEdgePaddingValue(defaultValue);
      m_Filters.push_back(leftResampleFilter.GetPointer());

      DisplacementFieldCastFilterType::Pointer rightGridCaster = DisplacementFieldCastFilterType::New();
      rightGridCaster->SetInput(epipolarGridSource->GetRightDisplacementFieldOutput());
      rightGridCaster->Update();

      DisplacementFieldType::Pointer rightDisplacement;
      rightDisplacement = rightGridCaster->GetOutput();
      rightDisplacement->DisconnectPipeline();
      m_Filters.push_back(rightDisplacement.GetPointer());

      ResampleFilterType::Pointer rightResampleFilter = ResampleFilterType::New();
      rightResampleFilter->SetInput(inright);
      rightResampleFilter->SetDisplacementField(rightDisplacement);
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
      std::ostringstream leftCondition;
      leftCondition << "(inleft > 0)";

      ResampleFilterType::Pointer leftMaskResampleFilter = ResampleFilterType::New();

      if (IsParameterEnabled("mask.left") && HasValue("mask.left"))
        {
        leftmask = this->GetParameterFloatImage("mask.left");

        leftMaskResampleFilter->SetInput(leftmask);
        leftMaskResampleFilter->SetDisplacementField(leftDisplacement);
        leftMaskResampleFilter->SetInterpolator(m_Interpolator);
        leftMaskResampleFilter->SetOutputSize(epiSize);
        leftMaskResampleFilter->SetOutputSpacing(epiSpacing);
        leftMaskResampleFilter->SetOutputOrigin(epiOrigin);
        leftMaskResampleFilter->SetEdgePaddingValue(defaultValue);

        lBandMathFilter->SetNthInput(inputIdLeft, leftMaskResampleFilter->GetOutput(), "maskleft");
        ++inputIdLeft;
        leftCondition << " and (maskleft > 0)";
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
        leftCondition << " and variance > " << GetParameterFloat("mask.variancet");
        m_Filters.push_back(leftVarianceFilter.GetPointer());
        }

      const std::string state = "255";
      const std::string elseState = "0";

      std::ostringstream leftFormula;

      #ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
      leftFormula << leftCondition.str() << " ? " << state << " : " << elseState;
      #else
      leftFormula << "if(" << leftCondition.str() << "," << state << "," << elseState << ")";
      #endif

      lBandMathFilter->SetExpression(leftFormula.str());

      m_Filters.push_back(leftMaskResampleFilter.GetPointer());

      rBandMathFilter->SetNthInput(inputIdRight, rightResampleFilter->GetOutput(), "inright");
      ++inputIdRight;
      std::ostringstream rightCondition;
      rightCondition << "(inright > 0)";
      ResampleFilterType::Pointer rightMaskResampleFilter = ResampleFilterType::New();

      if (IsParameterEnabled("mask.right") && HasValue("mask.right"))
        {
        rightmask = this->GetParameterFloatImage("mask.right");

        rightMaskResampleFilter->SetInput(rightmask);
        rightMaskResampleFilter->SetDisplacementField(rightDisplacement);
        rightMaskResampleFilter->SetInterpolator(m_Interpolator);
        rightMaskResampleFilter->SetOutputSize(epiSize);
        rightMaskResampleFilter->SetOutputSpacing(epiSpacing);
        rightMaskResampleFilter->SetOutputOrigin(epiOrigin);
        rightMaskResampleFilter->SetEdgePaddingValue(defaultValue);

        rBandMathFilter->SetNthInput(inputIdRight, rightMaskResampleFilter->GetOutput(), "maskright");
        ++inputIdRight;
        rightCondition << " and (maskright > 0)";
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
        rightCondition << " and variance > " << GetParameterFloat("mask.variancet");
        m_Filters.push_back(rightVarianceFilter.GetPointer());
        }

      std::ostringstream rightFormula;

      #ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
      rightFormula << rightCondition.str() << " ? " << state << " : " << elseState;
      #else
      rightFormula << "if(" << rightCondition.str() << "," << state << "," << elseState << ")";
      #endif

      rBandMathFilter->SetExpression(rightFormula.str());

      m_Filters.push_back(rightMaskResampleFilter.GetPointer());
      m_Filters.push_back(rBandMathFilter.GetPointer());
      m_Filters.push_back(lBandMathFilter.GetPointer());

      BandMathFilterType::Pointer finalMaskFilter;
      if (IsParameterEnabled("postproc.bij"))
        {
        finalMaskFilter = BandMathFilterType::New();
        finalMaskFilter->SetNthInput(0, lBandMathFilter->GetOutput(), "inmask");
        }
      else
        {
        finalMaskFilter = lBandMathFilter;
        }

      // Compute disparities
      FilterType* blockMatcherFilterPointer = NULL;
      FilterType* invBlockMatcherFilterPointer = NULL;
      FilterType* subPixelFilterPointer = NULL;
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
        case 0: //SSDDivMean
          otbAppLogINFO(<<"Using robust SSD Metric for BlockMatching.");

          SSDDivMeanBlockMatcherFilter = SSDDivMeanBlockMatchingFilterType::New();
          blockMatcherFilterPointer = SSDDivMeanBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("postproc.bij"))
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

          case 1: //SSD
          otbAppLogINFO(<<"Using SSD Metric for BlockMatching.");

          SSDBlockMatcherFilter = SSDBlockMatchingFilterType::New();
          blockMatcherFilterPointer = SSDBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("postproc.bij"))
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
        case 2: //NCC
          otbAppLogINFO(<<"Using NCC Metric for BlockMatching.");

          NCCBlockMatcherFilter = NCCBlockMatchingFilterType::New();
          blockMatcherFilterPointer = NCCBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("postproc.bij"))
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


        case 3: //LP
          otbAppLogINFO(<<"Using Lp Metric for BlockMatching.");

          LPBlockMatcherFilter = LPBlockMatchingFilterType::New();
          LPBlockMatcherFilter->GetFunctor().SetP(static_cast<double> (GetParameterFloat("bm.metric.lp.p")));

          blockMatcherFilterPointer = LPBlockMatcherFilter.GetPointer();
          m_Filters.push_back(blockMatcherFilterPointer);

          if (IsParameterEnabled("postproc.bij"))
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

       if (IsParameterEnabled("postproc.bij"))
        {
        otbAppLogINFO(<<"Using reverse block-matching to filter incoherent disparity values.");
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
        //finalMaskFilter->SetNthInput(0, lBandMathFilter->GetOutput(), "inmask");
        finalMaskFilter->SetNthInput(1, bijectFilter->GetOutput(), "lrrl");

        #ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
        finalMaskFilter->SetExpression("(inmask > 0 and lrrl > 0) ? 255 : 0");
        #else
        finalMaskFilter->SetExpression("if(inmask > 0 and lrrl > 0, 255, 0)");
        #endif
        //
        m_Filters.push_back(finalMaskFilter.GetPointer());
        }


     FloatImageType::Pointer hDispOutput = subPixelFilterPointer->GetOutput(0);
      FloatImageType::Pointer finalMaskImage=finalMaskFilter->GetOutput();
      if (IsParameterEnabled("postproc.med"))
        {
        MedianFilterType::Pointer hMedianFilter = MedianFilterType::New();
        hMedianFilter->SetInput(subPixelFilterPointer->GetOutput(0));
        hMedianFilter->SetRadius(2);
        hMedianFilter->SetIncoherenceThreshold(2.0);
        hMedianFilter->SetMaskInput(finalMaskFilter->GetOutput());
        hMedianFilter->UpdateOutputInformation();
        hDispOutput = hMedianFilter->GetOutput();
        finalMaskImage = hMedianFilter->GetOutputMask();
        m_Filters.push_back(hMedianFilter.GetPointer());
        }

      DisparityTranslateFilter::Pointer disparityTranslateFilter = DisparityTranslateFilter::New();
      disparityTranslateFilter->SetHorizontalDisparityMapInput(hDispOutput);
      disparityTranslateFilter->SetVerticalDisparityMapInput(subPixelFilterPointer->GetOutput(1));
      disparityTranslateFilter->SetInverseEpipolarLeftGrid(leftInverseDisplacement);
      disparityTranslateFilter->SetDirectEpipolarRightGrid(rightDisplacement);
      // disparityTranslateFilter->SetDisparityMaskInput()
      disparityTranslateFilter->SetLeftSensorImageInput(inleft);
      disparityTranslateFilter->SetNoDataValue(-32768);
      disparityTranslateFilter->UpdateOutputInformation();

      m_Filters.push_back(disparityTranslateFilter.GetPointer());

      NoDataMaskFilterType::Pointer dispTranslateMaskFilter = NoDataMaskFilterType::New();
      dispTranslateMaskFilter->SetInput(disparityTranslateFilter->GetHorizontalDisparityMapOutput());
      dispTranslateMaskFilter->SetInsideValue(1);
      dispTranslateMaskFilter->SetOutsideValue(0);
      m_Filters.push_back(dispTranslateMaskFilter.GetPointer());

      FloatImageType::Pointer hDispOutput2 = disparityTranslateFilter->GetHorizontalDisparityMapOutput();
      FloatImageType::Pointer vDispOutput2 = disparityTranslateFilter->GetVerticalDisparityMapOutput();
      FloatImageType::Pointer translatedMaskImage =  dispTranslateMaskFilter->GetOutput();
      if (IsParameterEnabled("postproc.med"))
        {
        MedianFilterType::Pointer hMedianFilter2 = MedianFilterType::New();
        MedianFilterType::Pointer vMedianFilter2 = MedianFilterType::New();

        //TODO JGT Check if medianfiltering is necessary after disparitytranslate
        hMedianFilter2->SetInput(disparityTranslateFilter->GetHorizontalDisparityMapOutput());
        hMedianFilter2->SetRadius(2);
        hMedianFilter2->SetIncoherenceThreshold(2.0);
        hMedianFilter2->SetMaskInput(dispTranslateMaskFilter->GetOutput());
        hMedianFilter2->UpdateOutputInformation();
        hDispOutput2 = hMedianFilter2->GetOutput();
        translatedMaskImage = hMedianFilter2->GetOutputMask();
        m_Filters.push_back(hMedianFilter2.GetPointer());

        vMedianFilter2->SetInput(disparityTranslateFilter->GetVerticalDisparityMapOutput());
        vMedianFilter2->SetRadius(2);
        vMedianFilter2->SetIncoherenceThreshold(2.0);
        vMedianFilter2->SetMaskInput(dispTranslateMaskFilter->GetOutput());
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
      m_MultiDisparityTo3DFilterList[i]->SetDisparityMaskInput(0, translatedMaskImage);
      m_MultiDisparityTo3DFilterList[i]->UpdateOutputInformation();

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
      dispMaskFilter->SetNthInput(1, finalMaskImage, "mask");
      std::ostringstream maskCondition;
      maskCondition << "(hdisp > " << minDisp << ") and (hdisp < " << maxDisp << ") and (mask>0)";
      if (IsParameterEnabled("postproc.metrict"))
        {
        dispMaskFilter->SetNthInput(2, subPixelFilterPointer->GetOutput(2), "metric");
        maskCondition << " and (metric ";
        if (minimize == true)
          {
          maskCondition << " < " << this->GetParameterFloat("postproc.metrict");
          }
        else
          {
          maskCondition << " > " << this->GetParameterFloat("postproc.metrict");
          }
        maskCondition << ")";
        }

      std::ostringstream maskFormula;

      #ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
      maskFormula << maskCondition.str() << " ? " << state << " : " << elseState;
      #else
      maskFormula << "if(" << maskCondition.str() << "," << state << "," << elseState << ")";
      #endif

      otbAppLogINFO(<<"Disparity mask formula : "<<maskFormula.str());
      dispMaskFilter->SetExpression(maskFormula.str());
      m_Filters.push_back(dispMaskFilter.GetPointer());

      disparityTranslateFilter->SetDisparityMaskInput(dispMaskFilter->GetOutput());

      m_Multi3DMapToDEMFilter->Set3DMapInput(i, m_MultiDisparityTo3DFilterList[i]->GetOutput());

      }

    if(GetParameterInt("output.mode") == 0)
      {
      otbAppLogINFO(<<"Output DSM parameters are estimated from sensor input data.");

      m_Multi3DMapToDEMFilter->SetOutputParametersFrom3DMap();
      }
    else
      {
      otbAppLogINFO(<<"DSM parameters are defined by the user.");
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
    otbAppLogINFO(<<"Output DSM projection reference: "<<std::endl<<m_OutputProjectionRef);
    m_Multi3DMapToDEMFilter->SetProjectionRef(m_OutputProjectionRef);

    m_Multi3DMapToDEMFilter->SetDEMGridStep(this->GetParameterFloat("output.res"));

    if(GetParameterString("output.fusionmethod") == "min")
      {
      m_Multi3DMapToDEMFilter ->SetCellFusionMode(otb::CellFusionMode::MIN);
      }
    else if(GetParameterString("output.fusionmethod") == "mean")
      {
      m_Multi3DMapToDEMFilter ->SetCellFusionMode(otb::CellFusionMode::MEAN);
      }
    else if(GetParameterString("output.fusionmethod") == "acc")
      {
      m_Multi3DMapToDEMFilter ->SetCellFusionMode(otb::CellFusionMode::ACC);
      }
    else
      {
      m_Multi3DMapToDEMFilter ->SetCellFusionMode(otb::CellFusionMode::MAX);
      }

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
