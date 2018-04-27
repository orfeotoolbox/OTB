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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

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
#include <itkAddImageFilter.h>
#include <itkVectorImageToImageAdaptor.h>
#include "itkCastImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkTimeProbe.h" //crono
#include "itkConstantBoundaryCondition.h"


namespace otb
{
  namespace Wrapper
  {

    class PatchMatch : public Application
    {
    public:
      /** Standard class typedefs. */
      typedef PatchMatch                 Self;
      typedef Application                   Superclass;
      typedef itk::SmartPointer<Self>       Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionTypeLeft;
      typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionTypeLeft> ConvFilterTypeLeft;
      typedef itk::ConstantBoundaryCondition<FloatImageType>  BoundaryConditionTypeRight;
      typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionTypeRight> ConvFilterTypeRight;
      typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterTypeLeft> VectorFilterTypeLeft;
      typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterTypeRight> VectorFilterTypeRight;
      typedef otb::CoefOfThePatchFilter< FloatVectorImageType, FloatVectorImageType> CoefPatchFilterType;         
      typedef otb::MinPlaneRefinementImageFilter< FloatVectorImageType, FloatVectorImageType > MinPlaneType; 

      typedef otb::SpatialPropagationImageFilter< FloatVectorImageType, FloatVectorImageType > SpatialPropType;
      typedef otb::ViewPropagationImageFilter< FloatVectorImageType, FloatVectorImageType > ViewPropType;
      typedef otb::PatchMatchRefinementImageFilter< FloatVectorImageType, FloatVectorImageType > RefinementType;
      typedef otb::PatchMatchDisparityImageFilter< FloatVectorImageType, FloatVectorImageType > DisparityType;
      typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType,FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType; 
      typedef otb::DisparityWeightedMedianImageFilter< FloatVectorImageType, FloatVectorImageType > DispMedianType;

      typedef double     PixelType;

      itkNewMacro(Self);
      itkTypeMacro(PatchMatch, otb::Application);





    private:
  
      void DoInit() ITK_OVERRIDE
      {     
        SetName("PatchMatch");
        SetDescription("Performs Fast Cost Volume Filter to estimate a disparty map between two images");

        SetDocName("PatchMatch");
        SetDocLongDescription("This application allows PatchMatch Filter to estimate a disparty map between two images" );
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
        SetDefaultParameterInt("radius",7.);      

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
        SetDocExampleParameterValue("radius","7");
        SetDocExampleParameterValue("io.out","MyPatchMatchDisparity.tif");   
      }


      PatchMatch()
      {  
        m_convFilterXL = ConvFilterTypeLeft::New();
        m_convFilterXR = ConvFilterTypeRight::New();         
        m_GradientXL = VectorFilterTypeLeft::New();        
        m_GradientXR = VectorFilterTypeRight::New();

        m_LeftCoefPatch = CoefPatchFilterType::New();
        m_RightCoefPatch = CoefPatchFilterType::New();
       
        m_LeftIn = FloatVectorImageType::New();

        m_LeftNormalPlane = FloatVectorImageType::New();
        m_RightNormalPlane = FloatVectorImageType::New();
        
        m_LeftCoefPlane  = FloatVectorImageType::New(); 
        m_RightCoefPlane = FloatVectorImageType::New();

        m_LeftMinPlaneView = MinPlaneType::New();
        m_RightMinPlaneView = MinPlaneType::New();

        m_LeftSpatial = SpatialPropType::New();
        m_RightSpatial = SpatialPropType::New();

        m_LeftView = ViewPropType::New();
        m_RightView = ViewPropType::New();

        m_LeftRefinement = RefinementType::New(); 
        m_RightRefinement = RefinementType::New();

        m_LeftDispFilter = DisparityType::New();
        m_RightDispFilter = DisparityType::New();

        m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
        m_ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
        m_LeftDispMedian = DispMedianType::New();
        m_RightDispMedian = DispMedianType::New();

      }

      void DoUpdateParameters() ITK_OVERRIDE
      {
      }

