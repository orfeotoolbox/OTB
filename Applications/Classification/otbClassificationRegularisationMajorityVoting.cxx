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
#include "itkBinaryBallStructuringElement.h"
#include "otbNeighborhoodMajorityVotingImageFilter.h"

namespace otb
{
namespace Wrapper
{

class ClassificationRegularisationMajorityVoting : public Application
{
public:
  /** Standard class typedefs. */
  typedef ClassificationRegularisationMajorityVoting            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ClassificationRegularisationMajorityVoting, otb::Application);

  /** Filters typedef */
  typedef unsigned char InputLabelPixelType;
  
  typedef UInt8ImageType InputLabelImageType;
  typedef UInt8ImageType OutputLabelImageType;

  //SE TYPE
  typedef itk::Neighborhood<InputLabelPixelType, 2> StructuringType;
  typedef StructuringType::RadiusType RadiusType;
  
  //BINARY BALL SE TYPE
  typedef itk::BinaryBallStructuringElement<InputLabelPixelType, 2> BallStructuringType;

  //NEIGHBORHOOD MAJORITY FILTER TYPE
  typedef otb::NeighborhoodMajorityVotingImageFilter<InputLabelImageType, OutputLabelImageType, StructuringType> NeighborhoodMajorityVotingFilterType;

private:
  void DoInit()
  {
    SetName("ClassificationRegularisationMajorityVoting");
    SetDescription("Filters the input labeled image using Majority Voting in a Ball shaped neighbordhood.");
    SetDocName("Regularisation of a labeled image by Majority Voting");
    SetDocLongDescription("This application filters the input labeled image using Majority Voting in a Ball shaped neighbordhood. Majority Voting takes the more representative value of all the pixels identified by the Ball shaped structuring element and then sets the center pixel to this majority label value.\n-'NoDataValue' is the label of the NOT classified pixels of the input image. These input pixels keep their 'NoDataValue' label in the output image.\n-If the 'UnDefinedValue if multiple majority labels' option is chosen, then 'UnDefinedValue' is the label of the output pixels, for which there are NOT unique majority labels. Otherwise, these output label pixels keep their Original values in the input image.");
    SetDocLimitations("The input image must be a single band labeled image. Both radii along Ox and Oy must have a minimum value equal to 1.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Documentation of the ClassificationRegularisationMajorityVoting application.");

    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);


    /** GROUP IO CLASSIFICATION */
    AddParameter(ParameterType_Group,"io","Input and output images");
    SetParameterDescription("io","This group of parameters allows to set input and output images for Majority Voting regularisation.");

    AddParameter(ParameterType_InputImage, "io.in",  "Input Image to REGULARIZE");
    SetParameterDescription( "io.in", "The input labeled image to regularize.");

    AddParameter(ParameterType_OutputImage, "io.out",  "Output REGULARIZED Image");
    SetParameterDescription( "io.out", "Output regularized labeled image.");
    SetParameterOutputImagePixelType( "io.out", ImagePixelType_uint8);


    AddParameter(ParameterType_Group,"ip","Input parameters");
    SetParameterDescription("ip","This group of parameters allows to set input parameters for Majority Voting regularisation.");

    AddParameter(ParameterType_Int, "ip.radiusx", "Radius along Ox");
    SetDefaultParameterInt("ip.radiusx", 1.0);

    AddParameter(ParameterType_Int, "ip.radiusy", "Radius along Oy");
    SetDefaultParameterInt("ip.radiusy", 1.0);

    AddParameter(ParameterType_Empty, "ip.suvbool", "UnDefinedValue if multiple majority labels");

    AddParameter(ParameterType_Int, "ip.nodatavalue", "Value of the NoDataValue input label");
    SetDefaultParameterInt("ip.nodatavalue", 0.0);

    AddParameter(ParameterType_Int, "ip.undefinedvalue", "Value of the UnDefinedValue output label");
    SetDefaultParameterInt("ip.undefinedvalue", 0.0);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in", "QB_1_ortho_4Cls_N_Classified_OTB.tif");
    SetDocExampleParameterValue("io.out", "QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif");
    SetDocExampleParameterValue("ip.radiusx", "2");
    SetDocExampleParameterValue("ip.radiusy", "5");
    SetDocExampleParameterValue("ip.suvbool", "true");
    SetDocExampleParameterValue("ip.nodatavalue", "10");
    SetDocExampleParameterValue("ip.undefinedvalue", "7");
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
    
    // NEIGHBORHOOD MAJORITY FILTER SETTINGS
    RadiusType rad;
    rad[0] = GetParameterInt("ip.radiusx");
    rad[1] = GetParameterInt("ip.radiusy");
    
    BallStructuringType seBall;
    seBall.SetRadius(rad);
    seBall.CreateStructuringElement();
    m_NeighMajVotingFilter->SetKernel(seBall);
    
    m_NeighMajVotingFilter->SetInput(inImage);
    m_NeighMajVotingFilter->SetNoDataValue(GetParameterInt("ip.nodatavalue"));
    m_NeighMajVotingFilter->SetUndefinedValue(GetParameterInt("ip.undefinedvalue"));
    
    // Set to UnDefinedValue if NOT unique Majority Voting
    if (IsParameterEnabled("ip.suvbool"))
      {
      m_NeighMajVotingFilter->SetKeepOriginalLabelBool(false);
      }
    // Keep original label value if NOT unique Majority Voting
    else
      {
      m_NeighMajVotingFilter->SetKeepOriginalLabelBool(true);
      }
    
    /** REGULARIZATION OF CLASSIFICATION */
    SetParameterOutputImage<OutputLabelImageType>("io.out", m_NeighMajVotingFilter->GetOutput());
  
  }//END DoExecute()


  NeighborhoodMajorityVotingFilterType::Pointer m_NeighMajVotingFilter;
}; //END class ClassificationRegularisationMajorityVoting


}//END namespace wrapper
}//END namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ClassificationRegularisationMajorityVoting)
