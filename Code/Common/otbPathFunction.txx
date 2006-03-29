/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Evaluate a function of an image over a specific path
  $Id:$

=========================================================================*/
#ifndef _otbPathFunction_txx
#define _otbPathFunction_txx

#include "otbPathFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputPath, class TOutput>
PathFunction< TInputPath,TOutput>
::PathFunction()
{

}


/**
 * Standard "PrintSelf" method
 */
template <class TInputPath, class TOutput>
void
PathFunction<TInputPath, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}





} // end namespace otb

#endif

