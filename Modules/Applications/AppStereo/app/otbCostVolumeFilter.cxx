/*
 * Copyright (C) 2017-2018 CS Systemes d'Information (CS SI)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>


#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageToVectorImageCastFilter.h"

#include "otbBijectionCoherencyFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbConvertDisparityValue.h"
#include "otbConvertionRGBToGrayLevelImageFilter.h"
#include "otbConvertValueFrom0To255.h"
#include "otbConvolutionImageFilter.h"
#include "otbCostVolumeFilter.h"
#include "otbFillOcclusionPixel.h"
#include "otbLocalGradientVectorImageFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbMinimumVectorImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbWeightsGuidedFilter.h"

#include "itkArray.h"
#include "itkMedianImageFilter.h"
#include "itkConstantBoundaryCondition.h"



namespace otb
{
namespace Wrapper
{
class CostVolumeFilter : public Application
  {
  public:
  /** Standard class typedefs. */
  typedef CostVolumeFilter                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;    

  static const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::VectorImage<int> IntVectorImageType;
  typedef otb::Image< float, Dimension > FloatImageType;
  typedef otb::Image< int, Dimension > IntImageType;

  typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionType> ConvFilterType;
  typedef otb::ConvertionRGBToGrayLevelImageFilter<FloatVectorImageType,FloatVectorImageType> RGBTograylevelFilter ;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterType> VectorFilterType;
  typedef otb::CostVolumeFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  CostVolumeType;  
  typedef otb::MinimumVectorImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;    
  typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
  typedef otb::MeanVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > MeanVectorImage;
  typedef otb::ConvertDisparityValue<IntImageType, IntImageType > ConvertToDisparityValue ;
  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  typedef itk::MedianImageFilter< FloatImageType, FloatImageType > MedianFilterType;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MedianFilterType> PerBand;
  typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastIntImageIntoFloatVecImageFilter; 
  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;
  typedef otb::FillPixelFilter< FloatVectorImageType, IntImageType > FillOccFilter ;
  typedef otb::ConvertValueFrom0To255<IntImageType, IntImageType > ConvertValue ;
  

  itkNewMacro(Self);
  itkTypeMacro(CostVolumeFilter, otb::Application);

  private:

  void DoInit() override 
    {
    SetDescription("Performs Fast Cost Volume Filter to estimate a disparty map between two images");

    SetDocName("CostVolumeFilter");
    SetDocLongDescription("This application allows Fast Cost Volume Filter to estimate a disparty map between two images "
     "to present parameters types," 
      " and Application class framework. " 
      "It is used to generate Software guide documentation" 
      " for Application chapter CostVolumeFilter.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("");


    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows setting the input and output images.");

    AddParameter(ParameterType_InputImage,"io.inleft","Left input image");
    SetParameterDescription("io.inleft","The left input image (reference)");

    AddParameter(ParameterType_InputImage,"io.inright","Right input image");
    SetParameterDescription("io.inright","The right input (secondary)");

    AddParameter(ParameterType_Int, "sense", "Direction of camera motion : '1' from left to right, '2' from right to left" );
    SetParameterDescription("sense", "Direction of camera motion : '1' from left to right, '2' from right to left");
    SetDefaultParameterInt("sense",1);

    AddParameter(ParameterType_OutputImage, "io.out", "The output disparity map");
    SetParameterDescription("io.out","An image containing the estimated disparities");

    AddParameter(ParameterType_Int,"radius","Radius of Mean, gradient and the guided Filter");
    SetParameterDescription("radius","The radius of the guided Filter.");
    SetDefaultParameterInt("radius",9);  

    AddParameter(ParameterType_Int,"dmin","Minimum horizontal disparity value");
    SetParameterDescription("dmin","Minimum horizontal disparity to explore (can be negative)");
    SetDefaultParameterInt("dmin",-12);

    AddParameter(ParameterType_Int,"dmax","Maximum horizontal disparity");
    SetParameterDescription("dmax","Maximum horizontal disparity to explore (can be negative)");
    SetDefaultParameterInt("dmax",-4);
    
    AddParameter(ParameterType_Int,"rwmf", "Radius for median filter");
    SetParameterDescription("rwmf", "Radius for median filter");
    SetDefaultParameterInt("rwmf",9);  

    AddParameter(ParameterType_Int,"tol", "Tolerance for left-right disp. diff");
    SetParameterDescription("tol", "Tolerance for left-right disp. diff");
    SetDefaultParameterInt("tol",2);

    AddParameter(ParameterType_Int, "conversion", "Range of the disparity map : '0' disparity from dmin to dmax, '1' disparity from 0 to 255");
    SetParameterDescription("conversion", "Range of the disparity map : '0' disparity from dmin to dmax, '1' disparity from 0 to 255" );
    SetDefaultParameterInt("conversion",0);

    AddParameter(ParameterType_Int, "choice", "Output image files : '1' DisparityMap after cost-volume filtering, '2' Occlusions Mask, '3' Disparity map after densification with weighted median filter");
    SetParameterDescription("choice", " '1' DisparityMap after cost-volume filtering, '2' Occlusions Mask, '3' Disparity map after densification with weighted median filter" );
    SetDefaultParameterInt("choice",3);

    AddParameter(ParameterType_Float, "alpha", "alpha parameter for the cost volume filter");
    SetParameterDescription("alpha", "alpha parameter for the cost volume filter" );
    SetDefaultParameterFloat("alpha", 0.9);

    AddParameter(ParameterType_Float, "tau1l", "max for color difference(left image)");
    SetParameterDescription("tau1l", "max for color difference (left image) " );
    SetDefaultParameterFloat("tau1l", 7.0);

    AddParameter(ParameterType_Float, "tau2l", "max for gradient difference (left image)");
    SetParameterDescription("tau2l", "max for gradient difference (left image)" );
    SetDefaultParameterFloat("tau2l", 2.0);

    AddParameter(ParameterType_Float, "tau1r", "max for gradient difference (left image)");
    SetParameterDescription("tau1r", "max for gradient difference (left image)" );
    SetDefaultParameterFloat("tau1r", 7.0);


    AddParameter(ParameterType_Float, "tau2r", "max for gradient difference (right image)");
    SetParameterDescription("tau2r", "max for gradient difference (right image)" );
    SetDefaultParameterFloat("tau2r", 2.0);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.inleft","StereoFixed.png");
    SetDocExampleParameterValue("io.inright","StereoMoving.png");
    SetDocExampleParameterValue("sense", "1") ;
    SetDocExampleParameterValue("dmin","-15");
    SetDocExampleParameterValue("dmax","0");
    SetDocExampleParameterValue("rwmf","9");
    SetDocExampleParameterValue("radius","9");
    SetDocExampleParameterValue("tol","2");
    SetDocExampleParameterValue("io.out","MyCostVolumeFilterDisparity.tif");
    SetDocExampleParameterValue("conversion","0");
    SetDocExampleParameterValue("choice","3");
    SetDocExampleParameterValue("alpha","0.9");
    SetDocExampleParameterValue("tau1l","7.0");
    SetDocExampleParameterValue("tau2l","2.0");
    SetDocExampleParameterValue("tau1r","7.0");
    SetDocExampleParameterValue("tau2r","2.0");
    }


  void DoUpdateParameters() override {}


  CostVolumeFilter()
    {
    m_convFilterXLeft = ConvFilterType::New() ;
    m_convFilterXRight = ConvFilterType::New() ;
    m_LeftGrayVectorImage = RGBTograylevelFilter::New() ;
    m_RightGrayVectorImage = RGBTograylevelFilter::New() ;
    m_GradientXLeft = VectorFilterType::New();
    m_GradientXRight = VectorFilterType::New();
    m_LeftCost = CostVolumeType::New() ;
    m_RightCost = CostVolumeType::New() ;
    m_minLeftGF = MinCostVolume::New() ;
    m_minRightGF = MinCostVolume::New() ;
    m_meanLeftCost = Weights_ak_bk::New() ; 
    m_meanRightCost = Weights_ak_bk::New() ; 
    m_meanLeftWeights = MeanVectorImage::New() ;
    m_meanRightWeights = MeanVectorImage::New() ;
    m_LeftDisparity = ConvertToDisparityValue::New();
    m_RightDisparity = ConvertToDisparityValue::New();
    m_OcclusionFilter = OcclusionType::New();
    LMedian = MedianFilterType::New();
    m_LeftMedianFilter = PerBand::New();
    m_RightMedianFilter = PerBand::New();
    m_CastOccMap = CastIntImageIntoFloatVecImageFilter::New(); 
    m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
    m_CastRightDisparity = CastIntImageIntoFloatVecImageFilter::New();
    m_ConcatenateMedianImageAndOccMap = ConcatenateVectorImageFilterType::New();
    m_ConcatenateCastOccMapAndInputImage = ConcatenateVectorImageFilterType::New() ;
    m_FillOcc = FillOccFilter::New();
    m_convertSmoothDisparity = ConvertValue::New() ;    
    }


  void DoExecute() override
    { 
    FloatVectorImageType::Pointer inLeft = GetParameterFloatVectorImage("io.inleft");
    inLeft->UpdateOutputInformation();
    FloatVectorImageType::Pointer inRight = GetParameterFloatVectorImage("io.inright");
    inRight->UpdateOutputInformation();

    int dispMin = GetParameterInt("dmin");
    int dispMax = GetParameterInt("dmax");
    long unsigned int r  = GetParameterInt("radius");  
    unsigned int sense = GetParameterInt("sense") ;
    unsigned int tol = GetParameterInt("tol") ;
    float alpha = GetParameterFloat("alpha");
    float tau1r = GetParameterFloat("tau1r");
    float tau2r = GetParameterFloat("tau2r");
    float tau1l = GetParameterFloat("tau1l");
    float tau2l = GetParameterFloat("tau2l");

    unsigned int conversion = GetParameterInt("conversion") ;
    unsigned int rwmf = GetParameterInt("rwmf") ; 
    unsigned int choice = GetParameterInt("choice") ;

      // GRADIENT CALCULATION
  ConvFilterType::InputSizeType radiusG;
  radiusG[0] = 1;
  radiusG[1] = 0;
  itk::Array< double > filterCoeffsX;
  filterCoeffsX.SetSize((2 * radiusG[0] + 1) ) ;
  filterCoeffsX.Fill(0.5);
  filterCoeffsX[0] = -0.5;
  filterCoeffsX[1] = 0;
  m_convFilterXLeft->SetRadius(radiusG);
  m_convFilterXLeft->SetFilter(filterCoeffsX);  
  m_convFilterXRight->SetRadius(radiusG);
  m_convFilterXRight->SetFilter(filterCoeffsX);
  
  //--Left---------------  
  m_GradientXLeft->SetFilter(m_convFilterXLeft);
  if( inLeft->GetNumberOfComponentsPerPixel() == 3)
    {
    m_LeftGrayVectorImage->SetInput(inLeft);
    m_GradientXLeft->SetInput(m_LeftGrayVectorImage->GetOutput());
    }
  else
    {
    m_GradientXLeft->SetInput(inLeft);
    }
  m_GradientXLeft->UpdateOutputInformation(); 

  //--Right--------------- 
  m_GradientXRight->SetFilter(m_convFilterXRight);
  if( inRight->GetNumberOfComponentsPerPixel() == 3)
    {
    m_RightGrayVectorImage->SetInput(inRight);
    m_GradientXRight->SetInput(m_RightGrayVectorImage->GetOutput());
    }
  else
    {
    m_GradientXRight->SetInput(inRight);
    }
  m_GradientXRight->UpdateOutputInformation(); 


  // COST VOLUME 
  m_LeftCost->SetLeftInputImage(inLeft);
  m_LeftCost->SetRightInputImage(inRight);  
  m_LeftCost->SetSide('l') ;
  m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );  

  m_RightCost->SetLeftInputImage(inRight);
  m_RightCost->SetRightInputImage(inLeft );  
  m_RightCost->SetSide('r');
  m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() ); 

  if(sense==1)
    {
    m_LeftCost->SetMinDisp(dispMin);
    m_LeftCost->SetMaxDisp(dispMax);
    m_RightCost->SetMinDisp(-dispMax);
    m_RightCost->SetMaxDisp(-dispMin);
    }
  else
    {
    m_LeftCost->SetMinDisp(-dispMax);
    m_LeftCost->SetMaxDisp(-dispMin); 
    m_RightCost->SetMinDisp(dispMin);
    m_RightCost->SetMaxDisp(dispMax); 
    }

  m_LeftCost->SetAlpha(alpha);
  m_LeftCost->SetTau1(tau1l);
  m_LeftCost->SetTau2(tau2l); 
  m_LeftCost->UpdateOutputInformation(); 
         
  m_RightCost->SetAlpha(alpha);
  m_RightCost->SetTau1(tau1r);
  m_RightCost->SetTau2(tau2r);    
  m_RightCost->UpdateOutputInformation(); 

  //WEIGHTS  
     // --- LEFT
  m_meanLeftCost->SetInput1(inLeft);
  m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
  m_meanLeftCost->SetRadius(0,r);  
  m_meanLeftCost->UpdateOutputInformation(); 
   // --- RIGHT
  m_meanRightCost->SetInput1(inRight);
  m_meanRightCost->SetInput2(m_RightCost->GetOutput());  
  m_meanRightCost->SetRadius(0,r);  
  m_meanRightCost->UpdateOutputInformation(); 
  //MEAN WEIGHTS   
    // --- LEFT
  m_meanLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
  m_meanLeftWeights->SetInput2(inLeft);
  m_meanLeftWeights->SetRadius(0,r);  
  m_meanLeftWeights->UpdateOutputInformation(); 
    // --- RIGHT
  m_meanRightWeights->SetInput1(m_meanRightCost->GetOutput());
  m_meanRightWeights->SetInput2(inRight);
  m_meanRightWeights->SetRadius(0,r); 
  m_meanRightWeights->UpdateOutputInformation(); 
  //DISPARITY MAP
    // --- LEFT   
  m_minLeftGF->SetInput(m_meanLeftWeights->GetOutput()); 
  m_minLeftGF->SetSide('l');
  m_minLeftGF->UpdateOutputInformation(); 
      // --- RIGHT
  m_minRightGF->SetInput(m_meanRightWeights->GetOutput());  
  m_minRightGF->SetSide('r');
  m_minRightGF->UpdateOutputInformation(); 

  m_LeftDisparity->SetInput(m_minLeftGF->GetOutput());
  m_RightDisparity->SetInput(m_minRightGF->GetOutput());

  if(sense==1)
    {
    m_LeftDisparity->SetDisp(dispMax);
    m_RightDisparity->SetDisp(-dispMax);
    }
  else
    {
    m_LeftDisparity->SetDisp(-dispMin);
    m_RightDisparity->SetDisp(dispMin);
    }

  if(sense==1)
    {
    m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
    m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparity->GetOutput());
    }
  else
    {
    m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_RightDisparity->GetOutput()); 
    m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_LeftDisparity->GetOutput());
    }
  
  m_OcclusionFilter->SetMaxHDisp(dispMax);
  m_OcclusionFilter->SetMinHDisp(dispMin); 
  m_OcclusionFilter->SetMinVDisp(0);
  m_OcclusionFilter->SetMaxVDisp(0);
  m_OcclusionFilter->SetTolerance(tol);


  //MEDIAN IMAGES
  MedianFilterType::RadiusType MedianRadius = {{rwmf,rwmf}};
  LMedian->SetRadius(MedianRadius); 
  m_LeftMedianFilter->SetFilter(LMedian);
  m_LeftMedianFilter->SetInput(inLeft); 
  m_RightMedianFilter->SetFilter(LMedian);
  m_RightMedianFilter->SetInput(inRight); 

  m_CastOccMap-> SetInput( m_OcclusionFilter->GetOutput() ); 
  m_CastLeftDisparity-> SetInput( m_LeftDisparity->GetOutput() );
  m_CastRightDisparity->SetInput( m_RightDisparity->GetOutput() );

  if(sense==1)
    {        
    m_ConcatenateMedianImageAndOccMap->SetInput1(m_LeftMedianFilter->GetOutput());
    m_ConcatenateMedianImageAndOccMap->SetInput2(m_CastOccMap->GetOutput());
    m_ConcatenateCastOccMapAndInputImage->SetInput1(m_CastLeftDisparity->GetOutput());
    m_ConcatenateCastOccMapAndInputImage->SetInput2(m_ConcatenateMedianImageAndOccMap->GetOutput());
    }
  else
    {      
    m_ConcatenateMedianImageAndOccMap->SetInput1(m_RightMedianFilter->GetOutput());
    m_ConcatenateMedianImageAndOccMap->SetInput2(m_CastOccMap->GetOutput());
    m_ConcatenateCastOccMapAndInputImage->SetInput1(m_CastRightDisparity->GetOutput());
    m_ConcatenateCastOccMapAndInputImage->SetInput2(m_ConcatenateMedianImageAndOccMap->GetOutput());
    } 
   
  m_FillOcc->SetInput(m_ConcatenateCastOccMapAndInputImage->GetOutput());
  FloatVectorImageType::SizeType radiusM;
  radiusM[0] = rwmf;
  radiusM[1] = rwmf;   
  m_FillOcc->SetRadius(radiusM);


  //OUTPUT  
  if(choice==1)
    {
    //std::cout << "DISPARITY MAP... " << std::endl ;
    if(sense==1)
      {
      if(conversion==1)
        {
        m_convertSmoothDisparity->SetInput(m_LeftDisparity->GetOutput());
        m_convertSmoothDisparity->SetDispMin(dispMin);
        m_convertSmoothDisparity->SetDispMax(dispMax);       
        m_convertSmoothDisparity->SetOffset(0);
        SetParameterOutputImage("io.out", m_convertSmoothDisparity->GetOutput());
        }
      else
        {
        SetParameterOutputImage("io.out", m_LeftDisparity->GetOutput()); 
        }
      }
    else
      {
      if(conversion==1)
        {
        m_convertSmoothDisparity->SetInput(m_RightDisparity->GetOutput());
        m_convertSmoothDisparity->SetDispMin(dispMin);
        m_convertSmoothDisparity->SetDispMax(dispMax);       
        m_convertSmoothDisparity->SetOffset(0);
        SetParameterOutputImage("io.out", m_convertSmoothDisparity->GetOutput());
        }
      else
        {
        SetParameterOutputImage("io.out", m_RightDisparity->GetOutput()); 
        }
      }
    }

  if(choice==2)
    {
    //std::cout << "OCCLUSIONS MASK... " << std::endl ;
    SetParameterOutputImage("io.out", m_OcclusionFilter->GetOutput());
    }

  if(choice==3)
    {
      //std::cout << "FILLED DISPARITY MAP... " << std::endl ;
    if(conversion==1)
      {
      m_convertSmoothDisparity->SetInput(m_FillOcc->GetOutput());
      m_convertSmoothDisparity->SetDispMin(dispMin);
      m_convertSmoothDisparity->SetDispMax(dispMax);       
      m_convertSmoothDisparity->SetOffset(0);
      SetParameterOutputImage("io.out", m_convertSmoothDisparity->GetOutput());
      }
    else
      {
      SetParameterOutputImage("io.out", m_FillOcc->GetOutput()); 
      }   
    }
  }

  ConvFilterType::Pointer m_convFilterXLeft;
  ConvFilterType::Pointer m_convFilterXRight;
  RGBTograylevelFilter::Pointer m_LeftGrayVectorImage;
  RGBTograylevelFilter::Pointer m_RightGrayVectorImage;
  VectorFilterType::Pointer m_GradientXLeft;
  VectorFilterType::Pointer m_GradientXRight ;
  CostVolumeType::Pointer m_LeftCost;
  CostVolumeType::Pointer m_RightCost;
  MinCostVolume::Pointer m_minLeftGF ;
  MinCostVolume::Pointer m_minRightGF ;
  Weights_ak_bk::Pointer m_meanLeftCost ;
  Weights_ak_bk::Pointer m_meanRightCost;
  MeanVectorImage::Pointer m_meanLeftWeights;
  MeanVectorImage::Pointer m_meanRightWeights;
  ConvertToDisparityValue::Pointer m_LeftDisparity;
  ConvertToDisparityValue::Pointer m_RightDisparity;
  OcclusionType::Pointer m_OcclusionFilter ;
  MedianFilterType::Pointer LMedian;
  PerBand::Pointer m_LeftMedianFilter;
  PerBand::Pointer m_RightMedianFilter ;

  CastIntImageIntoFloatVecImageFilter::Pointer m_CastOccMap ;
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity ;
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastRightDisparity ;
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateMedianImageAndOccMap;
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndInputImage;
  FillOccFilter::Pointer m_FillOcc ;
  ConvertValue::Pointer m_convertSmoothDisparity ;

 
  }; //end class

} // end Wrapper 
} //end otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CostVolumeFilter)