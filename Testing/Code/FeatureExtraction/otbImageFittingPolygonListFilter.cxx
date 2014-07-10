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

#include "otbImageFittingPolygonListFilter.h"
#include "otbPolygon.h"
#include "otbImage.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "otbObjectList.h"
#include <fstream>
#include <cstdlib>

int otbImageFittingPolygonListFilter(int itkNotUsed(argc), char * argv[])
{
  const char * imageFileName = argv[1];
  const char * polyFileName = argv[2];
  const char * outFileName = argv[3];
  const int    fittingRadius = atoi(argv[4]);
  const int    fittingIters = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef otb::Polygon<double>          PolygonType;
  typedef otb::Image<double, Dimension> ImageType;

  //Read the reference image and extract its contours
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFileName);

  typedef itk::CannyEdgeDetectionImageFilter<ImageType, ImageType> CannyFilterType;
  CannyFilterType::Pointer canny = CannyFilterType::New();
  canny->SetInput(reader->GetOutput());

  //Read the original polygon list (kml file)
  typedef otb::VectorData<>                         VectorDataType;
  typedef VectorDataType::DataTreeType              DataTreeType;
  typedef itk::PreOrderTreeIterator<DataTreeType>   TreeIteratorType;
  typedef VectorDataType::DataNodeType              DataNodeType;
  typedef DataNodeType::Pointer                     DataNodePointerType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer readerVector = VectorDataFileReaderType::New();

  readerVector->SetFileName(polyFileName);
  readerVector->Update();

  //Copy the polygons of the data tree in a polygon list
  typedef otb::ObjectList<PolygonType> PolygonListType;
  PolygonListType::Pointer polygonList = PolygonListType::New();

  TreeIteratorType it(readerVector->GetOutput()->GetDataTree());
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    DataNodePointerType dataNode = it.Get();
    if (dataNode->IsPolygonFeature())
      {
      polygonList->PushBack(dataNode->GetPolygonExteriorRing());
      }
    ++it;
    }

  //Fit the polygons on the image
  typedef otb::ImageFittingPolygonListFilter<PolygonType, ImageType> FittingPolygonType;

  FittingPolygonType::Pointer fittingPolygon = FittingPolygonType::New();
  fittingPolygon->SetInput(polygonList);
  fittingPolygon->SetInputImage(canny->GetOutput());
  fittingPolygon->SetRadius(fittingRadius);
  fittingPolygon->SetNumberOfIterations(fittingIters);
  fittingPolygon->Update();

  //Write the improved polygon list (kml file)
  VectorDataType::Pointer data = VectorDataType::New();

  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);

  document->SetNodeId("DOCUMENT");
  folder->SetNodeId("FOLDER");

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document, root);
  data->GetDataTree()->Add(folder, document);

  typedef PolygonListType::ConstIterator ListIteratorType;
  ListIteratorType listIt = fittingPolygon->GetOutput()->Begin();
  while (listIt  != fittingPolygon->GetOutput()->End())
    {
    DataNodeType::Pointer polygon = DataNodeType::New();
    polygon->SetNodeType(otb::FEATURE_POLYGON);
    polygon->SetNodeId("FEATURE_POLYGON");
    polygon->SetPolygonExteriorRing(listIt.Get());
    data->GetDataTree()->Add(polygon, folder);
    ++listIt;
    }

  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outFileName);
  writer->SetInput(data);
  writer->Update();

  return EXIT_SUCCESS;
}
