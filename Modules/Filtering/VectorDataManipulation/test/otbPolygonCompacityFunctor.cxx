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
#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkConnectedComponentImageFilter.h"
#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbImageToEdgePathFilter.h"
#include "otbPolygonCompacityFunctor.h"
#include "otbUnaryFunctorObjectListBooleanFilter.h"

int otbPolygonCompacityFunctor(int argc, char * argv[])
{

  if (argc != 3)
    {
    std::cout << "Usage: " << argv[0];
    std::cout << " inputImage outputFile" << std::endl;
    return 1;
    }

  typedef unsigned char  InputPixelType;
  typedef unsigned short LabelPixelType; //FIXME doesn't seem to work with long int (64 bits problem ?)

  typedef otb::Image<InputPixelType, 2> InputImageType;
  typedef otb::Image<LabelPixelType, 2> LabelImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::ConnectedComponentImageFilter<InputImageType, LabelImageType> ConnectedComponentsFilterType;
  ConnectedComponentsFilterType::Pointer connectedComponentsFilter = ConnectedComponentsFilterType::New();

//   connectedComponentsFilter->SetInput(reader->GetOutput());
  connectedComponentsFilter->SetInput(reader->GetOutput());
  connectedComponentsFilter->Update();

  std::cout << "# regions: " << connectedComponentsFilter->GetObjectCount() << std::endl;

  typedef otb::Polygon<double>                                    PolygonType;
  typedef otb::ObjectList<PolygonType>                            PolygonListType;
  typedef otb::ImageToEdgePathFilter<LabelImageType, PolygonType> PolygonFilterType;

  PolygonListType::Pointer polygonList = PolygonListType::New();

  for (LabelPixelType label = 1; label <= connectedComponentsFilter->GetObjectCount(); ++label)
    {
    std::cerr << ".";
    PolygonFilterType::Pointer polygonFilter = PolygonFilterType::New();
    polygonFilter->SetInput(connectedComponentsFilter->GetOutput());
    polygonFilter->SetForegroundValue(label);
    polygonFilter->Update();

    polygonList->PushBack(polygonFilter->GetOutput());
    }

  typedef otb::PolygonCompacityFunctor<PolygonType::Pointer>
  CompacityFunctorType;
  typedef otb::UnaryFunctorObjectListBooleanFilter<PolygonListType, PolygonListType,
      CompacityFunctorType> CompacityFilterType;
  CompacityFilterType::Pointer compacityFilter = CompacityFilterType::New();
  compacityFilter->SetInput(polygonList);
  compacityFilter->Update();

  const char *  outfile = argv[2];
  std::ofstream file;
  file.open(outfile);

  typedef PolygonType::VertexListType   VertexListType;
  typedef VertexListType::ConstIterator IteratorType;

  typedef  PolygonListType::ConstIterator PolygonListIteratorType;

  for (PolygonListIteratorType pIt = compacityFilter->GetOutput()->Begin();
       pIt != compacityFilter->GetOutput()->End();
       ++pIt)
    {
    file << "--- New Polygon ---" << std::endl;
    PolygonType::Pointer polygon = pIt.Get();
    IteratorType         it;
    for (it = polygon->GetVertexList()->Begin(); it != polygon->GetVertexList()->End(); ++it)
      {
      file << it.Value() << std::endl;

      }
    }

  file.close();

  return EXIT_SUCCESS;
}
