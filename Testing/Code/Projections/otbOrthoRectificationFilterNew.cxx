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

#include "otbMacro.h"
#include "otbImage.h"
#include "otbMapProjections.h"
#include "otbOrthoRectificationFilterBase.h"
#include "otbOrthoRectificationFilter.h"


int otbOrthoRectificationFilterNew( int argc, char* argv[] )
{
        typedef otb::Image<double, 2>     ImageType;
        typedef otb::UtmInverseProjection UtmMapProjectionType ;
        typedef otb::OrthoRectificationFilter<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType ;
        typedef otb::OrthoRectificationFilterBase<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterBaseType ;
				
        OrthoRectifFilterType::Pointer          orthoRectifFilter=OrthoRectifFilterType::New();
        OrthoRectifFilterBaseType::Pointer      orthoRectifFilterBase=OrthoRectifFilterBaseType::New();
				
        return EXIT_SUCCESS;

 }//Fin main()

