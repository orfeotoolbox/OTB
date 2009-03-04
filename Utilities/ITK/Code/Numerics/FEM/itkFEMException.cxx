/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMException.cxx,v $
  Language:  C++
  Date:      $Date: 2009-01-29 21:28:16 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEMException.h"

namespace itk {
namespace fem {

FEMException::FEMException(const char *file, unsigned int lineNumber, std::string location) :
    ExceptionObject(file,lineNumber)
{
  SetDescription("Unhandled exception in FEM class!");
  SetLocation(location);
}

FEMExceptionIO::FEMExceptionIO(const char *file, unsigned int lineNumber, std::string location, std::string moreDescription) :
    FEMException(file,lineNumber)
{
  SetDescription("IO error in FEM class: "+moreDescription);
  SetLocation(location);
}

FEMExceptionWrongClass::FEMExceptionWrongClass(const char *file, unsigned int lineNumber, std::string location)
  : FEMException(file, lineNumber, location)
{
  SetDescription("Object was of wrong class!");
}

FEMExceptionObjectNotFound::FEMExceptionObjectNotFound(const char *file, unsigned int lineNumber, std::string location, std::string baseClassName, int GN)
  : FEMException(file, lineNumber, location)
{
  m_baseClassName=baseClassName;
  m_GN=GN;
  OStringStream buf;
  buf<<"Object not found ("<<m_baseClassName<<", GN="<<m_GN<<")!";
  SetDescription(buf.str().c_str());
}

FEMExceptionSolution::FEMExceptionSolution(const char *file, unsigned int lineNumber, std::string location, std::string moreDescription) :
  FEMException(file,lineNumber)
{
  SetDescription("Error when solving FEM problem: "+moreDescription);
  SetLocation(location);
}

}} // end namespace itk::fem
