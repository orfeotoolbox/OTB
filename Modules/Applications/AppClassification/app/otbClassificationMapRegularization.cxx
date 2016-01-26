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
  typedef UInt16ImageType IOLabelImageType;

  // Neighborhood majority voting filter type
  typedef otb::NeighborhoodMajorityVotingImageFilter<IOLabelImageType> NeighborhoodMajorityVotingFilterType;

  // Binary ball Structuring Element type
  typedef NeighborhoodMajorityVotingFilterType::KernelType StructuringType;
  typedef StructuringType::RadiusType RadiusType;


private:
  void DoInit()
  {
    SetName("ClassificationMapRegularization");
    SetDescription("Filters the input labeled image using Majority Voting in a ball shaped neighbordhood.");
    SetDocName("Classification Map Regularization");
    SetDocLongDescription("This application filters the input labeled image (with a maximal class label = 65535) using Majority Voting in a ball shaped neighbordhood. Majority Voting takes the more representative value of all the pixels identified by the ball shaped structuring element and then sets the center pixel to this majority label value.\n\
    -NoData is the label of the NOT classified pixels in the input image. These input pixels keep their NoData label in the output image.\n\
    -Pixels with more than 1 majority class are marked as Undecided if the parameter 'ip.suvbool == true', or keep their Original labels otherwise.");
    SetDocLimitations("The input image must be a single band labeled image (with a maximal class label = 65535). The structuring element radius must have a minimum value equal to 1 pixel. Please note that the Undecided value must be different from existing labels in the input labeled image.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Documentation of the ClassificationMapRegularization application.");

    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);


    /** GROUP IO CLASSIFICATION */
    AddParameter(ParameterType_Group,"io","Input and output images");
    SetParameterDescription("io","This group of parameters allows setting input and output images for classification map regularization by Majority Voting.");

    AddParameter(ParameterType_InputImage, "io.in",  "Input classification image");
    SetParameterDescription( "io.in", "The input labeled image to regularize.");

    AddParameter(ParameterType_OutputImage, "io.out",  "Output regularized image");
    SetParameterDescription( "io.out", "The output regularized labeled image.");
    SetDefaultOutputPixelType( "io.out", ImagePixelType_uint8);


    AddParameter(ParameterType_Group,"ip","Regularization parameters");
    SetParameterDescription("ip","This group allows setting parameters for classification map regularization by Majority Voting.");

    AddParameter(ParameterType_Int, "ip.radius", "Structuring element radius (in pixels)");
    SetParameterDescription("ip.radius", "The radius of the ball shaped structuring element (expressed in pixels). By default, 'ip.radius = 1 pixel'.");
    SetDefaultParameterInt("ip.radius", 1.0);

    AddParameter(ParameterType_Empty, "ip.suvbool", "Multiple majority: Undecided(X)/Original");
    SetParameterDescription("ip.suvbool", "Pixels with more than 1 majority class are marked as Undecided if this parameter is checked (true), or keep their Original labels otherwise (false). Please note that the Undecided value must be different from existing labels in the input labeled image. By default, 'ip.suvbool = false'.");

    AddParameter(ParameterType_Int, "ip.nodatalabel", "Label for the NoData class");
    SetParameterDescription("ip.nodatalabel", "Label for the NoData class. Such input pixels keep their NoData label in the output image. By default, 'ip.nodatalabel = 0'.");
    SetDefaultParameterInt("ip.nodatalabel", 0.0);

    AddParameter(ParameterType_Int, "ip.undecidedlabel", "Label for the Undecided class");
    SetParameterDescription("ip.undecidedlabel", "Label for the Undecided class. By default, 'ip.undecidedlabel = 0'.");
    SetDefaultParameterInt("ip.undecidedlabel", 0.0);


    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in", "clLabeledImageQB123_1.tif");
    SetDocExampleParameterValue("io.out", "clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif");
    SetDocExampleParameterValue("ip.radius", "2");
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

    // Load input labeled image to regularize
    UInt16ImageType::Pointer inImage = GetParameterUInt16Image("io.in");

    // Neighborhood majority voting filter settings
    RadiusType rad;
    rad[0] = GetParameterInt("ip.radius");
    rad[1] = GetParameterInt("ip.radius");

    StructuringType seBall;
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
    SetParameterOutputImage<IOLabelImageType>("io.out", m_NeighMajVotingFilter->GetOutput());

  }// END DoExecute()


  NeighborhoodMajorityVotingFilterType::Pointer m_NeighMajVotingFilter;
}; // END class ClassificationMapRegularization


}// END namespace wrapper
}// END namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ClassificationMapRegularization)
