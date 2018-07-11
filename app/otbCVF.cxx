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

#include "otbBijectionCoherencyFilter.h"
#include "otbFillOcclusionPixel.h"

#include "otbImageToVectorImageCastFilter.h"
#include "otbFillOcclusionImageFilter.h"
#include "otbConvertValueFrom0To255.h"
#include "otbConvertionRGBToGrayLevelImageFilter.h"



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
  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastImageFilter;
  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  typedef otb::FillPixelFilter< FloatVectorImageType, IntImageType > FillOccFilter ;
  typedef otb::ConvertValueFrom0To255<FloatVectorImageType, IntImageType > ConvertValue ;

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
    
    AddParameter(ParameterType_Int,"rwmf", "Radiusf or median filter");
    SetParameterDescription("rwmf", "Radius for weighted median filter");
    SetDefaultParameterInt("rwmf",19);
  
    AddRAMParameter();
    // Doc example parameter settings
    SetDocExampleParameterValue("io.inleft","StereoFixed.png");
    SetDocExampleParameterValue("io.inright","StereoMoving.png");
    SetDocExampleParameterValue("dmin","-12");
    SetDocExampleParameterValue("dmax","-4");
    SetDocExampleParameterValue("rwmf","9");
    SetDocExampleParameterValue("radius","9");
    SetDocExampleParameterValue("io.out","MyCVFDisparity.tif");
    }



    void DoUpdateParameters() ITK_OVERRIDE {}


    void DoExecute() ITK_OVERRIDE
    { 
    FloatVectorImageType::Pointer inLeft = GetParameterFloatVectorImage("io.inleft");
    inLeft->UpdateOutputInformation();
    FloatVectorImageType::Pointer inRight = GetParameterFloatVectorImage("io.inright");
    inRight->UpdateOutputInformation();

    int dispMin = GetParameterInt("dmin");
    int dispMax = GetParameterInt("dmax");
    unsigned int r  = GetParameterInt("radius");
    unsigned int rwmf = GetParameterInt("rwmf") ;  

    m_convFilterXLeft = ConvFilterType::New();
    m_convFilterXRight = ConvFilterType::New();

    // GRADIENT CALCULATIONS OF LEFT AND RIGHT IMAGES(x direction)
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

    m_LeftGrayVectorImage = RGBTograylevelFilter::New();
    m_LeftGrayVectorImage->SetInput(inLeft);
    m_RightGrayVectorImage = RGBTograylevelFilter::New();
    m_RightGrayVectorImage->SetInput(inRight);

    m_GradientXLeft = VectorFilterType::New();
    m_GradientXLeft->SetFilter(m_convFilterXLeft);
    m_GradientXLeft->SetInput(m_LeftGrayVectorImage->GetOutput());
    m_GradientXLeft->UpdateOutputInformation(); 

    m_GradientXRight = VectorFilterType::New();
    m_GradientXRight->SetFilter(m_convFilterXRight);
    m_GradientXRight->SetInput(m_RightGrayVectorImage->GetOutput());
    m_GradientXRight->UpdateOutputInformation(); 

    // COST VOLUME 
      // --- LEFT
    m_LeftCost = LeftCostVolumeType::New();
    m_LeftCost->SetLeftInputImage(inLeft );
    m_LeftCost->SetRightInputImage(inRight );  
    m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
    m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
    m_LeftCost->SetMinDisp(dispMin);
    m_LeftCost->SetMaxDisp(dispMax); 
      //   // --- RIGHT
    m_RightCost = RightCostVolumeType::New();
    m_RightCost->SetLeftInputImage(inRight);
    m_RightCost->SetRightInputImage(inLeft);  
    m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
    m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
    m_RightCost->SetMinDisp(-dispMax);
    m_RightCost->SetMaxDisp(-dispMin);    

    //WEIGHTS  CALCULATIONS
      // --- LEFT
    m_meanLeftCost = Weights_ak_bk::New(); 
    m_meanLeftCost->SetInput1(inLeft);
    m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
    m_meanLeftCost->SetRadius(0,r);  
     // --- RIGHT
    m_meanRightCost = Weights_ak_bk::New(); 
    m_meanRightCost->SetInput1(inRight);
    m_meanRightCost->SetInput2(m_RightCost->GetOutput());  
    m_meanRightCost->SetRadius(0,r);  


    // MEAN OF WEIGHTS  
      // --- LEFT
    m_meanLeftWeights = MeanVectorImage::New();
    m_meanLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
    m_meanLeftWeights->SetInput2(inLeft);
    m_meanLeftWeights->SetRadius(0,r);  
      // --- RIGHT
    m_meanRightWeights = MeanVectorImage::New();
    m_meanRightWeights->SetInput1(m_meanRightCost->GetOutput());
    m_meanRightWeights->SetInput2(inRight);
    m_meanRightWeights->SetRadius(0,r);  


    // DISPARITY MAP
      // --- LEFT

    m_LeftDisparity = MinCostVolume::New();
    m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput());
      // --- RIGHT 
    m_RightDisparity = MaxCostVolume::New();
    m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());  


    // MEDIAN OF LEFT IMAGE
    m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
    m_CastLeftDisparity-> SetInput(  m_LeftDisparity->GetOutput());

    LMedian = MedianFilterType::New();
    MedianFilterType::RadiusType MedianRadius = {{rwmf,rwmf}};
    LMedian->SetRadius(MedianRadius); 
    m_inLeftMedian = PerBand::New();
    m_inLeftMedian->SetFilter(LMedian);
    m_inLeftMedian->SetInput(inLeft); 


    // OCCLUSION DETECTION
    m_OcclusionFilter = OcclusionType::New();  
    m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
    m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparity->GetOutput()); 
     
    m_OcclusionFilter->SetMaxHDisp(dispMax);
    m_OcclusionFilter->SetMinHDisp(dispMin);
    m_OcclusionFilter->SetMinVDisp(0);
    m_OcclusionFilter->SetMaxVDisp(0);
    m_OcclusionFilter->SetTolerance(2);

    // OCCLUSION FILLING 
    m_CastOccMap = CastImageFilter::New();
    m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_OcclusionFilter->GetOutput() ));

    m_CastLeftDisparity= CastImageFilter::New();
    m_CastLeftDisparity-> SetInput( const_cast <IntImageType *>( m_LeftDisparity->GetOutput() ));

    m_ConcatenateLeftMedianIAndOccMap = ConcatenateVectorImageFilterType::New();
    m_ConcatenateLeftMedianIAndOccMap->SetInput1(m_inLeftMedian->GetOutput());
    m_ConcatenateLeftMedianIAndOccMap->SetInput2(m_CastOccMap->GetOutput());

    m_ConcatenateCastOccMapAndLeftImage = ConcatenateVectorImageFilterType::New();
    m_ConcatenateCastOccMapAndLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
    m_ConcatenateCastOccMapAndLeftImage->SetInput2(m_ConcatenateLeftMedianIAndOccMap->GetOutput());

    m_FillOcc = FillOccFilter::New();
    m_FillOcc->SetInput(m_ConcatenateCastOccMapAndLeftImage->GetOutput());
    FloatVectorImageType::SizeType radiusM;
    radiusM[0] = rwmf;
    radiusM[1] = rwmf;   
    m_FillOcc->SetRadius(radiusM);

    m_CastFillOccFilter= CastImageFilter::New();
    m_CastFillOccFilter-> SetInput( m_FillOcc->GetOutput());

    m_convertSmoothDisparity = ConvertValue::New();
    m_convertSmoothDisparity->SetInput(m_CastFillOccFilter->GetOutput());
    m_convertSmoothDisparity->SetDispMin(dispMin);
    m_convertSmoothDisparity->SetDispMax(dispMax);

    SetParameterOutputImage("io.out", m_convertSmoothDisparity->GetOutput());

  }

  ConvFilterType::Pointer m_convFilterXLeft;
  ConvFilterType::Pointer m_convFilterXRight;
  RGBTograylevelFilter::Pointer m_LeftGrayVectorImage;
  RGBTograylevelFilter::Pointer m_RightGrayVectorImage ;
  VectorFilterType::Pointer m_GradientXLeft;
  VectorFilterType::Pointer m_GradientXRight;
    // --- LEFT;
  LeftCostVolumeType::Pointer m_LeftCost ;
  RightCostVolumeType::Pointer m_RightCost;
  Weights_ak_bk::Pointer m_meanLeftCost;
  Weights_ak_bk::Pointer m_meanRightCost ;
  MeanVectorImage::Pointer m_meanLeftWeights ;
  MeanVectorImage::Pointer m_meanRightWeights ;
  MinCostVolume::Pointer m_LeftDisparity;
  MaxCostVolume::Pointer m_RightDisparity ;
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity;
  MedianFilterType::Pointer LMedian;
  PerBand::Pointer m_inLeftMedian;

   OcclusionType::Pointer m_OcclusionFilter;
  CastImageFilter::Pointer m_CastOccMap;

  ConcatenateVectorImageFilterType::Pointer m_ConcatenateLeftMedianIAndOccMap ;
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndLeftImage;

   CastImageFilter::Pointer m_CastFillOccFilter;
 ConvertValue::Pointer m_convertSmoothDisparity;
 FillOccFilter::Pointer m_FillOcc  ;

  }; //end class

} // end Wrapper 
} //end otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CVF)