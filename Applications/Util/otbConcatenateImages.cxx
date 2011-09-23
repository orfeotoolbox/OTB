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
  typedef ImageListToVectorImageFilter<FloatImageListType, 
                                       FloatVectorImageType >                   ListConcatenerFilterType;
  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, 
                                       FloatImageType::PixelType>               ExtractROIFilterType;
  typedef ObjectList<ExtractROIFilterType>                                      ExtractROIFilterListType;

private:
   ConcatenateImages()
  {
    SetName("ConcatenateImages");
    SetDescription("Concatenate a list of image into a single mulit channel one.");
    m_Concatener = ListConcatenerFilterType::New();
    m_ExtractorList = ExtractROIFilterListType::New();
    m_ImageList = FloatImageListType::New();
  }

  virtual ~ConcatenateImages()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImageList,  "il",   "Input image list");
    SetParameterDescription("il", "Image list to concatenate");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Outmput multiband image");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
    
    // Reinitialize the object
    m_Concatener = ListConcatenerFilterType::New();
    m_ImageList = FloatImageListType::New();
    m_ExtractorList = ExtractROIFilterListType::New();
    this->ClearInternalProcessList();
  }

  void DoExecute()
  {  
    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    // Split each input vector image into image
    // and generate an mono channel image list
    for( unsigned int i=0; i<inList->Size(); i++ )
      {
      FloatVectorImageType::Pointer vectIm = inList->GetNthElement(i);
      vectIm->UpdateOutputInformation();
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
  

  ListConcatenerFilterType::Pointer m_Concatener;
  ExtractROIFilterListType::Pointer         m_ExtractorList;
  FloatImageListType::Pointer               m_ImageList;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConcatenateImages)

