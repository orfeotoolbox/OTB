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

#include "otbVarianceImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "otbSubPixelDisparityImageFilter.h"
#include "otbDisparityMapMedianFilter.h"

namespace otb
{
namespace Wrapper
{

class BlockMatching : public Application
{
public:
  /** Standard class typedefs. */
  typedef BlockMatching                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::Functor::SSDBlockMatching<FloatImageType,FloatImageType> SSDBlockMatchingFunctorType;
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
                                                           NCCBlockMatchingFunctorType> NCCBlockMatchingFilterType;

  typedef otb::PixelWiseBlockMatchingImageFilter<FloatImageType,
                                                           FloatImageType,
                                                           FloatImageType,
                                                           FloatImageType,
                                                           LPBlockMatchingFunctorType> LPBlockMatchingFilterType;

  typedef otb::VarianceImageFilter<FloatImageType,FloatImageType> VarianceFilterType;


  typedef otb::BandMathImageFilter<FloatImageType>         BandMathFilterType;

  typedef otb::ImageList<FloatImageType>                   ImageListType;

  typedef otb::ImageListToVectorImageFilter<ImageListType,
                                            FloatVectorImageType> ImageListToVectorImageFilterType;

  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                            FloatImageType,
                                            FloatImageType,
                                            FloatImageType,
                                            SSDBlockMatchingFunctorType> SSDSubPixelDisparityFilterType;

  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                            FloatImageType,
                                            FloatImageType,
                                            FloatImageType,
                                            NCCBlockMatchingFunctorType> NCCSubPixelDisparityFilterType;

  typedef otb::SubPixelDisparityImageFilter<FloatImageType,
                                            FloatImageType,
                                            FloatImageType,
                                            FloatImageType,
                                            LPBlockMatchingFunctorType> LPSubPixelDisparityFilterType;

  typedef otb::DisparityMapMedianFilter<FloatImageType,
                                        FloatImageType,
                                        FloatImageType>   MedianFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BlockMatching, otb::Application);

  /** Filters typedef */

private:
  BlockMatching()
  {
    // Initialize filters
    m_SSDBlockMatcher = SSDBlockMatchingFilterType::New();
    m_NCCBlockMatcher = NCCBlockMatchingFilterType::New();
    m_LPBlockMatcher  = LPBlockMatchingFilterType::New();
    m_SSDSubPixFilter = SSDSubPixelDisparityFilterType::New();
    m_NCCSubPixFilter = NCCSubPixelDisparityFilterType::New();
    m_LPSubPixFilter  = LPSubPixelDisparityFilterType::New();
    m_LVarianceFilter = VarianceFilterType::New();
    m_RVarianceFilter = VarianceFilterType::New();
    m_LBandMathFilter = BandMathFilterType::New();
    m_RBandMathFilter = BandMathFilterType::New();
    m_OutputImageList = ImageListType::New();
    m_ImageListFilter = ImageListToVectorImageFilterType::New();
    m_HMedianFilter   = MedianFilterType::New();
    m_VMedianFilter   = MedianFilterType::New();
  }

