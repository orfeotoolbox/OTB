/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
