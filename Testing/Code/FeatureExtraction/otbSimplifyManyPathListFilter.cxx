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

#include "otbSimplifyPathListFilter.h"
#include "itkPolyLineParametricPath.h"
#include <fstream>
#include <cstdlib>

/**
 * This filter is intended to test the multithreading capabilities
 * of the otb::UnaryFunctorObjectListFilter
 */

int otbSimplifyManyPathListFilter(int itkNotUsed(argc), char * argv[])
{
  const char * outfname = argv[1];
  const double tolerance = atof(argv[2]);

//   typedef std::vector<double> PointsVectorType;
//   typedef std::vector< PointsVectorType > PointsMatrixType;
//   PointsMatrixType MatricePoints;
//   PointsVectorType ListPoints;

  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension>    PathType;
  typedef otb::SimplifyPathListFilter<PathType>     SimplifyPathListFilterType;
  typedef SimplifyPathListFilterType::InputListType PathListType;
  PathType::ContinuousIndexType cindex;

  PathListType::Pointer InputPathList = PathListType::New();

  //Generate PathList
  unsigned long int numberOfPaths = 10000;
  srand(123456);

  for (unsigned long int i = 0; i < numberOfPaths; ++i)
    {
    PathType::Pointer path = PathType::New();
    //Generate PathList
    unsigned int numberOfPoints = static_cast<int>((rand() / (RAND_MAX + 1.0)) * 90) + 10;
//     std::cout << "List :" << numberOfPoints << " points" << std::endl;
    cindex[0] = 0;
    cindex[1] = 0;
    for (unsigned int j = 0; j < numberOfPoints; ++j)
      {
      cindex[0] += (rand() / (RAND_MAX + 1.0)) * 100 - 50;
      cindex[1] += (rand() / (RAND_MAX + 1.0)) * 100 - 50;
//       std::cout << "Point Index :"<<cindex[0]<<", "<<cindex[1]<<std::endl;
      path->AddVertex(cindex);
      }
    InputPathList->PushBack(path);
    }
  // Instantiating object
  SimplifyPathListFilterType::Pointer simplifyFilter = SimplifyPathListFilterType::New();

  simplifyFilter->SetInput(InputPathList);
  simplifyFilter->GetFunctor().SetTolerance(tolerance);
  simplifyFilter->Update();

  PathListType::Pointer OutputPathList = simplifyFilter->GetOutput();

  typedef PathListType::ConstIterator   PathListIteratorType;
  typedef PathType::VertexListType      VertexListType;
  typedef VertexListType::ConstIterator VertexIteratorType;

  std::ofstream file;
  file.open(outfname);
  unsigned int         counter = 1;
  PathListIteratorType pathListIt = InputPathList->Begin();

  file << "TOLERANCE: " << simplifyFilter->GetFunctor().GetTolerance() << "(" << tolerance << ")" << std::endl;

  pathListIt = OutputPathList->Begin();
  file << "OUTPUT list of Path " << ": " << std::endl;
  while (pathListIt != OutputPathList->End())
    {
    file << "Path " << counter << ": ";
    for (VertexIteratorType vIt = pathListIt.Get()->GetVertexList()->Begin();
         vIt != pathListIt.Get()->GetVertexList()->End();
         ++vIt)
      {
      if (vIt != pathListIt.Get()->GetVertexList()->Begin())
        {
        file << ", ";
        }
      file << vIt.Value();
      }
    file << std::endl;
    ++pathListIt;
    ++counter;
    }
  file.close();

  return EXIT_SUCCESS;
}
