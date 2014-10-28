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
#include "otbLabelObjectToPolygonFunctor.h"
#include "otbAttributesMapLabelObject.h"
#include <cstdlib>

int otbLabelObjectToPolygonFunctorNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;

  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>             LabelObjectType;
  typedef otb::Polygon<double>                                                    PolygonType;
  typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType, PolygonType> FunctorType;
  //typedef FunctorType::Pointer FunctorPointerType;
  // Instantiation
  //FunctorPointerType myFunctor = FunctorType::New();

  FunctorType myFunctor;

  return EXIT_SUCCESS;
}
