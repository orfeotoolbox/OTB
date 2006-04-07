/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   6 avril 2006
  Version   :   
  Role      :   Fill Gaps source Class
  $Id$

=========================================================================*/
#ifndef _otbFillGapsFilter_txx
#define _otbFillGapsFilter_txx

#include "otbFillGapsFilter.h"

namespace otb
{

/**
 *
 */
FillGapsFilter::FillGapsFilter()
{
        m_InputLineSpatialObjectList.clear();
        m_OutputLineSpatialObjectList.clear();
}

/**
 *
 */

void FillGapsFilter::
SetOutput(const LineSpatialObjectListType & pLineSpatialObjectList)
{
        m_InputLineSpatialObjectList = pLineSpatialObjectList;
        this->Modified();
}

FillGapsFilter::LineSpatialObjectListType &
FillGapsFilter::GetOutput(void)
{
        return(m_OutputLineSpatialObjectList);
}

/**
 *
 */

void 
FillGapsFilter
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
