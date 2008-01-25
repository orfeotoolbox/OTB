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
#include "otbRCC8Graph.h"
#include "otbImage.h"
#include "otbRCC8VertexBase.h"
#include "otbImageListToRCC8GraphFilter.h"
#include "otbPolygon.h"

int otbImageListToRCC8GraphFilterNew(int argc, char* argv[])
{
try 
  {
    const unsigned int Dimension = 2;
    typedef unsigned short LabelPixelType;
    typedef otb::Polygon<> PathType;
    typedef otb::Image<LabelPixelType,Dimension> LabelImageType;
    typedef otb::RCC8VertexBase<PathType> VertexType;
    typedef otb::RCC8Graph<VertexType> RCC8GraphType;
    typedef otb::ImageListToRCC8GraphFilter<LabelImageType,RCC8GraphType>
      ImageListToRCC8GraphFilterType;

    // Instanatiation
    ImageListToRCC8GraphFilterType::Pointer filter = ImageListToRCC8GraphFilterType::New();

  }
catch( itk::ExceptionObject & err ) 
  { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
catch( ... ) 
  { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
  } 
 return EXIT_SUCCESS;
}  
