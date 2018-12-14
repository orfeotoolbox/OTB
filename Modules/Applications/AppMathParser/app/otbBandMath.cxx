/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
 */

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
  void DoInit() override
  {
    SetName( "BandMath" );

    SetDescription(
      "Outputs a monoband image which is the result of a mathematical operation "
      "on several multi-band images."
    );

    SetDocName( "Band Math" );

    SetDocLongDescription(
      "This application performs a mathematical operation on several multi-band "
      "images and outputs the result into a monoband image. The given expression"
      " is computed at each pixel position. Evaluation of the "
      "mathematical formula is done by the muParser library.\n\n"

      "The formula can be written using:\n\n"
      "* numerical values ( 2.3, -5, 3.1e4, ...)\n"
      "* variables containing pixel values (e.g. ``im2b3`` is the pixel value"
      " in 2nd image, 3rd band)\n"
      "* binary operators:\n\n"

      "  * ``+`` addition, ``-`` subtraction, ``*`` multiplication, ``/`` division\n"
      "  * ``^`` raise x to the power of y\n"
      "  * ``<`` less than, ``>`` greater than, ``<=`` less or equal, ``>=`` greater or equal\n"
      "  * ``==`` equal, ``!=`` not equal\n"
#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
      "  * ``||`` logical or, ``&&`` logical and\n"
      "  * if-then-else operator: ``(condition ? value_true : value_false)``\n"
#else
      "  * ``or`` logical or, ``and`` logical and\n"
      "* if-then-else operator: ``if(condition;value_true;value_false)``\n"
#endif
      "* functions: ``exp()``, ``log()``, ``sin()``, ``cos()``, ``min()``, ``max()``, ...\n\n"

      "The full list of features and operators is available on the muParser website [1]."
      );

    SetDocLimitations( "None" );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso("[1] http://beltoforion.de/article.php?a=muparser");
    AddDocTag(Tags::Manip);

    AddParameter( ParameterType_InputImageList, "il", "Input image list" );
    SetParameterDescription(
      "il",
      "Image list of operands to the mathematical expression."
    );

    AddParameter( ParameterType_OutputImage, "out", "Output Image" );
    SetParameterDescription(
      "out",
      "Output image which is the result of the mathematical expressions on input"
      " image list operands.");

    AddRAMParameter();

    AddParameter( ParameterType_String, "exp", "Expression");
    SetParameterDescription(
      "exp",
      "The muParser mathematical expression to apply on input images."
    );

    // Doc example parameter settings
    SetDocExampleParameterValue(
      "il",
      "verySmallFSATSW_r.tif verySmallFSATSW_nir.tif verySmallFSATSW.tif"
    );
    SetDocExampleParameterValue(
      "out",
      "apTvUtBandMathOutput.tif"
    );
    SetDocExampleParameterValue(
      "exp", "'cos( im1b1 ) > cos( im2b1 ) ? im3b1 : im3b2'"
    );

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
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

  void DoExecute() override
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
