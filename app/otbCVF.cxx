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


  /*
  typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionType> ConvFilterType;

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

  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  typedef otb::WeightMedianImageFilter< FloatVectorImageType, IntImageType > WeightMedianType;
  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;

   typedef otb::FillOcclusionDisparityImageFilter<IntImageType, IntImageType, IntImageType> FillOcclusionFilter ;
   typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastImageFilter;



  typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatVectorImageType > WeightMedianFilter;
  typedef otb::ConvertValueFrom0To255<FloatVectorImageType, IntImageType > ConvertValue ;

  typedef otb::ConvertionRGBToGrayLevelImageFilter<FloatVectorImageType,FloatVectorImageType> RGBTograylevelFilter ;


*/


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
    SetParameterDescription("rwmf", "Radius for median filter");
    SetDefaultParameterInt("rwmf",19);
  

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.inleft","StereoFixed.png");
    SetDocExampleParameterValue("io.inright","StereoMoving.png");
    SetDocExampleParameterValue("dmin","-12");
    SetDocExampleParameterValue("dmax","-4");
    SetDocExampleParameterValue("rwmf","19");
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


  std::cout << "disMin : " << dispMin << std::endl ;
  std::cout << "disMax : " << dispMax << std::endl ;



 m_convFilterXLeft = ConvFilterType::New();
m_convFilterXRight = ConvFilterType::New();

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



 m_LeftGrayVectorImage = RGBTograylevelFilter::New();
  m_LeftGrayVectorImage->SetInput(inLeft);
 m_RightGrayVectorImage = RGBTograylevelFilter::New();
  m_RightGrayVectorImage->SetInput(inRight);

  
// //--Left---------------  

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


  //WEIGHTS  

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


  //MEAN WEIGHTS   

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


  //DISPARITY MAP
    // --- LEFT

 m_LeftDisparity = MinCostVolume::New();
  m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput());

      // --- RIGHT
 
 m_RightDisparity = MaxCostVolume::New();
  m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());  


  //FILTRAGE LEFT DISPARITY PAR FILTRE MEDIAN

m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
  m_CastLeftDisparity-> SetInput(  m_LeftDisparity->GetOutput());



LMedian = MedianFilterType::New();
  MedianFilterType::RadiusType MedianRadius = {{rwmf,rwmf}};
  LMedian->SetRadius(MedianRadius); 


      // ---LEFT
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
  




m_CastOccMap = CastImageFilter::New();
  m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_OcclusionFilter->GetOutput() ));



 m_CastLeftDisparity= CastImageFilter::New();
  m_CastLeftDisparity-> SetInput( const_cast <IntImageType *>( m_LeftDisparity->GetOutput() ));




m_ConcatenateLeftMedianIAndOccMap = ConcatenateVectorImageFilterType::New();
  m_ConcatenateLeftMedianIAndOccMap->SetInput1(m_inLeftMedian->GetOutput());
  m_ConcatenateLeftMedianIAndOccMap->SetInput2(m_CastOccMap->GetOutput());

  // FloatVectorImageWriterType::Pointer writer_ConcatenateLeftMedianIAndOccMap = FloatVectorImageWriterType::New();
  // writer_ConcatenateLeftMedianIAndOccMap->SetFileName( FILENAME("ConcatenateLeftMedianIAndOccMap.tif"));
  // writer_ConcatenateLeftMedianIAndOccMap->SetInput( m_ConcatenateLeftMedianIAndOccMap->GetOutput() ); 
  // writer_ConcatenateLeftMedianIAndOccMap->Update();



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














