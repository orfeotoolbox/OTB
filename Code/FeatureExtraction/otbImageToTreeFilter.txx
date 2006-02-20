/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Version   :   
  Role      :   Classe de base d'un filtre de "Tree" 
  $Id:$

=========================================================================*/

#ifndef __otbImageToTreeFilter_txx
#define __otbImageToTreeFilter_txx

#include "otbImageToTreeFilter.h"


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputTree>
ImageToTreeFilter<TInputImage,TOutputTree>
::ImageToTreeFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);
}

/**
 *
 */
template <class TInputImage, class TOutputTree>
void
ImageToTreeFilter<TInputImage,TOutputTree>
::SetImageInput(const InputImageType *image)
{
  // We have 1 input:  an image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, 
                                   const_cast< InputImageType * >( image ) );
}

template <class TInputImage, class TOutputTree>
const typename ImageToTreeFilter<TInputImage,TOutputTree>::InputImageType *
ImageToTreeFilter<TInputImage,TOutputTree>
::GetImageInput(void) 
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
template <class TInputImage, class TOutputTree>
void
ImageToTreeFilter<TInputImage,TOutputTree>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb

#endif
