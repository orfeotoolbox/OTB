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
#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbRCC8VertexBase.h"
#include "otbPolygonListToRCC8GraphFilter.h"

int otbPolygonListToRCC8GraphFilterNew(int argc, char* argv[])
{
  // typedefs
  typedef otb::Polygon<> PolygonType;
  typedef otb::ObjectList<PolygonType> PolygonListType;
  typedef otb::RCC8VertexBase<PolygonType> VertexType;
  typedef otb::RCC8Graph<VertexType> RCC8GraphType;
  typedef otb::PolygonListToRCC8GraphFilter<PolygonListType,RCC8GraphType>
  RCC8GraphFilterType;

  // Instanatiation
  RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();

  return EXIT_SUCCESS;
}
