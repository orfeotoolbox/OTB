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
#include <vector>

#include "otbCostVolumeImageFilter.h"
#include "otbCoeffGuidedBoxImageFilter.h"
#include "otbMinFilter.h"
#include "otbWeightedMedianImageFilter.h"
#include "otbBoxImageFilter.h"
#include "otbLocalCovarianceBoxVectorImageFilter.h"
#include "otbLocalMeanBoxVectorImageFilter.h"
#include "otbCovarianceImageCostVolumeBoxFilter.h"
#include "otbFillOcclusionImageFilter.h"
#include "otbMultiplyImagebandsFilter.h"
#include "otbMultiplyImageAndCostImageFilter.h"
#include "otbGuidedImageFilter.h"



#include <itkImage.h>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbLocalGradientVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbLocalGradientVectorImageFilter.h" 
#include "itkConstNeighborhoodIterator.h" 

#include "otbPerBandVectorImageFilter.h"

#include "otbConvolutionImageFilter.h"

#include "otbMultiplyByScalarImageFilter.h"
#include <itkAddImageFilter.h>
#include "otbThresholdVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h" //muti channel 
#include "otbVectorRescaleIntensityImageFilter.h" //muti channel

#include "otbImageToVectorImageCastFilter.h" // cast Image to vecteur image

#include "otbDisparityMapMedianFilter.h"
#include <itkVectorImageToImageAdaptor.h>

#include "otbStandardFilterWatcher.h"   // pour le watcher
#include "otbBijectionCoherencyFilter.h"
#include "itkCastImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkTimeProbe.h" //crono
#include "otbVectorImageToImageListFilter.h" //Conversion de vectorImageToOTBImage
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
  typedef double     PixelType;
  typedef otb::ImageList<FloatImageType>                   ImageListType;

   
 // class de functor
 typedef otb::LocalMeanBoxVectorImageFilter< FloatVectorImageType, FloatVectorImageType > MeanBoxFilterType;
 typedef otb::LocalCovarianceBoxVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CovBoxFilterType;
 typedef otb::MultiplyImageAndCostImageFilter< FloatVectorImageType,FloatVectorImageType, FloatVectorImageType> MImageAndCostType;
 typedef otb::CovarianceImageCostVolumeBoxFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CovImageWithCostBoxFilterType;
 typedef otb::GuidedImageFilter< FloatVectorImageType,FloatVectorImageType, FloatVectorImageType> GuidedFilterType;
 typedef otb::FillOcclusionImageFilter<FloatVectorImageType,FloatVectorImageType> FillOcclusionType;
 typedef otb::MultiplyImagebandsFilter< FloatVectorImageType, FloatVectorImageType > MultiplyFilterType;
 
 // class sans functor 
  typedef otb::BoxImageFilter< FloatVectorImageType, FloatVectorImageType > BoxFilterType;  
  typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionTypeLeft;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionTypeLeft> ConvFilterTypeLeft;
  typedef itk::ConstantBoundaryCondition<FloatImageType>  BoundaryConditionTypeRight;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionTypeRight> ConvFilterTypeRight;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterTypeLeft> VectorFilterTypeLeft;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterTypeRight> VectorFilterTypeRight;
  typedef otb::LocalGradientVectorImageFilter< FloatVectorImageType, FloatVectorImageType > GradientType;
  typedef otb::CostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CostVolumeType;
  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType,FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;
  typedef otb::CoeffGuidedBoxImageFilter< FloatVectorImageType, FloatVectorImageType> CoeffGuidedFilterType;
  typedef otb::MinFilter< FloatVectorImageType,FloatVectorImageType,FloatVectorImageType> MinFilterType;
  typedef itk::MedianImageFilter< FloatImageType, FloatImageType > MedianFilterType;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MedianFilterType> PerBand;
  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
   FloatVectorImageType::InternalPixelType> ExtractFilterType;
  typedef otb::ImageToVectorImageCastFilter<FloatImageType,FloatVectorImageType> CastFilterType;
  typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatVectorImageType > WeightMedianType;
  typedef otb::VectorImageToImageListFilter< FloatVectorImageType, ImageListType > ListType;
  typedef otb::BijectionCoherencyFilter< FloatImageType, FloatImageType > OcclusionType;
  
  
  
  
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(CostVolumeFilter, otb::Application);

  /** Filters typedef */

