/*=========================================================================

 *  Program:   ORFEO Toolbox
 *  Language:  C++
 *  Date:      $Date$
 * Version:   $Revision$
 *
 *  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 * See OTBCopyright.txt for details.
 *
 *
 *     This software is distributed WITHOUT ANY WARRANTY; without even
 *     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *    PURPOSE.  See the above copyright notices for more information.
 *
 *
 * Copyright (C) CS SI
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
 
=========================================================================*/

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

#include "otbLocalGradientVectorImageFilter.h"
#include "otbLeftCostVolumeImageFilter.h"
#include "otbRightCostVolumeImageFilter.h"
#include "otbMinimumNBandsImageFilter.h"
#include "otbMaximumNBandsImageFilter.h"
#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbWeightedMedianImageFilter.h"
#include "otbCovarianceMatrix.h"

#include <otbImageToVectorImageCastFilter.h>
#include <itkMedianImageFilter.h>
#include <otbPerBandVectorImageFilter.h>

#include <itkConstantBoundaryCondition.h>
#include <otbConvolutionImageFilter.h>
#include <itkArray.h>

//

#include "otbBijectionCoherencyFilter.h"

#include "otbImageToVectorImageCastFilter.h"
#include "otbFillOcclusionImageFilter.h"


#include "otbConvertionRGBToGrayLevelImageFilter.h"
#include "otbConvertValueFrom0To255.h"
#include "otbFillOcclusionPixel.h"

#include "otbConvertDisparityValue.h"



namespace otb
{
namespace Wrapper
{
class CVF : public Application
  {
  public:
  /** Standard class typedefs. */
  typedef CVF                 Self;
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
  typedef otb::LeftCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  LeftCostVolumeType;  
  typedef otb::RightCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > RightCostVolumeType; 
  typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
  typedef otb::MeanVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > MeanVectorImage;
  typedef otb::MinimumNBandsImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;  
  typedef otb::MaximumNBandsImageFilter< FloatVectorImageType, IntImageType > MaxCostVolume; 
  typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastIntImageIntoFloatVecImageFilter;
  typedef itk::MedianImageFilter< FloatImageType, FloatImageType > MedianFilterType;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MedianFilterType> PerBand; 
  typedef otb::ConvertValueFrom0To255<IntImageType, IntImageType > ConvertValue ;
  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;
  typedef otb::FillPixelFilter< FloatVectorImageType, IntImageType > FillOccFilter ;
  typedef otb::ConvertDisparityValue<IntImageType, IntImageType > ConvertToDisparityValue ;

  itkNewMacro(Self);
  itkTypeMacro(CVF, otb::Application);

  private:

