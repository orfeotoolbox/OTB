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
#include "otbRectangle.h"

#include <iostream>
#include <fstream>

int otbRectangle(int itkNotUsed(argc), char * argv[])
{
  const char * outfname = argv[1];

  typedef otb::Rectangle<>                   RectangleType;
  typedef RectangleType::ContinuousIndexType ContinuousIndexType;
  typedef RectangleType::VertexListType      VertexListType;
  typedef VertexListType::ConstIterator      IteratorType;

  // Instantiating object
  RectangleType::Pointer rectangle1 = RectangleType::New();

  ContinuousIndexType newVertex;

  newVertex[0] = atof(argv[2]);
  newVertex[1] = atof(argv[3]);
  rectangle1->AddVertex(newVertex);

  newVertex[0] = atof(argv[4]);
  newVertex[1] = atof(argv[5]);
  rectangle1->AddVertex(newVertex);

  rectangle1->SetWidth(atof(argv[6]));
  rectangle1->SetOrientation(atof(argv[7]));

  /** Inside the rectangle test*/
  ContinuousIndexType InsideVertex;
  InsideVertex[0] = atof(argv[8]);
  InsideVertex[1] = atof(argv[9]);

  std::ofstream outfile(outfname);

  if (rectangle1->IsInside(InsideVertex))
    outfile << "The point " <<  InsideVertex << " Is Inside the rectangle"  <<
    std::endl;
  else outfile << "The point " <<  InsideVertex << " Is Outside the rectangle"  << std::endl;

  outfile << "region Size" << rectangle1->GetBoundingRegion().GetSize() << std::endl;
  outfile << "region Origin" << rectangle1->GetBoundingRegion().GetIndex() << std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