      void DoExecute() ITK_OVERRIDE
      {          
        m_leftImage = GetParameterFloatVectorImage("io.inleft");
        m_rightImage = GetParameterFloatVectorImage("io.inright");

        unsigned int R  = GetParameterInt("radius");
        int          HdispMin = GetParameterInt("hdispmin");
        int          HdispMax = GetParameterInt("hdispmax");
        int rmf = GetParameterInt("rmf") ;

        /* ================================== Gradient =======================================================*/
     
        // Gradient X  
        ConvFilterTypeLeft::InputSizeType radiusGX;
        radiusGX[0] = 1;
        radiusGX[1] = 0;
        itk::Array< double > filterCoeffsX;

        filterCoeffsX.SetSize((2 * radiusGX[0] + 1) );
        filterCoeffsX.Fill(0.5);
        filterCoeffsX[0] = -0.5;
        filterCoeffsX[1] = 0;

        m_convFilterXL->SetRadius(radiusGX);
        m_convFilterXL->SetFilter(filterCoeffsX);
        
        m_convFilterXR->SetRadius(radiusGX);
        m_convFilterXR->SetFilter(filterCoeffsX);

        //--Left---------------         
        m_GradientXL->SetFilter(m_convFilterXL);
        m_GradientXL->SetInput(m_leftImage);
        //--Right--------------- 
        m_GradientXR->SetFilter(m_convFilterXR);
        m_GradientXR->SetInput(m_rightImage);

        /*=====================Compute Coeffs of the patch =========================================================*/
        //Left           
        m_LeftCoefPatch->SetInputImage(m_leftImage);
        m_LeftCoefPatch->SetMindisp(HdispMin);
        m_LeftCoefPatch->SetMaxdisp(HdispMax);

        // Right        
        m_RightCoefPatch->SetInputImage(m_leftImage);
        m_RightCoefPatch->SetMindisp(-HdispMax);
        m_RightCoefPatch->SetMaxdisp(-HdispMin);

        
        /*========================================== Spatial Propagation ===========================================*/
        itk::TimeProbe chrono1;
        chrono1.Start();
       

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

        typename FloatVectorImageType::PixelType In(3);
        In.Fill(0.);
        m_LeftCoefPatch->UpdateOutputInformation();
        m_RightCoefPatch->UpdateOutputInformation();
         
        m_LeftIn->CopyInformation(m_LeftCoefPatch->GetOutputCoefImage());
        m_LeftIn->SetNumberOfComponentsPerPixel(3);
        m_LeftIn->SetRegions(m_LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
        m_LeftIn->Allocate();   
        m_LeftIn->FillBuffer(In);

         /// créer une image pour permuter l'entrer et la sortie pour mettre a jour les coefs        
        m_LeftCoefPlane->CopyInformation(m_LeftCoefPatch->GetOutputCoefImage());
        m_LeftCoefPlane->SetNumberOfComponentsPerPixel(3);
        m_LeftCoefPlane->SetRegions(m_LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
        m_LeftCoefPlane->Allocate();   
        m_LeftCoefPlane->FillBuffer(In);  
        m_LeftCoefPlane = m_LeftCoefPatch->GetOutputCoefImage();       

        
        m_LeftNormalPlane->CopyInformation(m_LeftCoefPatch->GetOutputCoefImage());
        m_LeftNormalPlane->SetNumberOfComponentsPerPixel(3);
        m_LeftNormalPlane->SetRegions(m_LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
        m_LeftNormalPlane->Allocate();   
        m_LeftNormalPlane->FillBuffer(In);
        m_LeftNormalPlane = m_LeftCoefPatch->GetOutputNormalAndZValueImage(); 

       
        m_RightCoefPlane->CopyInformation(m_LeftCoefPatch->GetOutputCoefImage());
        m_RightCoefPlane->SetNumberOfComponentsPerPixel(3);
        m_RightCoefPlane->SetRegions(m_LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
        m_RightCoefPlane->Allocate();   
        m_RightCoefPlane->FillBuffer(In); 
        m_RightCoefPlane = m_RightCoefPatch->GetOutputCoefImage();

        
        m_RightNormalPlane->CopyInformation(m_LeftCoefPatch->GetOutputCoefImage());
        m_RightNormalPlane->SetNumberOfComponentsPerPixel(3);
        m_RightNormalPlane->SetRegions(m_LeftCoefPatch->GetOutputCoefImage()->GetLargestPossibleRegion());
        m_RightNormalPlane->Allocate();   
        m_RightNormalPlane->FillBuffer(In);
        m_RightNormalPlane = m_RightCoefPatch->GetOutputNormalAndZValueImage();
                     
        double MaxDz0 = HdispMin/2;
        double MaxDn = 1;      

        for(unsigned int iteration = 1 ; iteration < 4; iteration++)
        {
          std::cout<< "  iteration   " << iteration<< std::endl;
          m_LeftSpatial->SetLeftInputImage(m_leftImage);
          m_LeftSpatial->SetRightInputImage(m_rightImage);

          m_LeftSpatial->SetLeftGradientXInput(m_GradientXL->GetOutput());
          m_LeftSpatial->SetRightGradientXInput(m_GradientXR->GetOutput());  
          m_LeftSpatial->SetPatchInputImage(m_LeftCoefPlane);
          m_LeftSpatial->SetNormalAndZValueImage(m_LeftNormalPlane); //
          m_LeftSpatial->SetIteration(iteration);
          m_LeftSpatial->SetPatchSize(R,R);
          m_LeftSpatial->SetSense(1); 

          if(iteration % (2) == 1){
            m_LeftSpatial->SetOffsetPatch(Offset0,Offset1,Offset2);
          }
          else{
            m_LeftSpatial->SetOffsetPatch(Offset0,Offset1R,Offset2R);
          }

          m_RightSpatial->SetLeftInputImage(m_leftImage);
          m_RightSpatial->SetRightInputImage(m_rightImage);  
          m_RightSpatial->SetLeftGradientXInput(m_GradientXL->GetOutput());
          m_RightSpatial->SetRightGradientXInput(m_GradientXR->GetOutput());  
          m_RightSpatial->SetPatchInputImage(m_RightCoefPlane); 
          m_RightSpatial->SetNormalAndZValueImage(m_RightNormalPlane); //
          m_RightSpatial->SetIteration(iteration);
          m_RightSpatial->SetPatchSize(R,R); 
          m_RightSpatial->SetSense(0); 
         
          if(iteration % (2) == 1){
            m_RightSpatial->SetOffsetPatch(Offset0,Offset1,Offset2);
          }

          else{
            m_RightSpatial->SetOffsetPatch(Offset0,Offset1R,Offset2R);
          }      


        //Left view
        m_LeftView->SetSpatialCostImage(m_RightSpatial->GetOutputCostImage());
        m_LeftView->SetLeftInputImage(m_leftImage);
        m_LeftView->SetRightInputImage(m_rightImage);   
        m_LeftView->SetLeftPatchInputImage(m_LeftSpatial->GetOutputPatchImage()); 
        m_LeftView->SetLeftNormalAndZValueImage(m_LeftSpatial->GetOutputNormalAndZValueImage());
        m_LeftView->SetRightPatchInputImage(m_RightSpatial->GetOutputPatchImage());
        m_LeftView->SetRightNormalAndZValueImage(m_RightSpatial->GetOutputNormalAndZValueImage()); 
        m_LeftView->SetLeftGradientXInput(m_GradientXL->GetOutput());
        m_LeftView->SetRightGradientXInput(m_GradientXR->GetOutput());
        m_LeftView->SetIteration(iteration);
        m_LeftView->SetPatchSize(R,R);
        m_LeftView->SetDispMinMax(HdispMin,HdispMax); // pour deteminer la taille de la fenetre de recherche
        m_LeftView->SetRadius((unsigned int)((HdispMax-HdispMin)/2), 0);
       
         
      //Right view
        m_RightView->SetSpatialCostImage(m_LeftSpatial->GetOutputCostImage());
        m_RightView->SetLeftInputImage(m_rightImage);
        m_RightView->SetRightInputImage(m_leftImage);   
        m_RightView->SetLeftPatchInputImage(m_RightSpatial->GetOutputPatchImage()); 
        m_RightView->SetLeftNormalAndZValueImage(m_RightSpatial->GetOutputNormalAndZValueImage());
        m_RightView->SetRightPatchInputImage(m_LeftSpatial->GetOutputPatchImage());
        m_RightView->SetRightNormalAndZValueImage(m_LeftSpatial->GetOutputNormalAndZValueImage()); 
        m_RightView->SetLeftGradientXInput(m_GradientXR->GetOutput());
        m_RightView->SetRightGradientXInput(m_GradientXL->GetOutput());
        m_RightView->SetIteration(iteration);
        m_RightView->SetPatchSize(R,R);
        m_RightView->SetDispMinMax(HdispMin, HdispMax);
        m_RightView->SetRadius((unsigned int)((HdispMax-HdispMin)/2), 0);

     
        /* ************************************************************************* */  
        /*=================== Refinement ====================================================*/
        ///Left
        m_LeftRefinement->SetNormalAndZValueImage(m_LeftView->GetOutputNormalAndZValueImage());
        m_LeftRefinement->SetMaxDz0(MaxDz0); 
        m_LeftRefinement->SetMaxDn(MaxDn); 
        m_LeftRefinement->SetDispMinMax(HdispMin, HdispMax);   

        /// Right       
        m_RightRefinement->SetNormalAndZValueImage(m_RightView->GetOutputNormalAndZValueImage());
        m_RightRefinement->SetMaxDz0(MaxDz0);
        m_RightRefinement->SetMaxDn(MaxDn); 
        m_RightRefinement->SetDispMinMax(HdispMin, HdispMax); 


        /* ************************************************************************/
        /* =========================== MinPlaneRefinement =========================*/
        ///Left        
        m_LeftMinPlaneView->SetLeftInputImage(m_leftImage);
        m_LeftMinPlaneView->SetRightInputImage(m_rightImage);  
        m_LeftMinPlaneView->SetLeftGradientXInput(m_GradientXL->GetOutput());
        m_LeftMinPlaneView->SetRightGradientXInput(m_GradientXR->GetOutput()); 
         
        m_LeftMinPlaneView->SetPatchInputImage(m_LeftView->GetOutputPatchImage()); 
        m_LeftMinPlaneView->SetCostInputImage(m_LeftView->GetOutputCostImage()); //
        m_LeftMinPlaneView->SetNormalAndZValueImage(m_LeftView->GetOutputNormalAndZValueImage());
          
        m_LeftMinPlaneView->SetCoefRefinedInputImage(m_LeftRefinement->GetOutputPatchImage());
        m_LeftMinPlaneView->SetRefinedNormalAndZValueImage(m_LeftRefinement->GetOutputNormalAndZValueImage());
        
        m_LeftMinPlaneView->SetPatchSize(R,R); 
        m_LeftMinPlaneView->SetOffsetPatch(Offset0,Offset0,Offset0);                 
        m_LeftMinPlaneView->Update();

         ///Right    
        m_RightMinPlaneView->SetLeftInputImage(m_leftImage);
        m_RightMinPlaneView->SetRightInputImage(m_rightImage);  
        m_RightMinPlaneView->SetLeftGradientXInput(m_GradientXL->GetOutput());
        m_RightMinPlaneView->SetRightGradientXInput(m_GradientXR->GetOutput()); 
         
        m_RightMinPlaneView->SetPatchInputImage(m_RightView->GetOutputPatchImage()); 
        m_RightMinPlaneView->SetCostInputImage(m_RightView->GetOutputCostImage()); 
        m_RightMinPlaneView->SetNormalAndZValueImage(m_RightView->GetOutputNormalAndZValueImage());
          
        m_RightMinPlaneView->SetCoefRefinedInputImage(m_RightRefinement->GetOutputPatchImage());
        m_RightMinPlaneView->SetRefinedNormalAndZValueImage(m_RightRefinement->GetOutputNormalAndZValueImage());
        
        m_RightMinPlaneView->SetPatchSize(R,R); 
        m_RightMinPlaneView->SetOffsetPatch(Offset0,Offset0,Offset0);         
        m_RightMinPlaneView->Update();
 
       /// mèttre à jour les plan et les normales  
        m_LeftCoefPlane = m_LeftMinPlaneView->GetOutputPatchImage();
        m_LeftCoefPlane->DisconnectPipeline();
        m_LeftNormalPlane = m_LeftMinPlaneView->GetOutputNormalAndZValueImage(); 
        m_LeftNormalPlane->DisconnectPipeline();        

        m_RightCoefPlane = m_RightMinPlaneView->GetOutputPatchImage();
        m_RightCoefPlane->DisconnectPipeline();
        m_RightNormalPlane = m_RightMinPlaneView->GetOutputNormalAndZValueImage(); 
        m_RightNormalPlane->DisconnectPipeline();      
       
          
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

      }
    

      /* ************************************************************************* */ 
      /*=============================Disparity ======================================*/
      DisparityType::RadiusType radiusD;
      radiusD[0] = 0;
      radiusD[1] = 0;

      // Left
      m_LeftDispFilter->SetInput(m_LeftMinPlaneView->GetOutputPatchImage());
      m_LeftDispFilter->SetRadius(radiusD);
      /// Right
      m_RightDispFilter->SetInput(m_RightMinPlaneView->GetOutputPatchImage());
      m_RightDispFilter->SetRadius(radiusD);

     
      /// ======== Weighted Median Disparity ==============================    
      /** Concatenation de la disparité avec l'image de de droite  ou gauche*/

      m_ConcatenateDispEndInLeftImage->SetInput1(m_LeftDispFilter->GetOutput());
      m_ConcatenateDispEndInLeftImage->SetInput2( m_leftImage);

      m_ConcatenateDispEndInRightImage->SetInput1(m_RightDispFilter->GetOutput());
      m_ConcatenateDispEndInRightImage->SetInput2( m_rightImage); 

      DispMedianType::RadiusType radiusPMedian;
      radiusPMedian[0] = rmf;
      radiusPMedian[1] = rmf ;

      /// Left   
      m_LeftDispMedian->SetInput(m_ConcatenateDispEndInLeftImage->GetOutput());
      m_LeftDispMedian->SetRadius(radiusPMedian);
     // m_LeftDispMedian->Update();
      /// Right
      m_RightDispMedian->SetInput(m_ConcatenateDispEndInRightImage->GetOutput());
      m_RightDispMedian->SetRadius(radiusPMedian);
     //m_RightDispMedian->Update();

      SetParameterOutputImage("io.out",m_LeftDispMedian->GetOutput());  
 
    }

      FloatVectorImageType::Pointer m_leftImage ;
      FloatVectorImageType::Pointer m_rightImage ;

      ConvFilterTypeLeft::Pointer m_convFilterXL ;
      ConvFilterTypeRight::Pointer m_convFilterXR ; 

      VectorFilterTypeLeft::Pointer m_GradientXL ;
      VectorFilterTypeRight::Pointer m_GradientXR;

      CoefPatchFilterType::Pointer m_LeftCoefPatch ;
      CoefPatchFilterType::Pointer m_RightCoefPatch ;

      FloatVectorImageType::Pointer m_LeftCoefPlane ; 
      FloatVectorImageType::Pointer m_LeftIn ;
      FloatVectorImageType::Pointer m_LeftNormalPlane ; 
      FloatVectorImageType::Pointer m_RightCoefPlane ;
      FloatVectorImageType::Pointer m_RightNormalPlane ;

      MinPlaneType::Pointer m_LeftMinPlaneView ; 
      MinPlaneType::Pointer m_RightMinPlaneView ;  

      SpatialPropType::Pointer m_LeftSpatial ;
      SpatialPropType::Pointer m_RightSpatial ;

      ViewPropType::Pointer m_LeftView ;
      ViewPropType::Pointer m_RightView;

      RefinementType::Pointer m_LeftRefinement ; 

      RefinementType::Pointer m_RightRefinement;
      DisparityType::Pointer m_LeftDispFilter ; 
      DisparityType::Pointer m_RightDispFilter ; 

      ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage ;
      ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInRightImage ;
      DispMedianType::Pointer m_LeftDispMedian ;
      DispMedianType::Pointer m_RightDispMedian ;
  

    }; //end class

  } // end Wrapper 

} //end otb

OTB_APPLICATION_EXPORT(otb::Wrapper::PatchMatch)