  void DoInit()
  {
    SetName("BlockMatching");
    SetDescription("Performs block-matching to estimate pixel-wise disparities between two images");

    SetDocName("Pixel-wise Block-Matching");
    SetDocLongDescription("This application allows one to performs block-matching to estimate pixel-wise disparities "
      "between two images. One must chose block-matching method and input"
      " masks (related to the left and right input image) of pixels for which the disparity should be investigated. "
      "Additionally, two criteria can be optionally used to disable disparity investigation for some pixel: a "
      "no-data value, and a threshold on the local variance. This allows one to speed-up computation by avoiding to "
      "investigate disparities that will not be reliable anyway. For efficiency reasons, if the optimal metric values"
      " image is desired, it will be concatenated to the output image (which will then have three bands : horizontal "
      "disparity, vertical disparity and metric value). One can split these images afterward.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbStereoRectificationGridGenerator");

    AddDocTag(Tags::Stereo);

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows setting the input and output images.");

    AddParameter(ParameterType_InputImage,"io.inleft","Left input image");
    SetParameterDescription("io.inleft","The left input image (reference)");

    AddParameter(ParameterType_InputImage,"io.inright","Right input image");
    SetParameterDescription("io.inright","The right input (secondary)");

    AddParameter(ParameterType_OutputImage, "io.out", "The output disparity map");
    SetParameterDescription("io.out","An image containing the estimated disparities as well as the metric values if the option is used");

    AddParameter(ParameterType_OutputImage, "io.outmask", "The output mask corresponding to all criterions");
    SetParameterDescription("io.outmask","A mask image corresponding to all citerions (see masking parameters). Only required if variance threshold or nodata criterions are set.");
    SetDefaultOutputPixelType("io.outmask",ImagePixelType_uint8);
    DisableParameter("io.outmask");
    MandatoryOff("io.outmask");

    AddParameter(ParameterType_Empty,"io.outmetric","Output optimal metric values as well");
    SetParameterDescription("io.outmetric","If used, the output image will have a second component with metric optimal values");

    AddParameter(ParameterType_Group,"mask","Image masking parameters");
    SetParameterDescription("mask","This group of parameters allows determining the masking parameters to prevent disparities estimation for some pixels of the left image");

    AddParameter(ParameterType_InputImage,"mask.inleft","Discard left pixels from mask image");
    SetParameterDescription("mask.inleft","This parameter allows providing a custom mask for the left image.Block matching will be only perform on pixels inside the mask.");
    MandatoryOff("mask.inleft");

    AddParameter(ParameterType_InputImage,"mask.inright","Discard right pixels from mask image");
    SetParameterDescription("mask.inright","This parameter allows providing a custom mask for the right image.Block matching will be perform only on pixels inside the mask.");
    MandatoryOff("mask.inright");

    AddParameter(ParameterType_Float,"mask.nodata","Discard pixels with no-data value");
    SetParameterDescription("mask.nodata","This parameter allows discarding pixels whose value is equal to the user-defined no-data value.");
    MandatoryOff("mask.nodata");
    SetDefaultParameterFloat("mask.nodata",0.);
    DisableParameter("mask.nodata");

    AddParameter(ParameterType_Float,"mask.variancet","Discard pixels with low local variance");
    SetParameterDescription("mask.variancet","This parameter allows discarding pixels whose local variance is too small (the size of the neighborhood is given by the radius parameter)");
    MandatoryOff("mask.variancet");
    SetDefaultParameterFloat("mask.variancet",100.);
    DisableParameter("mask.variancet");

    AddParameter(ParameterType_Group,"bm","Block matching parameters");
    SetParameterDescription("bm","This group of parameters allow tuning the block-matching behaviour");

    AddParameter(ParameterType_Choice,   "bm.metric", "Block-matching metric");
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

    AddParameter(ParameterType_Int,"bm.radius","Radius of blocks");
    SetParameterDescription("bm.radius","The radius (in pixels) of blocks in Block-Matching");
    SetDefaultParameterInt("bm.radius",3);
    SetMinimumParameterIntValue("bm.radius",1);

    AddParameter(ParameterType_Int,"bm.minhd","Minimum horizontal disparity");
    SetParameterDescription("bm.minhd","Minimum horizontal disparity to explore (can be negative)");

    AddParameter(ParameterType_Int,"bm.maxhd","Maximum horizontal disparity");
    SetParameterDescription("bm.maxhd","Maximum horizontal disparity to explore (can be negative)");

    AddParameter(ParameterType_Int,"bm.minvd","Minimum vertical disparity");
    SetParameterDescription("bm.minvd","Minimum vertical disparity to explore (can be negative)");

    AddParameter(ParameterType_Int,"bm.maxvd","Maximum vertical disparity");
    SetParameterDescription("bm.maxvd","Maximum vertical disparity to explore (can be negative)");

    AddParameter(ParameterType_Choice,"bm.subpixel","Sub-pixel interpolation");
    SetParameterDescription("bm.subpixel", "Estimate disparities with sub-pixel precision");

    AddChoice("bm.subpixel.none", "None");
    SetParameterDescription("bm.subpixel.none", "No sub-pixel ");

    AddChoice("bm.subpixel.parabolic", "Parabolic");
    SetParameterDescription("bm.subpixel.parabolic", "Parabolic fit");

    AddChoice("bm.subpixel.triangular", "Triangular");
    SetParameterDescription("bm.subpixel.triangular", "Triangular fit");

    AddChoice("bm.subpixel.dichotomy", "Dichotomy");
    SetParameterDescription("bm.subpixel.dichotomy", "Dichotomic search");

    AddParameter(ParameterType_Int,"bm.step", "Computation step");
    SetParameterDescription("bm.step", "Location step between computed disparities");
    SetDefaultParameterInt("bm.step",1);
    SetMinimumParameterIntValue("bm.step",1);
    MandatoryOff("bm.step");

    AddParameter(ParameterType_Int,"bm.startx","X start index");
    SetParameterDescription("bm.startx","X start index of the subsampled grid (wrt the input image grid)");
    SetDefaultParameterInt("bm.startx",0);
    MandatoryOff("bm.startx");

    AddParameter(ParameterType_Int,"bm.starty","Y start index");
    SetParameterDescription("bm.starty","Y start index of the subsampled grid (wrt the input image grid)");
    SetDefaultParameterInt("bm.starty",0);
    MandatoryOff("bm.starty");

    AddParameter(ParameterType_Group,"bm.medianfilter","Median filtering");
    SetParameterDescription("bm.medianfilter","Use a median filter to get a smooth disparity map");

    AddParameter(ParameterType_Int,"bm.medianfilter.radius", "Radius");
    SetParameterDescription("bm.medianfilter.radius", "Radius for median filter");
    MandatoryOff("bm.medianfilter.radius");
    DisableParameter("bm.medianfilter.radius");

    AddParameter(ParameterType_Float,"bm.medianfilter.incoherence", "Incoherence threshold");
    SetParameterDescription("bm.medianfilter.incoherence", "Incoherence threshold between original and filtered disparity");
    MandatoryOff("bm.medianfilter.incoherence");
    DisableParameter("bm.medianfilter.incoherence");

    AddParameter(ParameterType_Choice, "bm.initdisp", "Initial disparities");
    AddChoice("bm.initdisp.none", "None");
    SetParameterDescription("bm.initdisp.none", "No initial disparity used");

    AddChoice("bm.initdisp.uniform", "Uniform initial disparity");
    SetParameterDescription("bm.initdisp.uniform", "Use an uniform initial disparity estimate");

    AddParameter(ParameterType_Int, "bm.initdisp.uniform.hdisp", "Horizontal initial disparity");
    SetParameterDescription("bm.initdisp.uniform.hdisp", "Value of the uniform horizontal disparity initial estimate (in pixels)");
    SetDefaultParameterInt("bm.initdisp.uniform.hdisp", 0);

    AddParameter(ParameterType_Int, "bm.initdisp.uniform.vdisp", "Vertical initial disparity");
    SetParameterDescription("bm.initdisp.uniform.vdisp", "Value of the uniform vertical disparity initial estimate (in pixels)");
    SetDefaultParameterInt("bm.initdisp.uniform.vdisp", 0);

    AddParameter(ParameterType_Int, "bm.initdisp.uniform.hrad", "Horizontal exploration radius");
    SetParameterDescription("bm.initdisp.uniform.hrad", "Horizontal exploration radius around the initial disparity estimate (in pixels)");
    SetDefaultParameterInt("bm.initdisp.uniform.hrad", 0);
    DisableParameter("bm.initdisp.uniform.hrad");

    AddParameter(ParameterType_Int, "bm.initdisp.uniform.vrad", "Vertical exploration radius");
    SetParameterDescription("bm.initdisp.uniform.vrad", "Vertical exploration radius around the initial disparity estimate (in pixels)");
    SetDefaultParameterInt("bm.initdisp.uniform.vrad", 0);
    DisableParameter("bm.initdisp.uniform.vrad");

    AddChoice("bm.initdisp.maps", "Initial disparity maps");
    SetParameterDescription("bm.initdisp.maps", "Use initial disparity maps");

    AddParameter(ParameterType_InputImage, "bm.initdisp.maps.hmap", "Horizontal initial disparity map");
    SetParameterDescription("bm.initdisp.maps.hmap", "Map of the initial horizontal disparities");

    AddParameter(ParameterType_InputImage, "bm.initdisp.maps.vmap", "Vertical initial disparity map");
    SetParameterDescription("bm.initdisp.maps.vmap", "Map of the initial vertical disparities");

    AddParameter(ParameterType_Int, "bm.initdisp.maps.hrad", "Horizontal exploration radius");
    SetParameterDescription("bm.initdisp.maps.hrad", "Horizontal exploration radius around the initial disparity estimate (in pixels)");
    SetDefaultParameterInt("bm.initdisp.maps.hrad", 0);
    DisableParameter("bm.initdisp.maps.hrad");

    AddParameter(ParameterType_Int, "bm.initdisp.maps.vrad", "Vertical exploration radius");
    SetParameterDescription("bm.initdisp.maps.vrad", "Vertical exploration radius around the initial disparity estimate (in pixels)");
    SetDefaultParameterInt("bm.initdisp.maps.vrad", 0);
    DisableParameter("bm.initdisp.maps.vrad");

    //this->DebugOn();

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.inleft","StereoFixed.png");
    SetDocExampleParameterValue("io.inright","StereoMoving.png");
    SetDocExampleParameterValue("bm.minhd","-10");
    SetDocExampleParameterValue("bm.maxhd","10");
    SetDocExampleParameterValue("mask.variancet","10");
    SetDocExampleParameterValue("io.out","MyDisparity.tif");
  }

