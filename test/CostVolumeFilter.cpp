/*=========================================================================

 *  Program:   ORFEO Toolbox
 *  Language:  C++
 *  Date:      $Date$
 * Version:   $Revision$
 *
 * Copyright (C) Damia Belazouz - 2017
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

/** Execution ./bin/otbCVFFiltersTestDriver test_CVF Radius Min_Disp Max_Disp
 * Left_Image(3bands) Right_Image(3bands) Path_Of_Outputs_Images 
 
 * **/ 

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
#include "otbRMSEVectorImageFilter.h"



#include <itkImage.h>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbLocalGradientVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbLocalGradientVectorImageFilter.h" 

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


	
int testCostVolumeFilter(int argc, char *argv[])
{
	

	if(argc < 8) {
		std::cerr << "Usage: " << argv[0] << " radius min_disp max_disp radius_to_mean_filter Left_Image Right_Image Out_Path" << std::endl;
		return EXIT_FAILURE;
	}
	
     long unsigned int r = atoi(argv[1]);
	 int HdispMin = atoi(argv[2]);
	 int HdispMax  = atoi(argv[3]);
   int rmf = atoi(argv[4]) ;
  
  // Main type definition
  const unsigned int Dimension = 2;
  typedef otb::Image< double, Dimension > OTBImageType;
  typedef otb::VectorImage< double , Dimension > ImageType;
  typedef otb::ImageList<OTBImageType>              ImageListType;

   
  typedef otb::ImageFileWriter< OTBImageType > OtbImageWriterType; 
  typedef otb::ImageFileWriter< ImageType > ImageWriterType;
    // Reading Leftinput images
    typedef otb::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer LeftReader = ReaderType::New();
    LeftReader->SetFileName(argv[5]); //LeftImage 
    LeftReader->UpdateOutputInformation();//*
    // Reading ReghtInput images
    typedef otb::ImageFileReader<ImageType> RightReaderType;
    RightReaderType::Pointer RightReader = RightReaderType::New();
    RightReader->SetFileName(argv[6]);//RightImage
    RightReader->UpdateOutputInformation();//*
   //argv[6] le chemin des images de sortie  
    std::string argv7 = std::string(argv[7]);
    #define FILENAME(n) std::string( argv7 + std::string(n)).c_str()
  

 //#if 0
  
/*============================== calcul de la multiplication pour la covariance =======*/
// covariance = Mean( im1*im2) - Mean(im1)*mean(im2) 

  typedef otb::MultiplyImagebandsFilter< ImageType, ImageType > MultiplyFilterType;
  
  MultiplyFilterType::Pointer MultiplyLeft = MultiplyFilterType::New();
  MultiplyLeft->SetInput( LeftReader->GetOutput() );
  
  
  MultiplyFilterType::Pointer MultiplyRight = MultiplyFilterType::New();
  MultiplyRight->SetInput( RightReader->GetOutput() ); 
  
  ImageWriterType::Pointer Multiplywriter = ImageWriterType::New();
  Multiplywriter->SetFileName(FILENAME("MultiplyImage.tif" ));
  Multiplywriter->SetInput( MultiplyLeft->GetOutput() );
  Multiplywriter->Update();

 /*==================================== calcul de l'image intégrales======================*/ 
  
  typedef otb::BoxImageFilter< ImageType, ImageType > BoxFilterType;
  
  BoxFilterType::Pointer LeftBox = BoxFilterType::New();
  LeftBox->SetInput( LeftReader->GetOutput() );
  
  BoxFilterType::Pointer RightBox = BoxFilterType::New();
  RightBox->SetInput( RightReader->GetOutput() );
  
  BoxFilterType::Pointer MultiplyLeftBox = BoxFilterType::New();
  MultiplyLeftBox->SetInput( MultiplyLeft->GetOutput() );
  
  BoxFilterType::Pointer MultiplyRightBox = BoxFilterType::New();
  MultiplyRightBox->SetInput( MultiplyRight->GetOutput() );
 
   
  ImageWriterType::Pointer Boxwriter = ImageWriterType::New();
  Boxwriter->SetFileName( FILENAME("integraleLeftImage.tif"));
  Boxwriter->SetInput( LeftBox->GetOutput() );
  otb::StandardFilterWatcher watcherIn(Boxwriter, "LeftBoxFilter");    
  Boxwriter->Update();
  
  
   /*==================================== calcul des moyennes avec le boxFilter=============================*/

  typedef otb::LocalMeanBoxVectorImageFilter< ImageType, ImageType > MeanBoxFilterType;
  
  MeanBoxFilterType::Pointer LeftMeanBoxFilter = MeanBoxFilterType::New();
  LeftMeanBoxFilter->SetInput( LeftBox->GetOutput() );  
  MeanBoxFilterType::RadiusType radius = {{r,r}};
  LeftMeanBoxFilter->SetRadius(radius);
  
  MeanBoxFilterType::Pointer RightMeanBoxFilter = MeanBoxFilterType::New();
  RightMeanBoxFilter->SetInput( RightBox->GetOutput() );  
  RightMeanBoxFilter->SetRadius(radius);
  
  ImageWriterType::Pointer LeftMeanwriter = ImageWriterType::New(); 
  LeftMeanwriter->SetFileName( FILENAME("LeftMean.tif"));
  LeftMeanwriter->SetInput( LeftMeanBoxFilter->GetOutput() );
  LeftMeanwriter->Update();
  
  ImageWriterType::Pointer RightMeanwriter = ImageWriterType::New();
  RightMeanwriter->SetFileName( FILENAME("RightMean.tif"));
  RightMeanwriter->SetInput( RightMeanBoxFilter->GetOutput() );
  RightMeanwriter->Update();
   
  // The mean multiplication
  MeanBoxFilterType::Pointer MultiplyLeftMeanBoxFilter = MeanBoxFilterType::New();
  MultiplyLeftMeanBoxFilter->SetInput( MultiplyLeftBox->GetOutput() );
  MultiplyLeftMeanBoxFilter->SetRadius(radius);
  
   MeanBoxFilterType::Pointer MultiplyRightMeanBoxFilter = MeanBoxFilterType::New();
  MultiplyRightMeanBoxFilter->SetInput( MultiplyRightBox->GetOutput() );
  MultiplyRightMeanBoxFilter->SetRadius(radius);

 
  //=============================== Calcul de la covariance avec le boxFiler ======================*/
  
  /** remarque: on a pas besoin de mettre le radius car le radius est utilisé uniquement pour les moyenne et 
   * les moyenne sont faite avec le box filter*/
  typedef otb::LocalCovarianceBoxVectorImageFilter< ImageType, ImageType, ImageType > CovBoxFilterType;
  
  CovBoxFilterType::Pointer LeftCovBoxFilter = CovBoxFilterType::New();  
  LeftCovBoxFilter->SetInput1( MultiplyLeftMeanBoxFilter->GetOutput() );
  LeftCovBoxFilter->SetInput2( LeftMeanBoxFilter->GetOutput() ); 
  
  CovBoxFilterType::Pointer RightCovBoxFilter = CovBoxFilterType::New();  
  RightCovBoxFilter->SetInput1( MultiplyRightMeanBoxFilter->GetOutput() );
  RightCovBoxFilter->SetInput2( RightMeanBoxFilter->GetOutput() ); 
  
  ImageWriterType::Pointer CovBoxwriter = ImageWriterType::New();
  CovBoxwriter->SetFileName( FILENAME("LeftCovBoxFilter.tif"));
  CovBoxwriter->SetInput( LeftCovBoxFilter->GetOutput() );  
  otb::StandardFilterWatcher watcherBoxVov(CovBoxwriter, "LeftCovBoxFilter");
  CovBoxwriter->Update();
 
/* ================================== Gradient =======================================================*/
// ConvolutionImageFilter Left  
  
  typedef itk::ConstantBoundaryCondition<OTBImageType> BoundaryConditionTypeLeft;
  typedef otb::ConvolutionImageFilter<OTBImageType, OTBImageType, BoundaryConditionTypeLeft> ConvFilterTypeLeft;
  ConvFilterTypeLeft::Pointer convFilterXL = ConvFilterTypeLeft::New();
  ConvFilterTypeLeft::Pointer convFilterYL = ConvFilterTypeLeft::New();
  
// ConvolutionImageFilter Right  
  
  typedef itk::ConstantBoundaryCondition<OTBImageType>  BoundaryConditionTypeRight;
  typedef otb::ConvolutionImageFilter<OTBImageType, OTBImageType, BoundaryConditionTypeRight> ConvFilterTypeRight;
  ConvFilterTypeRight::Pointer convFilterXR = ConvFilterTypeRight::New();
  ConvFilterTypeRight::Pointer convFilterYR= ConvFilterTypeRight::New();
  
// Gradient X  
  ConvFilterTypeLeft::InputSizeType radiusGX;
  radiusGX[0] = 1;
  radiusGX[1] = 0;
  itk::Array< double > filterCoeffsX;

  filterCoeffsX.SetSize((2 * radiusGX[0] + 1) );//* (2 * radiusG[1] + 1));
  filterCoeffsX.Fill(0.5);
  filterCoeffsX[0] = -0.5;
  filterCoeffsX[1] = 0;

  convFilterXL->SetRadius(radiusGX);
  convFilterXL->SetFilter(filterCoeffsX);
  
  convFilterXR->SetRadius(radiusGX);
  convFilterXR->SetFilter(filterCoeffsX);
//--Left---------------  
  typedef otb::PerBandVectorImageFilter<ImageType, ImageType, ConvFilterTypeLeft> VectorFilterTypeLeft;
  VectorFilterTypeLeft::Pointer GradientXL = VectorFilterTypeLeft::New();
  GradientXL->SetFilter(convFilterXL);
  GradientXL->SetInput(LeftReader->GetOutput());
 
  ImageWriterType::Pointer writerGradientXL = ImageWriterType::New();
  writerGradientXL->SetFileName( FILENAME("GradientXLeft.tif"));
  writerGradientXL->SetInput(GradientXL->GetOutput());
  writerGradientXL->Update();
  
//--Right---------------  
  typedef otb::PerBandVectorImageFilter<ImageType, ImageType, ConvFilterTypeRight> VectorFilterTypeRight;
  VectorFilterTypeRight::Pointer GradientXR = VectorFilterTypeRight::New();
  GradientXR->SetFilter(convFilterXR);
  GradientXR->SetInput(RightReader->GetOutput());
 
  ImageWriterType::Pointer writerGradientXR = ImageWriterType::New();
  writerGradientXR->SetFileName(FILENAME("GradientXRight.tif"));
  writerGradientXR->SetInput(GradientXR->GetOutput());
  writerGradientXR->Update();
  
   
// Gradient Y
  ConvFilterTypeLeft::InputSizeType radiusGY;
  radiusGY[0] = 0;
  radiusGY[1] = 1;
  itk::Array< double > filterCoeffsY;

  filterCoeffsY.SetSize((2 * radiusGY[1] + 1) );//* (2 * radiusG[1] + 1));
  filterCoeffsY.Fill(0.5);
  filterCoeffsY[0] = -0.5;
  filterCoeffsY[1] = 0;

  convFilterYL->SetRadius(radiusGY);
  convFilterYL->SetFilter(filterCoeffsY);
  
  convFilterYR->SetRadius(radiusGY);
  convFilterYR->SetFilter(filterCoeffsY);
  
//---Left -----
  VectorFilterTypeLeft::Pointer GradientYL = VectorFilterTypeLeft::New();
  GradientYL->SetFilter(convFilterYL);
  GradientYL->SetInput(LeftReader->GetOutput());
 
 
  ImageWriterType::Pointer writerGradientYL = ImageWriterType::New();
  writerGradientYL->SetFileName(FILENAME("GradientYLeft.tif"));
  writerGradientYL->SetInput(GradientYL->GetOutput());
  writerGradientYL->Update();
 
 //---Right -----
  VectorFilterTypeRight::Pointer GradientYR = VectorFilterTypeRight::New();
  GradientYR->SetFilter(convFilterYR);
  GradientYR->SetInput(RightReader->GetOutput());
 
 
  ImageWriterType::Pointer writerGradientYR = ImageWriterType::New();
  writerGradientYR->SetFileName( FILENAME("GradientYRight.tif"));
  writerGradientYR->SetInput(GradientYR->GetOutput());
  writerGradientYR->Update();
 
   
// Norme du gradient
 
  typedef otb::LocalGradientVectorImageFilter< ImageType, ImageType > GradientType;
  GradientType::Pointer filterG = GradientType::New();
  filterG->SetInput( LeftReader->GetOutput() );

  GradientType::RadiusType radiusG = {{r,r}};
  filterG->SetRadius(radiusG);

  ImageWriterType::Pointer writerG = ImageWriterType::New();
  writerG->SetFileName( FILENAME("GradientNormLeft.tif"));
  writerG->SetInput( filterG->GetOutput() );
  writerG->Update();

/*========================================== Cost Volume ===========================================*/
   /** sortie a une seule bande et elle contient le volume de cout */
  typedef otb::CostVolumeImageFilter< ImageType, ImageType, ImageType > CostVolumeType;
 
  CostVolumeType::Pointer LeftCost = CostVolumeType::New();
  LeftCost->SetLeftInputImage(LeftReader->GetOutput() );
  LeftCost->SetRightInputImage(RightReader->GetOutput() );  
  LeftCost->SetLeftGradientXInput(GradientXL->GetOutput() ); 
  LeftCost->SetRightGradientXInput(GradientXR->GetOutput() ); 
  
  
  CostVolumeType::Pointer RightCost = CostVolumeType::New();
  RightCost->SetLeftInputImage(RightReader->GetOutput() );
  RightCost->SetRightInputImage(LeftReader->GetOutput() );  
  RightCost->SetLeftGradientXInput(GradientXR->GetOutput() ); 
  RightCost->SetRightGradientXInput(GradientXL->GetOutput() );
   	
  LeftCost->SetDisp(HdispMin);
  RightCost->SetDisp(-HdispMin);
	

  ImageWriterType::Pointer LeftCostwriter = ImageWriterType::New();
  LeftCostwriter->SetFileName(FILENAME("LeftCostVolumeFilter.tif"));
  LeftCostwriter->SetInput( LeftCost->GetOutput() );   
  LeftCostwriter->Update();
  
  
  ImageWriterType::Pointer RightCostwriter = ImageWriterType::New();
  RightCostwriter->SetFileName(FILENAME("RightCostVolumeFilter.tif"));
  RightCostwriter->SetInput( RightCost->GetOutput() );    
  RightCostwriter->Update();
 

 /*========================================== Guided filter ==========================================================*/
 /** la moyenne du CostVolume en utilsant les images intégrales*/
 //Left
  BoxFilterType::Pointer LeftCostBox = BoxFilterType::New();
  LeftCostBox->SetInput( LeftCost->GetOutput() ); 

  MeanBoxFilterType::Pointer LeftMeanCostBoxFilter = MeanBoxFilterType::New();
  LeftMeanCostBoxFilter->SetInput( LeftCostBox->GetOutput() );  
  LeftMeanCostBoxFilter->SetRadius(radius);

  
  // Right
  BoxFilterType::Pointer RightCostBox = BoxFilterType::New();
  RightCostBox->SetInput( RightCost->GetOutput() ); 

  MeanBoxFilterType::Pointer RightMeanCostBoxFilter = MeanBoxFilterType::New();
  RightMeanCostBoxFilter->SetInput( RightCostBox->GetOutput() );  
  RightMeanCostBoxFilter->SetRadius(radius);
 
  
 /** sortie a 3 bandes et elle contient la multiplication de l'image avec le volume de cout*/
  typedef otb::MultiplyImageAndCostImageFilter< ImageType,ImageType, ImageType> MImageAndCostType; 
  //Left
  MImageAndCostType::Pointer MLeftAndCostFilter = MImageAndCostType::New();
  MLeftAndCostFilter->SetInput1( LeftReader->GetOutput() ); 
  MLeftAndCostFilter->SetInput2( LeftCost->GetOutput() ); 
  //Right
  MImageAndCostType::Pointer MRightAndCostFilter = MImageAndCostType::New();
  MRightAndCostFilter->SetInput1( RightReader->GetOutput() ); 
  MRightAndCostFilter->SetInput2( RightCost->GetOutput() ); 
 
  /** image integrale*/
  //Left
  BoxFilterType::Pointer MLeftAndCostBox = BoxFilterType::New();
  MLeftAndCostBox->SetInput( MLeftAndCostFilter->GetOutput() );
  //Right
  BoxFilterType::Pointer MRightAndCostBox = BoxFilterType::New();
  MRightAndCostBox->SetInput( MRightAndCostFilter->GetOutput() );
  
  /** Cacul de la moyenne de l'image integrales*/
  //Left
  MeanBoxFilterType::Pointer MLeftAndCostMeanBoxFilter = MeanBoxFilterType::New();
  MLeftAndCostMeanBoxFilter->SetInput( MLeftAndCostBox->GetOutput() );  
  MLeftAndCostMeanBoxFilter->SetRadius(radius);
  //Right
  MeanBoxFilterType::Pointer MRightAndCostMeanBoxFilter = MeanBoxFilterType::New();
  MRightAndCostMeanBoxFilter->SetInput( MRightAndCostBox->GetOutput() );  
  MRightAndCostMeanBoxFilter->SetRadius(radius);
  
  /** concate of Image and Cost */ 
  typedef otb::ConcatenateVectorImageFilter< ImageType,ImageType, ImageType> ConcatenateVectorImageFilterType;
  //Left
  ConcatenateVectorImageFilterType::Pointer ConcatenateLeftAndCostMean = ConcatenateVectorImageFilterType::New();
  ConcatenateLeftAndCostMean->SetInput1(LeftMeanBoxFilter->GetOutput());
  ConcatenateLeftAndCostMean->SetInput2(LeftMeanCostBoxFilter->GetOutput());

  //Right
  ConcatenateVectorImageFilterType::Pointer ConcatenateRightAndCostMean = ConcatenateVectorImageFilterType::New();
  ConcatenateRightAndCostMean->SetInput1(RightMeanBoxFilter->GetOutput());
  ConcatenateRightAndCostMean->SetInput2(RightMeanCostBoxFilter->GetOutput());
  
  /** Calcul de la covariance entre image et le cost*/
  typedef otb::CovarianceImageCostVolumeBoxFilter< ImageType, ImageType, ImageType > CovImageWithCostBoxFilterType;
  //Left
  CovImageWithCostBoxFilterType::Pointer CovLeftCostBoxFilter = CovImageWithCostBoxFilterType::New();  
  CovLeftCostBoxFilter->SetInput1( MLeftAndCostMeanBoxFilter->GetOutput() );
  CovLeftCostBoxFilter->SetInput2( ConcatenateLeftAndCostMean->GetOutput() ); 
  //Right
  CovImageWithCostBoxFilterType::Pointer CovRightCostBoxFilter = CovImageWithCostBoxFilterType::New();  
  CovRightCostBoxFilter->SetInput1( MRightAndCostMeanBoxFilter->GetOutput() );
  CovRightCostBoxFilter->SetInput2( ConcatenateRightAndCostMean->GetOutput() ); 

  /** sortie a 4 bandes, les 3 1ere contienent les ak et la 4eme contient bk  */
  typedef otb::CoeffGuidedBoxImageFilter< ImageType, ImageType> CoeffGuidedFilterType;
  //Left
  CoeffGuidedFilterType::Pointer LeftGuided = CoeffGuidedFilterType::New();
  LeftGuided->SetLeftImageInput(LeftReader->GetOutput() );
  LeftGuided->SetMeanImageInput(LeftMeanBoxFilter->GetOutput() ); 
  LeftGuided->SetCostVolumeImageInput(LeftCost->GetOutput() );
  LeftGuided->SetMeanCostVolumeImageInput(LeftMeanCostBoxFilter->GetOutput());   
  LeftGuided->SetCovarianceImageInput(LeftCovBoxFilter->GetOutput() );   
  LeftGuided->SetCovIm1CostImageInput(CovLeftCostBoxFilter->GetOutput()); 
  LeftGuided->SetRadius(radius);
  
  //Right
  CoeffGuidedFilterType::Pointer RightGuided = CoeffGuidedFilterType::New(); 
  RightGuided->SetLeftImageInput(RightReader->GetOutput() );
  RightGuided->SetMeanImageInput(RightMeanBoxFilter->GetOutput() );
  RightGuided->SetCostVolumeImageInput(RightCost->GetOutput() );
  RightGuided->SetMeanCostVolumeImageInput(RightMeanCostBoxFilter->GetOutput()); 
  RightGuided->SetCovarianceImageInput(RightCovBoxFilter->GetOutput() );   
  RightGuided->SetCovIm1CostImageInput(CovRightCostBoxFilter->GetOutput());
  RightGuided->SetRadius(radius); 


  ImageWriterType::Pointer LeftGuidedWriter = ImageWriterType::New();
  LeftGuidedWriter->SetFileName(FILENAME("CoeffLeftGuidedFilter.tif"));
  LeftGuidedWriter->SetInput( LeftGuided->GetOutput() );   
  otb::StandardFilterWatcher LeftGuidedWatcher(LeftGuidedWriter, "CoeffLeftGuidedFilter");  
  LeftGuidedWriter->Update(); 
  
  
  ImageWriterType::Pointer RightGuidedWriter = ImageWriterType::New();
  RightGuidedWriter->SetFileName( FILENAME("CoeffRightGuidedFilter.tif"));
  RightGuidedWriter->SetInput( RightGuided->GetOutput() );  
  otb::StandardFilterWatcher RightGuidedWatcher(RightGuidedWriter, "RightGuidedFilter");  
  RightGuidedWriter->Update(); 

/*============= La moyenne des coeffs du guided box filter avec les images integrales==========*/
/** la moyenne du CostVolume en utilsant les images intégrales*/
//Left
  BoxFilterType::Pointer LeftCoeffBox = BoxFilterType::New();
  LeftCoeffBox->SetInput( LeftGuided->GetOutput() );  

  MeanBoxFilterType::Pointer LeftMeanCoeffBoxFilter = MeanBoxFilterType::New();
  LeftMeanCoeffBoxFilter->SetInput( LeftCoeffBox->GetOutput() ); 
  LeftMeanCoeffBoxFilter->SetRadius(radius);
//Right 
  BoxFilterType::Pointer RightCoeffBox = BoxFilterType::New();
  RightCoeffBox->SetInput( RightGuided->GetOutput() );
  
  MeanBoxFilterType::Pointer RightMeanCoeffBoxFilter = MeanBoxFilterType::New();
  RightMeanCoeffBoxFilter->SetInput( RightCoeffBox->GetOutput() ); 
  RightMeanCoeffBoxFilter->SetRadius(radius);
  
/* =========== le cost volume filtré avec le guided filter =====================================*/
 typedef otb::GuidedImageFilter< ImageType,ImageType, ImageType> GuidedFilterType;
 //Left
  GuidedFilterType::Pointer LeftCVFFiltred = GuidedFilterType::New();  
  LeftCVFFiltred->SetInput1( LeftReader->GetOutput() );
  LeftCVFFiltred->SetInput2( LeftMeanCoeffBoxFilter->GetOutput() );

  //Right
  GuidedFilterType::Pointer RightCVFFiltred = GuidedFilterType::New();  
  RightCVFFiltred->SetInput1( RightReader->GetOutput() );
  RightCVFFiltred->SetInput2( RightMeanCoeffBoxFilter->GetOutput() );

  
  ImageWriterType::Pointer LeftCVFFiltredWriter = ImageWriterType::New();
  LeftCVFFiltredWriter->SetFileName(FILENAME("GuidedFilter.tif"));
  LeftCVFFiltredWriter->SetInput( LeftCVFFiltred->GetOutput() ); 
  otb::StandardFilterWatcher LeftCVFFiltredWatcher(LeftCVFFiltredWriter, "GuidedFilter");  


 /* =========================================== Min Filter ======================================*/
ImageWriterType::Pointer LeftMinWriter = ImageWriterType::New();
ImageWriterType::Pointer RightMinWriter = ImageWriterType::New();
ImageWriterType::Pointer writerMinHuit = ImageWriterType::New();
ImageWriterType::Pointer LeftMedianwriter = ImageWriterType::New();
ImageWriterType::Pointer RightMedianwriter = ImageWriterType::New();
ImageWriterType::Pointer occMedianwriter = ImageWriterType::New();
ImageWriterType::Pointer FillMedianwriter = ImageWriterType::New();


typename ImageType::PixelType Disp(1);
Disp[0] = HdispMin;

typename ImageType::PixelType costMax(1);
costMax[0] = 10e9;

LeftCVFFiltred->UpdateOutputInformation();
RightCVFFiltred->UpdateOutputInformation();

// céation des images de disparité et de cout initiales
//Left
	ImageType::Pointer LeftDispIn = ImageType::New(); 
	ImageType::Pointer LeftMinCost = ImageType::New(); 
	ImageType::Pointer LeftDispOut;
    ImageType::Pointer LeftMinIn;
    ImageType::Pointer LeftOutGuided = ImageType::New();
//Right    
    ImageType::Pointer RightDispIn = ImageType::New(); 
	ImageType::Pointer RightMinCost = ImageType::New(); 
	ImageType::Pointer RightDispOut;
    ImageType::Pointer RightMinIn;
    ImageType::Pointer RightOutGuided = ImageType::New();
    
//Left    
    LeftDispIn->CopyInformation(LeftCVFFiltred->GetOutput());
    LeftDispIn->SetNumberOfComponentsPerPixel(1);
    LeftDispIn->SetRegions(LeftCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    LeftDispIn->Allocate();   
    LeftDispIn->FillBuffer(Disp);
           
      
    LeftMinCost->CopyInformation(LeftCVFFiltred->GetOutput());
    LeftMinCost->SetNumberOfComponentsPerPixel(1);
    LeftMinCost->SetRegions(LeftCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    LeftMinCost->Allocate();   
    LeftMinCost->FillBuffer(costMax);
    
//Right 	
    
    RightDispIn->CopyInformation(RightCVFFiltred->GetOutput());
    RightDispIn->SetNumberOfComponentsPerPixel(1);
    RightDispIn->SetRegions(RightCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    RightDispIn->Allocate();   
    RightDispIn->FillBuffer(-Disp);
    
    RightMinCost->CopyInformation(RightCVFFiltred->GetOutput());
    RightMinCost->SetNumberOfComponentsPerPixel(1);
    RightMinCost->SetRegions(RightCVFFiltred->GetOutput()->GetLargestPossibleRegion());
    RightMinCost->Allocate();   
    RightMinCost->FillBuffer(costMax);

// concate of LeftDispIn and LeftMinCost  
   ConcatenateVectorImageFilterType::Pointer LeftConcatenateCostDisp = ConcatenateVectorImageFilterType::New();
   LeftConcatenateCostDisp->SetInput1(LeftDispIn);
   LeftConcatenateCostDisp->SetInput2(LeftMinCost);
  
  
// concate of RightDispIn and RightMinCost  
   ConcatenateVectorImageFilterType::Pointer RightConcatenateCostDisp = ConcatenateVectorImageFilterType::New();
   RightConcatenateCostDisp->SetInput1(RightDispIn);
   RightConcatenateCostDisp->SetInput2(RightMinCost); //RightMinCost = LeftMinCost(initilisation) 
   
   
   LeftMinIn = LeftConcatenateCostDisp->GetOutput();   
   RightMinIn = RightConcatenateCostDisp->GetOutput();
 
   LeftOutGuided->CopyInformation(LeftMinCost); 
   RightOutGuided->CopyInformation(RightMinCost);
  
  
   LeftCVFFiltred->GraftOutput(LeftOutGuided);
   LeftCVFFiltred->Modified();
   LeftCVFFiltred->UpdateOutputInformation();

   RightCVFFiltred->GraftOutput(RightOutGuided);
   RightCVFFiltred->Modified();
   RightCVFFiltred->UpdateOutputInformation();
  
/** Instanction du filtre MinFilter en mettant en entrée la sortie du filtre guidé avec la disp**/	 
   typedef otb::MinFilter< ImageType,ImageType,ImageType> MinFilterType;
   MinFilterType::Pointer LeftMin = MinFilterType::New();
   LeftMin->SetInput1(LeftMinIn); 
   LeftMin->SetInput2(LeftCVFFiltred->GetOutput()); 

  std::cout << " there are :  "<< HdispMax-HdispMin+1 << " Disparities \n "; 
  itk::TimeProbe chrono1;
  itk::TimeProbe chrono2;   
// left disp

for (int Hdisp = HdispMin ; Hdisp <=HdispMax ;Hdisp++){ 	 
 LeftCost->SetDisp(Hdisp); 
 LeftCost->Modified();

 chrono1.Start();
 LeftMin->GetFunctor().SetDisp(Hdisp); 
 LeftMin->Update();
 
 chrono1.Stop();
 std::cout << "Operation after update took "<< chrono1.GetTotal() << " sec" << std::endl;

 LeftDispOut = LeftMin->GetOutput();
 
 LeftMinIn = const_cast <ImageType *>( LeftMin->GetInput(0)); // GetInput() est de 3 image
 
 LeftDispOut->DisconnectPipeline();  // au lieu de déconnecter la sortie on vas échanger les buffers
 LeftMin->GraftOutput(LeftMinIn) ;

 LeftMin->SetInput1( LeftDispOut); // ne pas mettre le SetInput2 car il vas se mettre a j automatiquement avec le pipeline
  
}  
/** écriture du resultat de la disparité avec le cost volumebrute**/  
 LeftMinWriter->SetFileName(FILENAME("LeftDisparité.tif"));
 LeftMinWriter->SetInput(  LeftMin->GetOutput());  
 otb::StandardFilterWatcher LeftMinwatcher(LeftMin, "LeftMinFilter"); 
 LeftMinWriter->Update();

 
// Right disp 
 MinFilterType::Pointer RightMin = MinFilterType::New();
 RightMin->SetInput1(RightMinIn); 
 RightMin->SetInput2(RightCVFFiltred->GetOutput()); 
	 
for (int Hdisp = -HdispMax ; Hdisp <= -HdispMin ;Hdisp++){ 
 RightCost->SetDisp(Hdisp); 
 RightCost->Modified();
 
 RightMin->GetFunctor().SetDisp(Hdisp); 
 RightMin->Update();
 
 RightDispOut = RightMin->GetOutput();

 RightMinIn = const_cast <ImageType *>( RightMin->GetInput(0)); 
 
 RightDispOut->DisconnectPipeline(); 
 RightMin->GraftOutput(RightMinIn) ;

 RightMin->SetInput1(RightDispOut); 
  
}   

/** écriture du resultat de la disparité avec le cost volumebrute**/  
 RightMinWriter->SetFileName(FILENAME("RightDisparité.tif"));
 RightMinWriter->SetInput( RightMin->GetOutput());  
 otb::StandardFilterWatcher RightMinwatcher(RightMinWriter, "RightMinFilter"); 
 RightMinWriter->Update(); 
 


 /*==================================== calcul de la mediane===========================================*/

  typedef itk::MedianImageFilter< OTBImageType, OTBImageType > MedianFilterType;
  MedianFilterType::Pointer LMedian = MedianFilterType::New();
  MedianFilterType::RadiusType MRadius = {{1,1}};
  LMedian->SetRadius(MRadius);
	
  typedef otb::PerBandVectorImageFilter<ImageType, ImageType, MedianFilterType> PerBand;  
  PerBand::Pointer LeftMedianFilter = PerBand::New();
  LeftMedianFilter->SetFilter(LMedian);
  LeftMedianFilter->SetInput(LeftReader->GetOutput());//
  
  MedianFilterType::Pointer RMedian = MedianFilterType::New();
  RMedian->SetRadius(MRadius);  
  PerBand::Pointer RightMedianFilter = PerBand::New();
  RightMedianFilter->SetFilter(RMedian);
  RightMedianFilter->SetInput(RightReader->GetOutput());
  
 
  ImageWriterType::Pointer Medianwriter = ImageWriterType::New();
  Medianwriter->SetFileName( FILENAME("LeftMedian.tif"));
  Medianwriter->SetInput( LeftMedianFilter->GetOutput() );
  Medianwriter->Update();


/*========================================= filtrage de la carte de disparité  par Weighted Median  =====*/ 
   /** Extraction des bandes de l'image de MinFilter **/
   typedef otb::MultiToMonoChannelExtractROI<ImageType::InternalPixelType,
   ImageType::InternalPixelType> ExtractFilterType;
    // left                                                                              
   ExtractFilterType::Pointer Leftchannel1 = ExtractFilterType::New();   
   Leftchannel1-> SetInput(const_cast <ImageType *>( LeftMin->GetOutput() ));        
   Leftchannel1->SetChannel(1);
   
   
   // Right
   ExtractFilterType::Pointer Rightchannel1 = ExtractFilterType::New();   
   Rightchannel1-> SetInput(const_cast <ImageType *>( RightMin->GetOutput() ));         
   Rightchannel1->SetChannel(1);
   


  /** Conversion otb::Image (la 1ère composante de la sortie du Min filter) vers Otb::vecteurImage */
  typedef otb::ImageToVectorImageCastFilter<OTBImageType,ImageType> CastFilterType;
  CastFilterType::Pointer LeftDisparity = CastFilterType::New();
  LeftDisparity-> SetInput( const_cast <OTBImageType *>( Leftchannel1->GetOutput() ));
  
  CastFilterType::Pointer RightDisparity = CastFilterType::New();
  RightDisparity-> SetInput( const_cast <OTBImageType *>( Rightchannel1->GetOutput() ));

 
  /** Concatenation de la 1ère bande qui est la disparité avec l'image de de droite */
  ConcatenateVectorImageFilterType::Pointer ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  ConcatenateDispEndInLeftImage->SetInput1(LeftDisparity->GetOutput());
  ConcatenateDispEndInLeftImage->SetInput2( LeftMedianFilter->GetOutput() );

  ConcatenateVectorImageFilterType::Pointer ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
  ConcatenateDispEndInRightImage->SetInput1(RightDisparity->GetOutput());
  ConcatenateDispEndInRightImage->SetInput2( RightMedianFilter->GetOutput() );
  
  
  /** Instanciation du filtre weighted median en metant en entrée une image a 4 bandes (sortie de la concatenation) 
   * le 1ere bande etant la disparité et 3 autre l'image de droite */
   typedef  otb::WeightMedianImageFilter< ImageType, ImageType > WeightMedianType;
   //Left
   WeightMedianType::Pointer LeftMedian = WeightMedianType::New();
   LeftMedian-> SetInput(ConcatenateDispEndInLeftImage->GetOutput());
    
   
  ImageType::SizeType radiusM;
  radiusM[0] = rmf;
  radiusM[1] = rmf;   
  LeftMedian->SetRadius(radiusM) ;
   
   
  LeftMedianwriter->SetFileName(FILENAME("LeftMedianDispFilter.tif"));
 //  LeftMedianwriter->SetFileName(OUTFILE2);
  LeftMedianwriter->SetInput( LeftMedian->GetOutput() );  
  otb::StandardFilterWatcher watcherMedian(LeftMedianwriter, "LeftMedianDisp");  
  LeftMedianwriter->Update(); 
   
// right 
  WeightMedianType::Pointer RightMedian = WeightMedianType::New();
  RightMedian-> SetInput(ConcatenateDispEndInRightImage->GetOutput()); 
    
  
  RightMedian->SetRadius(radiusM) ;
   
  RightMedianwriter->SetFileName( FILENAME("RightMedianDispFilter.tif"));
  RightMedianwriter->SetInput( RightMedian->GetOutput() );  
  otb::StandardFilterWatcher watcher5(RightMedianwriter, "RightMedianDisp");  
  RightMedianwriter->Update(); 


/*========================== Detection d'occulusion =============================*/  
/** Conversion des vectorImage vers OtbImage*/
typedef otb::VectorImageToImageListFilter< ImageType, ImageListType > ListType;
ListType::Pointer OtbImageLeftMedian = ListType::New();
OtbImageLeftMedian->SetInput( LeftMedian->GetOutput() );
OtbImageLeftMedian->Update();

  
ListType::Pointer OtbImageRightMedian = ListType::New();
OtbImageRightMedian->SetInput( RightMedian->GetOutput() );
OtbImageRightMedian->Update();


/** Detection d'occlusion*/
 typedef otb::BijectionCoherencyFilter< OTBImageType, OTBImageType > OcclusionType;
  OcclusionType::Pointer Occlusionfilter = OcclusionType::New();
  Occlusionfilter->SetDirectHorizontalDisparityMapInput(OtbImageLeftMedian->GetOutput()->GetNthElement(0)); //LeftMedian
  Occlusionfilter->SetReverseHorizontalDisparityMapInput(OtbImageRightMedian->GetOutput()->GetNthElement(0)); //Rightmedian
   
  Occlusionfilter->SetMaxHDisp(HdispMax);
  Occlusionfilter->SetMinHDisp(HdispMin);
  Occlusionfilter->SetMinVDisp(0);
  Occlusionfilter->SetMaxVDisp(0);
 
  Occlusionfilter->SetTolerance(1);
 
/** écriture du resultat de l'oclusion**/ 
 OtbImageWriterType::Pointer OcclusionWriter = OtbImageWriterType::New(); 
 OcclusionWriter->SetFileName( FILENAME("Occlusion.tif"));
 OcclusionWriter->SetInput( Occlusionfilter->GetOutput());  
 otb::StandardFilterWatcher OcclusionWatcher(OcclusionWriter, "Occlusion"); 
 OcclusionWriter->Update(); 
 
/*========================== Fill occlusion =====================================*/
  CastFilterType::Pointer OccCastFiler = CastFilterType::New();
  OccCastFiler-> SetInput( const_cast <OTBImageType *>( Occlusionfilter->GetOutput() ));
  
  
  ConcatenateVectorImageFilterType::Pointer ConcatenateOccEndInLeftMedImage = ConcatenateVectorImageFilterType::New();
  ConcatenateOccEndInLeftMedImage->SetInput1(OccCastFiler->GetOutput());
  ConcatenateOccEndInLeftMedImage->SetInput2( LeftMedian->GetOutput() );
  
  typedef otb::FillOcclusionImageFilter<ImageType,ImageType> FillOcclusionType;
  FillOcclusionType::Pointer FillOcclusionfilter = FillOcclusionType::New();
  FillOcclusionfilter->SetInput(ConcatenateOccEndInLeftMedImage->GetOutput()); 
  ImageType::SizeType OccRadius;
  OccRadius[0] = 9;
  OccRadius[1] = 0;
  
  FillOcclusionfilter->SetRadius(OccRadius);
 
  
/** écriture du resultat de la disparité avec le cost volumebrute**/ 
 ImageWriterType::Pointer FillOcclusionWriter = ImageWriterType::New(); 
 FillOcclusionWriter->SetFileName( FILENAME("FillOcclusion.tif"));
 FillOcclusionWriter->SetInput( FillOcclusionfilter->GetOutput());  
 otb::StandardFilterWatcher FillOcclusionWatcher(FillOcclusionWriter, "FillOcclusion"); 
 FillOcclusionWriter->Update();
 
 
   /** Instanciation du filtre weighted median en metant en entrée une image a 4 bandes (sortie de la concatenation) 
   * le 1ere bande etant la carte de fill occlusion et 3 autre l'image de droite filtré avec un median simple */
   
  ConcatenateVectorImageFilterType::Pointer ConcatenateFillEndInLeftMedImage = ConcatenateVectorImageFilterType::New();
  ConcatenateFillEndInLeftMedImage->SetInput1(FillOcclusionfilter->GetOutput());
  ConcatenateFillEndInLeftMedImage->SetInput2( LeftMedianFilter->GetOutput() );
  
  
  WeightMedianType::Pointer FillMedian = WeightMedianType::New();
  FillMedian-> SetInput(ConcatenateFillEndInLeftMedImage->GetOutput()); 
  FillMedian->SetRadius(radiusM) ;
 
  FillMedianwriter->SetFileName( FILENAME("SmoothFillDisparity.tif"));
  FillMedianwriter->SetInput( FillMedian->GetOutput() );  
  FillMedianwriter->Update(); 
 
 //#endif 

  if(false){
    // ================ Test du RMSE =================

    ReaderType::Pointer GroundTruth = ReaderType::New();
    ReaderType::Pointer FillMedianI = ReaderType::New();

    GroundTruth->SetFileName("/home/julie/Documents/c++/data/truedisp.row3.col3.pgm"); 
    GroundTruth->UpdateOutputInformation();

    FillMedianI->SetFileName("/home/julie/Documents/c++/results/CVFDisparity.tif"); 
    FillMedianI->UpdateOutputInformation();

    typedef otb::RMSEVectorImageFilter< ImageType, ImageType > RMSEType;

    RMSEType::Pointer RMSEfilter = RMSEType::New();
    RMSEfilter->SetEstimatedInputImage(GroundTruth->GetOutput()); //groundTruth
    RMSEfilter->SetInputImage(FillMedianI->GetOutput()); //Fillmedian
    RMSEfilter->UpdateOutputInformation();


    ImageWriterType::Pointer RMSEWriter = ImageWriterType::New(); 
   RMSEWriter->SetFileName( FILENAME("RMSEfilter.tif"));
   RMSEWriter->SetInput( RMSEfilter->GetOutput());  
   otb::StandardFilterWatcher RMSEWatcher(RMSEWriter, "RMSEfilter"); 
   RMSEWriter->Update();

/*
    
   ReaderType::Pointer GroundTruth = ReaderType::New();
   
    GroundTruth->SetFileName("/home/julie/Documents/c++/data/scene1_row3_col3.ppm"); //LeftImage 
    GroundTruth->UpdateOutputInformation();//*


   typedef otb::RMSEVectorImageFilter< ImageType, ImageType > RMSEType;
    RMSEType::Pointer RMSEfilter = RMSEType::New();
    RMSEfilter->SetEstimatedInputImage(LeftReader->GetOutput()); //groundTruth
    RMSEfilter->SetInputImage(LeftReader->GetOutput()); //Fillmedian
    */
   
  /** écriture du resultat de la disparité avec le cost volumebrute**/ 
/*
    
   ImageWriterType::Pointer RMSEWriter = ImageWriterType::New(); 
   RMSEWriter->SetFileName( FILENAME("RMSEfilter.tif"));
   RMSEWriter->SetInput( RMSEfilter->GetOutput());  
   otb::StandardFilterWatcher RMSEWatcher(RMSEWriter, "RMSEfilter"); 
   RMSEWriter->Update();
   */

  }
 
return EXIT_SUCCESS;

}

 

