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

#include "otbImageListToVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"

namespace otb
{
namespace Wrapper
{


class ConcatenateImages : public Application
{
public:
  /** Standard class typedefs. */
  typedef ConcatenateImages             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConcatenateImages, otb::Application);

  /** Filters typedef */
  typedef otb::ImageList<FloatImageType>  ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType,
                                       FloatVectorImageType >                   ListConcatenerFilterType;
  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                       FloatImageType::PixelType>               ExtractROIFilterType;
  typedef ObjectList<ExtractROIFilterType>                                      ExtractROIFilterListType;

private:
  void DoInit() override
  {
    SetName("ConcatenateImages");
    SetDescription("Concatenate a list of images of the same size into a single multi-channel image.");

    // Documentation
    SetDocLongDescription("Concatenate a list of images of the same size into a single multi-channel image. "
      "It reads the input image list (single or multi-channel) "
      "and generates a single multi-channel image. The channel order is the same as the list.");
    SetDocLimitations("All input images must have the same size.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Rescale application, DynamicConvert, SplitImage");

    AddDocTag(Tags::Manip);
    AddDocTag("Concatenation");
    AddDocTag("Multi-channel");

    AddParameter(ParameterType_InputImageList,  "il",   "Input images list");
    SetParameterDescription("il", "The list of images to concatenate, must have the same size.");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "The concatenated output image.");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "GomaAvant.png GomaApres.png");
    SetDocExampleParameterValue("out", "otbConcatenateImages.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute() override
  {
    ListConcatenerFilterType::Pointer m_Concatener =
      ListConcatenerFilterType::New();
    ExtractROIFilterListType::Pointer m_ExtractorList = 
      ExtractROIFilterListType::New();
    ImageListType::Pointer m_ImageList =
      ImageListType::New();
    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    if( inList->Size() == 0 )
      {
      itkExceptionMacro("No input Image set...");
      }

    inList->GetNthElement(0)->UpdateOutputInformation();
    FloatVectorImageType::SizeType size = inList->GetNthElement(0)->GetLargestPossibleRegion().GetSize();

    // Split each input vector image into image
    // and generate an mono channel image list
    for( unsigned int i=0; i<inList->Size(); i++ )
      {
      FloatVectorImageType::Pointer vectIm = inList->GetNthElement(i);
      vectIm->UpdateOutputInformation();
      if( size != vectIm->GetLargestPossibleRegion().GetSize() )
        {
        itkExceptionMacro("Input Image size mismatch...");
        }

      for( unsigned int j=0; j<vectIm->GetNumberOfComponentsPerPixel(); j++)
        {
        ExtractROIFilterType::Pointer extractor = ExtractROIFilterType::New();
        extractor->SetInput( vectIm );
        extractor->SetChannel( j+1 );
        extractor->UpdateOutputInformation();
        m_ExtractorList->PushBack( extractor );
        m_ImageList->PushBack( extractor->GetOutput() );
        }
      }


    m_Concatener->SetInput( m_ImageList );

    SetParameterOutputImage("out", m_Concatener->GetOutput());
    RegisterPipeline();
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConcatenateImages)