  void DoInit() ITK_OVERRIDE 
    {
    SetDescription("Performs Fast Cost Volume Filter to estimate a disparty map between two images");

    SetDocName("CVF");
    SetDocLongDescription("This application allows Fast Cost Volume Filter to estimate a disparty map between two images "
     "to present parameters types," 
      " and Application class framework. " 
      "It is used to generate Software guide documentation" 
      " for Application chapter CVF.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("");


    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows setting the input and output images.");

    AddParameter(ParameterType_InputImage,"io.inleft","Left input image");
    SetParameterDescription("io.inleft","The left input image (reference)");

    AddParameter(ParameterType_InputImage,"io.inright","Right input image");
    SetParameterDescription("io.inright","The right input (secondary)");

    AddParameter(ParameterType_Int, "sense", "Direction of camera motion : '0' from left to right, '1' from right to left" );
    SetParameterDescription("sense", "Direction of camera motion : '0' from left to right, '1' from right to left");
    SetDefaultParameterInt("sense",0);

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

    AddParameter(ParameterType_Int, "range", "Range of the disparity map : '0' disparity from dmin to dmax, '1' disparity from 0 to 255");
    SetParameterDescription("range", "Range of the disparity map : '0' disparity from dmin to dmax, '1' disparity from 0 to 255" );
    SetDefaultParameterInt("range",0);

    AddParameter(ParameterType_Int, "choice", "Output image files : '1' DisparityMap after cost-volume filtering, '2' Occlusions Mask, '3' Disparity map after densification with weighted median filter");
    SetParameterDescription("choice", "DisparityMap after cost-volume filtering, '2' Occlusions Mask, '3' Disparity map after densification with weighted median filter" );
    SetDefaultParameterInt("choice",3);


    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.inleft","StereoFixed.png");
    SetDocExampleParameterValue("io.inright","StereoMoving.png");
    SetDocExampleParameterValue("sense", "0") ;
    SetDocExampleParameterValue("dmin","-15");
    SetDocExampleParameterValue("dmax","0");
    SetDocExampleParameterValue("rwmf","9");
    SetDocExampleParameterValue("radius","9");
    SetDocExampleParameterValue("tol","2");
    SetDocExampleParameterValue("io.out","MyCVFDisparity.tif");
    SetDocExampleParameterValue("range","0");
    SetDocExampleParameterValue("choice","3");

    }



  void DoUpdateParameters() ITK_OVERRIDE {}


  CVF()
    {
        m_convFilterXLeft = ConvFilterType::New();
        m_convFilterXRight = ConvFilterType::New();
        m_LeftGrayVectorImage = RGBTograylevelFilter::New();
        m_RightGrayVectorImage = RGBTograylevelFilter::New();
        m_GradientXLeft = VectorFilterType::New();
        m_GradientXRight = VectorFilterType::New();
        m_LeftCost = LeftCostVolumeType::New();
        m_RightCost = RightCostVolumeType::New();
        m_meanLeftCost = Weights_ak_bk::New(); 
        m_meanRightCost = Weights_ak_bk::New(); 
        m_meanLeftWeights = MeanVectorImage::New();
        m_meanRightWeights = MeanVectorImage::New();
        m_LeftDisparity = MinCostVolume::New();
        m_RightDisparity = MaxCostVolume::New();
        m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
        m_CastRightDisparity = CastIntImageIntoFloatVecImageFilter::New();
        LMedian = MedianFilterType::New();
        m_LeftMedianFilter = PerBand::New();
        m_RightMedianFilter = PerBand::New();
        m_OcclusionFilter = OcclusionType::New();  
        m_CastOccMap = CastIntImageIntoFloatVecImageFilter::New();
        m_ConcatenateMedianImageAndOccMap = ConcatenateVectorImageFilterType::New();
        m_ConcatenateCastOccMapAndInputImage = ConcatenateVectorImageFilterType::New();
        m_FillOcc = FillOccFilter::New();
        m_CastFillOccFilter= CastIntImageIntoFloatVecImageFilter::New();
        m_convertSmoothDisparity = ConvertValue::New();
        m_DispValueFilter = ConvertToDisparityValue::New();

    }

  void DoExecute() ITK_OVERRIDE
    { 
    FloatVectorImageType::Pointer inLeft = GetParameterFloatVectorImage("io.inleft");
    inLeft->UpdateOutputInformation();
    FloatVectorImageType::Pointer inRight = GetParameterFloatVectorImage("io.inright");
    inRight->UpdateOutputInformation();

    int dispMin = GetParameterInt("dmin");
    int dispMax = GetParameterInt("dmax");
    long unsigned int r  = GetParameterInt("radius");
    unsigned int rwmf = GetParameterInt("rwmf") ;   
    unsigned int s = GetParameterInt("sense") ;
    unsigned int range = GetParameterInt("range") ;
    unsigned int tol = GetParameterInt("tol") ;
    unsigned int choice = GetParameterInt("choice") ;

    std::cout << "dispMin : " << dispMin << std::endl ;
    std::cout << "dispMax : " << dispMax << std::endl ;

  // GRADIENT CALCULATIONS
  // ConvolutionImageFilter 
    // Gradient X  
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

    m_LeftGrayVectorImage->SetInput(inLeft);
    m_RightGrayVectorImage->SetInput(inRight);  
      //--Left---------------  
    m_GradientXLeft->SetFilter(m_convFilterXLeft);
    m_GradientXLeft->SetInput(inLeft);
    // m_GradientXLeft->SetInput(m_LeftGrayVectorImage->GetOutput());
    m_GradientXLeft->UpdateOutputInformation(); 
      //--Right--------------- 
    m_GradientXRight->SetFilter(m_convFilterXRight);
    m_GradientXRight->SetInput(inRight);
    // m_GradientXRight->SetInput(m_RightGrayVectorImage->GetOutput());
    m_GradientXRight->UpdateOutputInformation(); 


    // COST VOLUME  
    if(s==0)
      {          // --- LEFT
      m_LeftCost->SetLeftInputImage(inLeft);
      m_LeftCost->SetRightInputImage(inRight);  
      m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
      m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
      m_LeftCost->SetMinDisp(dispMin);
      m_LeftCost->SetMaxDisp(dispMax); 
      m_LeftCost->UpdateOutputInformation(); 
      //   // --- RIGHT
      m_RightCost->SetLeftInputImage(inRight);
      m_RightCost->SetRightInputImage(inLeft );  
      m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
      m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
      m_RightCost->SetMinDisp(-dispMax);
      m_RightCost->SetMaxDisp(-dispMin);     
      m_RightCost->UpdateOutputInformation();  
      }
    else
      {
      m_LeftCost->SetLeftInputImage(inLeft);
      m_LeftCost->SetRightInputImage(inRight);  
      m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
      m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
      m_LeftCost->SetMinDisp(-dispMax);
      m_LeftCost->SetMaxDisp(-dispMin); 
      m_LeftCost->UpdateOutputInformation(); 
      //   // --- RIGHT
      m_RightCost->SetLeftInputImage(inRight);
      m_RightCost->SetRightInputImage(inLeft );  
      m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
      m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
      m_RightCost->SetMinDisp(dispMin);
      m_RightCost->SetMaxDisp(dispMax);     
      m_RightCost->UpdateOutputInformation();   
      }
  

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
    m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput()); 
    m_LeftDisparity->UpdateOutputInformation(); 
        // --- RIGHT
    m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());  
    m_RightDisparity->UpdateOutputInformation(); 

