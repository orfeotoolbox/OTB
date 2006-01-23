#ifndef _otbExtractROI_txx
#define _otbExtractROI_txx

#include "otbExtractROI.h"

namespace otb
{

/**
 *
 */
template <class TInputPixel, class TOutputPixel,unsigned int VImageDimension>
ExtractROI<TInputPixel, TOutputPixel, VImageDimension>
::ExtractROI() : ExtractROIBase< itk::Image<TInputPixel,VImageDimension> , itk::Image<TOutputPixel,VImageDimension> >()
{
}

/**
 *
 */
template <class TInputPixel, class TOutputPixel,unsigned int VImageDimension>
void 
ExtractROI<TInputPixel, TOutputPixel, VImageDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}


/** 
 * ExtractROI can produce an image which is a different resolution
 * than its input image.  As such, ExtractROI needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template <class TInputPixel, class TOutputPixel,unsigned int VImageDimension>
void 
ExtractROI<TInputPixel, TOutputPixel, VImageDimension>
::GenerateOutputInformation()
{
        // Appel à la methode de la classe de base
        Superclass::GenerateOutputInformation();
}


} // end namespace otb

#endif
