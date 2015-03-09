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
#include "itkMacro.h"
#include "otbObjectListToObjectListFilter.h"
#include "otbObjectList.h"
#include <complex>
#include "otbPolygon.h"

int otbObjectListToObjectListFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::PolyLineParametricPathWithValue<std::complex<int>, 2> InputObjectType;
  typedef otb::Polygon<unsigned char>                                OutputObjectType;

  typedef otb::ObjectList<InputObjectType>  InputListType;
  typedef otb::ObjectList<OutputObjectType> OutputListType;

  typedef otb::ObjectListToObjectListFilter<InputListType, OutputListType> ObjectListFilterType;

  ObjectListFilterType::Pointer objectList = ObjectListFilterType::New();

  std::cout << objectList << std::endl;

  return EXIT_SUCCESS;
}
