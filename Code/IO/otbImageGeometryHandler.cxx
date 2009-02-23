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


#include "otbImageGeometryHandler.h"
#include "otbMacro.h"
#include "imaging/ossimImageHandlerRegistry.h"

namespace otb
{

void ImageGeometryHandler::SetFileName(char *src)
{
  otbDebugMacro(<<"Creation handler... ");
  handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(src));
  if (!handler)
  {
    itkExceptionMacro(<< "Unable to open input image: " << src);
  }
}


ossimKeywordlist ImageGeometryHandler::GetGeometryKeywordlist()
{
  otbDebugMacro( << "Get geometry handler " );
  handler->getImageGeometry(m_geom_kwl);
  return m_geom_kwl;
}

}



