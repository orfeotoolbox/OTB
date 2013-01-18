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

class ClassificationMapRegularization : public Application
{
public:
  /** Standard class typedefs. */
  typedef ClassificationMapRegularization            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ClassificationMapRegularization, otb::Application);

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
    SetName("ClassificationMapRegularization");
    SetDescription("Filters the input labeled image using Majority Voting in a Ball shaped neighbordhood.");
    SetDocName("Classification Map Regularization");
    SetDocLongDescription("This application filters the input labeled image using Majority Voting in a Ball shaped neighbordhood. Majority Voting takes the more representative value of all the pixels identified by the Ball shaped structuring element and then sets the center pixel to this majority label value.\n\
    -NoData is the label of the NOT classified pixels in the input image. These input pixels keep their NoData label in the output image.\n\
    -Pixels with more than 1 majority class are marked as Undecided if the parameter 'ip.suvbool == true', or keep their Original labels otherwise.");
    SetDocLimitations("The input image must be a single band labeled image. Both structuring element X and Y radii must have a minimum value equal to 1. Please note that the Undecided value must be different from existing labels in the input labeled image.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Documentation of the ClassificationMapRegularization application.");

    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);


    /** GROUP IO CLASSIFICATION */
    AddParameter(ParameterType_Group,"io","Input and output images");
    SetParameterDescription("io","This group of parameters allows to set input and output images for classification map regularization by Majority Voting.");

    AddParameter(ParameterType_InputImage, "io.in",  "Input classification image");
    SetParameterDescription( "io.in", "The input labeled image to regularize.");

    AddParameter(ParameterType_OutputImage, "io.out",  "Output regularized image");
    SetParameterDescription( "io.out", "The output regularized labeled image.");
    SetParameterOutputImagePixelType( "io.out", ImagePixelType_uint8);


    AddParameter(ParameterType_Group,"ip","Regularization parameters");
    SetParameterDescription("ip","This group allows to set parameters for classification map regularization by Majority Voting.");

    AddParameter(ParameterType_Int, "ip.radiusx", "Structuring Element X Radius");
    SetParameterDescription("ip.radiusx", "The Structuring Element X Radius.");
    SetDefaultParameterInt("ip.radiusx", 1.0);

    AddParameter(ParameterType_Int, "ip.radiusy", "Structuring Element Y Radius");
    SetParameterDescription("ip.radiusy", "The Structuring Element Y Radius.");
    SetDefaultParameterInt("ip.radiusy", 1.0);

    AddParameter(ParameterType_Empty, "ip.suvbool", "Multiple majority: Undecided(X)/Original");
    SetParameterDescription("ip.suvbool", "Pixels with more than 1 majority class are marked as Undecided if parameter is checked (true), or keep their Original labels otherwise (false). Please note that the Undecided value must be different from existing labels in the input labeled image.");

    AddParameter(ParameterType_Int, "ip.nodatalabel", "Label for the NoData class");
    SetParameterDescription("ip.nodatalabel", "Label for the NoData class. Such input pixels keep their NoData label in the output image.");
    SetDefaultParameterInt("ip.nodatalabel", 0.0);

    AddParameter(ParameterType_Int, "ip.undecidedlabel", "Label for the Undecided class");
    SetParameterDescription("ip.undecidedlabel", "Label for the Undecided class.");
    SetDefaultParameterInt("ip.undecidedlabel", 0.0);
    

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in", "QB_1_ortho_4Cls_N_Classified_OTB.tif");
    SetDocExampleParameterValue("io.out", "QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif");
    SetDocExampleParameterValue("ip.radiusx", "2");
    SetDocExampleParameterValue("ip.radiusy", "5");
    SetDocExampleParameterValue("ip.suvbool", "true");
    SetDocExampleParameterValue("ip.nodatalabel", "10");
    SetDocExampleParameterValue("ip.undecidedlabel", "7");
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
    m_NeighMajVotingFilter->SetLabelForNoDataPixels(GetParameterInt("ip.nodatalabel"));
    m_NeighMajVotingFilter->SetLabelForUndecidedPixels(GetParameterInt("ip.undecidedlabel"));
    
    // Set to Undecided label if NOT unique Majority Voting
    if (IsParameterEnabled("ip.suvbool"))
      {
      m_NeighMajVotingFilter->SetKeepOriginalLabelBool(false);
      }
    // Keep Original label value if NOT unique Majority Voting
    else
      {
      m_NeighMajVotingFilter->SetKeepOriginalLabelBool(true);
      }
    
    /** REGULARIZATION OF CLASSIFICATION */
    SetParameterOutputImage<OutputLabelImageType>("io.out", m_NeighMajVotingFilter->GetOutput());
  
  }//END DoExecute()


  NeighborhoodMajorityVotingFilterType::Pointer m_NeighMajVotingFilter;
}; //END class ClassificationMapRegularization


}//END namespace wrapper
}//END namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ClassificationMapRegularization)
