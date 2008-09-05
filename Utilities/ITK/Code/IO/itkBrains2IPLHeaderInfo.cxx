/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBrains2IPLHeaderInfo.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-05 20:57:46 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkBrains2IPLHeaderInfo.h"

namespace itk {
Brains2IPLHeaderInfo::Brains2IPLHeaderInfo()
{
  //Nothing to be done here.
}
Brains2IPLHeaderInfo::~Brains2IPLHeaderInfo()
{
  //Nothing to be done here.
}
std::string Brains2IPLHeaderInfo::GetHeaderBeginTag(void) const { return std::string("IPL_HEADER_BEGIN"); }
std::string Brains2IPLHeaderInfo::GetHeaderEndTag(void)   const { return std::string("IPL_HEADER_END"); }
} // end namespace itk
