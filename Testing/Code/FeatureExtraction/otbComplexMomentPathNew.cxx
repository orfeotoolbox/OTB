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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "otbComplexMomentPathFunction.h"
#include "itkPolyLineParametricPath.h"

int otbComplexMomentPathNew( int argc, char * argv[] )
{
  const   unsigned int      Dimension = 2;
  
  typedef itk::PolyLineParametricPath< Dimension >       PathType;
  typedef otb::ComplexMomentPathFunction<PathType>       CMType;  
  
  CMType::Pointer function = CMType::New();
  

 
  return EXIT_SUCCESS;
}

