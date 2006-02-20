/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Role      :  Definition de la classe TreeSource 
  $Id$ 

=========================================================================*/
 

#ifndef __otbTreeNeighborhood_txx
#define __otbTreeNeighborhood_txx

#include "otbTreeNeighborhood.h"

namespace otb
{

/**
 *
 */

otbTreeNeighborhood
::TreeNeighborhood()
{
  m_tabPoints.clear();
  m_iNbPoints = 0;
  m_type = AMBIGUOUS;
  m_otherBound = 0.0;
}

void
otbTreeNeighborhood
::SetSize(int Taille)
{
  assert( Taille > 0 );
  m_tabPoints.resize(Taille); 
}


/**
 *
 */

void 
otbTreeNeighborhood
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
