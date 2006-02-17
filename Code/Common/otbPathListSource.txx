/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   13 fevrier 2006
  Version   :   
  Role      :   Classe de base d'une PathList 
  $Id:$

=========================================================================*/

#ifndef _otbPathListSource_txx
#define _otbPathListSource_txx
#include "otbPathListSource.h"

namespace otb
{

/**
 *
 */
template<class TOutputPath>
PathListSource<TOutputPath>
::PathListSource()
{
	m_ListPath.clear();
}

/**
 *
 */

template<class TOutputPath>
void 
PathListSource<TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
