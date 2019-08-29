/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbObjectList.h"

#include "otbBandMathXImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "itksys/SystemTools.hxx"

namespace otb
{

namespace Wrapper
{

class BandMathX : public Application
{

public:
  /** Standard class typedefs. */
  typedef BandMathX                      Self;
  typedef Application                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BandMathX, otb::Application);

  typedef otb::MultiChannelExtractROI
    <FloatVectorImageType::InternalPixelType,
     FloatVectorImageType::InternalPixelType> MultiChannelExtractorType;
  typedef otb::BandMathXImageFilter<FloatVectorImageType>   BandMathImageFilterType;


private:
  void DoInit() override
  {
    SetName( "BandMathX" );

    SetDescription("This application performs mathematical operations on several multiband images.");

// As the doc for this application is quiet long the SetDocLongDescription() is 
// called in the following file.
    #include "otbBandMathXdoc.h"

    SetDocLimitations(
      "The application is currently unable to produce one output image per "
      "expression, contrary to otbBandMathXImageFilter. "
      "Separating expressions by semicolons ``;`` will concatenate their results "
      "into a unique multiband output image."
    );
    SetDocAuthors( "OTB-Team" );
    SetDocSeeAlso("[1] https://beltoforion.de/article.php?a=muparserx\n\n"
      "[2] BandMath");
    AddDocTag(Tags::Manip);

    AddParameter( ParameterType_InputImageList, "il", "Input image list" );
    SetParameterDescription( "il", "Image list to perform computation on." );

    AddParameter( ParameterType_OutputImage, "out", "Output Image" );
    SetParameterDescription( "out", "Output image." );

    AddParameter( ParameterType_String, "exp", "Expressions" );
    SetParameterDescription(
      "exp",
      "Mathematical expression to apply."
    );

    AddParameter( ParameterType_InputFilename, "incontext", "Import context" );
    SetParameterDescription(
      "incontext",
      "A txt file containing user's constants and expressions."
    );
    MandatoryOff( "incontext" );

    AddParameter( ParameterType_OutputFilename, "outcontext", "Export context" );
    SetParameterDescription(
      "outcontext",
      "A txt file where to save user's constants and expressions."
    );
    MandatoryOff( "outcontext" );

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue(
      "il",
      "verySmallFSATSW_r.tif verySmallFSATSW_nir.tif verySmallFSATSW.tif"
    );
    SetDocExampleParameterValue( "out", "apTvUtBandMathOutput.tif");
    SetDocExampleParameterValue(
      "exp",
      "\"cos( im1b1 ) + im2b1 * im3b1 - im3b2 + ndvi( im3b3, im3b4 )\""
    );

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // check if input context should be used
    bool context_exists = this->ContextCheck();
    // Check if the expression is correctly set
    BandMathImageFilterType::Pointer math_filter = BandMathImageFilterType::New();
    math_filter->SetManyExpressions(false);
    // first thing, load context if there is one
    if (context_exists)
      math_filter->ImportContext(GetParameterString("incontext"));
    // Only one expression is allowed '-exp'>'-incontext'
    if ( !HasValue("exp") ) 
      SetParameterString("exp", math_filter->GetExpression(0));
    if ( HasValue("il") && HasValue("exp") )
    {
      math_filter->ClearExpression(); // remove expression set by context
      math_filter->SetExpression(GetParameterString("exp")); //set expression
      LiveCheck(math_filter);
    }
  }

  // Check if the given filename is valid
  bool ContextCheck(void)
  {
    bool context_exists = false;
    if (IsParameterEnabled("incontext") && HasValue("incontext"))
    {
      std::string contextPath = GetParameterString("incontext");
      // check that file exists
      if (itksys::SystemTools::FileExists(contextPath,true))
      {
        context_exists = true;
      }
    }
    return context_exists;
  }

  void LiveCheck( BandMathImageFilterType::Pointer math_filter )
    {
    std::vector<MultiChannelExtractorType::Pointer> extractors;
    FloatVectorImageListType::Pointer inList = GetParameterImageList("il");
    for (unsigned int i = 0; i < inList->Size(); i++)
      {
      FloatVectorImageType::Pointer inImg = inList->GetNthElement(i);
      FloatVectorImageType::RegionType  largestRegion = inImg->GetLargestPossibleRegion();
      unsigned int nbChannels = inImg->GetNumberOfComponentsPerPixel();

      MultiChannelExtractorType::Pointer extract = MultiChannelExtractorType::New();
      extractors.push_back(extract);
      extract->SetInput(inImg);
      extract->SetStartX(largestRegion.GetIndex(0));
      extract->SetStartY(largestRegion.GetIndex(1));
      // Set extract size to 1 in case of global stats computation
      extract->SetSizeX(1);
      extract->SetSizeY(1);
      for (unsigned int j=0 ; j<nbChannels ; ++j)
        {
        extract->SetChannel(j+1);
        }
      math_filter->SetNthInput(i,extract->GetOutput());
      }
    try
      {
      math_filter->UpdateOutputInformation();
      SetParameterDescription("exp", "Valid expression");
      }
    catch(itk::ExceptionObject& err)
      {
      // Change the parameter description to be able to have the
      // parser errors in the tooltip
      SetParameterDescription("exp", err.GetDescription());
      // std::string error_string(err.GetDescription());
      // otbAppLogINFO("There was an error while parsing the expression given "
      //   "its input:" + error_string );
      }
    catch(...)
    {
      SetParameterDescription("exp", "Other exception catched");
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

    if ( (!IsParameterEnabled("exp")) && (!IsParameterEnabled("incontext")) )
    {
     itkExceptionMacro("No expression set...; please set and enable at least one expression");
    }

    BandMathImageFilterType::Pointer  math_filter = BandMathImageFilterType::New();
    math_filter->SetManyExpressions(false);

    for (unsigned int i = 0; i < nbImages; i++)
      {
        FloatVectorImageType::Pointer currentImage = inList->GetNthElement(i);
        currentImage->UpdateOutputInformation();

        otbAppLogINFO( << "Image #" << i + 1 << " has "
                       << currentImage->GetNumberOfComponentsPerPixel()
                       << " components");

        math_filter->SetNthInput(i,currentImage);

      }

    bool context_exists = this->ContextCheck();    
    // first thing, load context if there is one
    if (context_exists)
    {
      std::string context_string = GetParameterString("incontext");
      math_filter->ImportContext(context_string);
      otbAppLogINFO("Using Context: " << context_string
      << " for variables (and expression if no parameter -exp has been given)." );
    }
    // Only one expression is allowed '-exp'>'-incontext'
    math_filter->ClearExpression(); // remove expression set by context
    std::string expStr = GetParameterString("exp");
    otbAppLogINFO("Using expression: " << expStr );
    math_filter->SetExpression(expStr);

    if ( IsParameterEnabled("outcontext") && HasValue("outcontext") )
      math_filter->ExportContext(GetParameterString("outcontext"));

    // Set the output image
    SetParameterOutputImage("out", math_filter->GetOutput());
    RegisterPipeline();
  }

};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::BandMathX)
