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
#include "otbBijectionCoherencyFilter.h"
#include "otbFillOcclusionDisparityImageFilter.h"

#include <otbImageToVectorImageCastFilter.h>
#include <itkMedianImageFilter.h>
#include <otbPerBandVectorImageFilter.h>

#include <itkConstantBoundaryCondition.h>
#include <otbConvolutionImageFilter.h>
#include <itkArray.h>


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
  // typedef itk::SmartPointer<Self>       Pointer;
  // typedef itk::SmartPointer<const Self> ConstPointer;    
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
    SetDefaultParameterInt("dmin",-15);

    AddParameter(ParameterType_Int,"dmax","Maximum horizontal disparity");
    SetParameterDescription("dmax","Maximum horizontal disparity to explore (can be negative)");
    SetDefaultParameterInt("dmax",0);
    
    AddParameter(ParameterType_Int,"rwmf", "Radiusf or median filter");
    SetParameterDescription("rwmf", "Radius for median filter");
    SetDefaultParameterInt("rwmf",19);
  

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.inleft","StereoFixed.png");
    SetDocExampleParameterValue("io.inright","StereoMoving.png");
    SetDocExampleParameterValue("dmin","-15");
    SetDocExampleParameterValue("dmax","0");
    SetDocExampleParameterValue("rwmf","19");
    SetDocExampleParameterValue("radius","9");
    SetDocExampleParameterValue("io.out","MyCVFDisparity.tif");


    }



  CVF() {}

  void DoUpdateParameters() ITK_OVERRIDE {}


  void DoExecute() ITK_OVERRIDE
    { 
    const unsigned int Dimension = 2;
   // typedef otb::VectorImage<float> FloatVectorImageType;
    // typedef otb::VectorImage<int> IntVectorImageType;
   // typedef otb::Image< float, Dimension > FloatImageType;
    typedef otb::Image< int, Dimension > IntImageType;

    // typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorImageReaderType;
    // typedef otb::ImageFileWriter<FloatVectorImageType> FloatVectorImageWriterType;

    // typedef otb::ImageFileReader<IntVectorImageType> IntVectorImageReaderType;
    // typedef otb::ImageFileWriter<IntVectorImageType> IntVectorImageWriterType;    
    // typedef otb::ImageFileReader< IntImageType > IntImageReaderType;
    // typedef otb::ImageFileWriter< IntImageType > IntImageWriterType;  
    // typedef otb::ImageFileReader< FloatImageType > FloatImageReaderType;
    // typedef otb::ImageFileWriter< FloatImageType > FloatImageWriterType; 

    FloatVectorImageType::Pointer inLeft = GetParameterFloatVectorImage("io.inleft");
    FloatVectorImageType::Pointer inRight = GetParameterFloatVectorImage("io.inright");
    int dispMin = GetParameterInt("dmin");
    int dispMax = GetParameterInt("dmax");
    unsigned int r  = GetParameterInt("radius");
    int rwmf = GetParameterInt("rwmf") ;


    //   // CALCUL GRADIENT
    // typedef otb::LocalGradientVectorImageFilter<FloatVectorImageType, FloatVectorImageType> GradientType;
    // GradientType::Pointer m_GradientXLeft = GradientType::New();
    // GradientType::Pointer m_GradientXRight = GradientType::New();
    // m_GradientXLeft->SetInput(inLeft);
    // m_GradientXRight->SetInput(inRight);
    // m_GradientXRight->Update();
    // m_GradientXLeft->Update();



   // GRADIENT CALCULATIONS  
    typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionType;
    typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionType> ConvFilterType;
    ConvFilterType::Pointer m_convFilterXLeft = ConvFilterType::New();
    ConvFilterType::Pointer m_convFilterXRight = ConvFilterType::New();

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
 
    typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterType> VectorFilterType;
    VectorFilterType::Pointer m_GradientXLeft = VectorFilterType::New();
    m_GradientXLeft->SetFilter(m_convFilterXLeft);
    m_GradientXLeft->SetInput(inLeft);
    m_GradientXLeft->Update();

    VectorFilterType::Pointer m_GradientXRight = VectorFilterType::New();
    m_GradientXRight->SetFilter(m_convFilterXRight);
    m_GradientXRight->SetInput(inRight);
    m_GradientXRight->Update();


    // COST VOLUME  
    typedef otb::LeftCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  LeftCostVolumeType; 
      // --- LEFT
    LeftCostVolumeType::Pointer m_LeftCost = LeftCostVolumeType::New();
    m_LeftCost->SetLeftInputImage(inLeft);
    m_LeftCost->SetRightInputImage(inRight);  
    m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
    m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
    m_LeftCost->SetMinDisp(dispMin);
    m_LeftCost->SetMaxDisp(dispMax);


   
      // --- RIGHT
    typedef otb::RightCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > RightCostVolumeType; 
    RightCostVolumeType::Pointer m_RightCost = RightCostVolumeType::New();
    m_RightCost->SetLeftInputImage(inRight );
    m_RightCost->SetRightInputImage(inLeft);  
    m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
    m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
    m_RightCost->SetMinDisp(-dispMax);
    m_RightCost->SetMaxDisp(-dispMin);




      

    // COST VOLUME WEIGHTS 
    typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
      // --- LEFT
    Weights_ak_bk::Pointer m_meanLeftCost = Weights_ak_bk::New(); 
    m_meanLeftCost->SetInput1(inLeft);
    m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
    m_meanLeftCost->SetRadius(0,r); 
  //  m_meanLeftCost->Update();
      // --- RIGHT
    Weights_ak_bk::Pointer m_meanRightCost = Weights_ak_bk::New(); 
    m_meanRightCost->SetInput1(inRight);
    m_meanRightCost->SetInput2(m_RightCost->GetOutput());  
    m_meanRightCost->SetRadius(0,r);
  //  m_meanRightCost->Update();



    //MEAN WEIGHTS   
    typedef otb::MeanVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > MeanVectorImage;
      // --- LEFT
    MeanVectorImage::Pointer m_meanLeftWeights = MeanVectorImage::New();
    m_meanLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
    m_meanLeftWeights->SetInput2(inLeft);
    m_meanLeftWeights->SetRadius(0,r);

      // --- RIGHT
    MeanVectorImage::Pointer m_meanRightWeights = MeanVectorImage::New();
    m_meanRightWeights->SetInput1(m_meanRightCost->GetOutput());
    m_meanRightWeights->SetInput2(inRight);
    m_meanRightWeights->SetRadius(0,r);



    //DISPARITY MAP
      // --- LEFT
    typedef otb::MinimumNBandsImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;  
    MinCostVolume::Pointer m_LeftDisparity = MinCostVolume::New();
    m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput());



      // --- RIGHT
    typedef otb::MaximumNBandsImageFilter< FloatVectorImageType, IntImageType > MaxCostVolume;  
    MaxCostVolume::Pointer m_RightDisparity = MaxCostVolume::New();
    m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());




 

    //FILTRAGE LEFT DISPARITY PAR FILTRE MEDIAN    
    typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastIntImageIntoFloatVecImageFilter;
    CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
    m_CastLeftDisparity-> SetInput( const_cast <IntImageType *>( m_LeftDisparity->GetOutput() ));

    typedef itk::MedianImageFilter< FloatImageType, FloatImageType > MedianFilterType;
    MedianFilterType::Pointer LMedian = MedianFilterType::New();
    MedianFilterType::RadiusType MedianRadius = {{1,1}};
    LMedian->SetRadius(MedianRadius); 

    typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MedianFilterType> PerBand; 
    PerBand::Pointer m_LeftMedianFilter = PerBand::New();
    m_LeftMedianFilter->SetFilter(LMedian);
    m_LeftMedianFilter->SetInput(inLeft); 
    typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
    ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
    m_ConcatenateDispEndInLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
    m_ConcatenateDispEndInLeftImage->SetInput2(m_LeftMedianFilter->GetOutput());

    typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatImageType > WeightMedianType;
    WeightMedianType::Pointer m_LeftDispMedian = WeightMedianType::New();
    m_LeftDispMedian-> SetInput(m_ConcatenateDispEndInLeftImage->GetOutput());
    
    FloatVectorImageType::SizeType radiusWM;
    radiusWM[0] = r;
    radiusWM[1] = r;   
    m_LeftDispMedian->SetRadius(radiusWM) ;


    //FILTRAGE LEFT DISPARITY PAR FILTRE MEDIAN
    CastIntImageIntoFloatVecImageFilter::Pointer m_CastRightDisparity = CastIntImageIntoFloatVecImageFilter::New();
    m_CastRightDisparity-> SetInput( const_cast <IntImageType *>( m_RightDisparity->GetOutput() ));
    PerBand::Pointer m_RightMedianFilter = PerBand::New();
    m_RightMedianFilter->SetFilter(LMedian);
    m_RightMedianFilter->SetInput(inRight);    
    ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
    m_ConcatenateDispEndInRightImage->SetInput1(m_CastRightDisparity->GetOutput());
    m_ConcatenateDispEndInRightImage->SetInput2(m_RightMedianFilter->GetOutput());
    WeightMedianType::Pointer m_RightDispMedian = WeightMedianType::New();
    m_RightDispMedian-> SetInput(m_ConcatenateDispEndInRightImage->GetOutput());    
    m_RightDispMedian->SetRadius(radiusWM) ;


    typedef otb::BijectionCoherencyFilter< FloatImageType, FloatImageType > OcclusionType;
    OcclusionType::Pointer m_OcclusionFilter = OcclusionType::New();  
    m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_RightDispMedian->GetOutput()); 
    m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_LeftDispMedian->GetOutput()); 
     
    m_OcclusionFilter->SetMaxHDisp(dispMax);
    m_OcclusionFilter->SetMinHDisp(dispMin);
    m_OcclusionFilter->SetMinVDisp(0);
    m_OcclusionFilter->SetMaxVDisp(0);
    m_OcclusionFilter->SetTolerance(2);



    typedef otb::FillOcclusionDisparityImageFilter<FloatImageType, FloatImageType, FloatImageType> FillOcclusionFilter ;
    FillOcclusionFilter::Pointer m_FillOccDisparityMap = FillOcclusionFilter::New();
    m_FillOccDisparityMap->SetInput1(m_OcclusionFilter->GetOutput() );
    m_FillOccDisparityMap->SetInput2(m_LeftDispMedian->GetOutput() );
    m_FillOccDisparityMap->SetRadius(0,1);
    m_FillOccDisparityMap->SetDispMin(dispMin);
    m_FillOccDisparityMap->SetDispMax(dispMax);


    typedef otb::ImageToVectorImageCastFilter<FloatImageType,FloatVectorImageType> CastImageFilter;
    CastImageFilter::Pointer m_CastFillOccDisparityMap = CastImageFilter::New();
    m_CastFillOccDisparityMap-> SetInput( const_cast <FloatImageType *>( m_FillOccDisparityMap->GetOutput() ));

    typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
    ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndLeftImage = ConcatenateVectorImageFilterType::New();
    m_ConcatenateCastOccMapAndLeftImage->SetInput1(m_CastFillOccDisparityMap->GetOutput());
    m_ConcatenateCastOccMapAndLeftImage->SetInput2(inLeft);

    typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatImageType > WeightMedianFilter;
    WeightMedianFilter::Pointer m_WeightOccMapAndLeftImageFilter = WeightMedianFilter::New();
    m_WeightOccMapAndLeftImageFilter->SetInput(m_ConcatenateCastOccMapAndLeftImage->GetOutput());

    FloatVectorImageType::SizeType radiusM;
    radiusM[0] = rwmf;
    radiusM[1] = rwmf;   
    m_WeightOccMapAndLeftImageFilter->SetRadius(radiusM);
    m_WeightOccMapAndLeftImageFilter->Update();



    SetParameterOutputImage("io.out", m_WeightOccMapAndLeftImageFilter->GetOutput());






    }  
  }; //end class

} // end Wrapper 
} //end otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CVF)