private:
  CostVolumeFilter()
  {    
    m_MultiplyLeft = MultiplyFilterType::New();
	m_MultiplyRight = MultiplyFilterType::New();
  
    m_LeftBox = BoxFilterType::New();
    m_RightBox = BoxFilterType::New();
   
    m_MultiplyLeftBox = BoxFilterType::New();
    m_MultiplyRightBox = BoxFilterType::New();
   
   
	m_LeftMeanBoxFilter = MeanBoxFilterType::New();
	m_RightMeanBoxFilter = MeanBoxFilterType::New();
   
	m_MultiplyLeftMeanBoxFilter = MeanBoxFilterType::New();
	m_MultiplyRightMeanBoxFilter = MeanBoxFilterType::New();
    
	m_LeftCovBoxFilter = CovBoxFilterType::New(); 
	m_RightCovBoxFilter = CovBoxFilterType::New();
    
	m_convFilterXL = ConvFilterTypeLeft::New();
	m_convFilterYL = ConvFilterTypeLeft::New();
  
	m_convFilterXR = ConvFilterTypeRight::New();
	m_convFilterYR = ConvFilterTypeRight::New();
  
	m_GradientXL = VectorFilterTypeLeft::New();
	m_GradientXR = VectorFilterTypeRight::New();
    
	m_GradientYL = VectorFilterTypeLeft::New();
	m_GradientYR = VectorFilterTypeRight::New();
    
	m_LeftCost = CostVolumeType::New();
	m_RightCost = CostVolumeType::New();
      
	m_LeftCostBox = BoxFilterType::New();
	m_LeftMeanCostBoxFilter = MeanBoxFilterType::New();
	m_RightCostBox = BoxFilterType::New();
	m_RightMeanCostBoxFilter = MeanBoxFilterType::New();
			
	m_MLeftAndCostFilter = MImageAndCostType::New();
	m_MRightAndCostFilter = MImageAndCostType::New();
       
	m_MLeftAndCostBox = BoxFilterType::New();
	m_MRightAndCostBox = BoxFilterType::New();
         
	m_MLeftAndCostMeanBoxFilter = MeanBoxFilterType::New();
	m_MRightAndCostMeanBoxFilter = MeanBoxFilterType::New();
            
	m_ConcatenateLeftAndCostMean = ConcatenateVectorImageFilterType::New();
	m_ConcatenateRightAndCostMean = ConcatenateVectorImageFilterType::New();
				
	m_CovLeftCostBoxFilter = CovImageWithCostBoxFilterType::New(); 
	m_CovRightCostBoxFilter = CovImageWithCostBoxFilterType::New(); 
		
	m_LeftGuided = CoeffGuidedFilterType::New();
	m_RightGuided = CoeffGuidedFilterType::New();
        
	m_LeftCoeffBox = BoxFilterType::New();
	m_LeftMeanCoeffBoxFilter = MeanBoxFilterType::New();
	m_RightCoeffBox = BoxFilterType::New();
	m_RightMeanCoeffBoxFilter = MeanBoxFilterType::New();

	m_LeftCVFFiltred = GuidedFilterType::New();
	m_RightCVFFiltred = GuidedFilterType::New();
	
	m_LeftConcatenateCostDisp = ConcatenateVectorImageFilterType::New();
    m_RightConcatenateCostDisp = ConcatenateVectorImageFilterType::New();
    
    m_LeftMin = MinFilterType::New();
    m_RightMin = MinFilterType::New();
    
    m_LMedian = MedianFilterType::New();
    m_LeftMedianFilter = PerBand::New();
    m_RMedian = MedianFilterType::New();
      
    m_RightMedianFilter = PerBand::New();
    m_Leftchannel1 = ExtractFilterType::New(); 
    m_Rightchannel1 = ExtractFilterType::New();
        
    m_LeftDisparity = CastFilterType::New();
    m_RightDisparity = CastFilterType::New();
          
	m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
	m_ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
  
	m_LeftMedian = WeightMedianType::New();
	m_RightMedian = WeightMedianType::New();
  
	m_OtbImageLeftMedian = ListType::New();
	m_OtbImageRightMedian = ListType::New();
  
	m_Occlusionfilter = OcclusionType::New();
	m_OccCastFiler = CastFilterType::New();
  
	m_ConcatenateOccEndInLeftMedImage = ConcatenateVectorImageFilterType::New();
	m_FillOcclusionfilter = FillOcclusionType::New();

	m_ConcatenateFillEndInLeftMedImage = ConcatenateVectorImageFilterType::New();
	m_FillMedian = WeightMedianType::New();

   
  }

  void DoInit() ITK_OVERRIDE
  {
    SetName("CostVolumeFilter");
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

    AddParameter(ParameterType_OutputImage, "io.out", "The output disparity map");
    SetParameterDescription("io.out","An image containing the estimated disparities");

    AddParameter(ParameterType_Int,"radius","Radius of Mean, gradient and the guided Filter");
    SetParameterDescription("radius","The radius of Mean, gradient and the guided Filter.");
    SetDefaultParameterInt("radius",9.);
  

    AddParameter(ParameterType_Int,"hdispmin","Minimum horizontal disparity");
    SetParameterDescription("hdispmin","Minimum horizontal disparity to explore (can be negative)");
	SetDefaultParameterInt("hdispmin",-15);

    AddParameter(ParameterType_Int,"hdispmax","Maximum horizontal disparity");
    SetParameterDescription("hdispmax","Maximum horizontal disparity to explore (can be negative)");
	SetDefaultParameterInt("hdispmax",0);
    
    AddParameter(ParameterType_Int,"rmf", "Radiusf or median filter");
    SetParameterDescription("rmf", "Radius for median filter");
     SetDefaultParameterInt("rmf",19.);
  

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.inleft","StereoFixed.png");
    SetDocExampleParameterValue("io.inright","StereoMoving.png");
    SetDocExampleParameterValue("hdispmin","-15");
    SetDocExampleParameterValue("hdispmax","0");
    SetDocExampleParameterValue("rmf","19");
    SetDocExampleParameterValue("radius","9");
    SetDocExampleParameterValue("io.out","MyCVFDisparity.tif");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    
  }

  void DoExecute() ITK_OVERRIDE
  {
    FloatVectorImageType::Pointer leftImage = GetParameterFloatVectorImage("io.inleft");
    FloatVectorImageType::Pointer rightImage = GetParameterFloatVectorImage("io.inright");
 

    unsigned int R  = GetParameterInt("radius");
    int          HdispMin = GetParameterInt("hdispmin");
    int          HdispMax = GetParameterInt("hdispmax");


/*============================== calcul de la multiplication pour la covariance =======*/
// covariance = Mean( im1*im2) - Mean(im1)*mean(im2)  
  m_MultiplyLeft->SetInput( leftImage );
  m_MultiplyRight->SetInput( rightImage );
 

 /*==================================== calcul de l'image intégrales======================*/ 
  m_LeftBox->SetInput( leftImage );  
  m_RightBox->SetInput( rightImage );  
  
  m_MultiplyLeftBox->SetInput( m_MultiplyLeft->GetOutput() );  
  m_MultiplyRightBox->SetInput( m_MultiplyRight->GetOutput() );
   
  
   /*==================================== calcul des moyennes avec le boxFilter=============================*/
  m_LeftMeanBoxFilter->SetInput( m_LeftBox->GetOutput() );  
  MeanBoxFilterType::RadiusType radius = {{R,R}};
  m_LeftMeanBoxFilter->SetRadius(radius);
  
  m_RightMeanBoxFilter->SetInput( m_RightBox->GetOutput() );  
  m_RightMeanBoxFilter->SetRadius(radius);
  
   
  // The mean multiplication
  
  m_MultiplyLeftMeanBoxFilter->SetInput( m_MultiplyLeftBox->GetOutput() );
  m_MultiplyLeftMeanBoxFilter->SetRadius(radius);
  
   
  m_MultiplyRightMeanBoxFilter->SetInput( m_MultiplyRightBox->GetOutput() );
  m_MultiplyRightMeanBoxFilter->SetRadius(radius);

 
  //=============================== Calcul de la covariance avec le boxFiler ======================*/  
  /** remarque: on a pas besoin de mettre le radius car le radius est utilisé uniquement pour les moyenne et 
   * les moyenne sont faite avec le box filter*/
  m_LeftCovBoxFilter->SetInput1( m_MultiplyLeftMeanBoxFilter->GetOutput() );
  m_LeftCovBoxFilter->SetInput2( m_LeftMeanBoxFilter->GetOutput() ); 
  
   
  m_RightCovBoxFilter->SetInput1( m_MultiplyRightMeanBoxFilter->GetOutput() );
  m_RightCovBoxFilter->SetInput2( m_RightMeanBoxFilter->GetOutput() ); 
  
/* ================================== Gradient =======================================================*/  
// Gradient X  
  ConvFilterTypeLeft::InputSizeType radiusGX;
  radiusGX[0] = 1;
  radiusGX[1] = 0;
  itk::Array< double > filterCoeffsX;

  filterCoeffsX.SetSize((2 * radiusGX[0] + 1) );//* (2 * radiusG[1] + 1));
  filterCoeffsX.Fill(0.5);
  filterCoeffsX[0] = -0.5;
  filterCoeffsX[1] = 0;

  m_convFilterXL->SetRadius(radiusGX);
  m_convFilterXL->SetFilter(filterCoeffsX);
  
  m_convFilterXR->SetRadius(radiusGX);
  m_convFilterXR->SetFilter(filterCoeffsX);
//--Left---------------  
  m_GradientXL->SetFilter(m_convFilterXL);
  m_GradientXL->SetInput(leftImage); 
  
//--Right---------------  
  m_GradientXR->SetFilter(m_convFilterXR);
  m_GradientXR->SetInput(rightImage);
 
// Gradient Y
  ConvFilterTypeLeft::InputSizeType radiusGY;
  radiusGY[0] = 0;
  radiusGY[1] = 1;
  itk::Array< double > filterCoeffsY;

  filterCoeffsY.SetSize((2 * radiusGY[1] + 1) );//* (2 * radiusG[1] + 1));
  filterCoeffsY.Fill(0.5);
  filterCoeffsY[0] = -0.5;
  filterCoeffsY[1] = 0;

  m_convFilterYL->SetRadius(radiusGY);
  m_convFilterYL->SetFilter(filterCoeffsY);
  
  m_convFilterYR->SetRadius(radiusGY);
  m_convFilterYR->SetFilter(filterCoeffsY);
  
//---Left -----
 
  m_GradientYL->SetFilter(m_convFilterYL);
  m_GradientYL->SetInput(leftImage);
 
 //---Right -----
 
  m_GradientYR->SetFilter(m_convFilterYR);
  m_GradientYR->SetInput(rightImage);

/*========================================== Cost Volume ===========================================*/
   /** sortie a une seule bande et elle contient le volume de cout */
  m_LeftCost->SetLeftInputImage(leftImage );
  m_LeftCost->SetRightInputImage(rightImage );  
  m_LeftCost->SetLeftGradientXInput(m_GradientXL->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(m_GradientXR->GetOutput() ); 
  
  m_RightCost->SetLeftInputImage(rightImage );
  m_RightCost->SetRightInputImage(leftImage);  
  m_RightCost->SetLeftGradientXInput(m_GradientXR->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(m_GradientXL->GetOutput() );
   	
  m_LeftCost->SetDisp(HdispMin);
  m_RightCost->SetDisp(-HdispMin); 

/*========================================== Guided filter ==========================================================*/
 /** la moyenne du CostVolume en utilsant les images intégrales*/
 //Left
  m_LeftCostBox->SetInput( m_LeftCost->GetOutput() );   
  m_LeftMeanCostBoxFilter->SetInput( m_LeftCostBox->GetOutput() );  
  m_LeftMeanCostBoxFilter->SetRadius(radius);
  
  // Right  
  m_RightCostBox->SetInput( m_RightCost->GetOutput() );  
  m_RightMeanCostBoxFilter->SetInput( m_RightCostBox->GetOutput() );  
  m_RightMeanCostBoxFilter->SetRadius(radius); 
  
 /** sortie a 3 bandes et elle contient la multiplication de l'image avec le volume de cout*/ 
 //Left 
 m_MLeftAndCostFilter->SetInput1( leftImage); 
 m_MLeftAndCostFilter->SetInput2( m_LeftCost->GetOutput() ); 
 
 //Right 
 m_MRightAndCostFilter->SetInput1( rightImage ); 
 m_MRightAndCostFilter->SetInput2( m_RightCost->GetOutput() ); 
 
  /** image integrale*/
  //Left  
  m_MLeftAndCostBox->SetInput( m_MLeftAndCostFilter->GetOutput() );
  
  //Right  
  m_MRightAndCostBox->SetInput( m_MRightAndCostFilter->GetOutput() );
  
  /** Cacul de la moyenne de l'image integrales*/
  //Left 
  m_MLeftAndCostMeanBoxFilter->SetInput( m_MLeftAndCostBox->GetOutput() );  
  m_MLeftAndCostMeanBoxFilter->SetRadius(radius);
  
  //Right  
  m_MRightAndCostMeanBoxFilter->SetInput( m_MRightAndCostBox->GetOutput() );  
  m_MRightAndCostMeanBoxFilter->SetRadius(radius);

  /** concate of Image and Cost */  
  //Left  
  m_ConcatenateLeftAndCostMean->SetInput1(m_LeftMeanBoxFilter->GetOutput());
  m_ConcatenateLeftAndCostMean->SetInput2(m_LeftMeanCostBoxFilter->GetOutput());

  //Right  
  m_ConcatenateRightAndCostMean->SetInput1(m_RightMeanBoxFilter->GetOutput());
  m_ConcatenateRightAndCostMean->SetInput2(m_RightMeanCostBoxFilter->GetOutput());
  
  /** Calcul de la covariance entre image et le cost*/  
  //Left  
  m_CovLeftCostBoxFilter->SetInput1( m_MLeftAndCostMeanBoxFilter->GetOutput() );
  m_CovLeftCostBoxFilter->SetInput2( m_ConcatenateLeftAndCostMean->GetOutput() ); 
  
  //Right  
  m_CovRightCostBoxFilter->SetInput1( m_MRightAndCostMeanBoxFilter->GetOutput() );
  m_CovRightCostBoxFilter->SetInput2( m_ConcatenateRightAndCostMean->GetOutput() ); 

  /** sortie a 4 bandes, les 3 1ere contienent les ak et la 4eme contient bk  */ 
 //Left 
  m_LeftGuided->SetLeftImageInput(leftImage );
  m_LeftGuided->SetMeanImageInput(m_LeftMeanBoxFilter->GetOutput() ); 
  m_LeftGuided->SetCostVolumeImageInput(m_LeftCost->GetOutput() );
  m_LeftGuided->SetMeanCostVolumeImageInput(m_LeftMeanCostBoxFilter->GetOutput());   
  m_LeftGuided->SetCovarianceImageInput(m_LeftCovBoxFilter->GetOutput() );   
  m_LeftGuided->SetCovIm1CostImageInput(m_CovLeftCostBoxFilter->GetOutput()); 
  m_LeftGuided->SetRadius(radius);
  
 //Right
  CoeffGuidedFilterType::Pointer RightGuided = CoeffGuidedFilterType::New(); 
  m_RightGuided->SetLeftImageInput(rightImage );
  m_RightGuided->SetMeanImageInput(m_RightMeanBoxFilter->GetOutput() );
  m_RightGuided->SetCostVolumeImageInput(m_RightCost->GetOutput() );
  m_RightGuided->SetMeanCostVolumeImageInput(m_RightMeanCostBoxFilter->GetOutput()); 
  m_RightGuided->SetCovarianceImageInput(m_RightCovBoxFilter->GetOutput() );   
  m_RightGuided->SetCovIm1CostImageInput(m_CovRightCostBoxFilter->GetOutput());
  m_RightGuided->SetRadius(radius); 

/*============= La moyenne des coeffs du guided box filter avec les images integrales==========*/
/** la moyenne du CostVolume en utilsant les images intégrales*/
//Left 
  m_LeftCoeffBox->SetInput( m_LeftGuided->GetOutput() );  
  m_LeftMeanCoeffBoxFilter->SetInput( m_LeftCoeffBox->GetOutput() ); 
  m_LeftMeanCoeffBoxFilter->SetRadius(radius);
//Right   
  m_RightCoeffBox->SetInput( m_RightGuided->GetOutput() );  
  m_RightMeanCoeffBoxFilter->SetInput( m_RightCoeffBox->GetOutput() ); 
  m_RightMeanCoeffBoxFilter->SetRadius(radius);  
  

/* =========== le cost volume filtré avec le guided filter =====================================*/
 //Left
  m_LeftCVFFiltred->SetInput1( leftImage );
  m_LeftCVFFiltred->SetInput2( m_LeftMeanCoeffBoxFilter->GetOutput() );

  //Right
  m_RightCVFFiltred->SetInput1( rightImage );
  m_RightCVFFiltred->SetInput2( m_RightMeanCoeffBoxFilter->GetOutput() );
 
 /* =========================================== Min Filter ======================================*/
typename FloatVectorImageType::PixelType Disp(1);
Disp[0] = HdispMin;

typename FloatVectorImageType::PixelType costMax(1);
costMax[0] = 10e9;

m_LeftCVFFiltred->UpdateOutputInformation();
m_RightCVFFiltred->UpdateOutputInformation();


//Left
	FloatVectorImageType::Pointer LeftDispIn = FloatVectorImageType::New(); 
	FloatVectorImageType::Pointer LeftMinCost = FloatVectorImageType::New(); 
	FloatVectorImageType::Pointer LeftDispOut;
    FloatVectorImageType::Pointer LeftMinIn;
    FloatVectorImageType::Pointer LeftOutGuided = FloatVectorImageType::New();
//Right    
    FloatVectorImageType::Pointer RightDispIn = FloatVectorImageType::New(); 
	FloatVectorImageType::Pointer RightMinCost = FloatVectorImageType::New(); 
	FloatVectorImageType::Pointer RightDispOut;
    FloatVectorImageType::Pointer RightMinIn;
    FloatVectorImageType::Pointer RightOutGuided = FloatVectorImageType::New();
    
//Left    
    LeftDispIn->CopyInformation(m_LeftCVFFiltred->GetOutput());
    LeftDispIn->SetNumberOfComponentsPerPixel(1);
    LeftDispIn->SetRegions(m_LeftCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    LeftDispIn->Allocate();   
    LeftDispIn->FillBuffer(Disp);
           
      
    LeftMinCost->CopyInformation(m_LeftCVFFiltred->GetOutput());
    LeftMinCost->SetNumberOfComponentsPerPixel(1);
    LeftMinCost->SetRegions(m_LeftCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    LeftMinCost->Allocate();   
    LeftMinCost->FillBuffer(costMax);
    
//Right 	
    
    RightDispIn->CopyInformation(m_RightCVFFiltred->GetOutput());
    RightDispIn->SetNumberOfComponentsPerPixel(1);
    RightDispIn->SetRegions(m_RightCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    RightDispIn->Allocate();   
    RightDispIn->FillBuffer(-Disp);
    
    RightMinCost->CopyInformation(m_RightCVFFiltred->GetOutput());
    RightMinCost->SetNumberOfComponentsPerPixel(1);
    RightMinCost->SetRegions(m_RightCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    RightMinCost->Allocate();   
    RightMinCost->FillBuffer(costMax);

// concate of LeftDispIn and LeftMinCost  
  m_LeftConcatenateCostDisp->SetInput1(LeftDispIn);
  m_LeftConcatenateCostDisp->SetInput2(LeftMinCost);
    
// concate of RightDispIn and RightMinCost  
  m_RightConcatenateCostDisp->SetInput1(RightDispIn);
  m_RightConcatenateCostDisp->SetInput2(RightMinCost); //RightMinCost = LeftMinCost(initilisation) 
   
   
  LeftMinIn = m_LeftConcatenateCostDisp->GetOutput();   
  RightMinIn = m_RightConcatenateCostDisp->GetOutput();
 
  LeftOutGuided->CopyInformation(LeftMinCost); 
  RightOutGuided->CopyInformation(RightMinCost);
  
  
  m_LeftCVFFiltred->GraftOutput(LeftOutGuided);
  m_LeftCVFFiltred->Modified();
  m_LeftCVFFiltred->UpdateOutputInformation();

  m_RightCVFFiltred->GraftOutput(RightOutGuided);
  m_RightCVFFiltred->Modified();
  m_RightCVFFiltred->UpdateOutputInformation();
  
 

/** Instanction du filtre MinFilter en mettant en entrée la sortie du filtre guidé avec la disp**/	  
  m_LeftMin->SetInput1(LeftMinIn); 
  m_LeftMin->SetInput2(m_LeftCVFFiltred->GetOutput()); 

// left disp
for (int Hdisp = HdispMin ; Hdisp <=HdispMax ;Hdisp++){ 	 
 m_LeftCost->SetDisp(Hdisp); 
 m_LeftCost->Modified();


 m_LeftMin->GetFunctor().SetDisp(Hdisp); 
 m_LeftMin->Update();
 

 LeftDispOut = m_LeftMin->GetOutput();
 
 LeftMinIn = const_cast <FloatVectorImageType *>( m_LeftMin->GetInput(0)); // GetInput() est de 3 image
 
 LeftDispOut->DisconnectPipeline();  // au lieu de déconnecter la sortie on vas échanger les buffers
 m_LeftMin->GraftOutput(LeftMinIn) ;

 m_LeftMin->SetInput1( LeftDispOut); // ne pas mettre le SetInput2 car il vas se mettre a j automatiquement avec le pipeline
  
}  


// Right disp 
m_RightMin->SetInput1(RightMinIn); 
m_RightMin->SetInput2(m_RightCVFFiltred->GetOutput()); 
	 
for (int Hdisp = -HdispMax ; Hdisp <= -HdispMin ;Hdisp++){ 
 m_RightCost->SetDisp(Hdisp); 
 m_RightCost->Modified();
 
 m_RightMin->GetFunctor().SetDisp(Hdisp); 
 m_RightMin->Update();
 
 RightDispOut = m_RightMin->GetOutput();

 RightMinIn = const_cast <FloatVectorImageType *>( m_RightMin->GetInput(0)); 
 
 RightDispOut->DisconnectPipeline(); 
 m_RightMin->GraftOutput(RightMinIn) ;

 m_RightMin->SetInput1(RightDispOut); 
  
}   

 /*==================================== calcul de la mediane===========================================*/
  MedianFilterType::RadiusType MRadius = {{1,1}};
  m_LMedian->SetRadius(MRadius);
  
 // PerBand
  m_LeftMedianFilter->SetFilter(m_LMedian);
  m_LeftMedianFilter->SetInput(leftImage);//
    
  m_RMedian->SetRadius(MRadius);  
  
  m_RightMedianFilter->SetFilter(m_RMedian);
  m_RightMedianFilter->SetInput(rightImage);
  
/*========================================= filtrage de la carte de disparité  par Weighted Median  =====*/
/** Extraction des bandes de l'image de MinFilter **/
  // left                                                                                
   m_Leftchannel1-> SetInput(const_cast <FloatVectorImageType *>( m_LeftMin->GetOutput() ));        
   m_Leftchannel1->SetChannel(1);   
   
   // Right   
   m_Rightchannel1-> SetInput(const_cast <FloatVectorImageType *>( m_RightMin->GetOutput() ));         
   m_Rightchannel1->SetChannel(1);

  /** Conversion otb::Image (la 1ère composante de la sortie du Min filter) vers Otb::vecteurImage */
  m_LeftDisparity-> SetInput( const_cast <FloatImageType *>( m_Leftchannel1->GetOutput() ));  
  m_RightDisparity-> SetInput( const_cast <FloatImageType *>( m_Rightchannel1->GetOutput() ));

 
  /** Concatenation de la 1ère bande qui est la disparité avec l'image de de droite */
  m_ConcatenateDispEndInLeftImage->SetInput1(m_LeftDisparity->GetOutput());
  m_ConcatenateDispEndInLeftImage->SetInput2( m_LeftMedianFilter->GetOutput() );

  m_ConcatenateDispEndInRightImage->SetInput1(m_RightDisparity->GetOutput());
  m_ConcatenateDispEndInRightImage->SetInput2( m_RightMedianFilter->GetOutput() );
  
  
  /** Instanciation du filtre weighted median en metant en entrée une image a 4 bandes (sortie de la concatenation) 
   * le 1ere bande etant la disparité et 3 autre l'image de droite */
  
   //Left
   m_LeftMedian-> SetInput(m_ConcatenateDispEndInLeftImage->GetOutput());
    
   
  FloatVectorImageType::SizeType radiusM;
  radiusM[0] = 19;
  radiusM[1] = 19;   
  m_LeftMedian->SetRadius(radiusM) ;
   

   
// right 
  m_RightMedian-> SetInput(m_ConcatenateDispEndInRightImage->GetOutput());   
  m_RightMedian->SetRadius(radiusM) ;
/*========================== Detection d'occulusion =============================*/  
/** Conversion des vectorImage vers OtbImage*/

m_OtbImageLeftMedian->SetInput( m_LeftMedian->GetOutput() );
m_OtbImageLeftMedian->Update();

m_OtbImageRightMedian->SetInput( m_RightMedian->GetOutput() );
m_OtbImageRightMedian->Update();


/** Detection d'occlusion*/

  m_Occlusionfilter->SetDirectHorizontalDisparityMapInput(m_OtbImageLeftMedian->GetOutput()->GetNthElement(0)); //LeftMedian
  m_Occlusionfilter->SetReverseHorizontalDisparityMapInput(m_OtbImageRightMedian->GetOutput()->GetNthElement(0)); //Rightmedian
   
  m_Occlusionfilter->SetMaxHDisp(HdispMax);
  m_Occlusionfilter->SetMinHDisp(HdispMin);
  m_Occlusionfilter->SetMinVDisp(0);
  m_Occlusionfilter->SetMaxVDisp(0); 
  m_Occlusionfilter->SetTolerance(1);
 

/*========================== Fill occlusion =====================================*/  
  m_OccCastFiler-> SetInput( const_cast <FloatImageType *>( m_Occlusionfilter->GetOutput() ));  
  
  m_ConcatenateOccEndInLeftMedImage->SetInput1(m_OccCastFiler->GetOutput());
  m_ConcatenateOccEndInLeftMedImage->SetInput2( m_LeftMedian->GetOutput() );
  

  m_FillOcclusionfilter->SetInput(m_ConcatenateOccEndInLeftMedImage->GetOutput()); 
  FloatVectorImageType::SizeType OccRadius;
  OccRadius[0] = 9;
  OccRadius[1] = 0;
  
  m_FillOcclusionfilter->SetRadius(OccRadius);
 
 
   /** Instanciation du filtre weighted median en metant en entrée une image a 4 bandes (sortie de la concatenation) 
   * le 1ere bande etant la carte de fill occlusion et 3 autre l'image de droite filtré avec un median simple */
   
  m_ConcatenateFillEndInLeftMedImage->SetInput1(m_FillOcclusionfilter->GetOutput());
  m_ConcatenateFillEndInLeftMedImage->SetInput2( m_LeftMedianFilter->GetOutput() );
  

  m_FillMedian-> SetInput(m_ConcatenateFillEndInLeftMedImage->GetOutput()); 
  m_FillMedian->SetRadius(radiusM) ;
 
SetParameterOutputImage("io.out",m_FillMedian->GetOutput());
  
  }
  
	MultiplyFilterType::Pointer m_MultiplyLeft;
	MultiplyFilterType::Pointer m_MultiplyRight;
  
	BoxFilterType::Pointer m_LeftBox;
	BoxFilterType::Pointer m_RightBox;
   
	BoxFilterType::Pointer m_MultiplyLeftBox;
	BoxFilterType::Pointer m_MultiplyRightBox;
   
   
	MeanBoxFilterType::Pointer m_LeftMeanBoxFilter;
	MeanBoxFilterType::Pointer m_RightMeanBoxFilter;
   
	MeanBoxFilterType::Pointer m_MultiplyLeftMeanBoxFilter;
	MeanBoxFilterType::Pointer m_MultiplyRightMeanBoxFilter;
    
	CovBoxFilterType::Pointer m_LeftCovBoxFilter; 
	CovBoxFilterType::Pointer m_RightCovBoxFilter;
    
	ConvFilterTypeLeft::Pointer m_convFilterXL;
	ConvFilterTypeLeft::Pointer m_convFilterYL;
  
	ConvFilterTypeRight::Pointer m_convFilterXR;
	ConvFilterTypeRight::Pointer m_convFilterYR;
  
	VectorFilterTypeLeft::Pointer m_GradientXL;
	VectorFilterTypeRight::Pointer m_GradientXR;
    
	VectorFilterTypeLeft::Pointer m_GradientYL;
	VectorFilterTypeRight::Pointer m_GradientYR;
    
	CostVolumeType::Pointer m_LeftCost ;
	CostVolumeType::Pointer m_RightCost;
      
	BoxFilterType::Pointer m_LeftCostBox ;
	MeanBoxFilterType::Pointer m_LeftMeanCostBoxFilter ;
	BoxFilterType::Pointer m_RightCostBox ;
	MeanBoxFilterType::Pointer m_RightMeanCostBoxFilter ;
			
	MImageAndCostType::Pointer m_MLeftAndCostFilter ;
	MImageAndCostType::Pointer m_MRightAndCostFilter ;
       
	BoxFilterType::Pointer m_MLeftAndCostBox ;
	BoxFilterType::Pointer m_MRightAndCostBox ;
         
	MeanBoxFilterType::Pointer m_MLeftAndCostMeanBoxFilter ;
	MeanBoxFilterType::Pointer m_MRightAndCostMeanBoxFilter ;
            
	ConcatenateVectorImageFilterType::Pointer m_ConcatenateLeftAndCostMean ;
	ConcatenateVectorImageFilterType::Pointer m_ConcatenateRightAndCostMean ;
				
	CovImageWithCostBoxFilterType::Pointer m_CovLeftCostBoxFilter ; 
	CovImageWithCostBoxFilterType::Pointer m_CovRightCostBoxFilter; 
		
	CoeffGuidedFilterType::Pointer m_LeftGuided;
	CoeffGuidedFilterType::Pointer m_RightGuided ;
        
	BoxFilterType::Pointer m_LeftCoeffBox ;
	MeanBoxFilterType::Pointer m_LeftMeanCoeffBoxFilter ;
	BoxFilterType::Pointer m_RightCoeffBox;
	MeanBoxFilterType::Pointer m_RightMeanCoeffBoxFilter ;

	GuidedFilterType::Pointer m_LeftCVFFiltred ;
	GuidedFilterType::Pointer m_RightCVFFiltred ;

	ConcatenateVectorImageFilterType::Pointer m_LeftConcatenateCostDisp ;
    ConcatenateVectorImageFilterType::Pointer m_RightConcatenateCostDisp ;
    
    MinFilterType::Pointer m_LeftMin ;
    MinFilterType::Pointer m_RightMin ;
    
    MedianFilterType::Pointer m_LMedian ;
    PerBand::Pointer m_LeftMedianFilter ;
    MedianFilterType::Pointer m_RMedian ;
      
    PerBand::Pointer m_RightMedianFilter ;
    ExtractFilterType::Pointer m_Leftchannel1 ; 
    ExtractFilterType::Pointer m_Rightchannel1 ;
        
    CastFilterType::Pointer m_LeftDisparity ;
    CastFilterType::Pointer m_RightDisparity ;
          
	ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage ;
	ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInRightImage ;
  
	WeightMedianType::Pointer m_LeftMedian ;
	WeightMedianType::Pointer m_RightMedian ;
  
	ListType::Pointer m_OtbImageLeftMedian ;
	ListType::Pointer m_OtbImageRightMedian ;
  
	OcclusionType::Pointer m_Occlusionfilter;
	CastFilterType::Pointer m_OccCastFiler ;
  
	ConcatenateVectorImageFilterType::Pointer m_ConcatenateOccEndInLeftMedImage ;
	FillOcclusionType::Pointer m_FillOcclusionfilter ;

	ConcatenateVectorImageFilterType::Pointer m_ConcatenateFillEndInLeftMedImage ;
	WeightMedianType::Pointer m_FillMedian;



};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::CostVolumeFilter)