  void DoUpdateParameters()
  {
    if(IsParameterEnabled("mask.variancet") || IsParameterEnabled("mask.nodata"))
      {
      EnableParameter("io.outmask");
      }
    else
      {
      DisableParameter("io.outmask");
      }
    // enforce positive radii
    if (GetParameterInt("bm.radius") < 1)
      {
      SetParameterInt("bm.radius",1);
      }
    if(GetParameterInt("bm.initdisp.uniform.hrad") < 0)
      {
      SetParameterInt("bm.initdisp.uniform.hrad",0);
      }
    if(GetParameterInt("bm.initdisp.uniform.vrad") < 0)
      {
      SetParameterInt("bm.initdisp.uniform.vrad",0);
      }
    if(GetParameterInt("bm.initdisp.maps.hrad") < 0)
      {
      SetParameterInt("bm.initdisp.maps.hrad",0);
      }
    if(GetParameterInt("bm.initdisp.maps.vrad") < 0)
      {
      SetParameterInt("bm.initdisp.maps.vrad",0);
      }
  }

  void DoExecute()
  {
    FloatImageType::Pointer leftImage = GetParameterFloatImage("io.inleft");
    FloatImageType::Pointer rightImage = GetParameterFloatImage("io.inright");
    FloatImageType::Pointer leftmask;
    FloatImageType::Pointer rightmask;

    unsigned int radius  = GetParameterInt("bm.radius");
    int          minhdisp = GetParameterInt("bm.minhd");
    int          maxhdisp = GetParameterInt("bm.maxhd");
    int          minvdisp = GetParameterInt("bm.minvd");
    int          maxvdisp = GetParameterInt("bm.maxvd");
    unsigned int step     = GetParameterInt("bm.step");

    FloatImageType::IndexType gridIndex;
    gridIndex[0] = GetParameterInt("bm.startx");
    gridIndex[1] = GetParameterInt("bm.starty");

    std::ostringstream leftBandMathCondition;
    std::ostringstream rightBandMathCondition;

    unsigned int inputIdLeft = 0;
    unsigned int inputIdRight = 0;
    bool maskingLeft = false;
    bool maskingRight = false;
    bool useInitialDispUniform = false;
    bool useInitialDispMap = false;

    // Handle input mask if present
    if(IsParameterEnabled("mask.inleft"))
      {
      leftmask = GetParameterFloatImage("mask.inleft");
      m_LBandMathFilter->SetNthInput(inputIdLeft,leftmask,"inmask");
      maskingLeft = true;
      ++inputIdLeft;
      leftBandMathCondition<<"inmask > 0";
      }
    if(IsParameterEnabled("mask.inright"))
      {
      rightmask = GetParameterFloatImage("mask.inright");
      m_RBandMathFilter->SetNthInput(inputIdRight,rightmask,"inmask");
      maskingRight = true;
      ++inputIdRight;
      rightBandMathCondition<<"inmask > 0";
      }

    // Handle variance threshold if present
    if(IsParameterEnabled("mask.variancet"))
      {
      if(maskingLeft)
        {
        leftBandMathCondition<<" and ";
        }
      if(maskingRight)
        {
        rightBandMathCondition<<" and ";
        }
      // Left side
      m_LVarianceFilter->SetInput(leftImage);
      VarianceFilterType::InputSizeType vradius;
      vradius.Fill(radius);
      m_LVarianceFilter->SetRadius(vradius);

      m_LBandMathFilter->SetNthInput(inputIdLeft,m_LVarianceFilter->GetOutput(),"variance");
      leftBandMathCondition<<"variance > "<<GetParameterFloat("mask.variancet");
      ++inputIdLeft;

      // Right side
      m_RVarianceFilter->SetInput(rightImage);
      m_RVarianceFilter->SetRadius(vradius);

      m_RBandMathFilter->SetNthInput(inputIdRight,m_RVarianceFilter->GetOutput(),"variance");
      rightBandMathCondition<<"variance > "<<GetParameterFloat("mask.variancet");
      ++inputIdRight;

      maskingLeft = true;
      maskingRight = true;
      }

    // Handle nodata field if present
    if(IsParameterEnabled("mask.nodata"))
      {
      if(maskingLeft)
        {
        leftBandMathCondition<<" and ";
        }
      if(maskingRight)
        {
        rightBandMathCondition<<" and ";
        }
      // Left side
      m_LBandMathFilter->SetNthInput(inputIdLeft,leftImage,"leftimage");
      leftBandMathCondition<<"leftimage != "<<GetParameterFloat("mask.nodata");

      // Right side
      m_RBandMathFilter->SetNthInput(inputIdRight,rightImage,"rightimage");
      rightBandMathCondition<<"rightimage != "<<GetParameterFloat("mask.nodata");

      maskingLeft = true;
      maskingRight = true;
      }

    const std::string state = "255";
    const std::string elseState = "0";

    std::ostringstream leftBandMathExpression;
    std::ostringstream rightBandMathExpression;

    #ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
    leftBandMathExpression << leftBandMathCondition.str() << " ? " << state << " : " << elseState;
    rightBandMathExpression << rightBandMathCondition.str() << " ? " << state << " : " << elseState;
    #else
    leftBandMathExpression << "if(" << leftBandMathCondition.str() << "," << state << "," << elseState << ")";
    rightBandMathExpression << "if(" << rightBandMathCondition.str() << "," << state << "," << elseState << ")";
    #endif

    std::cout << leftBandMathCondition.str() << std::endl;
    if(maskingLeft)
      {
      GetLogger()->Info("Masking criterion on left image: " + leftBandMathExpression.str() + '\n');
      m_LBandMathFilter->SetExpression(leftBandMathExpression.str());
      }
    if(maskingRight)
      {
      GetLogger()->Info("Masking criterion on right image: " + rightBandMathExpression.str() + '\n');
      m_RBandMathFilter->SetExpression(rightBandMathExpression.str());
      }

    // Uniform initial disparity case
    if (GetParameterInt("bm.initdisp") == 1)
      {
      if (GetParameterInt("bm.initdisp.uniform.hrad") > 0 && GetParameterInt("bm.initdisp.uniform.vrad") > 0)
        {
        useInitialDispUniform = true;
        }
      }
    // Initial disparity map case
    if (GetParameterInt("bm.initdisp") == 2)
      {
      if (GetParameterInt("bm.initdisp.maps.hrad") > 0 && GetParameterInt("bm.initdisp.maps.vrad") > 0)
        {
        useInitialDispMap = true;
        }
      }

    FloatImageType * hdispImage;
    FloatImageType * vdispImage;
    FloatImageType * metricImage;
    FloatImageType * maskLeftImage;
    FloatImageType * maskRightImage;

    maskLeftImage = m_LBandMathFilter->GetOutput();
    maskRightImage = m_RBandMathFilter->GetOutput();

    // SSD case
    if(GetParameterInt("bm.metric") == 0)
      {
      m_SSDBlockMatcher->SetLeftInput(leftImage);
      m_SSDBlockMatcher->SetRightInput(rightImage);
      m_SSDBlockMatcher->SetRadius(radius);
      m_SSDBlockMatcher->SetStep(step);
      m_SSDBlockMatcher->SetGridIndex(gridIndex);
      m_SSDBlockMatcher->SetMinimumHorizontalDisparity(minhdisp);
      m_SSDBlockMatcher->SetMaximumHorizontalDisparity(maxhdisp);
      m_SSDBlockMatcher->SetMinimumVerticalDisparity(minvdisp);
      m_SSDBlockMatcher->SetMaximumVerticalDisparity(maxvdisp);

      AddProcess(m_SSDBlockMatcher,"SSD block matching");
      if(maskingLeft)
        {
        m_SSDBlockMatcher->SetLeftMaskInput(maskLeftImage);
        }
      if(maskingRight)
        {
        m_SSDBlockMatcher->SetRightMaskInput(maskRightImage);
        }
      if(useInitialDispUniform)
        {
        FloatImageType::SizeType expRadius;
        expRadius[0] = GetParameterInt("bm.initdisp.uniform.hrad");
        expRadius[1] = GetParameterInt("bm.initdisp.uniform.vrad");
        m_SSDBlockMatcher->SetExplorationRadius(expRadius);
        m_SSDBlockMatcher->SetInitHorizontalDisparity(GetParameterInt("bm.initdisp.uniform.hdisp"));
        m_SSDBlockMatcher->SetInitVerticalDisparity(GetParameterInt("bm.initdisp.uniform.vdisp"));
        }
      if(useInitialDispMap)
        {
        FloatImageType::SizeType expRadius;
        expRadius[0] = GetParameterInt("bm.initdisp.maps.hrad");
        expRadius[1] = GetParameterInt("bm.initdisp.maps.vrad");
        m_SSDBlockMatcher->SetExplorationRadius(expRadius);
        m_SSDBlockMatcher->SetHorizontalDisparityInput(GetParameterFloatImage("bm.initdisp.maps.hmap"));
        m_SSDBlockMatcher->SetVerticalDisparityInput(GetParameterFloatImage("bm.initdisp.maps.vmap"));
        }

      if (GetParameterInt("bm.subpixel") > 0)
        {
        m_SSDSubPixFilter->SetInputsFromBlockMatchingFilter(m_SSDBlockMatcher);
        AddProcess(m_SSDSubPixFilter,"Sub-pixel refinement");
        switch (GetParameterInt("bm.subpixel"))
          {
          case 1 : m_SSDSubPixFilter->SetRefineMethod(SSDSubPixelDisparityFilterType::PARABOLIC);
            break;
          case 2 : m_SSDSubPixFilter->SetRefineMethod(SSDSubPixelDisparityFilterType::TRIANGULAR);
            break;
          case 3 : m_SSDSubPixFilter->SetRefineMethod(SSDSubPixelDisparityFilterType::DICHOTOMY);
            break;
          default : break;
          }
        hdispImage = m_SSDSubPixFilter->GetHorizontalDisparityOutput();
        vdispImage = m_SSDSubPixFilter->GetVerticalDisparityOutput();
        metricImage = m_SSDSubPixFilter->GetMetricOutput();
        }
      else
        {
        hdispImage = m_SSDBlockMatcher->GetHorizontalDisparityOutput();
        vdispImage = m_SSDBlockMatcher->GetVerticalDisparityOutput();
        metricImage = m_SSDBlockMatcher->GetMetricOutput();
        }
      }
    // NCC case
    else if (GetParameterInt("bm.metric") == 1)
      {
      m_NCCBlockMatcher->SetLeftInput(leftImage);
      m_NCCBlockMatcher->SetRightInput(rightImage);
      m_NCCBlockMatcher->SetRadius(radius);
      m_NCCBlockMatcher->SetStep(step);
      m_NCCBlockMatcher->SetGridIndex(gridIndex);
      m_NCCBlockMatcher->SetMinimumHorizontalDisparity(minhdisp);
      m_NCCBlockMatcher->SetMaximumHorizontalDisparity(maxhdisp);
      m_NCCBlockMatcher->SetMinimumVerticalDisparity(minvdisp);
      m_NCCBlockMatcher->SetMaximumVerticalDisparity(maxvdisp);
      m_NCCBlockMatcher->MinimizeOff();

      AddProcess(m_NCCBlockMatcher,"NCC block matching");

      if(maskingLeft)
        {
        m_NCCBlockMatcher->SetLeftMaskInput(maskLeftImage);
        }
      if(maskingRight)
        {
        m_NCCBlockMatcher->SetRightMaskInput(maskRightImage);
        }
      if(useInitialDispUniform)
        {
        FloatImageType::SizeType expRadius;
        expRadius[0] = GetParameterInt("bm.initdisp.uniform.hrad");
        expRadius[1] = GetParameterInt("bm.initdisp.uniform.vrad");
        m_NCCBlockMatcher->SetExplorationRadius(expRadius);
        m_NCCBlockMatcher->SetInitHorizontalDisparity(GetParameterInt("bm.initdisp.uniform.hdisp"));
        m_NCCBlockMatcher->SetInitVerticalDisparity(GetParameterInt("bm.initdisp.uniform.vdisp"));
        }
      if(useInitialDispMap)
        {
        FloatImageType::SizeType expRadius;
        expRadius[0] = GetParameterInt("bm.initdisp.maps.hrad");
        expRadius[1] = GetParameterInt("bm.initdisp.maps.vrad");
        m_NCCBlockMatcher->SetExplorationRadius(expRadius);
        m_NCCBlockMatcher->SetHorizontalDisparityInput(GetParameterFloatImage("bm.initdisp.maps.hmap"));
        m_NCCBlockMatcher->SetVerticalDisparityInput(GetParameterFloatImage("bm.initdisp.maps.vmap"));
        }

      if (GetParameterInt("bm.subpixel") > 0)
        {
        m_NCCSubPixFilter->SetInputsFromBlockMatchingFilter(m_NCCBlockMatcher);
        AddProcess(m_NCCSubPixFilter,"Sub-pixel refinement");
        switch (GetParameterInt("bm.subpixel"))
          {
          case 1 : m_NCCSubPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::PARABOLIC);
            break;
          case 2 : m_NCCSubPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::TRIANGULAR);
            break;
          case 3 : m_NCCSubPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::DICHOTOMY);
            break;
          default : break;
          }
        hdispImage = m_NCCSubPixFilter->GetHorizontalDisparityOutput();
        vdispImage = m_NCCSubPixFilter->GetVerticalDisparityOutput();
        metricImage = m_NCCSubPixFilter->GetMetricOutput();
        }
      else
        {
        hdispImage = m_NCCBlockMatcher->GetHorizontalDisparityOutput();
        vdispImage = m_NCCBlockMatcher->GetVerticalDisparityOutput();
        metricImage = m_NCCBlockMatcher->GetMetricOutput();
        }
      }
    // Lp case
    else
      {
      m_LPBlockMatcher->SetLeftInput(leftImage);
      m_LPBlockMatcher->SetRightInput(rightImage);
      m_LPBlockMatcher->SetRadius(radius);
      m_LPBlockMatcher->SetStep(step);
      m_LPBlockMatcher->SetGridIndex(gridIndex);
      m_LPBlockMatcher->GetFunctor().SetP(static_cast<double>(GetParameterFloat("bm.metric.lp.p")));
      m_LPBlockMatcher->SetMinimumHorizontalDisparity(minhdisp);
      m_LPBlockMatcher->SetMaximumHorizontalDisparity(maxhdisp);
      m_LPBlockMatcher->SetMinimumVerticalDisparity(minvdisp);
      m_LPBlockMatcher->SetMaximumVerticalDisparity(maxvdisp);

      AddProcess(m_LPBlockMatcher,"Lp block matching");

      if(maskingLeft)
        {
        m_LPBlockMatcher->SetLeftMaskInput(maskLeftImage);
        }
      if(maskingRight)
        {
        m_LPBlockMatcher->SetRightMaskInput(maskRightImage);
        }
      if(useInitialDispUniform)
        {
        FloatImageType::SizeType expRadius;
        expRadius[0] = GetParameterInt("bm.initdisp.uniform.hrad");
        expRadius[1] = GetParameterInt("bm.initdisp.uniform.vrad");
        m_LPBlockMatcher->SetExplorationRadius(expRadius);
        m_LPBlockMatcher->SetInitHorizontalDisparity(GetParameterInt("bm.initdisp.uniform.hdisp"));
        m_LPBlockMatcher->SetInitVerticalDisparity(GetParameterInt("bm.initdisp.uniform.vdisp"));
        }
      if(useInitialDispMap)
        {
        FloatImageType::SizeType expRadius;
        expRadius[0] = GetParameterInt("bm.initdisp.maps.hrad");
        expRadius[1] = GetParameterInt("bm.initdisp.maps.vrad");
        m_LPBlockMatcher->SetExplorationRadius(expRadius);
        m_LPBlockMatcher->SetHorizontalDisparityInput(GetParameterFloatImage("bm.initdisp.maps.hmap"));
        m_LPBlockMatcher->SetVerticalDisparityInput(GetParameterFloatImage("bm.initdisp.maps.vmap"));
        }

      if (GetParameterInt("bm.subpixel") > 0)
        {
        m_LPSubPixFilter->SetInputsFromBlockMatchingFilter(m_LPBlockMatcher);
        AddProcess(m_LPSubPixFilter,"Sub-pixel refinement");
        switch (GetParameterInt("bm.subpixel"))
          {
          case 1 : m_LPSubPixFilter->SetRefineMethod(LPSubPixelDisparityFilterType::PARABOLIC);
            break;
          case 2 : m_LPSubPixFilter->SetRefineMethod(LPSubPixelDisparityFilterType::TRIANGULAR);
            break;
          case 3 : m_LPSubPixFilter->SetRefineMethod(LPSubPixelDisparityFilterType::DICHOTOMY);
            break;
          default : break;
          }
        hdispImage = m_LPSubPixFilter->GetHorizontalDisparityOutput();
        vdispImage = m_LPSubPixFilter->GetVerticalDisparityOutput();
        metricImage = m_LPSubPixFilter->GetMetricOutput();
        }
      else
        {
        hdispImage = m_LPBlockMatcher->GetHorizontalDisparityOutput();
        vdispImage = m_LPBlockMatcher->GetVerticalDisparityOutput();
        metricImage = m_LPBlockMatcher->GetMetricOutput();
        }
      }

    if (IsParameterEnabled("bm.medianfilter.radius") && IsParameterEnabled("bm.medianfilter.incoherence"))
      {
      otbAppLogINFO("Applies a median filter to smooth the disparity map"<<std::endl);
      if (minhdisp < maxhdisp)
        {
        m_HMedianFilter->SetInput(hdispImage);
        m_HMedianFilter->SetRadius(GetParameterInt("bm.medianfilter.radius"));
        m_HMedianFilter->SetIncoherenceThreshold(GetParameterFloat("bm.medianfilter.incoherence"));
        if (maskingLeft)
          {
          m_HMedianFilter->SetMaskInput(maskLeftImage);
          }
        hdispImage = m_HMedianFilter->GetOutput();
        }

      if (minvdisp < maxvdisp)
        {
        m_VMedianFilter->SetInput(vdispImage);
        m_VMedianFilter->SetRadius(GetParameterInt("bm.medianfilter.radius"));
        m_VMedianFilter->SetIncoherenceThreshold(GetParameterFloat("bm.medianfilter.incoherence"));
        if (maskingLeft)
          {
          m_VMedianFilter->SetMaskInput(maskLeftImage);
          }
        vdispImage = m_VMedianFilter->GetOutput();
        }
      }

    m_OutputImageList->Clear();

    m_OutputImageList->PushBack(hdispImage);
    m_OutputImageList->PushBack(vdispImage);

    if(IsParameterEnabled("io.outmetric"))
      {
      m_OutputImageList->PushBack(metricImage);
      }

    m_ImageListFilter->SetInput(m_OutputImageList);

    SetParameterOutputImage("io.out",m_ImageListFilter->GetOutput());

    if(IsParameterEnabled("io.outmask"))
      {
      SetParameterOutputImage("io.outmask",maskLeftImage);
      // The right side mask is not saved because the application computes disparities from left to right
      }
  }

  // SSD Block matching filter
  SSDBlockMatchingFilterType::Pointer m_SSDBlockMatcher;

  // NCC Block matching filter
  NCCBlockMatchingFilterType::Pointer m_NCCBlockMatcher;

  // Lp Block matching filter
  LPBlockMatchingFilterType::Pointer  m_LPBlockMatcher;

  // SSD sub-pixel disparity filter
  SSDSubPixelDisparityFilterType::Pointer m_SSDSubPixFilter;

  // NCC sub-pixel disparity filter
  NCCSubPixelDisparityFilterType::Pointer m_NCCSubPixFilter;

  // LP sub-pixel disparity filter
  LPSubPixelDisparityFilterType::Pointer  m_LPSubPixFilter;

  // Variance filter for left image
  VarianceFilterType::Pointer         m_LVarianceFilter;

  // Variance filter for right image
  VarianceFilterType::Pointer         m_RVarianceFilter;

  // Band-math filter for left mask
  BandMathFilterType::Pointer         m_LBandMathFilter;

  // Band-math filter for right mask
  BandMathFilterType::Pointer         m_RBandMathFilter;

  // The Image list
  ImageListType::Pointer              m_OutputImageList;

  // Image list to VectorImage filter
  ImageListToVectorImageFilterType::Pointer m_ImageListFilter;

  // Horizontal Median filter
  MedianFilterType::Pointer           m_HMedianFilter;

  // Vertical Median filter
  MedianFilterType::Pointer           m_VMedianFilter;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::BlockMatching)
