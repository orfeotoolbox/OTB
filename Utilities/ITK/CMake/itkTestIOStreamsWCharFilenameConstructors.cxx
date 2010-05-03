/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTestIOStreamsWCharFilenameConstructors.cxx,v $
  Language:  C++
  Date:      $Date: 2009-11-24 15:16:01 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/*
   This file tests whether fstream's have a wchar_t * constructor
*/

#include <fstream>

int main() 
{
  std::ofstream ostr( L"tmptest.txt" );
  std::ifstream istr( L"tmptest.txt" );
  return 0;
}

