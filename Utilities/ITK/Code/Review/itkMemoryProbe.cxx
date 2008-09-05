/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMemoryProbe.cxx,v $
  Language:  C++
  Date:      $Date: 2008-04-03 16:21:58 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkMemoryProbe.h"

namespace itk
{

MemoryProbe
::MemoryProbe()
:Probe<MemoryProbe::MemoryLoadType, double>("Memory","kB")
{
}
MemoryProbe
::~MemoryProbe()
{
}

MemoryProbe::MemoryLoadType
MemoryProbe
::GetInstantValue(void) const{
  return static_cast<MemoryProbe::MemoryLoadType>(m_MemoryObserver.GetMemoryUsage());
}

} // end namespace itk
