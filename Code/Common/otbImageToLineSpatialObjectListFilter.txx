/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   11 avril 2006
  Version   :   
  Role      :   Image to Line Spatial Object List Filter
  $Id: $

=========================================================================*/
#ifndef _otbImageToLineSpatialObjectListFilter_txx
#define _otbImageToLineSpatialObjectListFilter_txx

#include "otbImageToLineSpatialObjectListFilter.h"

namespace otb
{

/**
 *
 */
template<class TInputImage>
ImageToLineSpatialObjectListFilter<TInputImage>::ImageToLineSpatialObjectListFilter()
{
   
   m_OutputLinesList.clear();
}

template<class TInputImage>
typename ImageToLineSpatialObjectListFilter<TInputImage>::LinesListType &
ImageToLineSpatialObjectListFilter<TInputImage>::GetOutput(void)
{
        return(m_OutputLinesList);
}

/**
 *
 */
template<class TInputImage>
void 
ImageToLineSpatialObjectListFilter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
