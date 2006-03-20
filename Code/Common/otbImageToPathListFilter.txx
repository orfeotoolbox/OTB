/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   13 fevrier 2006
  Version   :   
  Role      :   Classe de base d'une PathList 
  $Id:$

=========================================================================*/

#ifndef __otbImageToPathListFilter_txx
#define __otbImageToPathListFilter_txx

#include "otbImageToPathListFilter.h"


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPath>
ImageToPathListFilter<TInputImage,TOutputPath>
::ImageToPathListFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);
}

/**
 *
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathListFilter<TInputImage,TOutputPath>
//OTB-FA-00010-CS  
::SetInput(const InputImageType *image)
{
  // We have 1 input:  an image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, 
                                   const_cast< InputImageType * >( image ) );
}

template <class TInputImage, class TOutputPath>
const typename ImageToPathListFilter<TInputImage,TOutputPath>::InputImageType *
ImageToPathListFilter<TInputImage,TOutputPath>
//OTB-FA-00010-CS  
::GetInput(void) 
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<const TInputImage * >
    (this->itk::ProcessObject::GetInput(0) );
}
  
/**
 *
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathListFilter<TInputImage,TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb

#endif
