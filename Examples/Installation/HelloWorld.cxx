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

//  Software Guide : BeginLatex
//
//  The following code is an implementation of a small OTB
//  program. It tests including header files and linking with OTB
//  libraries.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include <iostream>

int main()
{
  typedef otb::Image<unsigned short, 2> ImageType;

  ImageType::Pointer image = ImageType::New();

  std::cout << "OTB Hello World !" << std::endl;

  return EXIT_SUCCESS;
}
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  This code instantiates an image whose pixels are represented with
//  type \code{unsigned short}. The image is then constructed and assigned to a
//  \doxygen{itk}{SmartPointer}. Although later in the text we will discuss
//  \code{SmartPointer}'s in detail, for now think of it as a handle on an
//  instance of an object (see section \ref{sec:SmartPointers} for more
//  information).  The \doxygen{itk}{Image} class will be described in
//  Section~\ref{sec:ImageSection}.
//
//  Software Guide : EndLatex
