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

//  Software Guide : BeginLatex
//
//  Although \ref{DXFExample}
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "itkPreOrderTreeIterator.h"
#include "otbObjectList.h"
#include "otbPolygon.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{

  if( argc != 3 )
    {
    std::cerr << "Usage: " << argv[0] ;
    std::cerr << "inputFile outputFile" << std::endl;
    return EXIT_FAILURE;
    }

  typedef short unsigned int PixelType;
  typedef otb::VectorData<PixelType,2>           VectorDataType;
  typedef VectorDataType::Pointer           VectorDataPointerType;
  typedef otb::VectorDataFileReader<VectorDataType>
                                                 VectorDataFileReaderType;
  typedef VectorDataFileReaderType::Pointer   VectorDataFileReaderPointerType;

  typedef VectorDataType::DataTreeType      DataTreeType;
  typedef DataTreeType::Pointer             DataTreePointerType;
  typedef itk::PreOrderTreeIterator<DataTreeType>    TreeIteratorType;


  VectorDataFileReaderPointerType reader = VectorDataFileReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();


  typedef otb::Polygon<PixelType> PolygonType;
  typedef PolygonType::VertexListIteratorType PolygonIteratorType;
  typedef otb::ObjectList<PolygonType> PolygonListType;

  typedef PolygonListType::Iterator PolygonListIteratorType;


  PolygonListType::Pointer polygonList = PolygonListType::New();
  
  TreeIteratorType it(reader->GetOutput()->GetDataTree());

  it.GoToBegin();
  
  while(!it.IsAtEnd())
    {
      if(it.Get()->IsPolygonFeature())
	{
	  polygonList->PushBack(it.Get()->GetPolygonExteriorRing());
	}
      ++it;
    }
  
  polygonList->PushBack(PolygonType::New());
  //polygonList->Back()->SetValue(m_LeftViewer->GetNextROILabel());
  

  typedef VectorDataType::DataNodeType DataNodeType;

  typedef DataNodeType::PointType PointType;
  typedef DataNodeType::LineType LineType;
  typedef LineType::VertexType VertexType;

  VectorDataType::Pointer data = VectorDataType::New();
 
  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder1 = DataNodeType::New();
  DataNodeType::Pointer folder2 = DataNodeType::New();
  DataNodeType::Pointer folder3 = DataNodeType::New();
  DataNodeType::Pointer line = DataNodeType::New();
  

  document->SetNodeType(otb::DOCUMENT);
  folder1->SetNodeType(otb::FOLDER);
  folder2->SetNodeType(otb::FOLDER);
  folder3->SetNodeType(otb::FOLDER);
  line->SetNodeType(otb::FEATURE_LINE);

  document->SetNodeId("DOCUMENT");
  folder1->SetNodeId("FOLDER1");
  folder2->SetNodeId("FOLDER2");
  folder3->SetNodeId("FOLDER3");
  line->SetNodeId("FEATURE_LINE");
  
  VertexType p1;
  p1.Fill(5);
  
  VertexType p3;
  p3.Fill(0);
  
  VertexType p2;
  p2[0]=0;
  p2[1]=10;

  
  LineType::Pointer l = LineType::New();
  l->AddVertex(p1);
  l->AddVertex(p2);
  l->AddVertex(p3);
  line->SetLine(l);


  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document,root);
  data->GetDataTree()->Add(folder1,document);
  data->GetDataTree()->Add(folder2,document);
  data->GetDataTree()->Add(folder3,document);
  data->GetDataTree()->Add(line,folder2);

  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
  WriterType::Pointer writer = WriterType::New();  
  writer->SetFileName(argv[2]);
  writer->SetInput(data);
  writer->Update();

  return EXIT_SUCCESS;
}