/*

  m_convFilterXLeft = ConvFilterType::New();
  m_convFilterXRight = ConvFilterType::New();

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


  m_LeftGrayVectorImage = RGBTograylevelFilter::New();
  m_LeftGrayVectorImage->SetInput(inLeft);
  m_RightGrayVectorImage = RGBTograylevelFilter::New();
  m_RightGrayVectorImage->SetInput(inRight);

  
//--Left---------------  
  m_GradientXLeft = VectorFilterType::New();
  // std::cout << " Begin Gradient Left "  << std::endl ;
  m_GradientXLeft->SetFilter(m_convFilterXLeft);
  m_GradientXLeft->SetInput(m_LeftGrayVectorImage->GetOutput());
  m_GradientXLeft->UpdateOutputInformation();
  // std::cout << " End Gradient Left "  << std::endl ;
 

  m_GradientXRight = VectorFilterType::New();
  // std::cout << " Begin Gradient Right "  << std::endl ;
  m_GradientXRight->SetFilter(m_convFilterXRight);
  m_GradientXRight->SetInput(m_RightGrayVectorImage->GetOutput());
  m_GradientXRight->UpdateOutputInformation();
  // std::cout << " End Gradient Right "  << std::endl ;

  
  // COST VOLUME      // --- LEFT
  
  m_LeftCost = LeftCostVolumeType::New();
  // std::cout << " Begin Left Cost "  << std::endl ;
  m_LeftCost->SetLeftInputImage(inLeft);
  m_LeftCost->SetRightInputImage(inRight);  
  m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
  m_LeftCost->SetMinDisp(dispMin);
  m_LeftCost->SetMaxDisp(dispMax);
  // std::cout << " End Left Cost "  << std::endl ;
  
 
    // --- RIGHT
  m_RightCost = RightCostVolumeType::New();
  // std::cout << " Begin Right Cost "  << std::endl ;
  m_RightCost->SetLeftInputImage(inRight );
  m_RightCost->SetRightInputImage(inLeft );  
  m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
  m_RightCost->SetMinDisp(-dispMax);
  m_RightCost->SetMaxDisp(-dispMin);
  m_RightCost->UpdateOutputInformation();
  // std::cout << " End Right Cost "  << std::endl ;

  //WEIGHTS  

    // --- LEFT
  m_meanLeftCost = Weights_ak_bk::New(); 
  // std::cout << " Begin m_meanLeftCost "  << std::endl ;
  m_meanLeftCost->SetInput1(inLeft);
  m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
  m_meanLeftCost->SetRadius(0,r);
   // std::cout << " End m_meanLeftCost "  << std::endl ;
   // // --- RIGHT
  m_meanRightCost = Weights_ak_bk::New(); 
  // std::cout << " Begin m_meanRightCost "  << std::endl ;
  m_meanRightCost->SetInput1(inRight);
  m_meanRightCost->SetInput2(m_RightCost->GetOutput());  
  m_meanRightCost->SetRadius(0,r);
  // std::cout << " End m_meanRightCost "  << std::endl ;
  


  //MEAN WEIGHTS   

    // --- LEFT
  m_meanLeftWeights = MeanVectorImage::New();
  // std::cout << " Begin m_meanLeftWeights "  << std::endl ;
  m_meanLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
  m_meanLeftWeights->SetInput2(inLeft);
  m_meanLeftWeights->SetRadius(0,r);
  // std::cout << " End m_meanLeftWeights "  << std::endl ;

    // --- RIGHT
  m_meanRightWeights = MeanVectorImage::New();
  // std::cout << " Begin m_meanRightWeights "  << std::endl ;
  m_meanRightWeights->SetInput1(m_meanRightCost->GetOutput());
  m_meanRightWeights->SetInput2(inRight);
  m_meanRightWeights->SetRadius(0,r);
  // std::cout << " End m_meanRightWeights "  << std::endl ;
  


  //DISPARITY MAP
    // --- LEFT

 m_LeftDisparity = MinCostVolume::New();
  // std::cout << " Begin m_LeftDisparity "  << std::endl ;
  m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput());
  m_LeftDisparity->UpdateOutputInformation();
      // --- RIGHT

  m_RightDisparity = MaxCostVolume::New();
  // std::cout << " Begin m_RightDisparity "  << std::endl ;
  m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());
  m_RightDisparity->UpdateOutputInformation();
  


  // //FILTRAGE LEFT DISPARITY PAR FILTRE MEDIAN
 // std::cout << " Begin m_CastLeftDisparity "  << std::endl ;
   m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
  m_CastLeftDisparity-> SetInput(  m_LeftDisparity->GetOutput());




 LMedian = MedianFilterType::New();
  MedianFilterType::RadiusType MedianRadius = {{rmwf,rmwf}};
  LMedian->SetRadius(MedianRadius); 


      // ---LEFT
  m_inLeftMedian = PerBand::New();
  // std::cout << " Begin m_inLeftMedian "  << std::endl ;
  m_inLeftMedian->SetFilter(LMedian);
  m_inLeftMedian->SetInput(inLeft); 


  m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  // std::cout << " Begin m_ConcatenateDispEndInLeftImage "  << std::endl ;
  m_ConcatenateDispEndInLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
  m_ConcatenateDispEndInLeftImage->SetInput2(m_inLeftMedian->GetOutput());
  m_ConcatenateDispEndInLeftImage->UpdateOutputInformation();



   //Left
    m_LeftDispMedian = WeightMedianType::New();
    // std::cout << " Begin m_LeftDispMedian "  << std::endl ;
   m_LeftDispMedian-> SetInput(m_ConcatenateDispEndInLeftImage->GetOutput());    
   
  FloatVectorImageType::SizeType radiusWM;
  radiusWM[0] = r;
  radiusWM[1] = r;   
  m_LeftDispMedian->SetRadius(radiusWM) ;


  //FAIRE PAREIL POUR RIGHT DISPARITY
   m_CastRightDisparity = CastIntImageIntoFloatVecImageFilter::New();
     // std::cout << " Begin m_CastRightDisparity "  << std::endl ;
  m_CastRightDisparity-> SetInput( m_RightDisparity->GetOutput() );


  m_RightMedianFilter = PerBand::New();
  // std::cout << " Begin m_RightMedianFilter "  << std::endl ;
  m_RightMedianFilter->SetFilter(LMedian);
  m_RightMedianFilter->SetInput(inRight); 

 m_ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
  // std::cout << " Begin m_ConcatenateDispEndInRightImage "  << std::endl ;
  m_ConcatenateDispEndInRightImage->SetInput1(m_CastRightDisparity->GetOutput());
  m_ConcatenateDispEndInRightImage->SetInput2(m_RightMedianFilter->GetOutput());

  m_RightDispMedian = WeightMedianType::New();
  // std::cout << " Begin m_RightDispMedian "  << std::endl ;
  m_RightDispMedian-> SetInput(m_ConcatenateDispEndInRightImage->GetOutput());    
  m_RightDispMedian->SetRadius(radiusWM) ;




  m_OcclusionFilter = OcclusionType::New();  
   // std::cout << " Begin m_OcclusionFilter "  << std::endl ;
  m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_RightDispMedian->GetOutput()); 
  m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_LeftDispMedian->GetOutput()); 
   
  m_OcclusionFilter->SetMaxHDisp(-dispMin);
  m_OcclusionFilter->SetMinHDisp(-dispMax);
  m_OcclusionFilter->SetMinVDisp(0);
  m_OcclusionFilter->SetMaxVDisp(0);
  m_OcclusionFilter->SetTolerance(2);  
  m_OcclusionFilter->UpdateOutputInformation();

   m_FillOccDisparityMap = FillOcclusionFilter::New();
    // std::cout << " Begin m_FillOccDisparityMap "  << std::endl ;
   m_FillOccDisparityMap->SetInput1(m_OcclusionFilter->GetOutput() );
   m_FillOccDisparityMap->SetInput2(m_LeftDispMedian->GetOutput() );
   m_FillOccDisparityMap->SetRadius(0,1);
   m_FillOccDisparityMap->UpdateOutputInformation();


  m_CastOccMap = CastImageFilter::New();
   // std::cout << " Begin m_CastOccMap "  << std::endl ;
  m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_FillOccDisparityMap->GetOutput() ));




   m_ConcatenateCastOccMapAndLeftImage = ConcatenateVectorImageFilterType::New();
    // std::cout << " Begin m_ConcatenateCastOccMapAndLeftImage "  << std::endl ;
  m_ConcatenateCastOccMapAndLeftImage->SetInput1(m_CastOccMap->GetOutput());
  m_ConcatenateCastOccMapAndLeftImage->SetInput2(m_inLeftMedian->GetOutput());
  m_ConcatenateCastOccMapAndLeftImage->UpdateOutputInformation();

  m_WeightOccMapAndLeftImageFilter = WeightMedianFilter::New();
  // std::cout << " Begin m_WeightOccMapAndLeftImageFilter "  << std::endl ;
  m_WeightOccMapAndLeftImageFilter->SetInput(m_ConcatenateCastOccMapAndLeftImage->GetOutput());

  FloatVectorImageType::SizeType radiusM;
  radiusM[0] = rwmf;
  radiusM[1] = rwmf;   
  m_WeightOccMapAndLeftImageFilter->SetRadius(radiusM);
  m_WeightOccMapAndLeftImageFilter->UpdateOutputInformation();




  m_convertSmoothDisparity = ConvertValue::New();
  // std::cout << " Begin m_convertSmoothDisparity "  << std::endl ;
  m_convertSmoothDisparity->SetInput(m_WeightOccMapAndLeftImageFilter->GetOutput());
  m_convertSmoothDisparity->SetDispMin(dispMin);
  m_convertSmoothDisparity->SetDispMax(dispMax);
  m_convertSmoothDisparity->UpdateOutputInformation();


  SetParameterOutputImage("io.out", m_convertSmoothDisparity->GetOutput());
*/



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

  /*
  ConvFilterType::Pointer m_convFilterXLeft;
  ConvFilterType::Pointer m_convFilterXRight;
  VectorFilterType::Pointer m_GradientXLeft;
  VectorFilterType::Pointer m_GradientXRight;
  LeftCostVolumeType::Pointer m_LeftCost ;
  RightCostVolumeType::Pointer m_RightCost ;
Weights_ak_bk::Pointer m_meanLeftCost ;
Weights_ak_bk::Pointer m_meanRightCost;
MeanVectorImage::Pointer m_meanLeftWeights ;
MeanVectorImage::Pointer m_meanRightWeights ;
MinCostVolume::Pointer m_LeftDisparity ;
MaxCostVolume::Pointer m_RightDisparity;
 CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity ;
 MedianFilterType::Pointer LMedian;
PerBand::Pointer m_inLeftMedian ;
ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage;
WeightMedianType::Pointer m_LeftDispMedian;
 
CastIntImageIntoFloatVecImageFilter::Pointer m_CastRightDisparity;
PerBand::Pointer m_RightMedianFilter;
ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInRightImage;

  WeightMedianType::Pointer m_RightDispMedian;
OcclusionType::Pointer m_OcclusionFilter;
FillOcclusionFilter::Pointer m_FillOccDisparityMap ;
 CastImageFilter::Pointer m_CastOccMap;
ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndLeftImage;
WeightMedianFilter::Pointer m_WeightOccMapAndLeftImageFilter;
 ConvertValue::Pointer m_convertSmoothDisparity;

  RGBTograylevelFilter::Pointer m_LeftGrayVectorImage ;
 RGBTograylevelFilter::Pointer m_RightGrayVectorImage;

 */
  }; //end class

} // end Wrapper 
} //end otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CVF)