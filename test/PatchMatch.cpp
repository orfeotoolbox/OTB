/** Execution ./bin/otbPatchMatchFiltersTestDriver main_PatchMatch radius dispMin dispMax
 * Left_Image(3bands) Right_Image(3bands) Path_Of_Outputs_Images/ 
 
 
 Exemple:
 * make -j3 OTBPatchMatchFilters-all && ./bin/otbPatchMatchFiltersTestDriver main_PatchMatch 9 -15 0
   /home/dbelazou/src/otb/Modules/Remote/MatchingFilters/data/ImageRGB/middlebury/tsukuba0.png 
  /home/dbelazou/src/otb/Modules/Remote/MatchingFilters/data/ImageRGB/middlebury/tsukuba1.png 
  /home/dbelazou/src/otb/Modules/Remote/MatchingFilters/data/SortiePatchMatch/

 * **/ 

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
#include "itkAddConstantToImageFilter.h"
#include <itkVectorImageToImageAdaptor.h>
#include "itkCastImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkTimeProbe.h" //crono
#include "itkConstantBoundaryCondition.h"

	
int main_PatchMatch(int argc, char *argv[])
{
	
if(argc < 7) {
		std::cerr << "Usage: " << argv[0] << " radius min_disp max_disp Left_Image Right_Image Out_Path" << std::endl;
		return EXIT_FAILURE;
	}
	
     long unsigned int PatchSize = atoi(argv[1]);
	 int HdispMin = atoi(argv[2]);
	 int HdispMax  = atoi(argv[3]);
	 
	
  
// Main type definition
const unsigned int Dimension = 2;
typedef otb::Image< double, Dimension > OTBImageType;
typedef otb::VectorImage< double , Dimension > ImageType;

typedef otb::ImageFileWriter< ImageType > ImageWriterType;
 
  // Reading Leftinput images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer LeftReader = ReaderType::New();
  LeftReader->SetFileName(argv[4]); //LeftImage 
  LeftReader->UpdateOutputInformation();
 
  // Reading ReghtInput images
  typedef otb::ImageFileReader<ImageType> RightReaderType;
  RightReaderType::Pointer RightReader = RightReaderType::New();
  RightReader->SetFileName(argv[5]);//RightImage
  RightReader->UpdateOutputInformation();
  
 //argv[6] le chemin des images de sortie  
  std::string argv6 = std::string(argv[6]);
  #define FILENAME(n) std::string( argv6 + std::string(n)).c_str()
  
 
  


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
  RightCoefPatch->SetMindisp(HdispMin);
  RightCoefPatch->SetMaxdisp(HdispMax);
 // RightCoefPatch->UpdateOutputInformation(); 

 //~ ImageWriterType::Pointer CoefPatchWriter = ImageWriterType::New();
  //~ CoefPatchWriter->SetFileName(FILENAME("CoefPatch.tif"));
  //~ CoefPatchWriter->SetInput( RightCoefPatch->GetOutputCoefImage() );   
  //~ CoefPatchWriter->Update();
  
/* ************************************************************************* */
/*========================================== Spatial Propagation ===========================================*/

//unsigned int iteration = 1;

	 
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
  
 SpatialPropType::Pointer LeftSpatial = SpatialPropType::New();
 SpatialPropType::Pointer RightSpatial = SpatialPropType::New();
 
 for(unsigned int iteration = 1 ; iteration <= 3; iteration++){ 
 std::cout<< "  iteration Spatial  " << iteration<< std::endl; 
 //Left  
  LeftSpatial->SetLeftInputImage(LeftReader->GetOutput());
  LeftSpatial->SetRightInputImage(RightReader->GetOutput());  
  LeftSpatial->SetLeftGradientXInput(GradientXL->GetOutput());
  LeftSpatial->SetRightGradientXInput(GradientXR->GetOutput());  
  LeftSpatial->SetPatchInputImage(LeftCoefPatch->GetOutputCoefImage());
  LeftSpatial->SetNormalAndZValueImage(LeftCoefPatch->GetOutputNormalAndZValueImage()); //
  LeftSpatial->SetIteration(iteration);
  LeftSpatial->SetPatchSize(PatchSize,PatchSize); 
 
if(iteration % (2) == 1){
  LeftSpatial->SetOffsetPatch(Offset0,Offset1,Offset2);
}

else{
  LeftSpatial->SetOffsetPatch(Offset0,Offset1R,Offset2R);
}
 
// Right
  
  RightSpatial->SetLeftInputImage(LeftReader->GetOutput());
  RightSpatial->SetRightInputImage(RightReader->GetOutput());  
  RightSpatial->SetLeftGradientXInput(GradientXL->GetOutput());
  RightSpatial->SetRightGradientXInput(GradientXR->GetOutput());  
  RightSpatial->SetPatchInputImage(RightCoefPatch->GetOutputCoefImage()); 
  RightSpatial->SetNormalAndZValueImage(RightCoefPatch->GetOutputNormalAndZValueImage()); //
  RightSpatial->SetIteration(iteration);
  RightSpatial->SetPatchSize(PatchSize,PatchSize); 
 
if(iteration % (2) == 1){
  RightSpatial->SetOffsetPatch(Offset0,Offset1,Offset2);
}

else{
  RightSpatial->SetOffsetPatch(Offset0,Offset1R,Offset2R);
}
}

 //~ ImageWriterType::Pointer SpatialPlaneWriter = ImageWriterType::New();
  //~ SpatialPlaneWriter->SetFileName(FILENAME("SpatialPropPlaneOutputImage.tif"));
  //~ SpatialPlaneWriter->SetInput( LeftSpatial->GetOutputPatchImage() ); 
//~ otb::StandardFilterWatcher watcherPlane(SpatialPlaneWriter, "SpatialPropFilter");  
  //~ SpatialPlaneWriter->Update();  
  
  
   //~ ImageWriterType::Pointer SpatialCostWriter = ImageWriterType::New();
  //~ SpatialCostWriter->SetFileName(FILENAME("SpatialPropCostOutputImage.tif"));  
  //~ SpatialCostWriter->SetInput( LeftSpatial->GetOutputCostImage() );
 //~ otb::StandardFilterWatcher watcherCost(SpatialCostWriter, "SpatialPropFilter");  
  //~ SpatialCostWriter->Update();
  
/* ************************************************************************* */  
/* ===================Spatial Propagation Rafinement=========================*/   
  typedef otb::PatchMatchRefinementImageFilter< ImageType, ImageType > RefinementType;
  ///Left
  RefinementType::Pointer LeftRefinementSpatial = RefinementType::New(); 
  LeftRefinementSpatial->SetPatchInputImage(LeftSpatial->GetOutputPatchImage()); 
  LeftRefinementSpatial->SetNormalAndZValueImage(LeftSpatial->GetOutputNormalAndZValueImage());
  LeftRefinementSpatial->SetMaxdisp(HdispMax);

  
  /// Right
  RefinementType::Pointer RightRefinementSpatial = RefinementType::New(); 
  RightRefinementSpatial->SetPatchInputImage(RightSpatial->GetOutputPatchImage()); 
  RightRefinementSpatial->SetNormalAndZValueImage(RightSpatial->GetOutputNormalAndZValueImage());
  RightRefinementSpatial->SetMaxdisp(HdispMax);
 


  //~ ImageWriterType::Pointer SpatialRefineWriter = ImageWriterType::New();
  //~ SpatialRefineWriter->SetFileName(FILENAME("RightRefinementSpatial.tif"));  
  //~ SpatialRefineWriter->SetInput( RightRefinementSpatial->GetOutputPatchImage());   
 //~ otb::StandardFilterWatcher watcherLastRafine(SpatialRefineWriter, "RightRefinementSpatial");  
  //~ SpatialRefineWriter->Update(); 
/* ************************************************************************/
/* ===========================Spatial MinPlaneRefinement =========================*/

 typedef otb::MinPlaneRefinementImageFilter< ImageType, ImageType > MinPlaneType;
 ///Left
  MinPlaneType::Pointer LeftMinPlaneSpatial = MinPlaneType::New();  
  LeftMinPlaneSpatial->SetLeftInputImage(LeftReader->GetOutput());
  LeftMinPlaneSpatial->SetRightInputImage(RightReader->GetOutput());  
  LeftMinPlaneSpatial->SetLeftGradientXInput(GradientXL->GetOutput());
  LeftMinPlaneSpatial->SetRightGradientXInput(GradientXR->GetOutput()); 
   
  LeftMinPlaneSpatial->SetPatchInputImage(LeftSpatial->GetOutputPatchImage()); 
  LeftMinPlaneSpatial->SetCostInputImage(LeftSpatial->GetOutputCostImage()); //
  LeftMinPlaneSpatial->SetNormalAndZValueImage(LeftSpatial->GetOutputNormalAndZValueImage());
    
  LeftMinPlaneSpatial->SetCoefRefinedInputImage(LeftRefinementSpatial->GetOutputPatchImage());
  LeftMinPlaneSpatial->SetRefinedNormalAndZValueImage(LeftRefinementSpatial->GetOutputNormalAndZValueImage());
  
  LeftMinPlaneSpatial->SetPatchSize(PatchSize,PatchSize); 
  LeftMinPlaneSpatial->SetOffsetPatch(Offset0,Offset0,Offset0); 

  
 ///Right
  MinPlaneType::Pointer RightMinPlaneSpatial = MinPlaneType::New();  
  RightMinPlaneSpatial->SetLeftInputImage(LeftReader->GetOutput());
  RightMinPlaneSpatial->SetRightInputImage(RightReader->GetOutput());  
  RightMinPlaneSpatial->SetLeftGradientXInput(GradientXL->GetOutput());
  RightMinPlaneSpatial->SetRightGradientXInput(GradientXR->GetOutput());
    
  RightMinPlaneSpatial->SetPatchInputImage(RightSpatial->GetOutputPatchImage()); 
  RightMinPlaneSpatial->SetCostInputImage(RightSpatial->GetOutputCostImage()); //
  RightMinPlaneSpatial->SetNormalAndZValueImage(RightSpatial->GetOutputNormalAndZValueImage());
  
  RightMinPlaneSpatial->SetCoefRefinedInputImage(RightRefinementSpatial->GetOutputPatchImage());
  RightMinPlaneSpatial->SetRefinedNormalAndZValueImage(RightRefinementSpatial->GetOutputNormalAndZValueImage());
  
  RightMinPlaneSpatial->SetPatchSize(PatchSize,PatchSize); 
  RightMinPlaneSpatial->SetOffsetPatch(Offset0,Offset0,Offset0); 

  
  
   ImageWriterType::Pointer MinPlaneWriter = ImageWriterType::New();
  MinPlaneWriter->SetFileName(FILENAME("LeftMinPlaneSpatial.tif"));  
  MinPlaneWriter->SetInput( LeftMinPlaneSpatial->GetOutputPatchImage() );   
 otb::StandardFilterWatcher MinS(MinPlaneWriter, "LeftMinPlaneSpatial");  
  MinPlaneWriter->Update();
  
/* ************************************************************************* */ 
/*=====================  View Propagation =======================================================*/
   /** sortie a une seule bande et elle contient le volume de cout */
typedef otb::ViewPropagationImageFilter< ImageType, ImageType > ViewPropType;

ViewPropType::Pointer LeftView = ViewPropType::New();
ViewPropType::Pointer RightView = ViewPropType::New();

 for(unsigned int iteration = 1 ; iteration <= 3; iteration++){ 
 std::cout<< "  iteration View  " << iteration<< std::endl;
 
//Left view
  LeftView->SetLeftInputImage(LeftReader->GetOutput());
  LeftView->SetRightInputImage(RightReader->GetOutput());   
  LeftView->SetLeftPatchInputImage(LeftMinPlaneSpatial->GetOutputPatchImage()); 
  LeftView->SetLeftNormalAndZValueImage(LeftMinPlaneSpatial->GetOutputNormalAndZValueImage());
  LeftView->SetRightPatchInputImage(RightMinPlaneSpatial->GetOutputPatchImage());
  LeftView->SetRightNormalAndZValueImage(RightMinPlaneSpatial->GetOutputNormalAndZValueImage()); 
  LeftView->SetLeftGradientXInput(GradientXL->GetOutput());
  LeftView->SetRightGradientXInput(GradientXR->GetOutput());
  LeftView->SetIteration(iteration);
  LeftView->SetPatchSize(PatchSize,PatchSize);
 
   
//Right view
  RightView->SetLeftInputImage(RightReader->GetOutput());
  RightView->SetRightInputImage(LeftReader->GetOutput());   
  RightView->SetLeftPatchInputImage(RightMinPlaneSpatial->GetOutputPatchImage()); 
  RightView->SetLeftNormalAndZValueImage(RightMinPlaneSpatial->GetOutputNormalAndZValueImage());
  RightView->SetRightPatchInputImage(LeftMinPlaneSpatial->GetOutputPatchImage());
  RightView->SetRightNormalAndZValueImage(LeftMinPlaneSpatial->GetOutputNormalAndZValueImage()); 
  RightView->SetLeftGradientXInput(GradientXR->GetOutput());
  RightView->SetRightGradientXInput(GradientXL->GetOutput());
  RightView->SetIteration(iteration);
  RightView->SetPatchSize(PatchSize,PatchSize);
  
}

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
/*======View Propagation Rafinement =======================================================================*/

//typedef otb::PatchMatchRefinementImageFilter< ImageType, ImageType > RefinementType;
RefinementType::Pointer LeftRefinementView = RefinementType::New();
 
  LeftRefinementView->SetPatchInputImage(LeftView->GetOutputPatchImage()); 
  LeftRefinementView->SetNormalAndZValueImage(LeftView->GetOutputNormalAndZValueImage());
  LeftRefinementView->SetMaxdisp(HdispMax); 
 

/// Right
RefinementType::Pointer RightRefinementView = RefinementType::New();
 
  RightRefinementView->SetPatchInputImage(RightView->GetOutputPatchImage()); 
  RightRefinementView->SetNormalAndZValueImage(RightView->GetOutputNormalAndZValueImage());
  RightRefinementView->SetMaxdisp(HdispMax);  
  
  //~ ImageWriterType::Pointer RefineViewWriter = ImageWriterType::New();
  //~ RefineViewWriter->SetFileName(FILENAME("LeftRefinementView.tif"));  
  //~ RefineViewWriter->SetInput( LeftRefinementView->GetOutputPatchImage() );   
 //~ otb::StandardFilterWatcher watcherRV(RefineViewWriter, "LeftRefinementView");  
  //~ RefineViewWriter->Update(); 
 
/* ************************************************************************/
/* ===========================View MinPlaneRefinement =========================*/

 //typedef otb::MinPlaneRefinementImageFilter< ImageType, ImageType > MinPlaneType;
 ///Left
  MinPlaneType::Pointer LeftMinPlaneView = MinPlaneType::New();  
  LeftMinPlaneView->SetLeftInputImage(LeftReader->GetOutput());
  LeftMinPlaneView->SetRightInputImage(RightReader->GetOutput());  
  LeftMinPlaneView->SetLeftGradientXInput(GradientXL->GetOutput());
  LeftMinPlaneView->SetRightGradientXInput(GradientXR->GetOutput()); 
   
  LeftMinPlaneView->SetPatchInputImage(LeftView->GetOutputPatchImage()); 
  LeftMinPlaneView->SetCostInputImage(LeftView->GetOutputCostImage()); //
  LeftMinPlaneView->SetNormalAndZValueImage(LeftView->GetOutputNormalAndZValueImage());
    
  LeftMinPlaneView->SetCoefRefinedInputImage(LeftRefinementView->GetOutputPatchImage());
  LeftMinPlaneView->SetRefinedNormalAndZValueImage(LeftRefinementView->GetOutputNormalAndZValueImage());
  
  LeftMinPlaneView->SetPatchSize(PatchSize,PatchSize); 
  LeftMinPlaneView->SetOffsetPatch(Offset0,Offset0,Offset0); 
 
   ///Right
  MinPlaneType::Pointer RightMinPlaneView = MinPlaneType::New();  
  RightMinPlaneView->SetLeftInputImage(LeftReader->GetOutput());
  RightMinPlaneView->SetRightInputImage(RightReader->GetOutput());  
  RightMinPlaneView->SetLeftGradientXInput(GradientXL->GetOutput());
  RightMinPlaneView->SetRightGradientXInput(GradientXR->GetOutput()); 
   
  RightMinPlaneView->SetPatchInputImage(RightView->GetOutputPatchImage()); 
  RightMinPlaneView->SetCostInputImage(RightView->GetOutputCostImage()); 
  RightMinPlaneView->SetNormalAndZValueImage(RightView->GetOutputNormalAndZValueImage());
    
  RightMinPlaneView->SetCoefRefinedInputImage(RightRefinementView->GetOutputPatchImage());
  RightMinPlaneView->SetRefinedNormalAndZValueImage(RightRefinementView->GetOutputNormalAndZValueImage());
  
  RightMinPlaneView->SetPatchSize(PatchSize,PatchSize); 
  RightMinPlaneView->SetOffsetPatch(Offset0,Offset0,Offset0); 


  ImageWriterType::Pointer MinRVWriter = ImageWriterType::New();
  MinRVWriter->SetFileName(FILENAME("LeftMinPlaneView.tif"));  
  MinRVWriter->SetInput( RightMinPlaneView->GetOutputPatchImage());   
 otb::StandardFilterWatcher watcherMinRV(MinRVWriter, "LeftMinPlaneView");  
  MinRVWriter->Update();
  
  
// ====  Plane Weightes Median ==========================
   /** Concatenation de la disparité avec l'image de de droite  ou gauche*/
 typedef otb::ConcatenateVectorImageFilter< ImageType,ImageType, ImageType> ConcatenateVectorImageFilterType;
  ConcatenateVectorImageFilterType::Pointer DispEndLeftImage = ConcatenateVectorImageFilterType::New();
  DispEndLeftImage->SetInput1(LeftMinPlaneView->GetOutputPatchImage());
  DispEndLeftImage->SetInput2( LeftReader->GetOutput() );

  ConcatenateVectorImageFilterType::Pointer DispEndRightImage = ConcatenateVectorImageFilterType::New();
  DispEndRightImage->SetInput1(RightMinPlaneView->GetOutputPatchImage());
  DispEndRightImage->SetInput2( RightReader->GetOutput() ); 


 typedef otb::PatchWeightedMedianImageFilter< ImageType, ImageType > PlaneMedianType;
 //Left
PlaneMedianType::Pointer LeftPMedianFilter = PlaneMedianType::New();

PlaneMedianType::RadiusType radiusPMedian;
  radiusPMedian[0] = 19;
  radiusPMedian[1] = 19;

LeftPMedianFilter->SetInput(DispEndLeftImage->GetOutput());
LeftPMedianFilter->SetRadius(radiusPMedian);

 //Right
PlaneMedianType::Pointer RightPMedianFilter = PlaneMedianType::New();
RightPMedianFilter->SetInput(DispEndRightImage->GetOutput());
RightPMedianFilter->SetRadius(radiusPMedian);
 
  ImageWriterType::Pointer LeftPMedianWriter = ImageWriterType::New();
  LeftPMedianWriter->SetFileName(FILENAME("LeftPMedian.tif"));  
  LeftPMedianWriter->SetInput( LeftPMedianFilter->GetOutput() );   
 otb::StandardFilterWatcher watcherLPM(LeftPMedianWriter, "LeftPMedian");  
  LeftPMedianWriter->Update(); 
  


 
  
  
  
  
  
     
 /* ************************************************************************* */ 
/*=============================Disparity ======================================*/
typedef otb::PatchMatchDisparityImageFilter< ImageType, ImageType > DisparityType;
DisparityType::Pointer LeftDispFilter = DisparityType::New();

DisparityType::RadiusType radiusD;
  radiusD[0] = 0;
  radiusD[1] = 0;

LeftDispFilter->SetInput(LeftPMedianFilter->GetOutput());
LeftDispFilter->SetRadius(radiusD);
/// Right

DisparityType::Pointer RightDispFilter = DisparityType::New();
RightDispFilter->SetInput(RightMinPlaneView->GetOutput());
RightDispFilter->SetRadius(radiusD);

   ImageWriterType::Pointer DispWriter = ImageWriterType::New();
  DispWriter->SetFileName(FILENAME("LeftDisp.tif"));  
  DispWriter->SetInput( LeftDispFilter->GetOutput() );   
 otb::StandardFilterWatcher watcherDisp(DispWriter, "LeftDispFilter");  
  DispWriter->Update();
  
// ======== Weighted Median Disparity ==============================  
  
   /** Concatenation de la disparité avec l'image de de droite  ou gauche*/
 
  ConcatenateVectorImageFilterType::Pointer ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  ConcatenateDispEndInLeftImage->SetInput1(LeftDispFilter->GetOutput());
  ConcatenateDispEndInLeftImage->SetInput2( LeftReader->GetOutput() );

  ConcatenateVectorImageFilterType::Pointer ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
  ConcatenateDispEndInRightImage->SetInput1(RightDispFilter->GetOutput());
  ConcatenateDispEndInRightImage->SetInput2( RightReader->GetOutput() ); 
  


typedef otb::DisparityWeightedMedianImageFilter< ImageType, ImageType > DispMedianType;
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

 

