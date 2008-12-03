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

#include "otbImageFittingPolygonListFilter.h"
#include "otbPolygon.h"
#include "otbImage.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include <fstream>
#include <cstdlib>

int otbImageFittingPolygonListFilter(int argc, char * argv[])
{
  const char * imageFileName = argv[1];
  const char * polyFileName = argv[2];
  const char * outFileName = argv[3];
  const int fittingRadius = ato(argv[4]);
  const int fittingIters = ato(argv[5]);
  
  const unsigned int Dimension =2;
  typedef otb::Polygon<double>           PolygonType;
  typedef otb::Image<double,Dimension>   ImageType;
  
  //Read the reference image and extract its contours
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFileName);
  
  typedef itk::CannyEdgeDetectionImageFilter<ImageType,ImageType> CannyFilterType;
  CannyFilterType::Pointer canny = CannyFilterType::New();
  canny->SetInput(reader->GetOutput());
  
  
  //Read the original polygon list (kml file)
  typedef otb::VectorData<> VectorDataType;
  typedef VectorDataType::DataTreeType         DataTreeType;
  typedef itk::PreOrderTreeIterator<DataTreeType>       TreeIteratorType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
 
  readerVector->SetFileName(polyFileName);
  readerVector->Update();

  typedef otb::ObjectList<PolygonType> PolygonListType;
  PolygonListType::Pointer polygonList = PolygonListType::New();
  
  TreeIteratorType it(vectorReader->GetOutput()->GetDataTree());
  it.GoToBegin();
    
  while(!it.IsAtEnd())
  {
    DataNodePointerType dataNode = it.Get();
    if(dataNode->IsPolygonFeature())
    {
      polygonList->PushBack(dataNode->GetPolygonExteriorRing());
    }
  }
  
  //Fit the polygons on the image
  typedef otb::ImageFittingPolygonListFilter<PolygonType,ImageType> FittingPolygonType;
  
  FittingPolygonType::Pointer fittingPolygon = FittingPolygonType::New();
  fittingPolygon->SetInput(polygonList);
  fittingPolygon->SetInputImage(canny->GetOutput());
  fittingPolygon->SetRadius(fittingRadius);
  fittingPolygon->SetNumberOfIterations(fittingIters);
  fittingPolygon->Update();

  
  //Read the improved polygon list (kml file)
  VectorDataType::Pointer data = VectorDataType::New();
 
  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder = DataNodeType::New();
  DataNodeType::Pointer polygon = DataNodeType::New();
  

  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);
  polygon->SetNodeType(otb::POLYGON);

  document->SetNodeId("DOCUMENT");
  folder->SetNodeId("FOLDER");
  polygon->SetNodeId("POLYGON");

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document,root);
  data->GetDataTree()->Add(folder,document);

  ListIteratorType listIt = fittingPolygon->GetOutput()->Begin();
  while(listIt  != fittingPolygon->GetOutput()->End())
  {
    polygon->SetPolygon(listIt.Get());
    data->GetDataTree()->Add(polygon,folder);
  }
  
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
  writer->SetFileName(outFileName);
  writer->SetInput(data);
  writer->Update();
  
  return EXIT_SUCCESS;
}
