#ifndef _otbImage_txx
#define _otbImage_txx
#include "otbImage.h"

namespace otb
{

/**
 *
 */
template<class TPixel, unsigned int VImageDimension>
Image<TPixel, VImageDimension>
::Image() : itk::Image<TPixel, VImageDimension>()
{
}


/**
 *
 */
template<class TPixel, unsigned int VImageDimension>
void 
Image<TPixel, VImageDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
