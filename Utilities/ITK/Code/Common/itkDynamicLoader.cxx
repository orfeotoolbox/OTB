/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDynamicLoader.cxx,v $
  Language:  C++
  Date:      $Date: 2008-10-07 09:09:39 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkDynamicLoader.h"

namespace itk
{

DynamicLoader::DynamicLoader()
{
}

//----------------------------------------------------------------------------
DynamicLoader::~DynamicLoader()
{
}

//----------------------------------------------------------------------------
LibHandle DynamicLoader::OpenLibrary(const char* libname )
{
  return itksys::DynamicLoader::OpenLibrary(libname);
}

//----------------------------------------------------------------------------
int DynamicLoader::CloseLibrary(LibHandle lib)
{
  return itksys::DynamicLoader::CloseLibrary(lib);
}

//----------------------------------------------------------------------------
//itkSymbolPointer
void* DynamicLoader::GetSymbolAddress(LibHandle lib, const char* sym)
{
  return (void*)itksys::DynamicLoader::GetSymbolAddress(lib, sym);
}

//----------------------------------------------------------------------------
const char* DynamicLoader::LibPrefix()
{
  return itksys::DynamicLoader::LibPrefix();
}

//----------------------------------------------------------------------------
const char* DynamicLoader::LibExtension()
{
  return itksys::DynamicLoader::LibExtension();
}

//----------------------------------------------------------------------------
const char* DynamicLoader::LastError()
{
  return itksys::DynamicLoader::LastError();
}

} // end namespace itk
