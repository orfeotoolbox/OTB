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

    AddParameter(ParameterType_OutputImage, "io.outmask", "The output mask corresponding to all criterions");
    SetParameterDescription("io.outmask","A mask image corresponding to all citerions (see masking parameters). Only required if variance threshold or nodata criterions are set.");
    DisableParameter("io.outmask");
    MandatoryOff("io.outmask");

    AddParameter(ParameterType_Empty,"io.outmetric","Output optimal metric values as well");
    SetParameterDescription("io.outmetric","If used, the output image will have a second component with metric optimal values");

    AddParameter(ParameterType_Group,"mask","Image masking parameters");
    SetParameterDescription("mask","This group of parameters allows to determine the masking parameters to prevent disparities estimation for some pixels of the left image");
    
    AddParameter(ParameterType_InputImage,"mask.in","Discard pixels from mask image");
    SetParameterDescription("mask.in","This parameter allows to provide a custom mask");
    MandatoryOff("mask.in");

    AddParameter(ParameterType_Float,"mask.nodata","Discard pixels with no-data value");
    SetParameterDescription("mask.in","This parameter allows to discard pixels whose value is equal to the user-defined no-data value.");
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
      EnableParameter("io.outmask");
      }
    else
      {
      DisableParameter("io.outmask");
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

    // Handle input mask if present
    if(IsParameterEnabled("mask.in"))
      {
      leftmask = GetParameterFloatImage("mask.in");
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
      if(masking)
        {
        m_SSDBlockMatcher->SetMaskInput(m_BandMathFilter->GetOutput());
        }

      hdispImage = m_SSDBlockMatcher->GetHorizontalDisparityOutput();
      vdispImage = m_SSDBlockMatcher->GetVerticalDisparityOutput();
      metricImage = m_SSDBlockMatcher->GetMetricOutput();
      }
    // NCC case
    else
      {
      m_NCCBlockMatcher->SetLeftInput(leftImage);
      m_NCCBlockMatcher->SetRightInput(rightImage);
      m_NCCBlockMatcher->SetRadius(radius);
      m_NCCBlockMatcher->SetMinimumHorizontalDisparity(minhdisp);
      m_NCCBlockMatcher->SetMaximumHorizontalDisparity(maxhdisp);
      m_NCCBlockMatcher->SetMinimumVerticalDisparity(minvdisp);
      m_NCCBlockMatcher->SetMaximumVerticalDisparity(maxvdisp);
      m_NCCBlockMatcher->MinimizeOff();

      if(masking)
        {
        m_NCCBlockMatcher->SetMaskInput(m_BandMathFilter->GetOutput());
        }

      hdispImage = m_NCCBlockMatcher->GetHorizontalDisparityOutput();
      vdispImage = m_NCCBlockMatcher->GetVerticalDisparityOutput();
      metricImage = m_NCCBlockMatcher->GetMetricOutput();
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
      SetParameterOutputImage("io.outmask",m_BandMathFilter->GetOutput());
      }
  }

  // SSD Block matching functor
  SSDBlockMatchingFilterType::Pointer m_SSDBlockMatcher;
  
  // NCC Block matching functor
  NCCBlockMatchingFilterType::Pointer m_NCCBlockMatcher;
  
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
