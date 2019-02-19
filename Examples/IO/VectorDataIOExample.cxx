/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


//  Unfortunately, many vector data formats do not
//  share the models for the data they represent. However, in some
//  cases, when simple data is stored, it can be decomposed in simple
//  objects as for instance polylines, polygons and points. This is
//  the case for the Shapefile and the KML (Keyhole Markup Language) formats,
//  for instance.
//
//  Even though specific reader/writer for Shapefile and the Google KML
//  are available in OTB, we designed a generic approach for the IO of
//  this kind of data.

//  The reader/writer for VectorData in OTB is able to access a variety of
//  vector file formats (all OGR supported formats)
//
//  In section \ref{sec:VectorDataProjection}, you will find more information on
//  how projections work for the vector data and how you can export
//  the results obtained with OTB to the real world.
//
//  This example illustrates the use of OTB's vector data IO
//  framework.
//
//  We will start by including the header files for the classes
//  describing the vector data and the corresponding reader and writer.

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
//  We will also need to include the header files for the classes
//  which model the individual objects that we get from the vector
//  data structure.

#include "itkPreOrderTreeIterator.h"
#include "otbObjectList.h"
#include "otbPolygon.h"

int main(int argc, char* argv[])
{

  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "inputFile outputFile" << std::endl;
    return EXIT_FAILURE;
  }

  typedef unsigned short int PixelType;

  //  We define the types for the vector data structure and the
  //  corresponding file reader.

  typedef otb::VectorData<PixelType, 2> VectorDataType;

  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  //  We can now instantiate the reader and read the data.

  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  //  The vector data obtained from the reader will provide a tree of
  //  nodes containing the actual objects of the scene. This tree will
  //  be accessed using an \doxygen{itk}{PreOrderTreeIterator}.

  typedef VectorDataType::DataTreeType            DataTreeType;
  typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;
  //  In this example we will only read polygon objects from the input
  //  file before writing them to the output file. We define the type
  //  for the polygon object as well as an iterator to the vertices. The
  //  polygons obtained will be stored in an \doxygen{otb}{ObjectList}.

  typedef otb::Polygon<double>         PolygonType;
  typedef otb::ObjectList<PolygonType> PolygonListType;

  PolygonListType::Pointer polygonList = PolygonListType::New();
  //  We get the data tree and instantiate an iterator to walk through it.

  TreeIteratorType it(reader->GetOutput()->GetDataTree());

  it.GoToBegin();
  //  We check that the current object is a polygon using the
  //  \code{IsPolygonFeature()} method and get its exterior ring in
  //  order to store it into the list.

  while (!it.IsAtEnd())
  {
    if (it.Get()->IsPolygonFeature())
    {
      polygonList->PushBack(it.Get()->GetPolygonExteriorRing());
    }
    ++it;
  }

  polygonList->PushBack(PolygonType::New());

  //  Before writing the polygons to the output file, we have to build
  //  the vector data structure. This structure will be built up of
  //  nodes. We define the types needed for that.

  VectorDataType::Pointer outVectorData = VectorDataType::New();

  typedef VectorDataType::DataNodeType DataNodeType;
  //  We fill the data structure with the nodes. The root node is a
  //  document which is composed of folders. A list of polygons can be
  //  seen as a multi polygon object.

  DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  document->SetNodeId("polygon");
  DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  DataNodeType::Pointer multiPolygon = DataNodeType::New();
  multiPolygon->SetNodeType(otb::FEATURE_MULTIPOLYGON);
  //  We assign these objects to the data tree stored by the vector data object.

  DataTreeType::Pointer tree = outVectorData->GetDataTree();
  DataNodeType::Pointer root = tree->GetRoot()->Get();

  tree->Add(document, root);
  tree->Add(folder, document);
  tree->Add(multiPolygon, folder);
  //  We can now iterate through the polygon list and fill the vector
  //  data structure.

  for (PolygonListType::Iterator pit = polygonList->Begin(); pit != polygonList->End(); ++pit)
  {
    DataNodeType::Pointer newPolygon = DataNodeType::New();
    newPolygon->SetPolygonExteriorRing(pit.Get());
    tree->Add(newPolygon, multiPolygon);
  }
  //  And finally we write the vector data to a file using a generic
  //  \doxygen{otb}{VectorDataFileWriter}.

  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outVectorData);
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;

  //  This example can  convert an ESRI Shapefile to
  //  a MapInfo File but you can also access with the same OTB source code
  //  to a PostgreSQL datasource, using a connection string as :
  //  PG:"dbname='databasename' host='addr' port='5432' user='x' password='y'"
  //  Starting with GDAL 1.6.0, the set of tables to be scanned can be overridden
  //  by specifying tables=schema.table.
}
