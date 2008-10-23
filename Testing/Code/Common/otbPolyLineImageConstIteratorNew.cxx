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
#include "itkExceptionObject.h"

#include "itkPolyLineParametricPath.h"
#include "otbImage.h"
#include "otbPolyLineImageConstIterator.h"

int otbPolyLineImageConstIteratorNew(int argc, char * argv[])
{
  const unsigned int Dimension =2;
  typedef unsigned char PixelType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typededef otb::Image<PixelType,Dimension> ImageType;
  typedef otb::PolyLineImageConstIterator<ImageType,PathType> ConstIteratorType;

  PathType::Pointer path = PathType::New();
  ImageType::Pointer image = ImageType::New();
      
  // Instantiating object
  ConstIteratorType it(image,path);
 
 
  return EXIT_SUCCESS;
}
