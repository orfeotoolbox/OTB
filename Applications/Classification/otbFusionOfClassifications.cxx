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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "itkLabelVotingImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Wrapper
{

class FusionOfClassifications: public Application
{
public:
  /** Standard class typedefs. */
  typedef FusionOfClassifications Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                            UInt16ImageType::InternalPixelType> ConverterType;
  typedef itk::LabelVotingImageFilter<UInt16ImageType,UInt16ImageType> LabelVotingFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(FusionOfClassifications, otb::Application);

private:
  void DoInit()
  {
    SetName("FusionOfClassifications");
    SetDescription("Fuses several classifications maps of the same image by majority voting on class labels.");
    SetDocName("Fusion of Classifications");
    SetDocLongDescription("This application allows to fuse several classifications maps and produce a single more robust classification map. Fusion is done by mean of majority voting on class labels: for each pixel, the class with the highest number of votes is selected. In case of number of votes equality, the undecided label is attributed to the pixel.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("SVMImagesClassifier application");
 
    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);

    AddParameter(ParameterType_InputImageList, "il", "Input classifications");
    SetParameterDescription( "il", "List of input classification to fuse. Labels in each classification image must represent the same class." );

    AddParameter(ParameterType_Int,"undecided","Label for the undecided class");
    SetParameterDescription("undecided","Label for the undecided class. Pixels with more than 1 majority class are marked as undecided. Please note that the undecided value must be different from existing labels in the input classifications.");
    SetDefaultParameterInt("undecided",0);

    AddParameter(ParameterType_OutputImage,"out","The output classification image");
    SetParameterDescription("out","The output classification image resulting from the fusion of the input classification images");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "classification1.tif classification2.tif");
    SetDocExampleParameterValue("out","classification_fused.tif");
    SetDocExampleParameterValue("undecided","10");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    // Clear any previous filter
    m_Filters.clear();

    FloatVectorImageListType* imageList = GetParameterImageList("il");

    LabelVotingFilterType::Pointer labelVotingFilter = LabelVotingFilterType::New();
    labelVotingFilter->SetLabelForUndecidedPixels(GetParameterInt("undecided"));
    
    m_Filters.push_back(labelVotingFilter.GetPointer());

    //Iterate over all input images
    for (unsigned int imageId = 0; imageId < imageList->Size(); ++imageId)
      {
      FloatVectorImageType* image = imageList->GetNthElement(imageId);

      ConverterType::Pointer converter = ConverterType::New();
      converter->SetInput(image);
      converter->SetChannel(1);

      labelVotingFilter->SetInput(imageId,converter->GetOutput());

      // Register filter
      m_Filters.push_back(converter.GetPointer());
      }

     SetParameterOutputImage("out", labelVotingFilter->GetOutput());
  }
  
  std::vector<itk::LightObject::Pointer> m_Filters;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::FusionOfClassifications)
