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

#include "otbBandMathImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbObjectList.h"

namespace otb
{

namespace Wrapper
{

class BandMath : public Application
{

public:
  /** Standard class typedefs. */
  typedef BandMath                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BandMath, otb::Application);

  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                            FloatImageType::PixelType>    ExtractROIFilterType;
  typedef otb::ObjectList<ExtractROIFilterType>                           ExtractROIFilterListType;
  typedef otb::BandMathImageFilter<FloatImageType>                             BandMathImageFilterType;


private:
  void DoInit()
  {
    SetName("BandMath");
    SetDescription("Perform a mathematical operation on monoband images");

    SetDocName("Band Math Application");
    SetDocLongDescription("This application performs a mathematical operation on monoband images."
      " Mathematical formula interpretation is done via MuParser libraries http://muparser.sourceforge.net/");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    AddDocTag("Util");

    AddParameter(ParameterType_InputImageList,  "il",   "Input image list");
    SetParameterDescription("il", "Image list to perform computation on.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image.");

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

    AddParameter(ParameterType_String, "exp", "Expression");
    SetParameterDescription("exp",
                            "The mathematical expression to apply. \nUse im1b1 for the first band, im1b2 for the second one...");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "verySmallFSATSW_r.tif verySmallFSATSW_nir.tif verySmallFSATSW.tif");
    SetDocExampleParameterValue("out", "apTvUtBandMathOutput.tif");
    SetDocExampleParameterValue("exp", "\"cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3, im3b4)\"");
  }
  
  void DoUpdateParameters()
  {
    // Check if the expression is correctly set
    if (HasValue("il"))
      {
      LiveCheck();
      }
  }

  void LiveCheck()
  {
    // Initialize a bandMath Filter first
    m_ChannelExtractorList = ExtractROIFilterListType::New();
    m_Filter               = BandMathImageFilterType::New();
    
    FloatVectorImageListType::Pointer inList   = GetParameterImageList("il");
    unsigned int                      nbInputs = inList->Size();
    unsigned int                      imageId  = 0, bandId = 0;
    
    for (unsigned int i = 0; i < nbInputs; i++)
      {
      FloatVectorImageType::Pointer currentImage = inList->GetNthElement(i);
      currentImage->UpdateOutputInformation();
   
      for (unsigned int j = 0; j < currentImage->GetNumberOfComponentsPerPixel(); j++)
        {
        std::ostringstream tmpParserVarName;
        tmpParserVarName << "im" << imageId + 1 << "b" << j + 1;

        m_ExtractROIFilter = ExtractROIFilterType::New();
        m_ExtractROIFilter->SetInput(currentImage);
        m_ExtractROIFilter->SetChannel(j + 1);
        m_ExtractROIFilter->GetOutput()->UpdateOutputInformation();
        m_ChannelExtractorList->PushBack(m_ExtractROIFilter);
        m_Filter->SetNthInput(bandId, m_ChannelExtractorList->Back()->GetOutput(), tmpParserVarName.str());

        bandId++;
        }
      imageId++;
      }

    otb::Parser::Pointer dummyParser = otb::Parser::New();
    std::vector<double> dummyVars;
    std::string         success = "The expression is Valid";
    SetParameterDescription("exp", success);
    std::ostringstream  failure;

    if (HasValue("exp"))
      {
      // Setup the dummy parser
      for (unsigned int i = 0; i < bandId; i++)
        {
        dummyVars.push_back(1.);
        dummyParser->DefineVar(m_Filter->GetNthInputName(i), &(dummyVars.at(i)));
        }
      dummyParser->SetExpr(GetParameterString("exp"));

      // Check the expression
      try
        {
        dummyParser->Eval();
        }
      catch(itk::ExceptionObject& err)
        {
        // Change the parameter description to be able to have the
        // parser errors in the tooltip
        SetParameterDescription("exp", err.GetDescription());
        }
      }
  }
  
  void DoExecute()
  {
    // Get the input image list
    FloatVectorImageListType::Pointer inList = GetParameterImageList("il");

    // checking the input images list validity
    const unsigned int nbImages = inList->Size();

    if (nbImages == 0)
      {
       itkExceptionMacro("No input Image set...; please set at least one input image");
      }

    m_ChannelExtractorList = ExtractROIFilterListType::New();
    m_Filter               = BandMathImageFilterType::New();

    unsigned int bandId = 0;
    unsigned int imageId = 0;

    for (unsigned int i = 0; i < nbImages; i++)
      {
      FloatVectorImageType::Pointer currentImage = inList->GetNthElement(i);
      currentImage->UpdateOutputInformation();

      otbAppLogINFO( << "Image #" << i + 1 << " has "
                     << currentImage->GetNumberOfComponentsPerPixel()
                     << " components" << std::endl );

      for (unsigned int j = 0; j < currentImage->GetNumberOfComponentsPerPixel(); j++)
        {
        std::ostringstream tmpParserVarName;
        tmpParserVarName << "im" << imageId + 1 << "b" << j + 1;

        m_ExtractROIFilter = ExtractROIFilterType::New();
        m_ExtractROIFilter->SetInput(currentImage);
        m_ExtractROIFilter->SetChannel(j + 1);
        m_ExtractROIFilter->GetOutput()->UpdateOutputInformation();
        m_ChannelExtractorList->PushBack(m_ExtractROIFilter);
        m_Filter->SetNthInput(bandId, m_ChannelExtractorList->Back()->GetOutput(), tmpParserVarName.str());

        bandId++;
        }
      imageId++;
      }

    m_Filter->SetExpression(GetParameterString("exp"));

    // Set the output image
    SetParameterOutputImage("out", m_Filter->GetOutput());
  }

  ExtractROIFilterType::Pointer     m_ExtractROIFilter;
  ExtractROIFilterListType::Pointer m_ChannelExtractorList;
  BandMathImageFilterType::Pointer  m_Filter;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::BandMath)

  
  
