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

#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"

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
  typedef otb :: ImageList< FloatImageType > ImageListType;
  typedef otb::VectorImageToImageListFilter< FloatVectorImageType, ImageListType > 
          VectorToImageListFilterType;
  typedef otb::ImageListToVectorImageFilter< ImageListType, FloatVectorImageType > 
          ImageListToVectorFilterType;

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
    SetParameterString("in", "/home/antoine/dev/my_data/lena/lena.jpg");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");
    SetParameterString("out", "/home/antoine/dev/my_data/lenaapp.jpg");

    AddRAMParameter();

    AddParameter(ParameterType_Int,"bin","Bin number");
    SetDefaultParameterInt("bin", 256);
    SetParameterDescription("bin","Number of bin in the histogram");

    AddParameter(ParameterType_Float,"hfact","contrast Limitation");  
    SetParameterDescription("hfact","This parameter will set the maximum "
      "height accepted in a bin on the histogram of the input image. "
      "the height will be computated as hfact*eqHeight where eqHeight "
      "is the height of the theorical flat histogram");
    MandatoryOff("hfact");

    AddParameter(ParameterType_Float,"nodata","nodata value");
    MandatoryOff("nodata");

    AddParameter(ParameterType_Group, "thumb", "Thumbnail size");

    AddParameter(ParameterType_Int,"thumb.h","Thumbnail height in pixel");
    AddParameter(ParameterType_Int,"thumb.w","Thumbnail width in pixel");

	}

	void DoUpdateParameters() ITK_OVERRIDE
  {
    if (HasValue("in") )
      {
      if ( !HasUserValue("thumb.w") )
        {
        SetParameterInt( "thumb.w" , 
          GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[0] );
        }
      if ( !HasUserValue("thumb.h") )
        {
      SetParameterInt( "thumb.h" , 
        GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[1] );
        }
      if ( HasUserValue("thumb.h") && 
        GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[1]%GetParameterInt("thumb.h") != 0 )
        {
        std::cout<<"error : hThumbnail = "<<GetParameterInt("thumb.h")<<" is not a divider of the input's height"<<std::endl;
        std::cout<<"Image Height = "<<GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[1]<<std::endl;
        }
      if ( HasUserValue("thumb.w") && 
        GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[0]%GetParameterInt("thumb.w") != 0 )
        {
        std::cout<<"error : wThumbnail = "<<GetParameterInt("thumb.w")<<"is not a divider of the input's width"<<std::endl;
        std::cout<<"Image Width = "<<GetParameterImage("in")->GetLargestPossibleRegion().GetSize()[0]<<std::endl;
        }
      }
  }

  void DoExecute() ITK_OVERRIDE
  {
    ImageListToVectorFilterType::Pointer imageListToVectorFilterOut( ImageListToVectorFilterType::New() );
    FloatVectorImageType * inImage = GetParameterImage("in");
    VectorToImageListFilterType::Pointer vectorToImageListFilter ( VectorToImageListFilterType::New() );
    vectorToImageListFilter->SetInput( inImage );
    vectorToImageListFilter->Update();
    ImageListType::Pointer inputImageList = vectorToImageListFilter->GetOutput();
    ImageListType::Pointer outputImageList ( ImageListType::New() );
    int m = inImage->GetVectorLength ();
    for (int chanel = 0 ; chanel<m ; chanel++ ) 
      {
      std::cout<<"channel m ="<<m<<std::endl;
      FilterType::Pointer filter( FilterType::New() );
      filter->SetInput( inputImageList->GetNthElement(chanel) ) ;
      if ( HasValue("hfact") )
        {
        filter->setHistoThreshFactor( GetParameterInt("hfact") );
        }
      filter->setHistoSize(GetParameterInt("bin"));
      if ( HasUserValue("nodata") )
        {
        filter->setNoData( GetParameterFloat("nodata") );
        }
      filter->setThumbnailSize( GetParameterInt("thumb.w") , 
                              GetParameterInt("thumb.h") );
      filter->Update();
      outputImageList->PushBack( filter->GetOutput() );
      }

    imageListToVectorFilterOut->SetInput(outputImageList);
    imageListToVectorFilterOut->Update();

    SetParameterOutputImage("out", imageListToVectorFilterOut->GetOutput());
  }
};

} //End namespace Wrapper
} //End namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ContrastEnhancement)