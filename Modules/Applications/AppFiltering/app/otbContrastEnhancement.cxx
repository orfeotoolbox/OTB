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

#include "otbContrastEnhancementFilter.h"

namespace otb
{
namespace Wrapper
{

class ContrastEnhancement : public Application
{
public:
  /** Standard class typedefs. */
  typedef ContrastEnhancement	              Self;
  typedef Application	                      Superclass;
  typedef itk::SmartPointer < Self >	      Pointer;
  typedef itk::SmartPointer < const Self >	ConstPointer;

  typedef otb::ContrastEnhancementFilter < FloatImageType , FloatImageType >
  				FilterType;
  /** Standard macro */
  itkNewMacro( Self );
 
  itkTypeMacro( ContrastEnhancement , otb::Application );

private:
	void DoInit() ITK_OVERRIDE
	{
		SetName("Contrast Enhancement");
    SetDescription("");

    // Documentation
    SetDocName("Contrast Enhancement");
    SetDocLongDescription("");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("");

    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");

    AddRAMParameter();

    AddParameter(ParameterType_Int,"bin","Bin number");
    SetParameterDescription("bin","Number of bin in the histogram");

    AddParameter(ParameterType_Int,"hfact","Limitation of the contrast");
    SetParameterDescription("hfact","This parameter will set the maximum "
    	"height accepted in a bin on the histogram of the input image. "
    	"the height will be computated as hfact*eqHeight where eqHeight "
    	"is the height of the theorical flat histogram");

    AddParameter(ParameterType_Group, "thumb", "Thumbnail size");

    AddParameter(ParameterType_Int,"thumb.h","Thumbnail height in pixel");
    AddParameter(ParameterType_Int,"thumb.w","Thumbnail width in pixel");

	}

	void DoUpdateParameters() ITK_OVERRIDE
  {

  }

  void DoExecute() ITK_OVERRIDE
  {
  	
  }
};

} //End namespace Wrapper
} //End namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ContrastEnhancement)