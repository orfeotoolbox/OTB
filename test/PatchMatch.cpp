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


/* Execution ./bin/otbPatchMatchFiltersTestDriver testPatchMatch radius dispMin dispMax
  Left_Image(3bands) Right_Image(3bands) Path_Of_Outputs_Images/  
 
 Exemple:
  make -j3 OTBPatchMatchFilters-all && ./bin/otbPatchMatchFiltersTestDriver testPatchMatch 7 -15 0 19
   tsukuba0.png tsukuba1.png SortiePatchMatch/

 */ 

//standard
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

//PatchMatch

#include "otbCoefOfThePatchFilter.h"
#include "otbAggregatedCostVectorPixelFilter.h"
#include "otbSpatialPropagationImageFilter.h"
#include "otbViewPropagationImageFilter.h"
#include "otbPatchMatchDisparityImageFilter.h"
#include "otbPatchMatchRefinementImageFilter.h"
#include "otbMinPlaneRefinementImageFilter.h"
#include "otbPatchWeightedMedianImageFilter.h"
#include "otbDisparityWeightedMedianImageFilter.h"

#include "otbBoxImageFilter.h"
#include "otbLocalMeanBoxVectorImageFilter.h"
//OTB
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbLocalGradientVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbLocalGradientVectorImageFilter.h" 
#include "otbThresholdVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h" //muti channel 
#include "otbVectorRescaleIntensityImageFilter.h" //muti channel
#include "otbImageToVectorImageCastFilter.h" // cast Image to vecteur image
#include "otbDisparityMapMedianFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbConvolutionImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "otbStandardFilterWatcher.h"   // pour le watcher
#include "otbBijectionCoherencyFilter.h"
#include "otbVectorImageToImageListFilter.h" //Conversion de vectorImageToOTBImage

