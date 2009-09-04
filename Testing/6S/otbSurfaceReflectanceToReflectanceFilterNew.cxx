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
#include "otbMacro.h"
#include "otbVectorImage.h"

#include "otbSurfaceReflectanceToReflectanceFilter.h"

int main(int argc, char * argv[])
{
  
  const unsigned int                            Dimension = 1;
  typedef double                                PixelType;
  typedef otb::VectorImage<PixelType,Dimension> ImageType;

  
  //typedef ResponseType::PairType    PairType;
  //typedef otb::ObjectList< PairType > PairListType;
//   PairListType::Pointer pairList = PairListType::New();

  typedef otb::SurfaceReflectanceToReflectanceFilter< ImageType,ImageType>  SurfaceReflectanceToReflectanceFilterType;
  typedef SurfaceReflectanceToReflectanceFilterType::Pointer  SurfaceReflectanceToReflectanceFilterPointerType;
  
  
  //Instantiation
  SurfaceReflectanceToReflectanceFilterPointerType  myFilter=SurfaceReflectanceToReflectanceFilterType::New();

  return EXIT_SUCCESS;
}
