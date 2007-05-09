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
#ifndef _otbLikehoodPathFilter_txx
#define _otbLikehoodPathFilter_txx

#include "otbLikehoodPathFilter.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkMetaDataObject.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath, class TImage>
LikehoodPathFilter<TPath, TImage>
::LikehoodPathFilter()
{
  m_Key = "Value";
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfInputs(2);
}
template <class TPath, class TImage>
void
LikehoodPathFilter<TPath, TImage>
::SetInputImage(const ImageType * image)
{
  this->itk::ProcessObject::SetNthInput(1,const_cast<ImageType *>(image));
}
template <class TPath, class TImage>
const typename LikehoodPathFilter<TPath, TImage>
::ImageType *
LikehoodPathFilter<TPath, TImage>
::GetInputImage(void)
{
  if(this->GetNumberOfInputs()<1)
    {
      return 0;
    }
  return static_cast<const ImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TPath, class TImage>
void
LikehoodPathFilter<TPath, TImage>
::GenerateData()
{
  // I/O wiring
  ImageConstPointerType inputImagePtr = this->GetInputImage();
  const PathListType *  inputPtr  = this->GetInput();
  PathListType *  outputPtr = this->GetOutput();

  typedef otb::PolyLineImageConstIterator<ImageType,PathType> ImageIteratorType;

  for(IteratorType it = inputPtr->Begin(); it != inputPtr->End(); ++it)
    {
      PathPointerType path = it.Get();
      ImageIteratorType imageIt(inputImagePtr,path);
      double cumulatedValues = 0.0;
      double nbPoints = 0.0;
      for(imageIt.GoToBegin();!imageIt.IsAtEnd();++imageIt,++nbPoints)
	{
	  cumulatedValues+=static_cast<double>(imageIt.Get());
	}
      itk::MetaDataDictionary & dict = path->GetMetaDataDictionary();
      itk::EncapsulateMetaData<double>(dict,m_Key,cumulatedValues/nbPoints);
      outputPtr->PushBack(path);
    }
}
/**
 * PrintSelf Method
 */
template <class TPath, class TImage>
void
LikehoodPathFilter<TPath, TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
