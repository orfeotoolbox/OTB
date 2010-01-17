/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVersion.cxx,v $
  Language:  C++
  Date:      $Date: 2009-07-28 19:00:40 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkVersion.h"

namespace itk
{

Version::Version()
{
}

Version::~Version()
{
}

const char *
Version::GetITKVersion()
{
  return ITK_VERSION;
}

int
Version::GetITKMajorVersion()
{
  return ITK_VERSION_MAJOR;
}

int
Version::GetITKMinorVersion()
{
  return ITK_VERSION_MINOR;
}

int
Version::GetITKBuildVersion()
{
  return ITK_VERSION_PATCH;
}

const char *
Version::GetITKSourceVersion()
{
  return ITK_SOURCE_VERSION;
}

} // end namespace itk
