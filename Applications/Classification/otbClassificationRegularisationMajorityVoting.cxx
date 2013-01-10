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

=========================================================================*/

// Wrappers
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

// Majority Voting filter includes
#include "otbNeighborhoodMajorityVotingImageFilter.h"


namespace otb
{
  namespace Wrapper
  {
    
    class ClassificationRegularisationMajorityVoting : public Application
    {
    public:
      /** Standard class typedefs. */
      typedef ClassificationRegularisationMajorityVoting             Self;
      typedef Application                   Superclass;
      typedef itk::SmartPointer<Self>       Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      
      /** Standard macro */
      itkNewMacro(Self);
      
      itkTypeMacro(ClassificationRegularisationMajorityVoting, otb::Application);
      
      
      
      typedef UInt8ImageType LabelImageType;

	  typedef otb::NeighborhoodMajorityVotingImageFilter<LabelImageType> NeighborhoodMajorityVotingFilterType;
	  typedef NeighborhoodMajorityVotingFilterType::RadiusType RadiusType;    
      
      
      
      
    private:
      void DoInit()
      {
	      SetName("ClassificationRegularisationMajorityVoting");
	      
	      SetDescription("Filters an input labeled image using Majority Voting in a Ball shaped specified neighbordhood.");
	      SetDocName("Regularisation of a labeled image by Majority Voting");
	      SetDocLongDescription("This application filters an input labeled image using Majority Voting in a specified neighbordhood. Majority Voting takes the more representative value of all the pixels identified by the Ball shaped structuring element and then sets the center pixel to this label value.\n-NoDataValue is the value of the not classified pixels of the Input image.\n-UnDefinedValue is the value of the output label for input pixels with NoDataValue AND for pixels for which the Majority Voting does not return a unique label.");
	      SetDocLimitations("Both input and output images must be unsigned char (8 bits) labeled images.");
	      SetDocAuthors("OTB-Team");
	      SetDocSeeAlso("Documentation of the ClassificationRegularisationMajorityVoting application.");
	      
	      AddDocTag(Tags::Learning);
	      AddDocTag(Tags::Analysis);
	      
	      
	      
	      /** GROUP IO CLASSIFICATION */
	      AddParameter(ParameterType_Group,"io","Input and output images for Majority Voting regularization");
	      SetParameterDescription("io","This group of parameters allows to set input and output images for Majority Voting regularisation.");
	      
	      AddParameter(ParameterType_InputImage, "io.in",  "Input Image to REGULARIZE");
	      SetParameterDescription( "io.in", "The input labeled image to regularize.");
	            
	      AddParameter(ParameterType_OutputImage, "io.out",  "Output REGULARIZED Image");
	      SetParameterDescription( "io.out", "Output regularized labeled image.");
	      SetParameterOutputImagePixelType( "io.out", ImagePixelType_uint8);
	      
	      
	      AddParameter(ParameterType_Group,"ip","Input parameters for Majority Voting regularization");
	      SetParameterDescription("ip","This group of parameters allows to set input parameters for Majority Voting regularisation.");
	      
	      AddParameter(ParameterType_Int, "ip.radiusx", "Size of the neighborhood along Ox");
	      SetDefaultParameterInt("ip.radiusx", 1.0);
	      
	      AddParameter(ParameterType_Int, "ip.radiusy", "Size of the neighborhood along Oy");
	      SetDefaultParameterInt("ip.radiusy", 1.0);
	      
	      AddParameter(ParameterType_Int, "ip.nodatavalue", "Value of the NoDataValue input label");
	      SetDefaultParameterInt("ip.nodatavalue", 0.0);
	      
	      AddParameter(ParameterType_Int, "ip.undefinedvalue", "Value of the UndeFinedValue output label");
	      SetDefaultParameterInt("ip.undefinedvalue", 0.0);
	      
	      AddRAMParameter();
	      
	      // Doc example parameter settings
	      SetDocExampleParameterValue("io.in", "QB_1_ortho_4Cls_N_Classified_OTB.tif");
	      SetDocExampleParameterValue("io.out", "QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5.tif");
	      SetDocExampleParameterValue("ip.radiusx", "2");
	      SetDocExampleParameterValue("ip.radiusy", "5");
	      SetDocExampleParameterValue("ip.nodatavalue", "128");
	      SetDocExampleParameterValue("ip.undefinedvalue", "10");
	      
      }
      
      
      void DoUpdateParameters()
      {
        // Nothing to do here : all parameters are independent
      }
      
      
      void DoExecute()
      {
		// Majority Voting
	    m_NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();
      	      
		// Load input image to CLASSIFY
		UInt8ImageType::Pointer inImage = GetParameterUInt8Image("io.in");
	      
	    // NEIGHBORHOOD MAJORITY FILTER
	    RadiusType rad;
		rad[0] = GetParameterInt("ip.radiusx");
		rad[1] = GetParameterInt("ip.radiusy");
		
		m_NeighMajVotingFilter->SetInput(inImage);	
		m_NeighMajVotingFilter->SetRadiusNeighborhood(rad);
		m_NeighMajVotingFilter->SetNoDataValue(GetParameterInt("ip.nodatavalue"));
		m_NeighMajVotingFilter->SetUndefinedValue(GetParameterInt("ip.undefinedvalue"));

	    /** REGULARIZATION OF CLASSIFICATION */
	    SetParameterOutputImage<UInt8ImageType>("io.out", m_NeighMajVotingFilter->GetOutput());
      
      }//END DoExecute()
      
      NeighborhoodMajorityVotingFilterType::Pointer m_NeighMajVotingFilter;
      
    }; //END class ClassificationRegularisationMajorityVoting
    
  }//END namespace wrapper
}//END namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ClassificationRegularisationMajorityVoting)



