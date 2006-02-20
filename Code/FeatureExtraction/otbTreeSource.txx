/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Role      :  Definition de la classe TreeSource 
  $Id$ 

=========================================================================*/
 
#ifndef _otbTreeSource_txx
#define _otbTreeSource_txx

#include "otbTreeSource.h"

namespace otb
{

/**
 *
 */
template<class TOutputTree>
TreeSource<TOutputTree>
::TreeSource()
{
  m_TreeList.clear();
  
  this->ProcessObject::SetNumberOfRequiredOutputs(1);

}


/**
 *
 */
template<class TOutputTree>
void 
TreeSource<TOutputTree>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
