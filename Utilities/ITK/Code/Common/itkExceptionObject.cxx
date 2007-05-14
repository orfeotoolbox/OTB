/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkExceptionObject.cxx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:07 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"
#include "itkIndent.h"

namespace itk
{

void
ExceptionObject
::Print(std::ostream& os) const
{
  Indent indent;

  // Print header
  os << std::endl;
  os << indent << "itk::" << this->GetNameOfClass() << " (" << this << ")\n";

  // Print self
  indent.GetNextIndent();
  if (! m_Location.empty()) 
    {
    os << indent << "Location: \"" << m_Location << "\" " << std::endl;
    }
  
  if (! m_File.empty()) 
    {
    os << indent << "File: " << m_File << std::endl;
    os << indent << "Line: " << m_Line << std::endl;
    }
  
  if (! m_Description.empty()) 
    {
    os << indent << "Description: " << m_Description << std::endl;  
    }

  // Print trailer
  os << indent << std::endl;
}  

} // end namespace itk