    if(choice == 1)
      {
     
      if(s==0)
        {
        m_DispValueFilter->SetInput(m_LeftDisparity->GetOutput());
        m_DispValueFilter->SetDisp(dispMax);
        SetParameterOutputImage("io.out", m_LeftDisparity->GetOutput());
        }
      else
        {
          SetParameterOutputImage("io.out", m_RightDisparity->GetOutput());
        // m_DispValueFilter->SetInput(m_RightDisparity->GetOutput());
        // m_DispValueFilter->SetDisp(dispMin);
        }
      // SetParameterOutputImage("io.out", m_DispValueFilter->GetOutput());
      }
    else
      {

      //FILTRAGE LEFT DISPARITY PAR FILTRE MEDIAN
      m_CastLeftDisparity-> SetInput(  m_LeftDisparity->GetOutput());
      m_CastRightDisparity->SetInput( m_RightDisparity->GetOutput());

      MedianFilterType::RadiusType MedianRadius = {{rwmf,rwmf}};
      LMedian->SetRadius(MedianRadius); 

      m_LeftMedianFilter->SetFilter(LMedian);
      m_LeftMedianFilter->SetInput(inLeft); 

      m_RightMedianFilter->SetFilter(LMedian);
      m_RightMedianFilter->SetInput(inRight);

      if(s==0)
        {

        m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
        m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparity->GetOutput());
        m_OcclusionFilter->SetMaxHDisp(0);
        m_OcclusionFilter->SetMinHDisp(dispMin); 
        // m_OcclusionFilter->SetMaxHDisp(0);
        // m_OcclusionFilter->SetMinHDisp(-(dispMax-dispMin));
        }
      else
        {
        m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_RightDisparity->GetOutput()); 
        m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
        m_OcclusionFilter->SetMaxHDisp(dispMax);
        m_OcclusionFilter->SetMinHDisp(0);
        }

      m_OcclusionFilter->SetMinVDisp(0);
      m_OcclusionFilter->SetMaxVDisp(0);
      m_OcclusionFilter->SetTolerance(tol);

      if(choice == 2)
        {
        SetParameterOutputImage("io.out", m_OcclusionFilter->GetOutput());
        }
      else
        {
        m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_OcclusionFilter->GetOutput() )); 

        if(s==0)
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

            
        if(range==1)
          {
          m_convertSmoothDisparity->SetInput(m_FillOcc->GetOutput());
          if(s==0)
            {
            m_convertSmoothDisparity->SetDispMin(dispMin);
            m_convertSmoothDisparity->SetDispMax(dispMax); 
            m_convertSmoothDisparity->SetOffset(0);
            }
          else
            {
            m_convertSmoothDisparity->SetDispMin(-dispMax);
            m_convertSmoothDisparity->SetDispMax(-dispMin); 
            m_convertSmoothDisparity->SetOffset(255); 
            }
          
          SetParameterOutputImage("io.out", m_convertSmoothDisparity->GetOutput());
          }
        else
          {
          m_DispValueFilter->SetInput(m_FillOcc->GetOutput());
          if(s==0)
            {
            m_DispValueFilter->SetDisp(dispMax);
            }
          else
            {
            m_DispValueFilter->SetDisp(dispMin);
            }
          SetParameterOutputImage("io.out", m_DispValueFilter->GetOutput());
          }
        }   
      }      
    }

    ConvFilterType::Pointer m_convFilterXLeft ;
    ConvFilterType::Pointer m_convFilterXRight ;
    RGBTograylevelFilter::Pointer m_LeftGrayVectorImage ;
    RGBTograylevelFilter::Pointer m_RightGrayVectorImage ;
    VectorFilterType::Pointer m_GradientXLeft ;
    VectorFilterType::Pointer m_GradientXRight;
    LeftCostVolumeType::Pointer m_LeftCost;
    RightCostVolumeType::Pointer m_RightCost ;
    Weights_ak_bk::Pointer m_meanLeftCost;
    Weights_ak_bk::Pointer m_meanRightCost;
    MeanVectorImage::Pointer m_meanLeftWeights;
    MeanVectorImage::Pointer m_meanRightWeights ;
    MinCostVolume::Pointer m_LeftDisparity ;
    MaxCostVolume::Pointer m_RightDisparity ;
    CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity;
    CastIntImageIntoFloatVecImageFilter::Pointer m_CastRightDisparity ;
    PerBand::Pointer m_LeftMedianFilter ;
    PerBand::Pointer m_RightMedianFilter;
    MedianFilterType::Pointer LMedian ;
    OcclusionType::Pointer m_OcclusionFilter ;
    CastIntImageIntoFloatVecImageFilter::Pointer m_CastOccMap;
    ConcatenateVectorImageFilterType::Pointer m_ConcatenateMedianImageAndOccMap ;
    ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndInputImage;
    FillOccFilter::Pointer m_FillOcc ;
    CastIntImageIntoFloatVecImageFilter::Pointer m_CastFillOccFilter;
    ConvertValue::Pointer m_convertSmoothDisparity ;
    ConvertToDisparityValue::Pointer m_DispValueFilter ;
 
  }; //end class

} // end Wrapper 
} //end otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CVF)