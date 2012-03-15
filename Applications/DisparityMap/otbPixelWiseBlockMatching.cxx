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

#include "otbPixelWiseBlockMatchingImageFilter.h"
#include "otbVarianceImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class PixelWiseBlockMatching : public Application
{
public:
  /** Standard class typedefs. */
  typedef PixelWiseBlockMatching              Self;
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


  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(PixelWiseBlockMatching, otb::Application);

  /** Filters typedef */

private:
  PixelWiseBlockMatching()
  {
    // Initialize filters
    m_SSDBlockMatcher = SSDBlockMatchingFilterType::New();
    m_NCCBlockMatcher = NCCBlockMatchingFilterType::New();
    m_LPBlockMatcher  = LPBlockMatchingFilterType::New();
    m_VarianceFilter  = VarianceFilterType::New();
    m_BandMathFilter  = BandMathFilterType::New();
    m_OutputImageList = ImageListType::New();
    m_ImageListFilter = ImageListToVectorImageFilterType::New();
  }

  void DoInit()
  {
    SetName("PixelWiseBlockMatching");
    SetDescription("Performs block-matching to estimate pixel-wise disparities between two images");

    SetDocName(" Pixel-wise Block-Matching");
    SetDocLongDescription("This application allows to performs block-matching to estimate pixel-wise disparities between two images. The application allows to choose the block-matching method to use. It also allows to input a mask (related to the left input image) of pixels for which the disparity should be investigated. Additionnaly, two criterions can be optionnaly use to disable disparity investigation for some pixel: a no-data value, and a threshold on the local variance. This allows to speed-up computation by avoiding to investigate disparities that will not be reliable anyway. For efficiency reasons, if the optimal metric values image is desired, it will be concatenated to the output image (which will then have two bands, the first being the disparity, and the second the metric values). One can split these images afterward.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbStereoRectificationGridGenerator");

    AddDocTag(Tags::Stereo);

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows to set the input and output images.");
    
    AddParameter(ParameterType_InputImage,"io.inleft","Left input image");
    SetParameterDescription("io.inleft","The left input image (reference)");

    AddParameter(ParameterType_InputImage,"io.inright","Right input image");
    SetParameterDescription("io.inright","The right input (secondary)");

    AddParameter(ParameterType_OutputImage, "io.out", "The output disparity map");
    SetParameterDescription("io.out","An image containing the estimated disparities as well as the metric values if the option is used");

    AddParameter(ParameterType_OutputImage, "io.outmaskleft", "The left output mask corresponding to all criterions");
    SetParameterDescription("io.outmaskleft","A left mask image corresponding to all citerions (see masking parameters). Only required if variance threshold or nodata criterions are set.");
    DisableParameter("io.outmaskleft");
    MandatoryOff("io.outmaskleft");
    
    AddParameter(ParameterType_OutputImage, "io.outmaskright", "The right output mask corresponding to all criterions");
    SetParameterDescription("io.outmaskright","A right mask image corresponding to all citerions (see masking parameters). Only required if variance threshold or nodata criterions are set.");
    DisableParameter("io.outmaskright");
    MandatoryOff("io.outmaskright");

    AddParameter(ParameterType_Empty,"io.outmetric","Output optimal metric values as well");
    SetParameterDescription("io.outmetric","If used, the output image will have a second component with metric optimal values");

    AddParameter(ParameterType_Group,"mask","Image masking parameters");
    SetParameterDescription("mask","This group of parameters allows to determine the masking parameters to prevent disparities estimation for some pixels of the left image");
    
    AddParameter(ParameterType_InputImage,"mask.inleft","Discard left pixels from mask image");
    SetParameterDescription("mask.inleft","This parameter allows to provide a custom mask for the left image");
    MandatoryOff("mask.inleft");
    
    AddParameter(ParameterType_InputImage,"mask.inright","Discard right pixels from mask image");
    SetParameterDescription("mask.inright","This parameter allows to provide a custom mask for the right image");
    MandatoryOff("mask.inright");

    AddParameter(ParameterType_Float,"mask.nodata","Discard pixels with no-data value");
    SetParameterDescription("mask.nodata","This parameter allows to discard pixels whose value is equal to the user-defined no-data value.");
    MandatoryOff("mask.nodata");
    SetDefaultParameterFloat("mask.nodata",0.);
    DisableParameter("mask.nodata");
    
    AddParameter(ParameterType_Float,"mask.variancet","Discard pixels with low local variance");
    SetParameterDescription("mask.variancet","This parameter allows to discard pixels whose local variance is too small (the size of the neighborhood is given by the radius parameter)");
    MandatoryOff("mask.variancet");
    SetDefaultParameterFloat("mask.variancet",100.);
    DisableParameter("mask.variancet");

    AddParameter(ParameterType_Group,"bm","Block matching parameters");
    SetParameterDescription("bm","This group of parameters allow to tune the block-matching behaviour");
    
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
    
    AddParameter(ParameterType_Empty,"bm.subpixel","Do sub-pixel interpolation");
    SetParameterDescription("bm.subpixel", "Estimate disparities with sub-pixel precision");
    
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
    SetDocExampleParameterValue("bm.mind","-10");
    SetDocExampleParameterValue("bm.maxd","10");
    SetDocExampleParameterValue("masking.variancet","10");
    SetDocExampleParameterValue("io.out","MyDisparity.tif");
  }

  void DoUpdateParameters()
  {
    if(IsParameterEnabled("mask.variancet") || IsParameterEnabled("mask.nodata"))
      {
      EnableParameter("io.outmaskleft");
      EnableParameter("io.outmaskright");
      }
    else
      {
      DisableParameter("io.outmaskleft");
      DisableParameter("io.outmaskright");
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

    unsigned int radius  = GetParameterInt("bm.radius");
    int          minhdisp = GetParameterInt("bm.minhd");
    int          maxhdisp = GetParameterInt("bm.maxhd");
    int          minvdisp = GetParameterInt("bm.minvd");
    int          maxvdisp = GetParameterInt("bm.maxvd");
    
    itk::OStringStream bandMathExpression;
    bandMathExpression<<"if(";
    
    unsigned int inputId = 0;
    bool masking = false;
    bool useInitialDispUniform = false;
    bool useInitialDispMap = false;

    // Handle input mask if present
    if(IsParameterEnabled("mask.inleft"))
      {
      leftmask = GetParameterFloatImage("mask.inleft");
      m_BandMathFilter->SetNthInput(inputId,leftmask,"inmask");
      masking = true;
      ++inputId;
      bandMathExpression<<"inmask > 0";
      }
    
    // Handle variance threshold if present
    if(IsParameterEnabled("mask.variancet"))
      {
      if(masking)
        {
        bandMathExpression<<" and ";
        }

      m_VarianceFilter->SetInput(leftImage);
      VarianceFilterType::InputSizeType vradius;
      vradius.Fill(radius);
      m_VarianceFilter->SetRadius(vradius);

      m_BandMathFilter->SetNthInput(inputId,m_VarianceFilter->GetOutput(),"variance");
      bandMathExpression<<"variance > "<<GetParameterFloat("mask.variancet");
      ++inputId;
      masking = true;
      }

    // Handle nodata field if present
    if(IsParameterEnabled("mask.nodata"))
      {
      if(masking)
        {
        bandMathExpression<<" and ";
        }
      m_BandMathFilter->SetNthInput(inputId,leftImage,"leftimage");
      bandMathExpression<<"leftimage != "<<GetParameterFloat("mask.nodata");
      masking = true;
      }

    bandMathExpression<<",255,0)";

    if(masking)
      {
      GetLogger()->Info("Masking criterion: " + bandMathExpression.str());
      m_BandMathFilter->SetExpression(bandMathExpression.str());
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

    // SSD case
    if(GetParameterInt("bm.metric") == 0)
      {
      m_SSDBlockMatcher->SetLeftInput(leftImage);
      m_SSDBlockMatcher->SetRightInput(rightImage);
      m_SSDBlockMatcher->SetRadius(radius);
      m_SSDBlockMatcher->SetMinimumHorizontalDisparity(minhdisp);
      m_SSDBlockMatcher->SetMaximumHorizontalDisparity(maxhdisp);
      m_SSDBlockMatcher->SetMinimumVerticalDisparity(minvdisp);
      m_SSDBlockMatcher->SetMaximumVerticalDisparity(maxvdisp);
      if (IsParameterEnabled("bm.subpixel"))
        {
        m_SSDBlockMatcher->DoSubPixelInterpolationOn();
        }
      
      AddProcess(m_SSDBlockMatcher,"SSD block matching");
      if(masking)
        {
        m_SSDBlockMatcher->SetLeftMaskInput(m_BandMathFilter->GetOutput());
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

      hdispImage = m_SSDBlockMatcher->GetHorizontalDisparityOutput();
      vdispImage = m_SSDBlockMatcher->GetVerticalDisparityOutput();
      metricImage = m_SSDBlockMatcher->GetMetricOutput();
      }
    // NCC case
    else if (GetParameterInt("bm.metric") == 1)
      {
      m_NCCBlockMatcher->SetLeftInput(leftImage);
      m_NCCBlockMatcher->SetRightInput(rightImage);
      m_NCCBlockMatcher->SetRadius(radius);
      m_NCCBlockMatcher->SetMinimumHorizontalDisparity(minhdisp);
      m_NCCBlockMatcher->SetMaximumHorizontalDisparity(maxhdisp);
      m_NCCBlockMatcher->SetMinimumVerticalDisparity(minvdisp);
      m_NCCBlockMatcher->SetMaximumVerticalDisparity(maxvdisp);
      m_NCCBlockMatcher->MinimizeOff();
      if (IsParameterEnabled("bm.subpixel"))
        {
        m_NCCBlockMatcher->DoSubPixelInterpolationOn();
        }
      
      AddProcess(m_NCCBlockMatcher,"NCC block matching");

      if(masking)
        {
        m_NCCBlockMatcher->SetLeftMaskInput(m_BandMathFilter->GetOutput());
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

      hdispImage = m_NCCBlockMatcher->GetHorizontalDisparityOutput();
      vdispImage = m_NCCBlockMatcher->GetVerticalDisparityOutput();
      metricImage = m_NCCBlockMatcher->GetMetricOutput();
      }
    // Lp case
    else
      {
      m_LPBlockMatcher->SetLeftInput(leftImage);
      m_LPBlockMatcher->SetRightInput(rightImage);
      m_LPBlockMatcher->SetRadius(radius);
      m_LPBlockMatcher->GetFunctor().SetP(static_cast<double>(GetParameterFloat("bm.metric.lp.p")));
      m_LPBlockMatcher->SetMinimumHorizontalDisparity(minhdisp);
      m_LPBlockMatcher->SetMaximumHorizontalDisparity(maxhdisp);
      m_LPBlockMatcher->SetMinimumVerticalDisparity(minvdisp);
      m_LPBlockMatcher->SetMaximumVerticalDisparity(maxvdisp);
      if (IsParameterEnabled("bm.subpixel"))
        {
        m_LPBlockMatcher->DoSubPixelInterpolationOn();
        }
      
      AddProcess(m_LPBlockMatcher,"Lp block matching");

      if(masking)
        {
        m_LPBlockMatcher->SetLeftMaskInput(m_BandMathFilter->GetOutput());
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

      hdispImage = m_LPBlockMatcher->GetHorizontalDisparityOutput();
      vdispImage = m_LPBlockMatcher->GetVerticalDisparityOutput();
      metricImage = m_LPBlockMatcher->GetMetricOutput();
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

    if(IsParameterEnabled("io.outmaskleft"))
      {
      SetParameterOutputImage("io.outmaskleft",m_BandMathFilter->GetOutput());
      }
  }

  // SSD Block matching filter
  SSDBlockMatchingFilterType::Pointer m_SSDBlockMatcher;
  
  // NCC Block matching filter
  NCCBlockMatchingFilterType::Pointer m_NCCBlockMatcher;
  
  // Lp Block matching filter
  LPBlockMatchingFilterType::Pointer  m_LPBlockMatcher;
  
  // Variance filter
  VarianceFilterType::Pointer         m_VarianceFilter;
  
  // Band-math filter
  BandMathFilterType::Pointer         m_BandMathFilter;

  // The Image list
  ImageListType::Pointer              m_OutputImageList;

  // Image list to VectorImage filter
  ImageListToVectorImageFilterType::Pointer m_ImageListFilter;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::PixelWiseBlockMatching)
