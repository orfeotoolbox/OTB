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

#include "otbLineSpatialObjectList.h"
#include <list>

int otbLineSpatialObjectList(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::LineSpatialObjectList                         LineSpatialObjectListType;
  typedef LineSpatialObjectListType::LineType                LineSpatialObjecType;
  typedef LineSpatialObjectListType::LineType::PointListType PointListType;
  typedef LineSpatialObjectListType::const_iterator          LineSpatialObjectListConstIterator;

  LineSpatialObjectListType::Pointer listLines = LineSpatialObjectListType::New();
  for (int i = 0; i < 10; ++i)
    {
    LineSpatialObjecType::Pointer lLine = LineSpatialObjecType::New();
    listLines->push_back(lLine);
    }

  LineSpatialObjectListConstIterator lIter;
  lIter = listLines->begin();
  while (lIter != listLines->end())
    {
    LineSpatialObjecType::Pointer lLine = (*lIter);
    PointListType                 lPoints = lLine->GetPoints();
    lIter++;
    }

  return EXIT_SUCCESS;
}