//ITK
#include <itkImage.h>
//#include "itkAddConstantToImageFilter.h"
#include <itkAddImageFilter.h>
#include <itkVectorImageToImageAdaptor.h>
#include "itkCastImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkTimeProbe.h" //crono
#include "itkConstantBoundaryCondition.h"

	
int testPatchMatch(int argc, char *argv[])
{

	
if(argc < 8) {

		std::cerr << "Usage: " << argv[0] << " radius min_disp max_disp radius_to_mean_filter Left_Image Right_Image Out_Path" << std::endl;
		return EXIT_FAILURE;
	}
	
  long unsigned int PatchSize = atoi(argv[1]);
	int HdispMin = atoi(argv[2]);
	int HdispMax  = atoi(argv[3]);
  int rmf  = atoi(argv[4]);
	 
	
  
// Main type definition
const unsigned int Dimension = 2;
typedef otb::Image< double, Dimension > OTBImageType;
typedef otb::VectorImage< double , Dimension > ImageType;

typedef otb::ImageFileWriter< ImageType > ImageWriterType;
 
  // Reading Leftinput images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer LeftReader = ReaderType::New();
  LeftReader->SetFileName(argv[5]); //LeftImage 
  LeftReader->UpdateOutputInformation();
 
  // Reading RightInput images
  typedef otb::ImageFileReader<ImageType> RightReaderType;
  RightReaderType::Pointer RightReader = RightReaderType::New();
  RightReader->SetFileName(argv[6]);//RightImage
  RightReader->UpdateOutputInformation();
  
  
  
   //~ // Reading Leftinput images
  //~ typedef otb::ImageFileReader<ImageType> ReaderType;
  //~ ReaderType::Pointer LeftImage = ReaderType::New();
  //~ LeftImage->SetFileName(argv[4]); //LeftImage 
  //~ LeftImage->UpdateOutputInformation();
 
  //~ // Reading ReghtInput images
  //~ typedef otb::ImageFileReader<ImageType> RightReaderType;
  //~ RightReaderType::Pointer RightImage = RightReaderType::New();
  //~ RightImage->SetFileName(argv[5]);//RightImage
  //~ RightImage->UpdateOutputInformation();
  
 //~ typedef otb::BoxImageFilter< ImageType, ImageType > BoxFilterType;  
  //~ BoxFilterType::Pointer LeftBox = BoxFilterType::New();
  //~ LeftBox->SetInput( LeftImage->GetOutput() );
  
  //~ BoxFilterType::Pointer RightBox = BoxFilterType::New();
  //~ RightBox->SetInput( RightImage->GetOutput() );
  
 
//~ typedef otb::LocalMeanBoxVectorImageFilter< ImageType, ImageType > MeanBoxFilterType;  
  //~ MeanBoxFilterType::Pointer LeftReader = MeanBoxFilterType::New();
  //~ LeftReader->SetInput( LeftBox->GetOutput() );  
  //~ MeanBoxFilterType::RadiusType radius = {{9,9}};
  //~ LeftReader->SetRadius(radius);
  
  //~ MeanBoxFilterType::Pointer RightReader = MeanBoxFilterType::New();
  //~ RightReader->SetInput( RightBox->GetOutput() );  
  //~ RightReader->SetRadius(radius); 
  
  
  
  
 //argv[7] le chemin des images de sortie  
  std::string argv7 = std::string(argv[7]);
  #define FILENAME(n) std::string( argv7 + std::string(n)).c_str()
  
 
  


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
  //GradientXL->SetNumberOfThreads(1);
 
  //~ ImageWriterType::Pointer writerGradientXL = ImageWriterType::New();
  //~ writerGradientXL->SetFileName( FILENAME("GradientXLeft.tif"));
  //~ writerGradientXL->SetInput(GradientXL->GetOutput());
  //~ writerGradientXL->Update();
  
//--Right---------------  
 typedef otb::PerBandVectorImageFilter<ImageType, ImageType, ConvFilterTypeRight> VectorFilterTypeRight;
  VectorFilterTypeRight::Pointer GradientXR = VectorFilterTypeRight::New();
  GradientXR->SetFilter(convFilterXR);
  GradientXR->SetInput(RightReader->GetOutput());
 
  //~ ImageWriterType::Pointer writerGradientXR = ImageWriterType::New();
  //~ writerGradientXR->SetFileName(FILENAME("GradientXRight.tif"));
  //~ writerGradientXR->SetInput(GradientXR->GetOutput());
  //~ writerGradientXR->Update();

/*=====================Compute Coeffs of the patch =========================================================*/

typedef otb::CoefOfThePatchFilter< ImageType, ImageType> CoefPatchFilterType;
  //Left
  CoefPatchFilterType::Pointer LeftCoefPatch = CoefPatchFilterType::New();
  LeftCoefPatch->SetInputImage(LeftReader->GetOutput());
  LeftCoefPatch->SetMindisp(HdispMin);
  LeftCoefPatch->SetMaxdisp(HdispMax);
// LeftCoefPatch->UpdateOutputInformation();
  // Right
  CoefPatchFilterType::Pointer RightCoefPatch = CoefPatchFilterType::New();
  RightCoefPatch->SetInputImage(RightReader->GetOutput());
  RightCoefPatch->SetMindisp(-HdispMax);
  RightCoefPatch->SetMaxdisp(-HdispMin);
 // RightCoefPatch->UpdateOutputInformation(); 

 ImageWriterType::Pointer CoefPatchWriter = ImageWriterType::New();
  CoefPatchWriter->SetFileName(FILENAME("CoefPatch.tif"));
  CoefPatchWriter->SetInput( RightCoefPatch->GetOutputCoefImage() );   
  CoefPatchWriter->Update();
  
/* ************************************************************************* */
/*========================================== Spatial Propagation ===========================================*/

itk::TimeProbe chrono1;
chrono1.Start();
	 
   /** sortie a une seule bande et elle contient le volume de cout */
typedef otb::SpatialPropagationImageFilter< ImageType, ImageType > SpatialPropType;
// Left 
	 
  SpatialPropType::OffsetType Offset1R;
  SpatialPropType::OffsetType Offset2R;
    
  Offset1R[0] = 0;
  Offset1R[1] = 1;
  
  Offset2R[0] = 1;
  Offset2R[1] = 0;
  
  SpatialPropType::OffsetType Offset0;
  SpatialPropType::OffsetType Offset1;
  SpatialPropType::OffsetType Offset2;
  Offset0[0] = 0;
  Offset0[1] = 0;
  
  Offset1[0] = -1;
  Offset1[1] = 0;
  
  Offset2[0] = 0;
  Offset2[1] = -1;
  
 typename ImageType::PixelType In(3);
 In.Fill(0.);
  
 SpatialPropType::Pointer LeftSpatial = SpatialPropType::New();
 SpatialPropType::Pointer RightSpatial = SpatialPropType::New();
 /// créer les images a mettre en sortie du filtre a déconnecter
 
LeftCoefPatch->UpdateOutputInformation();
RightCoefPatch->UpdateOutputInformation();
 
 ImageType::Pointer LeftIn = ImageType::New(); 
   LeftIn->CopyInformation(LeftCoefPatch->GetOutputCoefImage());
    LeftIn->SetNumberOfComponentsPerPixel(3);
    LeftIn->SetRegions(LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
    LeftIn->Allocate();   
    LeftIn->FillBuffer(In);
  

 
 /// créer une image pour permuter l'entrer et la sortie pour mettre a jour les coefs
	ImageType::Pointer LeftCoefPlane  = ImageType::New();; 
	LeftCoefPlane->CopyInformation(LeftCoefPatch->GetOutputCoefImage());
    LeftCoefPlane->SetNumberOfComponentsPerPixel(3);
    LeftCoefPlane->SetRegions(LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
    LeftCoefPlane->Allocate();   
    LeftCoefPlane->FillBuffer(In);	
	LeftCoefPlane = LeftCoefPatch->GetOutputCoefImage();
	
	//std::cout<< "  origin   " << LeftCoefPlane->GetOrigin()<< std::endl;
	
	ImageType::Pointer LeftNormalPlane = ImageType::New();; 
	LeftNormalPlane->CopyInformation(LeftCoefPatch->GetOutputCoefImage());
    LeftNormalPlane->SetNumberOfComponentsPerPixel(3);
    LeftNormalPlane->SetRegions(LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
    LeftNormalPlane->Allocate();   
    LeftNormalPlane->FillBuffer(In);
	LeftNormalPlane = LeftCoefPatch->GetOutputNormalAndZValueImage(); 

	ImageType::Pointer RightCoefPlane = ImageType::New();;
	RightCoefPlane->CopyInformation(LeftCoefPatch->GetOutputCoefImage());
    RightCoefPlane->SetNumberOfComponentsPerPixel(3);
    RightCoefPlane->SetRegions(LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
    RightCoefPlane->Allocate();   
    RightCoefPlane->FillBuffer(In);	
	RightCoefPlane = RightCoefPatch->GetOutputCoefImage();
	
	//std::cout<< "  origin   " << RightCoefPlane->GetOrigin()<< std::endl;
	 
	ImageType::Pointer RightNormalPlane = ImageType::New();;
	RightNormalPlane->CopyInformation(LeftCoefPatch->GetOutputCoefImage());
    RightNormalPlane->SetNumberOfComponentsPerPixel(3);
    RightNormalPlane->SetRegions(LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
    RightNormalPlane->Allocate();   
    RightNormalPlane->FillBuffer(In);
	RightNormalPlane = RightCoefPatch->GetOutputNormalAndZValueImage();
	
 typedef otb::MinPlaneRefinementImageFilter< ImageType, ImageType > MinPlaneType;	
   MinPlaneType::Pointer LeftMinPlaneView = MinPlaneType::New(); 
  MinPlaneType::Pointer RightMinPlaneView = MinPlaneType::New();  
 
double MaxDz0 = HdispMin/2;
double MaxDn = 1;
//char Sense;
for(unsigned int iteration = 1 ; iteration < 4; iteration++){
 std::cout<< "  iteration   " << iteration<< std::endl;
 ///Left  
  LeftSpatial->SetLeftInputImage(LeftReader->GetOutput());
  LeftSpatial->SetRightInputImage(RightReader->GetOutput());  
  LeftSpatial->SetLeftGradientXInput(GradientXL->GetOutput());
  LeftSpatial->SetRightGradientXInput(GradientXR->GetOutput());  
  LeftSpatial->SetPatchInputImage(LeftCoefPlane);
  LeftSpatial->SetNormalAndZValueImage(LeftNormalPlane); //
  LeftSpatial->SetIteration(iteration);
  LeftSpatial->SetPatchSize(PatchSize,PatchSize);
  LeftSpatial->SetSense(1); 
 
if(iteration % (2) == 1){
  LeftSpatial->SetOffsetPatch(Offset0,Offset1,Offset2);
}

else{
  LeftSpatial->SetOffsetPatch(Offset0,Offset1R,Offset2R);
}

/// Right
  
  RightSpatial->SetLeftInputImage(LeftReader->GetOutput());
  RightSpatial->SetRightInputImage(RightReader->GetOutput());  
  RightSpatial->SetLeftGradientXInput(GradientXL->GetOutput());
  RightSpatial->SetRightGradientXInput(GradientXR->GetOutput());  
  RightSpatial->SetPatchInputImage(RightCoefPlane); 
  RightSpatial->SetNormalAndZValueImage(RightNormalPlane); //
  RightSpatial->SetIteration(iteration);
  RightSpatial->SetPatchSize(PatchSize,PatchSize); 
  RightSpatial->SetSense(0); 
 
if(iteration % (2) == 1){
  RightSpatial->SetOffsetPatch(Offset0,Offset1,Offset2);
}

else{
  RightSpatial->SetOffsetPatch(Offset0,Offset1R,Offset2R);
}


 ImageWriterType::Pointer SpatialPlaneWriter = ImageWriterType::New();
  SpatialPlaneWriter->SetFileName(FILENAME("SpatialPropPlaneOutputImage.tif"));
  SpatialPlaneWriter->SetInput( LeftSpatial->GetOutputPatchImage() ); 
otb::StandardFilterWatcher watcherPlane(SpatialPlaneWriter, "SpatialPropFilter");  
  SpatialPlaneWriter->Update();   

  
   ImageWriterType::Pointer SpatialCostWriter = ImageWriterType::New();
  SpatialCostWriter->SetFileName(FILENAME("SpatialPropCostOutputImage.tif"));  
  SpatialCostWriter->SetInput( LeftSpatial->GetOutputCostImage() );
 otb::StandardFilterWatcher watcherCost(SpatialCostWriter, "SpatialPropFilter");  
  SpatialCostWriter->Update();
  


  
/* ************************************************************************* */ 
/*=====================  View Propagation =======================================================*/
   /** sortie a une seule bande et elle contient le volume de cout */
typedef otb::ViewPropagationImageFilter< ImageType, ImageType > ViewPropType;

ViewPropType::Pointer LeftView = ViewPropType::New();
ViewPropType::Pointer RightView = ViewPropType::New();


//Left view
  LeftView->SetSpatialCostImage(RightSpatial->GetOutputCostImage());
  LeftView->SetLeftInputImage(LeftReader->GetOutput());
  LeftView->SetRightInputImage(RightReader->GetOutput());   
  LeftView->SetLeftPatchInputImage(LeftSpatial->GetOutputPatchImage()); 
  LeftView->SetLeftNormalAndZValueImage(LeftSpatial->GetOutputNormalAndZValueImage());
  LeftView->SetRightPatchInputImage(RightSpatial->GetOutputPatchImage());
  LeftView->SetRightNormalAndZValueImage(RightSpatial->GetOutputNormalAndZValueImage()); 
  LeftView->SetLeftGradientXInput(GradientXL->GetOutput());
  LeftView->SetRightGradientXInput(GradientXR->GetOutput());
  LeftView->SetIteration(iteration);
  LeftView->SetPatchSize(PatchSize,PatchSize);
  LeftView->SetDispMinMax(HdispMin,HdispMax); // pour deteminer la taille de la fenetre de recherche
  LeftView->SetRadius((unsigned int)((HdispMax-HdispMin)/2), 0);
 
   
//Right view
  RightView->SetSpatialCostImage(LeftSpatial->GetOutputCostImage());
  RightView->SetLeftInputImage(RightReader->GetOutput());
  RightView->SetRightInputImage(LeftReader->GetOutput());   
  RightView->SetLeftPatchInputImage(RightSpatial->GetOutputPatchImage()); 
  RightView->SetLeftNormalAndZValueImage(RightSpatial->GetOutputNormalAndZValueImage());
  RightView->SetRightPatchInputImage(LeftSpatial->GetOutputPatchImage());
  RightView->SetRightNormalAndZValueImage(LeftSpatial->GetOutputNormalAndZValueImage()); 
  RightView->SetLeftGradientXInput(GradientXR->GetOutput());
  RightView->SetRightGradientXInput(GradientXL->GetOutput());
  RightView->SetIteration(iteration);
  RightView->SetPatchSize(PatchSize,PatchSize);
  RightView->SetDispMinMax(HdispMin, HdispMax);
  RightView->SetRadius((unsigned int)((HdispMax-HdispMin)/2), 0);


ImageWriterType::Pointer CostViewWriter = ImageWriterType::New();
  CostViewWriter->SetFileName(FILENAME("ViewPropCost.tif"));  
  CostViewWriter->SetInput( LeftView->GetOutputCostImage() );
 otb::StandardFilterWatcher watcherCView(CostViewWriter, "ViewCostFilter");  
  CostViewWriter->Update(); 
 
   ImageWriterType::Pointer PlaneViewWriter = ImageWriterType::New();
  PlaneViewWriter->SetFileName(FILENAME("ViewPropPlane.tif"));  
  PlaneViewWriter->SetInput( LeftView->GetOutputPatchImage() );
 otb::StandardFilterWatcher watcherPView(PlaneViewWriter, "ViewPlaneFilter");  
  PlaneViewWriter->Update(); 
/* ************************************************************************* */  
/*=================== Refinement ====================================================*/

typedef otb::PatchMatchRefinementImageFilter< ImageType, ImageType > RefinementType;
///Left
  RefinementType::Pointer LeftRefinement = RefinementType::New(); 
  LeftRefinement->SetNormalAndZValueImage(LeftView->GetOutputNormalAndZValueImage());
  LeftRefinement->SetMaxDz0(MaxDz0); 
  LeftRefinement->SetMaxDn(MaxDn); 
 LeftRefinement->SetDispMinMax(HdispMin, HdispMax);
 


/// Right
  RefinementType::Pointer RightRefinement = RefinementType::New(); 
  RightRefinement->SetNormalAndZValueImage(RightView->GetOutputNormalAndZValueImage());
  RightRefinement->SetMaxDz0(MaxDz0);
  RightRefinement->SetMaxDn(MaxDn); 
 RightRefinement->SetDispMinMax(HdispMin, HdispMax); 
  
  ImageWriterType::Pointer RefineViewWriter = ImageWriterType::New();
  RefineViewWriter->SetFileName(FILENAME("LeftRefinement.tif")); 
  RefineViewWriter->SetInput( LeftRefinement->GetOutputPatchImage() );   
 otb::StandardFilterWatcher watcherRV(RefineViewWriter, "LeftRefinement");  
  RefineViewWriter->Update();  
/* ************************************************************************/
/* =========================== MinPlaneRefinement =========================*/

 //~ typedef otb::MinPlaneRefinementImageFilter< ImageType, ImageType > MinPlaneType;
 ///Left
  //MinPlaneType::Pointer LeftMinPlaneView = MinPlaneType::New();  
  LeftMinPlaneView->SetLeftInputImage(LeftReader->GetOutput());
  LeftMinPlaneView->SetRightInputImage(RightReader->GetOutput());  
  LeftMinPlaneView->SetLeftGradientXInput(GradientXL->GetOutput());
  LeftMinPlaneView->SetRightGradientXInput(GradientXR->GetOutput()); 
   
  LeftMinPlaneView->SetPatchInputImage(LeftView->GetOutputPatchImage()); 
  LeftMinPlaneView->SetCostInputImage(LeftView->GetOutputCostImage()); //
  LeftMinPlaneView->SetNormalAndZValueImage(LeftView->GetOutputNormalAndZValueImage());
    
  LeftMinPlaneView->SetCoefRefinedInputImage(LeftRefinement->GetOutputPatchImage());
  LeftMinPlaneView->SetRefinedNormalAndZValueImage(LeftRefinement->GetOutputNormalAndZValueImage());
  
  LeftMinPlaneView->SetPatchSize(PatchSize,PatchSize); 
  LeftMinPlaneView->SetOffsetPatch(Offset0,Offset0,Offset0); 
 
  LeftMinPlaneView->Update();
   ///Right
 
  //~ MinPlaneType::Pointer RightMinPlaneView = MinPlaneType::New();  
  RightMinPlaneView->SetLeftInputImage(LeftReader->GetOutput());
  RightMinPlaneView->SetRightInputImage(RightReader->GetOutput());  
  RightMinPlaneView->SetLeftGradientXInput(GradientXL->GetOutput());
  RightMinPlaneView->SetRightGradientXInput(GradientXR->GetOutput()); 
   
  RightMinPlaneView->SetPatchInputImage(RightView->GetOutputPatchImage()); 
  RightMinPlaneView->SetCostInputImage(RightView->GetOutputCostImage()); 
  RightMinPlaneView->SetNormalAndZValueImage(RightView->GetOutputNormalAndZValueImage());
    
  RightMinPlaneView->SetCoefRefinedInputImage(RightRefinement->GetOutputPatchImage());
  RightMinPlaneView->SetRefinedNormalAndZValueImage(RightRefinement->GetOutputNormalAndZValueImage());
  
  RightMinPlaneView->SetPatchSize(PatchSize,PatchSize); 
  RightMinPlaneView->SetOffsetPatch(Offset0,Offset0,Offset0);
   
  RightMinPlaneView->Update();

  ImageWriterType::Pointer LeftMinRVWriter = ImageWriterType::New();
  LeftMinRVWriter->SetFileName(FILENAME("LeftMinCost.tif"));  
  LeftMinRVWriter->SetInput( LeftMinPlaneView->GetOutputCostImage());   
 otb::StandardFilterWatcher LwatcherMinRV(LeftMinRVWriter, "LeftMinPlane");  
  LeftMinRVWriter->Update();
  
   ImageWriterType::Pointer RightMinRVWriter = ImageWriterType::New();
  RightMinRVWriter->SetFileName(FILENAME("LeftMinCost.tif"));  
  RightMinRVWriter->SetInput( RightMinPlaneView->GetOutputCostImage());   
 otb::StandardFilterWatcher RwatcherMinRV(RightMinRVWriter, "LeftMinPlane");  
  RightMinRVWriter->Update();

 /// mèttre à jour les plan et les normales  

	LeftCoefPlane = LeftMinPlaneView->GetOutputPatchImage();
	LeftCoefPlane->DisconnectPipeline();
	// std::cout<< "  origin   " << LeftCoefPlane->GetOrigin()<< std::endl;
	LeftNormalPlane = LeftMinPlaneView->GetOutputNormalAndZValueImage(); 
	LeftNormalPlane->DisconnectPipeline();
	

	RightCoefPlane = RightMinPlaneView->GetOutputPatchImage();
	RightCoefPlane->DisconnectPipeline();
	RightNormalPlane = RightMinPlaneView->GetOutputNormalAndZValueImage(); 
  RightNormalPlane->DisconnectPipeline();
    
    
   /// refinement Disparity
   
if(MaxDz0>0.1){		
MaxDz0 = MaxDz0/2;
MaxDn = MaxDn/2; 
}
else{
MaxDz0 = MaxDz0;
MaxDn = MaxDn;
} 
chrono1.Stop();
std::cout << "Operation after update took "<< chrono1.GetTotal() << " sec" << std::endl;
} // end of for
 
 
 
  
//~ // ====  Plane Weighted Median ==========================
   //~ /** Concatenation de la disparité avec l'image de de droite  ou gauche*/
 //~ typedef otb::ConcatenateVectorImageFilter< ImageType,ImageType, ImageType> ConcatenateVectorImageFilterType;
  //~ ConcatenateVectorImageFilterType::Pointer DispEndLeftImage = ConcatenateVectorImageFilterType::New();
  //~ DispEndLeftImage->SetInput1(LeftMinPlaneView->GetOutputPatchImage());
  //~ DispEndLeftImage->SetInput2( LeftReader->GetOutput() );

  //~ ConcatenateVectorImageFilterType::Pointer DispEndRightImage = ConcatenateVectorImageFilterType::New();
  //~ DispEndRightImage->SetInput1(RightMinPlaneView->GetOutputPatchImage());
  //~ DispEndRightImage->SetInput2( RightReader->GetOutput() ); 


 //~ typedef otb::PatchWeightedMedianImageFilter< ImageType, ImageType > PlaneMedianType;
 //~ //Left
//~ PlaneMedianType::Pointer LeftPMedianFilter = PlaneMedianType::New();

//~ PlaneMedianType::RadiusType radiusPMedian;
  //~ radiusPMedian[0] = 19;
  //~ radiusPMedian[1] = 19;

//~ LeftPMedianFilter->SetInput(DispEndLeftImage->GetOutput());
//~ LeftPMedianFilter->SetRadius(radiusPMedian);

 //~ //Right
//~ PlaneMedianType::Pointer RightPMedianFilter = PlaneMedianType::New();
//~ RightPMedianFilter->SetInput(DispEndRightImage->GetOutput());
//~ RightPMedianFilter->SetRadius(radiusPMedian);
 
  //~ ImageWriterType::Pointer LeftPMedianWriter = ImageWriterType::New();
  //~ LeftPMedianWriter->SetFileName(FILENAME("LeftPMedian.tif"));  
  //~ LeftPMedianWriter->SetInput( LeftPMedianFilter->GetOutput() );   
 //~ otb::StandardFilterWatcher watcherLPM(LeftPMedianWriter, "LeftPMedian");  
  //~ LeftPMedianWriter->Update(); 
  

  
  
  
     
 /* ************************************************************************* */ 
/*=============================Disparity ======================================*/
typedef otb::PatchMatchDisparityImageFilter< ImageType, ImageType > DisparityType;
DisparityType::Pointer LeftDispFilter = DisparityType::New();

DisparityType::RadiusType radiusD;
  radiusD[0] = 0;
  radiusD[1] = 0;

LeftDispFilter->SetInput(LeftMinPlaneView->GetOutputPatchImage());
LeftDispFilter->SetRadius(radiusD);
/// Right

DisparityType::Pointer RightDispFilter = DisparityType::New();
RightDispFilter->SetInput(RightMinPlaneView->GetOutputPatchImage());
RightDispFilter->SetRadius(radiusD);

   ImageWriterType::Pointer DispWriter = ImageWriterType::New();
  DispWriter->SetFileName(FILENAME("LeftDisp.tif"));  
  DispWriter->SetInput( LeftDispFilter->GetOutput() );   
 otb::StandardFilterWatcher watcherDisp(DispWriter, "LeftDispFilter");  
  DispWriter->Update();
  
// ======== Weighted Median Disparity ==============================  
  
   /** Concatenation de la disparité avec l'image de de droite  ou gauche*/
 typedef otb::ConcatenateVectorImageFilter< ImageType,ImageType, ImageType> ConcatenateVectorImageFilterType; 
  ConcatenateVectorImageFilterType::Pointer ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  ConcatenateDispEndInLeftImage->SetInput1(LeftDispFilter->GetOutput());
  ConcatenateDispEndInLeftImage->SetInput2( LeftReader->GetOutput() );

  ConcatenateVectorImageFilterType::Pointer ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
  ConcatenateDispEndInRightImage->SetInput1(RightDispFilter->GetOutput());
  ConcatenateDispEndInRightImage->SetInput2( RightReader->GetOutput() ); 
  


typedef otb::DisparityWeightedMedianImageFilter< ImageType, ImageType > DispMedianType;
DispMedianType::RadiusType radiusPMedian;
  radiusPMedian[0] = rmf;
  radiusPMedian[1] = rmf;
/// Left
DispMedianType::Pointer LeftDispMedian = DispMedianType::New();
LeftDispMedian->SetInput(ConcatenateDispEndInLeftImage->GetOutput());
LeftDispMedian->SetRadius(radiusPMedian);
/// Right

DispMedianType::Pointer RightDispMedian = DispMedianType::New();
RightDispMedian->SetInput(ConcatenateDispEndInRightImage->GetOutput());
RightDispMedian->SetRadius(radiusPMedian);

 ImageWriterType::Pointer DispMedWriter = ImageWriterType::New();
 DispMedWriter->SetFileName(FILENAME("LeftDispMedian.tif"));  
 DispMedWriter->SetInput(LeftDispMedian->GetOutput() );   
 otb::StandardFilterWatcher watcherMedDisp(DispMedWriter, "LeftDispMedian");  
 DispMedWriter->Update();
 
/*========================== Detection d'occulusion =============================*/
  
//~ /** Conversion des vectorImage vers OtbImage*/
//~ typedef otb::VectorImageToImageListFilter< ImageType, ImageListType > ListType;
//~ ListType::Pointer OtbImageLeftDispFilter = ListType::New();
//~ OtbImageLeftDispFilter->SetInput( LeftDispFilter->GetOutput() );
//~ OtbImageLeftDispFilter->Update();

  
//~ ListType::Pointer OtbImageRightDispFilter = ListType::New();
//~ OtbImageRightDispFilter->SetInput( RightMedian->GetOutput() );
//~ OtbImageRightDispFilter->Update();


//~ /** Detection d'occlusion*/
 //~ typedef otb::BijectionCoherencyFilter< OTBImageType, OTBImageType > OcclusionType;
  //~ OcclusionType::Pointer Occlusionfilter = OcclusionType::New();
  //~ Occlusionfilter->SetDirectHorizontalDisparityMapInput(OtbImageLeftDispFilter->GetOutput()->GetNthElement(0)); //LeftMedian
  //~ Occlusionfilter->SetReverseHorizontalDisparityMapInput(OtbImageRightDispFilter->GetOutput()->GetNthElement(0)); //Rightmedian
   
  //~ Occlusionfilter->SetMaxHDisp(HdispMax);
  //~ Occlusionfilter->SetMinHDisp(HdispMin);
  //~ Occlusionfilter->SetMinVDisp(0);
  //~ Occlusionfilter->SetMaxVDisp(0);
 
  //~ Occlusionfilter->SetTolerance(1);
 
//~ /** écriture du resultat de l'oclusion**/ 
 //~ OtbImageWriterType::Pointer OcclusionWriter = OtbImageWriterType::New(); 
 //~ OcclusionWriter->SetFileName( FILENAME("Occlusion.tif"));
 //~ OcclusionWriter->SetInput( Occlusionfilter->GetOutput());  
 //~ otb::StandardFilterWatcher OcclusionWatcher(OcclusionWriter, "Occlusion"); 
 //~ OcclusionWriter->Update();
 


 


return EXIT_SUCCESS;
}

 

