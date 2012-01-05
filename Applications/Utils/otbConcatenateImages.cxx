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

#include "otbImageListToVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbObjectList.h"
#include "otbWrapperTypes.h"
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
  void DoInit()
  {
    SetName("ConcatenateImages");
    SetDescription("Concatenate a list of images into a single multi channel one.");

    // Documentation
    SetDocName("Images Concatenation");
    SetDocLongDescription("This application performs images concatenation. It will take the input image list (mono or multi channel) and generate a single multi channel image. The channel order is the one of the list.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Rescale application, Convert application");
   
    AddDocTag(Tags::Manip);
    AddDocTag("Concatenation");
    AddDocTag("Multi channel");

    m_Concatener = ListConcatenerFilterType::New();
    m_ExtractorList = ExtractROIFilterListType::New();
    m_ImageList = ImageListType::New();

    AddParameter(ParameterType_InputImageList,  "il",   "Input image list");
    SetParameterDescription("il", "Image list to concatenate");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output multiband image");
    
    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");
 
    // Doc example parameter settings
    SetDocExampleParameterValue("il", "GomaAvant.png GomaApres.png");
    SetDocExampleParameterValue("out", "otbConcatenateImages.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
    
    // Reinitialize the object
    m_Concatener = ListConcatenerFilterType::New();
    m_ImageList = ImageListType::New();
    m_ExtractorList = ExtractROIFilterListType::New();
  }

  void DoExecute()
  {
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
  }
  

  ListConcatenerFilterType::Pointer  m_Concatener;
  ExtractROIFilterListType::Pointer  m_ExtractorList;
  ImageListType::Pointer        m_ImageList;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConcatenateImages)
